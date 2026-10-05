#include "EditorTab.hpp"
#include "EditorUI.hpp"
#include "../include/Tab.hpp"

namespace alpha::editor_tabs {

// hack.
EditorTab::EditorTab() : EditButtonBar(ZeroConstructor) {
    m_uReference = 1;

    auto director = CCDirector::sharedDirector();
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
    
    m_scrollLayerRef = BoomScrollLayer::create(arr, 0, true);
    m_scrollLayer = m_scrollLayerRef;

    m_pagesArrayRef = CCArray::create();
    m_pagesArray = m_pagesArrayRef;

    m_hasCreateItems = hasCreateItems;
    m_tabIndex = tab;

    setAnchorPoint({0.5f, 0.f});
    setPosition({winSize.width / 2.f, 0.f});

    m_navigationMenu = CCMenu::create();
    m_navigationMenu->ignoreAnchorPointForPosition(false);
    m_navigationMenu->setAnchorPoint({0.5f, 1.f});
    m_navigationMenu->setID("navigation-menu"_spr);

    if (!noItems) {
        addChild(m_navigationMenu);
    }

    auto prevArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    prevArrow->setScale(0.6f);
    m_prevButton = CCMenuItemSpriteExtra::create(prevArrow, this, menu_selector(EditButtonBar::onLeft));
    m_prevButton->setID("prev-button"_spr);

    m_navigationMenu->addChild(m_prevButton);

    auto nextArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    nextArrow->setScale(0.6f);
    nextArrow->setFlipX(true);
    m_nextButton = CCMenuItemSpriteExtra::create(nextArrow, this, menu_selector(EditButtonBar::onRight));
    m_nextButton->setID("next-button"_spr);

    m_navigationMenu->addChild(m_nextButton);

    m_dotContainer = CCMenu::create();
    m_dotContainer->ignoreAnchorPointForPosition(false);
    m_dotContainer->setAnchorPoint({0.5f, 0.f});
    m_dotContainer->setID("dot-container"_spr);
    m_dotContainer->setLayout(SimpleRowLayout::create()
        ->setGap(15.f)
    );

    if (!noItems) {
        addChild(m_dotContainer);
    }

    m_itemContainer = CCMenu::create();
    m_itemContainer->ignoreAnchorPointForPosition(false);
    m_itemContainer->setAnchorPoint({0.5f, 1.f});
    m_itemContainer->setID("item-container"_spr);
    m_itemContainer->setLayout(RowLayout::create()
        ->setGrowCrossAxis(true)
        ->setCrossAxisOverflow(false)
        ->setAutoScale(false)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setCrossAxisAlignment(AxisAlignment::End)
    );

    if (!noItems) {
        addChild(m_itemContainer);
    }

    m_container = CCNode::create();
    m_container->setAnchorPoint({0.5f, 0.f});
    m_container->setID("container"_spr);

    if (noItems) {
        addChild(m_container);
    }

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

    alpha::editor_tabs::tab::UpdateTabUIEvent(m_tab).send(this);
}

void EditorTab::setContentSize(const CCSize& contentSize) {
    CCNode::setContentSize(contentSize);

    if (!m_initialized) return;

    updateUI();
    alpha::editor_tabs::tab::UpdateTabUIEvent(m_tab).send(this);
}

void EditorTab::setPosition(const CCPoint& position) {
    CCNode::setPosition(position);

    alpha::editor_tabs::tab::UpdateTabUIEvent(m_tab).send(this);
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
    auto editor = ETEditorUI::get();

    if (!editor) return {};

    auto winSize = CCDirector::get()->getWinSize();

    auto spacerLeft = editor->getChildByID("spacer-line-left");
    auto spacerRight = editor->getChildByID("spacer-line-right");
    
    auto rightSpace = winSize.width - spacerRight->getPositionX() + 3.f;
    auto leftSpace = spacerLeft->getPositionX() + 3.f;

    return CCSize{winSize.width - rightSpace - leftSpace, 90.f};
}

void EditorTab::updateUI() {
    auto winSize = CCDirector::get()->getWinSize();
    auto maxSize = getMaxSize();

    if (m_hasAutoScale) {
        float scaleX = getContentWidth() / maxSize.width;
        float scaleY = getContentHeight() / maxSize.height;

        float scale = std::min(scaleX, scaleY);
        m_container->setScale(scale);
    }
    else {
        m_container->setScale(1.f);
    }

    m_container->setPosition(getContentSize() / 2.f);
    
    m_dotContainer->setContentSize({getContentWidth(), 6.f});
    m_dotContainer->setPosition({getContentWidth() / 2.f, 0.f});

    updatePages();

    goToPage_(m_page);
    
    m_itemContainer->setPosition({getContentWidth() / 2.f, getContentHeight() - 5.f});
    m_navigationMenu->setPosition({getContentWidth() / 2.f, m_itemContainer->getPositionY()});
}

void EditorTab::updateNavMenu() {
    m_navigationMenu->setContentSize({getContentWidth(), m_itemContainer->getScaledContentHeight()});
    m_navigationMenu->setPosition({getContentWidth() / 2.f, m_itemContainer->getPositionY()});

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
        dot->setEnabled(m_pageDotsClickable);
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
        goToPage(page);
    });
    ret->setTag(page);
    ret->setZOrder(page);
    ret->setCascadeColorEnabled(true);
    ret->setCascadeOpacityEnabled(true);
    ret->setID(fmt::format("page-{}-dot"_spr, page + 1));

    return ret;
}

