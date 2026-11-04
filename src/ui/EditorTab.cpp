#define GEODE_DEFINE_EVENT_EXPORTS
#include "../../include/ui/EditorTab.hpp"
#include "../../include/api/Tab.hpp"
#include "../../include/api/Mode.hpp"
#include "../../include/api/Editor.hpp"

#include "../hooks/EditorUI.hpp"
#include "DummyBoom.hpp"

namespace alpha::editor_tabs {

struct EditorTab::Impl : public ZImpl<EditorTab::Impl> {
    Ref<BoomScrollLayer> m_scrollLayerRef = nullptr;
    Ref<CCArray> m_buttonArrayRef = nullptr;
    Ref<CCArray> m_pagesArrayRef = nullptr;
    Ref<CCMenu> m_itemContainer = nullptr;
    Ref<CCMenu> m_dotContainer = nullptr;
    Ref<CCMenu> m_navigationMenu = nullptr;
    CCMenuItemSpriteExtra* m_prevButton = nullptr;
    CCMenuItemSpriteExtra* m_nextButton = nullptr;
    std::vector<Ref<CCMenuItemSpriteExtra>> m_dots{};
    Ref<CCNode> m_container = nullptr;

    Tab* m_tab = nullptr;

    bool m_initialized = false;

    bool m_itemless = false;
    bool m_overrideSize = false;
    bool m_hasAutoScale = true;
    bool m_disableSizeOverrideIfExceeded = false;

    AxisLayout* m_itemLayout = nullptr;

    alpha::editor_tabs::Tab::TabAlignment m_tabAlignment = alpha::editor_tabs::Tab::TabAlignment::Top;

    geode::AxisAlignment m_horizontalButtonAlignment = geode::AxisAlignment::Start;
    geode::AxisAlignment m_verticalButtonAlignment = geode::AxisAlignment::End;

    geode::Padding m_padding = {8.f, 0.f, 8.f, 0.f};
    float m_dotScale = 1.f;

    int m_rows = 0;
    int m_columns = 0;

