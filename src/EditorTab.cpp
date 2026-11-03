#include "EditorTab.hpp"
#include "EditorUI.hpp"
#include "EarlyEditorUI.hpp"
#include "../include/Tab.hpp"
#include "../include/Mode.hpp"
#include "../include/Optional.hpp"
#include "DummyBoom.hpp"

namespace alpha::editor_tabs {

// hack.
EditorTab::EditorTab() : EditButtonBar(ZeroConstructor) {
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
    m_itemless = true;

    init(nullptr, 0, false);
    return true;
}

bool EditorTab::init(CCArray* items, int tab, bool hasCreateItems) {
    auto winSize = CCDirector::get()->getWinSize();

    auto noItems = m_itemless;
    m_itemless = false;

    m_buttonArrayRef = items ? items->shallowCopy() : CCArray::create();
    m_buttonArray = m_buttonArrayRef;

    auto arr = CCArray::create();
    arr->addObject(CCNode::create()); //fixes a bug in vanilla where 0 elements will loop forever
    
    m_scrollLayerRef = DummyBoom::create();
    m_scrollLayer = m_scrollLayerRef;

    m_pagesArrayRef = CCArray::create();
    m_pagesArray = m_pagesArrayRef;

    m_hasCreateItems = hasCreateItems;
    m_tabIndex = tab;

    setAnchorPoint({0.5f, 0.f});
    setPosition({winSize.width / 2.f, 0.f});

    m_navigationMenu = CCMenu::create();
    m_navigationMenu->ignoreAnchorPointForPosition(false);
    m_navigationMenu->setAnchorPoint({0.5f, 0.5f});
    m_navigationMenu->setID("arrows-menu");

    if (!noItems) {
        addChild(m_navigationMenu);
    }

    auto prevArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    prevArrow->setScale(0.6f);
    m_prevButton = CCMenuItemSpriteExtra::create(prevArrow, this, menu_selector(EditButtonBar::onLeft));
    m_prevButton->setID("left-button");

    m_navigationMenu->addChild(m_prevButton);

    auto nextArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    nextArrow->setScale(0.6f);
    nextArrow->setFlipX(true);
    m_nextButton = CCMenuItemSpriteExtra::create(nextArrow, this, menu_selector(EditButtonBar::onRight));
    m_nextButton->setID("right-button");

    m_navigationMenu->addChild(m_nextButton);

    m_dotContainer = CCMenu::create();
    m_dotContainer->ignoreAnchorPointForPosition(false);
    m_dotContainer->setAnchorPoint({0.5f, 0.f});
    m_dotContainer->setID("dots-menu");

    if (!noItems) {
        addChild(m_dotContainer);
    }

    m_itemContainer = CCMenu::create();
    m_itemContainer->ignoreAnchorPointForPosition(false);
    m_itemContainer->setID("button-menu");

    m_itemLayout = RowLayout::create()
        ->setGrowCrossAxis(true)
        ->setCrossAxisOverflow(false)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setCrossAxisAlignment(AxisAlignment::End);
    
    m_itemContainer->setLayout(m_itemLayout);

    if (!noItems) {
        addChild(m_itemContainer);
    }

    m_container = CCNode::create();
    m_container->setAnchorPoint({0.5f, 0.f});
    m_container->setID("container"_spr);

    if (noItems) {
        addChild(m_container);
    }

    m_rows = GameManager::get()->getIntGameVariable(GameVar::EditorButtonRows);
    m_columns = GameManager::get()->getIntGameVariable(GameVar::EditorButtonsPerRow);

    m_itemless = noItems;
    m_initialized = true;

    return true;
}

void EditorTab::updateTabUIEvent() {
    auto winSize = CCDirector::get()->getWinSize();
    auto maxSize = getMaxSize();

    float scaleX = getContentWidth() / maxSize.width;
    float scaleY = getContentHeight() / maxSize.height;

    float scale = std::min(scaleX, scaleY);

    alpha::editor_tabs::Tab::UpdateTabUIEvent(m_tab).send(this);
}

void EditorTab::setContentSize(const CCSize& contentSize) {
    CCNode::setContentSize(contentSize);
    if (!m_initialized) return;

    updateUI();
    alpha::editor_tabs::Tab::UpdateTabUIEvent(m_tab).send(this);
    alpha::editor_tabs::Tab::UpdateTabSizeEvent(m_tab).send(this, contentSize);
}

void EditorTab::setPosition(const CCPoint& position) {
    CCNode::setPosition(position);
    if (!m_initialized) return;

    alpha::editor_tabs::Tab::UpdateTabUIEvent(m_tab).send(this);
}

void EditorTab::setVisible(bool visible) {
    CCNode::setVisible(visible);
    if (!m_initialized) return;
    
    alpha::editor_tabs::Tab::UpdateTabVisibilityEvent(m_tab).send(this, visible);
}

void EditorTab::setItemless(bool itemless) {
    if (m_itemless == itemless) return;

    m_itemless = false;

    if (itemless) {
        m_navigationMenu->removeFromParent();
        m_dotContainer->removeFromParent();
        m_itemContainer->removeFromParent();
        addChild(m_container);
    }
    else {
        m_container->removeFromParent();
        addChild(m_navigationMenu);
        addChild(m_dotContainer);
        addChild(m_itemContainer);
    }

    m_itemless = itemless;

    updateUI();
}

CCArray* EditorTab::getButtons() {
    if (m_tab && m_tab->getMode() && m_tab->getMode()->getID() == alpha::editor_tabs::Edit && m_tab->getID() == "edit") {
        auto ret = modify_cast<ETEditorUI*>(EarlyEditorUI::get())->getEditButtons();
        if (!ret) return CCArray::create();
        return ret;
    }

    return m_buttonArray;
}

CCArray* EditorTab::getAllButtons() {
    if (m_tab && m_tab->getMode() && m_tab->getMode()->getID() == alpha::editor_tabs::Edit && m_tab->getID() == "edit") {
        auto allButtons = m_buttonArray->shallowCopy();

        auto buttons = modify_cast<ETEditorUI*>(EarlyEditorUI::get())->getEditButtons();
        if (buttons) {
            allButtons->addObjectsFromArray(buttons);
        }

        return allButtons;
    }

    return m_buttonArray;
}

void EditorTab::overrideSize(int rows, int columns) {
    m_overrideSize = true;
    m_rows = rows;
    m_columns = columns;

    loadFromItems_(m_buttonArray, columns, rows, true);
}

void EditorTab::overrideSize(bool override, int rows, int columns) {
    m_overrideSize = override;

    if (override) {
        m_rows = rows;
        m_columns = columns;
    }
    else {
        m_rows = GameManager::get()->getIntGameVariable(GameVar::EditorButtonRows);
        m_columns = GameManager::get()->getIntGameVariable(GameVar::EditorButtonsPerRow);
    }

    loadFromItems_(m_buttonArray, m_columns, m_rows, true);
}

int EditorTab::getRows() {
    return m_rows;
}

int EditorTab::getColumns() {
    return m_columns;
}

CCSize EditorTab::getMaxSize() {
    return getFullMaxSize() - CCSize{16.f, 0.f};
}

CCSize EditorTab::getFullMaxSize() {
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
    auto maxSize = getMaxSize();

    if (m_hasAutoScale) {
        float scaleX = getRealWidth() / maxSize.width;
        float scaleY = getContentHeight() / maxSize.height;

        float scale = std::min(scaleX, scaleY);
        m_container->setScale(scale);
    }
    else {
        m_container->setScale(1.f);
    }

    m_container->setPosition(getContentSize() / 2.f);
    
    m_dotContainer->setContentSize({getRealWidth(), 6.f});
    m_dotContainer->setPosition({getContentWidth() / 2.f, 0.75f});

    updatePages();

    goToPage_(m_page);

    float y = 0.f;
    if (m_tabAlignment == alpha::editor_tabs::TabAlignment::Center) {
        m_itemContainer->setAnchorPoint({0.5f, 0.5f});
        y = getContentHeight() / 2.f + m_dotContainer->getContentHeight() / 2.f + m_dotContainer->getPositionY();
    }
    else {
        m_itemContainer->setAnchorPoint({0.5f, 1.f});
        y = getContentHeight() - 5.f;
    }
    
    m_itemContainer->setPosition({getContentWidth() / 2.f, y});
    m_navigationMenu->setPosition({getContentWidth() / 2.f, m_itemContainer->boundingBox().getMidY()});
}

float EditorTab::getRealWidth() {
    return getContentWidth() - 16.f;
}

void EditorTab::updateNavMenu() {
    m_navigationMenu->setContentSize({getRealWidth(), m_itemContainer->getScaledContentHeight()});
    m_navigationMenu->setPosition({getContentWidth() / 2.f, m_itemContainer->boundingBox().getMidY()});

    m_prevButton->setPosition({m_prevButton->getScaledContentWidth() / 2.f, m_navigationMenu->getContentHeight() / 2.f});
    m_nextButton->setPosition({m_navigationMenu->getContentWidth() - m_prevButton->getScaledContentWidth() / 2.f, m_navigationMenu->getContentHeight() / 2.f});
}

void EditorTab::updateDots() {
    for (const auto& dot : m_dots) {
        dot->setColor(m_page == dot->getTag() ? ccColor3B{255, 255, 255} : ccColor3B{125, 125, 125});
    }
}

void EditorTab::updatePages() {    
    m_dots.clear();
    m_dotContainer->removeAllChildren();

    for (int i = 0; i < m_pageCount; i++) {
        auto dot = createDot(i);
        dot->setEnabled(true);
        m_dots.push_back(dot);
        m_dotContainer->addChild(dot);
    }

    m_dotContainer->updateLayout();
    updateDots();

    m_scrollLayer->m_pages->removeAllObjects();
    m_scrollLayer->m_page = m_page;

    for (int i = 0; i < m_pageCount; i++) {
        m_scrollLayer->m_pages->addObject(CCNode::create());
    }
}

CCMenuItemSpriteExtra* EditorTab::createDot(int page) {
    auto ret = CCMenuItemExt::createSpriteExtraWithFilename("smallDot.png", 0.5f, [this, page] (CCMenuItemSpriteExtra* sender) {
        if (m_page != page && m_hasCreateItems) {
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

void EditorTab::reloadItems() {
    loadFromItems_(m_buttonArray, m_columns, m_rows, true);
}

void EditorTab::updateItemSize(int rows, int columns) {
    auto allButtons = getAllButtons();

    auto noOverride = m_disableSizeOverrideIfExceeded && allButtons->count() > (m_rows * m_columns);

    if (!m_overrideSize || noOverride) {
        m_rows = rows;
        m_columns = columns;
    }

    reloadItems();
}

void EditorTab::loadFromItems_(CCArray* items, int columns, int rows, bool preserve) {
    if (items != m_buttonArray) {
        m_buttonArrayRef = items ? items->shallowCopy() : CCArray::create();
        m_buttonArray = m_buttonArrayRef;
    }

    auto allButtons = getAllButtons();

    auto noOverride = m_disableSizeOverrideIfExceeded && allButtons->count() > (m_rows * m_columns);

    if (!m_overrideSize || noOverride) {
        m_rows = rows;
        m_columns = columns;
    }

    if (m_itemless) {
        m_pageCount = 1;
    }
    else {
        auto perPage = m_rows * m_columns;
        m_pageCount = (allButtons->count() + perPage - 1) / perPage;
    }

    if (!preserve) {
        m_page = 0;
    }

    if (m_hasCreateItems) {
        int pageIdx = 0;
        int idx = 0;

        for (auto obj : allButtons->asExt<CreateMenuItem>()) {
            obj->m_pageIndex = pageIdx;
            obj->m_tabIndex = m_tabIndex;

            obj->setID(fmt::format("object-{}-button", obj->m_objectID));

            if (idx % m_pageCount == 0) {
                pageIdx++;
            }
        }
    }

    if (m_itemless) {
        if (!m_tab) return;
        alpha::editor_tabs::Tab::TabLoadedEvent(m_tab).send(this);
        return;
    }

    m_dotContainer->setContentSize({getRealWidth(), 6.f});
    m_dotContainer->setLayout(SimpleRowLayout::create()
        ->setGap(12.8f)
        ->setMainAxisScaling(AxisScaling::ScaleDownGaps)
        ->setCrossAxisScaling(AxisScaling::Fit)
    );

    m_dotContainer->updateLayout();

    updatePages();
    goToPage_(m_page);

    if (!m_tab) return;
    alpha::editor_tabs::Tab::TabLoadedEvent(m_tab).send(this);
}

void EditorTab::goToPage_(int page) {
    if (m_itemless || m_pageCount == 0) return;

    m_page = page;

    if (m_page < 0) {
        m_page = std::max(m_pageCount - 1, 0);
    }
    if (m_page >= m_pageCount) {
        m_page = 0;
    }

    m_navigationMenu->setVisible(m_pageCount > 1);
    m_itemContainer->removeAllChildren();

    int perPage = m_rows * m_columns;
    int start = perPage * m_page;
    int end = start + perPage;

    float gap = 5.f;

    float width = m_columns * (40.f + gap) - gap;
    float height = m_rows * (40.f + gap) - gap;

    float widthOffset = 0.f;
    if (m_pageCount > 1) {
        widthOffset = m_prevButton->getScaledContentWidth() + m_nextButton->getScaledContentWidth() + 10.f;
    }

    auto size = CCSize{
        getRealWidth() - widthOffset, 
        getContentHeight() - 14.f
    };

    float scaleX = size.width / width;
    float scaleY = size.height / height;

    float scale = std::min(scaleX, scaleY);

    m_itemContainer->setContentSize({width, height});
    m_itemContainer->setScale(scale);

    auto allButtons = getAllButtons();

    for (int i = start; i < end && i < allButtons->count(); i++) {
        auto item = static_cast<CCNode*>(allButtons->objectAtIndex(i));
        item->setScale(1.f);
        auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(item);
        if (button) {
            button->m_baseScale = 1.f;
        }

        m_itemContainer->addChild(static_cast<CCNode*>(allButtons->objectAtIndex(i)));
    }

    m_itemLayout->setAxisAlignment(m_horizontalButtonAlignment);
    m_itemLayout->setCrossAxisAlignment(m_verticalButtonAlignment);

    m_itemContainer->updateLayout();

    updateDots();
    updateNavMenu();

    m_scrollLayer->m_page = m_page;

    Tab::SwitchPageEvent(m_tab).send(this, page);
}

void EditorTab::onLeft_(CCObject* sender) {
    if (m_hasCreateItems) {
        EditorSFXManager::sharedState()->playAction(278);
    }
    goToPage_(m_page - 1);
}

void EditorTab::onRight_(CCObject* sender) {
    if (m_hasCreateItems) {
        EditorSFXManager::sharedState()->playAction(278);
    }
    goToPage_(m_page + 1);
}

void EditorTab::setTab(alpha::editor_tabs::Tab* tab) {
    m_tab = tab;
}

alpha::editor_tabs::Tab* EditorTab::getTab() {
    return m_tab;
}

bool EditorTab::isItemless() {
    return m_itemless;
}

void EditorTab::setAutoScale(bool enabled) {
    m_hasAutoScale = enabled;
    updateUI();
}

bool EditorTab::hasAutoScale() {
    return m_hasAutoScale;
}

void EditorTab::addChild(CCNode* child, int zOrder, int tag) {
    if (m_itemless) {
        m_container->addChild(child, zOrder, tag);
        return;
    }
    CCNode::addChild(child, zOrder, tag);
}

void EditorTab::disableSizeOverrideIfExceeded(bool disable) {
    m_disableSizeOverrideIfExceeded = disable;
}

bool EditorTab::isDisableSizeOverrideIfExceeded() {
    return m_disableSizeOverrideIfExceeded;
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
            reloadItems();
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
            reloadItems();
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
            reloadItems();
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

void EditorTab::setTabAlignment(alpha::editor_tabs::TabAlignment alignment) {
    m_tabAlignment = alignment;
    updateUI();
}

alpha::editor_tabs::TabAlignment EditorTab::getTabAlignment() {
    return m_tabAlignment;
}

void EditorTab::setHorizontalButtonAlignment(geode::AxisAlignment alignment) {
    m_horizontalButtonAlignment = alignment;
}

geode::AxisAlignment EditorTab::getHorizontalButtonAlignment() {
    return m_horizontalButtonAlignment;
}

void EditorTab::setVerticalButtonAlignment(geode::AxisAlignment alignment) {
    m_verticalButtonAlignment = alignment;
}

geode::AxisAlignment EditorTab::getVerticalButtonAlignment() {
    return m_verticalButtonAlignment;
}

}