void EditorTab::loadFromItems_(CCArray* items, int columns, int rows, bool preserve) {
    m_buttonArrayRef = items ? items->shallowCopy() : CCArray::create();
    m_buttonArray = m_buttonArrayRef;

    if (!m_overrideSize) {
        m_rows = rows;
        m_columns = columns;
    }

    if (m_itemless) {
        m_pageCount = 1;
    }
    else {
        m_pageCount = std::ceil(m_buttonArray->count() / (m_rows * m_columns)) + 1;
    }

    if (!preserve) {
        m_page = 0;
    }

    if (m_itemless) {
        if (!m_tab) return;
        alpha::editor_tabs::tab::TabLoadedEvent(m_tab).send(this);
        return;
    }

    updatePages();
    goToPage_(m_page);

    if (!m_tab) return;
    alpha::editor_tabs::tab::TabLoadedEvent(m_tab).send(this);
}

void EditorTab::goToPage_(int page) {
    if (m_itemless) return;

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

    auto size = CCSize{
        getContentWidth() - m_prevButton->getScaledContentWidth() - m_nextButton->getScaledContentWidth() - 10.f, 
        getContentHeight() - m_dotContainer->getScaledContentHeight() - 10.f
    };

    float scaleX = size.width / width;
    float scaleY = size.height / height;

    float scale = std::min(scaleX, scaleY);

    m_itemContainer->setContentSize({width, height});
    m_itemContainer->setScale(scale);

    for (int i = start; i < end && i < m_buttonArray->count(); i++) {
        auto item = static_cast<CCNode*>(m_buttonArray->objectAtIndex(i));
        item->setScale(1.f);
        auto button = typeinfo_cast<CCMenuItemSpriteExtra*>(item);
        if (button) {
            button->m_baseScale = 1.f;
        }

        m_itemContainer->addChild(static_cast<CCNode*>(m_buttonArray->objectAtIndex(i)));
    }

    m_itemContainer->updateLayout();

    updateDots();
    updateNavMenu();

    m_scrollLayer->m_page = m_page;

    tab::SwitchPageEvent(m_tab).send(this, page);
}

void EditorTab::onLeft_(CCObject* sender) {
    goToPage_(m_page - 1);
}

void EditorTab::onRight_(CCObject* sender) {
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

void EditorTab::setPageDotsClickable(bool clickable) {
    m_pageDotsClickable = clickable;
    updateDots();
}

bool EditorTab::arePageDotsClickable() {
    return m_pageDotsClickable;
}

}