    int m_page = 0;
    int m_pageCount = 0;
};

void EditorTab::construct() {
    m_impl = std::make_unique<Impl>();

    m_uReference = 1;

    auto director = CCDirector::get();
    m_pActionManager = director->getActionManager();
    m_pActionManager->retain();
    m_pScheduler = director->getScheduler();
    m_pScheduler->retain();

    auto pEngine = CCScriptEngineManager::sharedManager()->getScriptEngine();
    m_eScriptType = pEngine != nullptr ? pEngine->getScriptType() : kScriptTypeNone;

    m_fScaleX = 1.f;
    m_fScaleY = 1.f;

    m_nTag = -1;
    m_sAdditionalTransform = CCAffineTransformMakeIdentity();
    m_bTransformDirty = true;
    m_bInverseDirty = true;
    m_bVisible = true;

    auto obj = new CCObject();
    m_uID = obj->m_uID;
    obj->release();

    m_pComponentContainer = new CCComponentContainer(ZeroConstructor);
    m_pComponentContainer->m_pComponents = nullptr;
    m_pComponentContainer->m_pOwner = this;
}

EditorTab* EditorTab::create(CCArray* items, int tab, bool hasCreateItems) {
    auto ret = new EditorTab();
    if (ret->init(items, tab, hasCreateItems)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

EditorTab* EditorTab::create(std::span<CCNode*> items, int tab, bool hasCreateItems) {
    auto ret = new EditorTab();

    auto itemsArr = CCArray::create();
    for (auto& item : items) {
        itemsArr->addObject(item);
    }

    if (ret->init(itemsArr, tab, hasCreateItems)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

EditorTab* EditorTab::create(std::span<Ref<CCNode>> items, int tab, bool hasCreateItems) {
    auto ret = new EditorTab();

    auto itemsArr = CCArray::create();
    for (auto& item : items) {
        itemsArr->addObject(item);
    }

    if (ret->init(itemsArr, tab, hasCreateItems)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

EditorTab* EditorTab::create() {
    auto ret = new EditorTab();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool EditorTab::init() {
    m_impl->get()->m_itemless = true;

    init(nullptr, 0, false);
    return true;
}

bool EditorTab::init(CCArray* items, int tab, bool hasCreateItems) {
    auto winSize = CCDirector::get()->getWinSize();

    auto noItems = m_impl->get()->m_itemless;
    m_impl->get()->m_itemless = false;

    m_impl->get()->m_buttonArrayRef = items ? items->shallowCopy() : CCArray::create();
    m_buttonArray = m_impl->get()->m_buttonArrayRef;

    auto arr = CCArray::create();
    arr->addObject(CCNode::create()); //fixes a bug in vanilla where 0 elements will loop forever
    
    m_impl->get()->m_scrollLayerRef = DummyBoom::create();
    m_scrollLayer = m_impl->get()->m_scrollLayerRef;

    m_impl->get()->m_pagesArrayRef = CCArray::create();
    m_pagesArray = m_impl->get()->m_pagesArrayRef;

    m_hasCreateItems = hasCreateItems;
    m_tabIndex = tab;

    setAnchorPoint({0.5f, 0.f});
    setPosition({winSize.width / 2.f, 0.f});

    m_impl->get()->m_navigationMenu = CCMenu::create();
    m_impl->get()->m_navigationMenu->ignoreAnchorPointForPosition(false);
    m_impl->get()->m_navigationMenu->setAnchorPoint({0.5f, 0.5f});
    m_impl->get()->m_navigationMenu->setID("arrows-menu");
    m_impl->get()->m_navigationMenu->setLayout(SimpleRowLayout::create()
        ->setMainAxisScaling(AxisScaling::None)
        ->setCrossAxisScaling(AxisScaling::Fit)
        ->setMainAxisAlignment(MainAxisAlignment::Between)
    );

    if (!noItems) {
        addChild(m_impl->get()->m_navigationMenu);
    }

    auto prevArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    prevArrow->setScale(0.6f);
    m_impl->get()->m_prevButton = CCMenuItemSpriteExtra::create(prevArrow, this, menu_selector(EditButtonBar::onLeft));
    m_impl->get()->m_prevButton->setID("left-button");

    m_impl->get()->m_navigationMenu->addChild(m_impl->get()->m_prevButton);

    auto nextArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    nextArrow->setScale(0.6f);
    nextArrow->setFlipX(true);
    m_impl->get()->m_nextButton = CCMenuItemSpriteExtra::create(nextArrow, this, menu_selector(EditButtonBar::onRight));
    m_impl->get()->m_nextButton->setID("right-button");

    m_impl->get()->m_navigationMenu->addChild(m_impl->get()->m_nextButton);

    m_impl->get()->m_dotContainer = CCMenu::create();
    m_impl->get()->m_dotContainer->ignoreAnchorPointForPosition(false);
    m_impl->get()->m_dotContainer->setAnchorPoint({0.5f, 0.f});
    m_impl->get()->m_dotContainer->setID("dots-menu");

    if (!noItems) {
        addChild(m_impl->get()->m_dotContainer);
    }

    m_impl->get()->m_itemContainer = CCMenu::create();
    m_impl->get()->m_itemContainer->ignoreAnchorPointForPosition(false);
    m_impl->get()->m_itemContainer->setID("button-menu");

    m_impl->get()->m_itemLayout = RowLayout::create()
        ->setGrowCrossAxis(true)
        ->setCrossAxisOverflow(false)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setCrossAxisAlignment(AxisAlignment::End);
    
    m_impl->get()->m_itemContainer->setLayout(m_impl->get()->m_itemLayout);

    if (!noItems) {
        addChild(m_impl->get()->m_itemContainer);
    }

    m_impl->get()->m_container = CCNode::create();
    m_impl->get()->m_container->setAnchorPoint({0.5f, 0.f});
    m_impl->get()->m_container->setID("container"_spr);

    if (noItems) {
        addChild(m_impl->get()->m_container);
    }

    m_impl->get()->m_rows = GameManager::get()->getIntGameVariable(GameVar::EditorButtonRows);
    m_impl->get()->m_columns = GameManager::get()->getIntGameVariable(GameVar::EditorButtonsPerRow);

    m_impl->get()->m_itemless = noItems;
    m_impl->get()->m_initialized = true;

    return true;
}

void EditorTab::setContentSize(const CCSize& contentSize) {
    CCNode::setContentSize(contentSize);
    if (!m_impl->get()->m_initialized) return;

    updateUI();
    alpha::editor_tabs::Tab::UpdateTabUIEvent(m_impl->get()->m_tab).send(this);
    alpha::editor_tabs::Tab::UpdateTabSizeEvent(m_impl->get()->m_tab).send(this, contentSize);
}

void EditorTab::setPosition(const CCPoint& position) {
    CCNode::setPosition(position);
    if (!m_impl->get()->m_initialized) return;

    alpha::editor_tabs::Tab::UpdateTabUIEvent(m_impl->get()->m_tab).send(this);
}

void EditorTab::setVisible(bool visible) {
    CCNode::setVisible(visible);
    if (!m_impl->get()->m_initialized) return;
    
    alpha::editor_tabs::Tab::UpdateTabVisibilityEvent(m_impl->get()->m_tab).send(this, visible);
}

void EditorTab::setItemless(bool itemless) {
    if (m_impl->get()->m_itemless == itemless) return;

    m_impl->get()->m_itemless = false;

    if (itemless) {
        m_impl->get()->m_navigationMenu->removeFromParent();
        m_impl->get()->m_dotContainer->removeFromParent();
        m_impl->get()->m_itemContainer->removeFromParent();
        addChild(m_impl->get()->m_container);
    }
    else {
        m_impl->get()->m_container->removeFromParent();
        addChild(m_impl->get()->m_navigationMenu);
        addChild(m_impl->get()->m_dotContainer);
        addChild(m_impl->get()->m_itemContainer);
    }

    m_impl->get()->m_itemless = itemless;

    updateUI();
}

CCArray* EditorTab::getButtons() {
    if (m_impl->get()->m_tab && m_impl->get()->m_tab->getMode() && m_impl->get()->m_tab->getMode()->getID() == alpha::editor_tabs::Edit && m_impl->get()->m_tab->getID() == "edit") {
        auto ret = ETEditorUI::get()->getEditButtons();
        if (!ret) return CCArray::create();
        return ret;
    }

    return m_buttonArray;
}

CCArray* EditorTab::getAllButtons() {
    if (m_impl->get()->m_tab && m_impl->get()->m_tab->getMode() && m_impl->get()->m_tab->getMode()->getID() == alpha::editor_tabs::Edit && m_impl->get()->m_tab->getID() == "edit") {
        auto allButtons = m_buttonArray->shallowCopy();

        auto buttons = ETEditorUI::get()->getEditButtons();
        if (buttons) {
            allButtons->addObjectsFromArray(buttons);
        }

        return allButtons;
    }

    return m_buttonArray;
}

void EditorTab::overrideSize(int rows, int columns) {
    m_impl->get()->m_overrideSize = true;
    m_impl->get()->m_rows = rows;
    m_impl->get()->m_columns = columns;

    loadFromItems_(m_buttonArray, columns, rows, true);
}

void EditorTab::overrideSize(bool override, int rows, int columns) {
    m_impl->get()->m_overrideSize = override;

    if (override) {
        m_impl->get()->m_rows = rows;
        m_impl->get()->m_columns = columns;
    }
    else {
        m_impl->get()->m_rows = GameManager::get()->getIntGameVariable(GameVar::EditorButtonRows);
        m_impl->get()->m_columns = GameManager::get()->getIntGameVariable(GameVar::EditorButtonsPerRow);
    }

    loadFromItems_(m_buttonArray, m_impl->get()->m_columns, m_impl->get()->m_rows, true);
}

int EditorTab::getRows() {
    return m_impl->get()->m_rows;
}

int EditorTab::getColumns() {
    return m_impl->get()->m_columns;
}

CCSize EditorTab::getAvailableSize() {
    auto editor = ETEditorUI::get();

    if (!editor) return {};

    auto winSize = CCDirector::get()->getWinSize();

    auto spacerLeft = editor->getChildByID("spacer-line-left");
    auto spacerRight = editor->getChildByID("spacer-line-right");
    
    if (!spacerLeft || !spacerRight) {
        return {(CCDirector::get()->getScreenRight() - 90.f) - (CCDirector::get()->getScreenLeft() + 90.f), 90.f};
    }

    auto rightSpace = winSize.width - spacerRight->getPositionX();
    auto leftSpace = spacerLeft->getPositionX();

    return CCSize{winSize.width - rightSpace - leftSpace, 90.f};
}

void EditorTab::updateUI() {
    auto winSize = CCDirector::get()->getWinSize();
    auto maxSize = getAvailablePaddedSize();

    if (m_impl->get()->m_hasAutoScale) {
        float scaleX = getPaddedSize().width / maxSize.width;
        float scaleY = getPaddedSize().height / maxSize.height;

        float scale = std::min(scaleX, scaleY);
        m_impl->get()->m_container->setScale(scale);
    }
    else {
       m_impl->get()-> m_container->setScale(1.f);
    }

    m_impl->get()->m_container->setPosition(getContentSize() / 2.f);
    
    m_impl->get()->m_dotContainer->setContentSize({getPaddedSize().width, 6.f});
    m_impl->get()->m_dotContainer->setPosition({getContentWidth() / 2.f, 0.75f});

    updatePages();

    goToPage_(m_impl->get()->m_page);

    float y = 0.f;
    if (m_impl->get()->m_tabAlignment == alpha::editor_tabs::Tab::TabAlignment::Center) {
        m_impl->get()->m_itemContainer->setAnchorPoint({0.5f, 0.5f});
        y = getPaddedSize().height / 2.f + m_impl->get()->m_dotContainer->getContentHeight() / 2.f + m_impl->get()->m_dotContainer->getPositionY();
    }
    else {
        m_impl->get()->m_itemContainer->setAnchorPoint({0.5f, 1.f});
        y = getPaddedSize().height - 5.f;
    }
    
    m_impl->get()->m_itemContainer->setPosition({getContentWidth() / 2.f, y});

    m_impl->get()->m_navigationMenu->setContentSize({getPaddedSize().width, m_impl->get()->m_itemContainer->getScaledContentHeight()});
    m_impl->get()->m_navigationMenu->updateLayout();
}

cocos2d::CCSize EditorTab::getPaddedSize() {
    auto p = m_impl->get()->m_padding;
    return getContentSize() - CCSize{p.left + p.right, p.top + p.bottom};
}

cocos2d::CCSize EditorTab::getAvailablePaddedSize() {
    auto p = m_impl->get()->m_padding;
    return getAvailableSize() - CCSize{p.left + p.right, p.top + p.bottom};
}

void EditorTab::updateDots() {
    for (const auto& dot : m_impl->get()->m_dots) {
        dot->setColor(m_impl->get()->m_page == dot->getTag() ? ccColor3B{255, 255, 255} : ccColor3B{125, 125, 125});
    }
}

void EditorTab::updatePages() {    
    m_impl->get()->m_dots.clear();
    m_impl->get()->m_dotContainer->removeAllChildren();

    for (int i = 0; i < m_impl->get()->m_pageCount; i++) {
        auto dot = createDot(i);
        dot->setEnabled(true);
        dot->setScale(m_impl->get()->m_dotScale);
        m_impl->get()->m_dots.push_back(dot);
        m_impl->get()->m_dotContainer->addChild(dot);
    }

    m_impl->get()->m_dotContainer->updateLayout();
    updateDots();

    m_scrollLayer->m_pages->removeAllObjects();
    m_scrollLayer->m_page = m_impl->get()->m_page;

    for (int i = 0; i < m_impl->get()->m_pageCount; i++) {
        m_scrollLayer->m_pages->addObject(CCNode::create());
    }
}

CCMenuItemSpriteExtra* EditorTab::createDot(int page) {
    auto ret = CCMenuItemExt::createSpriteExtraWithFilename("smallDot.png", 0.5f, [this, page] (CCMenuItemSpriteExtra* sender) {
        if (m_impl->get()->m_page != page && m_hasCreateItems) {
            EditorSFXManager::sharedState()->playAction(278);
        }
        goToPage(page);
    });
    ret->setTag(page);
    ret->setZOrder(page);
    ret->setCascadeColorEnabled(true);
    ret->setCascadeOpacityEnabled(true);
    ret->setID(fmt::format("page-{}-dot", page));

    return ret;
}

void EditorTab::reloadAllItems() {
    loadFromItems_(m_buttonArray, m_impl->get()->m_columns, m_impl->get()->m_rows, true);
}

void EditorTab::updateItemSize(int rows, int columns) {
    auto allButtons = getAllButtons();

    auto noOverride = m_impl->get()->m_disableSizeOverrideIfExceeded && allButtons->count() > (m_impl->get()->m_rows * m_impl->get()->m_columns);

    if (!m_impl->get()->m_overrideSize || noOverride) {
        m_impl->get()->m_rows = rows;
        m_impl->get()->m_columns = columns;
    }

    reloadAllItems();
}

void EditorTab::loadFromItems_(CCArray* items, int columns, int rows, bool preserve) {
    if (items != m_buttonArray) {
        m_impl->get()->m_buttonArrayRef = items ? items->shallowCopy() : CCArray::create();
        m_buttonArray = m_impl->get()->m_buttonArrayRef;
    }

    auto allButtons = getAllButtons();

    auto noOverride = m_impl->get()->m_disableSizeOverrideIfExceeded && allButtons->count() > (m_impl->get()->m_rows * m_impl->get()->m_columns);

    if (!m_impl->get()->m_overrideSize || noOverride) {
        m_impl->get()->m_rows = rows;
        m_impl->get()->m_columns = columns;
    }

    auto perPage = m_impl->get()->m_rows * m_impl->get()->m_columns;
    if (m_impl->get()->m_itemless) {
        m_impl->get()->m_pageCount = 1;
    }
    else {
        m_impl->get()->m_pageCount = (allButtons->count() + perPage - 1) / perPage;
    }

    if (!preserve) {
        m_impl->get()->m_page = 0;
    }

    if (m_hasCreateItems) {
        int pageIdx = -1;
        int idx = 0;

        for (auto obj : allButtons->asExt<CreateMenuItem>()) {
            if (idx % perPage == 0) {
                pageIdx++;
            }

            obj->m_pageIndex = pageIdx;
            obj->m_tabIndex = m_tabIndex;

            obj->setID(fmt::format("object-{}-button", obj->m_objectID));

            idx++;
        }
    }

    if (m_impl->get()->m_itemless) {
        if (!m_impl->get()->m_tab) return;
        alpha::editor_tabs::Tab::TabLoadedEvent(m_impl->get()->m_tab).send(this);
        return;
    }

    m_impl->get()->m_dotContainer->setContentSize({getPaddedSize().width, 6.f});
    m_impl->get()->m_dotContainer->setLayout(SimpleRowLayout::create()
        ->setGap(12.8f)
        ->setMainAxisScaling(AxisScaling::ScaleDownGaps)
        ->setCrossAxisScaling(AxisScaling::Fit)
    );

    m_impl->get()->m_dotContainer->updateLayout();

    updatePages();
    goToPage_(m_impl->get()->m_page);

    if (!m_impl->get()->m_tab) return;
    alpha::editor_tabs::Tab::TabLoadedEvent(m_impl->get()->m_tab).send(this);
}

void EditorTab::goToPage_(int page) {
    if (m_impl->get()->m_itemless || m_impl->get()->m_pageCount == 0) return;

    m_impl->get()->m_page = page;

    if (m_impl->get()->m_page < 0) {
        m_impl->get()->m_page = std::max(m_impl->get()->m_pageCount - 1, 0);
    }
    if (m_impl->get()->m_page >= m_impl->get()->m_pageCount) {
        m_impl->get()->m_page = 0;
    }

    m_impl->get()->m_navigationMenu->setVisible(m_impl->get()->m_pageCount > 1);
    m_impl->get()->m_itemContainer->removeAllChildren();

    int perPage = m_impl->get()->m_rows * m_impl->get()->m_columns;
    int start = perPage * m_impl->get()->m_page;
    int end = start + perPage;

    float gap = 5.f;

    float width = m_impl->get()->m_columns * (40.f + gap) - gap;
    float height = m_impl->get()->m_rows * (40.f + gap) - gap;

    float widthOffset = 0.f;
    if (m_impl->get()->m_pageCount > 1) {
        widthOffset = m_impl->get()->m_prevButton->getScaledContentWidth() + m_impl->get()->m_nextButton->getScaledContentWidth() + 10.f;
    }

    auto size = CCSize{
        getPaddedSize().width - widthOffset, 
        getPaddedSize().height - 14.f
    };

    float scaleX = size.width / width;
    float scaleY = size.height / height;

    float scale = std::min(scaleX, scaleY);

    m_impl->get()->m_itemContainer->setContentSize({width, height});
    m_impl->get()->m_itemContainer->setScale(scale);

    auto allButtons = getAllButtons();

    for (int i = start; i < end && i < allButtons->count(); i++) {
        auto item = static_cast<CCNode*>(allButtons->objectAtIndex(i));
        item->setScale(1.f);
        auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(item);
        if (button) {
            button->m_baseScale = 1.f;
        }

        m_impl->get()->m_itemContainer->addChild(static_cast<CCNode*>(allButtons->objectAtIndex(i)));
    }

    m_impl->get()->m_itemLayout->setAxisAlignment(m_impl->get()->m_horizontalButtonAlignment);
    m_impl->get()->m_itemLayout->setCrossAxisAlignment(m_impl->get()->m_verticalButtonAlignment);

    m_impl->get()->m_itemContainer->updateLayout();
    m_impl->get()->m_navigationMenu->setPosition({getContentWidth() / 2.f, m_impl->get()->m_itemContainer->boundingBox().getMidY()});

    updateDots();

    m_scrollLayer->m_page = m_impl->get()->m_page;

    Tab::SwitchPageEvent(m_impl->get()->m_tab).send(this, page);
}

void EditorTab::onLeft_(CCObject* sender) {
    if (m_hasCreateItems) {
        EditorSFXManager::sharedState()->playAction(278);
    }
    goToPage_(m_impl->get()->m_page - 1);
}

void EditorTab::onRight_(CCObject* sender) {
    if (m_hasCreateItems) {
        EditorSFXManager::sharedState()->playAction(278);
    }
    goToPage_(m_impl->get()->m_page + 1);
}

void EditorTab::setTab(alpha::editor_tabs::Tab* tab) {
    m_impl->get()->m_tab = tab;
}

alpha::editor_tabs::Tab* EditorTab::getTab() {
    return m_impl->get()->m_tab;
}

bool EditorTab::isItemless() {
    return m_impl->get()->m_itemless;
}

void EditorTab::setAutoScale(bool enabled) {
    m_impl->get()->m_hasAutoScale = enabled;
    updateUI();
}

bool EditorTab::hasAutoScale() {
    return m_impl->get()->m_hasAutoScale;
}

void EditorTab::addChild(CCNode* child, int zOrder, int tag) {
    if (m_impl->get()->m_itemless) {
        m_impl->get()->m_container->addChild(child, zOrder, tag);
        return;
    }
    CCNode::addChild(child, zOrder, tag);
}

void EditorTab::disableSizeOverrideIfExceeded(bool disable) {
    m_impl->get()->m_disableSizeOverrideIfExceeded = disable;
}

bool EditorTab::isDisableSizeOverrideIfExceeded() {
    return m_impl->get()->m_disableSizeOverrideIfExceeded;
}

void EditorTab::addItem(cocos2d::CCNode* item) {
    addItems(std::span{&item, 1});
}

void EditorTab::removeItem(cocos2d::CCNode* item) {
    removeItems(std::span{&item, 1});
}

void EditorTab::removeItemByID(geode::ZStringView ID) {
    int idx = 0;
    for (auto item : getButtons()->asExt<CCNode>()) {
        if (item->getID() == ID) {
            getButtons()->removeObjectAtIndex(idx);
            reloadAllItems();
            return;
        }
        idx++;
    }
}

void EditorTab::removeItemByTag(int tag) {
    int idx = 0;
    for (auto item : getButtons()->asExt<CCNode>()) {
        if (item->getTag() == tag) {
            getButtons()->removeObjectAtIndex(idx);
            reloadAllItems();
            return;
        }
        idx++;
    }
}

void EditorTab::removeItemByObjectID(int objectID) {
    int idx = 0;
    for (auto item : getButtons()->asExt<CCNode>()) {
        auto cmi = typeinfo_cast<CreateMenuItem*>(item);
        if (cmi && cmi->m_objectID == objectID) {
            getButtons()->removeObjectAtIndex(idx);
            reloadAllItems();
            return;
        }
        idx++;
    }
}

void EditorTab::insertItem(cocos2d::CCNode* item, unsigned int index) {
    insertItems(std::span{&item, 1}, index);
}

void EditorTab::insertItemAfter(cocos2d::CCNode* item, cocos2d::CCNode* after) {
    insertItemsAfter(std::span{&item, 1}, after);
}

void EditorTab::insertItemBefore(cocos2d::CCNode* item, cocos2d::CCNode* before) {
    insertItemsBefore(std::span{&item, 1}, before);
}

void EditorTab::insertItemAfter(CreateMenuItem* item, int objectID) {
    insertItemsAfter(std::span{&item, 1}, objectID);
}

void EditorTab::insertItemBefore(CreateMenuItem* item, int objectID) {
    insertItemsBefore(std::span{&item, 1}, objectID);
}

cocos2d::CCNode* EditorTab::findItemByID(geode::ZStringView ID) {
    for (auto node : getButtons()->asExt<CCNode>()) {
        if (node->getID() == ID) return node;
    }

    return nullptr;
}

cocos2d::CCNode* EditorTab::findItemByTag(int tag) {
    for (auto node : getButtons()->asExt<CCNode>()) {
        if (node->getTag() == tag) return node;
    }

    return nullptr;
}

CreateMenuItem* EditorTab::findItemByObjectID(int objectID) {
    if (!m_hasCreateItems) return nullptr;

    for (auto node : getButtons()->asExt<CreateMenuItem>()) {
        if (node->m_objectID == objectID) return node;
    }

    return nullptr;
}

void EditorTab::setTabAlignment(alpha::editor_tabs::Tab::TabAlignment alignment) {
    m_impl->get()->m_tabAlignment = alignment;
    updateUI();
}

alpha::editor_tabs::Tab::TabAlignment EditorTab::getTabAlignment() {
    return m_impl->get()->m_tabAlignment;
}

void EditorTab::setHorizontalButtonAlignment(geode::AxisAlignment alignment) {
    m_impl->get()->m_horizontalButtonAlignment = alignment;
}

geode::AxisAlignment EditorTab::getHorizontalButtonAlignment() {
    return m_impl->get()->m_horizontalButtonAlignment;
}

void EditorTab::setVerticalButtonAlignment(geode::AxisAlignment alignment) {
    m_impl->get()->m_verticalButtonAlignment = alignment;
}

geode::AxisAlignment EditorTab::getVerticalButtonAlignment() {
    return m_impl->get()->m_verticalButtonAlignment;
}

void EditorTab::setPadding(geode::Padding padding) {
    m_impl->get()->m_padding = padding;
    updateUI();
}

geode::Padding EditorTab::getPadding() {
    return m_impl->get()->m_padding;
}

void EditorTab::setDotScale(float scale) {
    m_impl->get()->m_dotScale = scale;
    for (const auto& dot : m_impl->get()->m_dots) {
        dot->setScale(scale);
    }
}

float EditorTab::getDotScale() {
    return m_impl->get()->m_dotScale;
}

}