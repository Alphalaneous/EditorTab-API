#include "TabMenu.hpp"

namespace alpha::editor_tabs {

class MenuUpdate : public geode::Layout {
public:
    static MenuUpdate* create() {
        auto ret = new MenuUpdate();
        ret->autorelease();
        return ret;
    }

    void apply(cocos2d::CCNode* on) override {
        auto tabMenu = static_cast<TabMenu*>(on);

        auto mainMenu = tabMenu->getMainMenu();
        if (!mainMenu) return;

        mainMenu->updateLayout();
        tabMenu->setContentHeight(mainMenu->getContentHeight());
        tabMenu->updateUI();
    }

    cocos2d::CCSize getSizeHint(cocos2d::CCNode* on) const override {
        return {};
    }
};

TabMenu* TabMenu::create() {
    auto ret = new TabMenu();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool TabMenu::init() {
    if (!CCMenu::init()) return false;
    setAnchorPoint({0.5f, 0.5f});

    auto menu = CCMenu::create();
    menu->ignoreAnchorPointForPosition(false);
    menu->setAnchorPoint({0.5f, 0.5f});
    menu->setID("main-menu"_spr);
    menu->setLayout(SimpleRowLayout::create()
        ->setGap(2.f)
        ->setMainAxisScaling(AxisScaling::Fit)
        ->setCrossAxisScaling(AxisScaling::Fit)
    );
    
    auto navMenu = CCMenu::create();
    navMenu->ignoreAnchorPointForPosition(false);
    navMenu->setAnchorPoint({0.5f, 0.5f});
    navMenu->setID("navigation-menu"_spr);
    navMenu->setLayout(SimpleRowLayout::create()
        ->setMainAxisAlignment(MainAxisAlignment::Between)
        ->setPadding({5.f, 0.f, 5.f, 0.f})
    );

    auto prevArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    prevArrow->setScale(0.4f);
    m_prevButton = CCMenuItemSpriteExtra::create(prevArrow, this, menu_selector(TabMenu::onLeft));
    m_prevButton->setID("prev-button"_spr);

    navMenu->addChild(m_prevButton);

    auto nextArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    nextArrow->setScale(0.4f);
    nextArrow->setFlipX(true);
    m_nextButton = CCMenuItemSpriteExtra::create(nextArrow, this, menu_selector(TabMenu::onRight));
    m_nextButton->setID("next-button"_spr);

    navMenu->addChild(m_nextButton);

    setLayout(MenuUpdate::create());

    addChild(menu);
    addChild(navMenu);

    m_menu = menu;
    m_navigationMenu = navMenu;

    return true;
}

void TabMenu::onLeft(CCObject* sender) {
    m_page--;
    goToPage();
}

void TabMenu::onRight(CCObject* sender) {
    m_page++;
    goToPage();
}

void TabMenu::goToPage() {
    if (m_page < 0) {
        m_page = m_pages.size() - 1;
    }
    if (m_page >= m_pages.size()) {
        m_page = 0;
    }
    
    int idx = 0;
    for (const auto& page : m_pages) {
        for (auto node : page) {
            node->setVisible(idx == m_page);
        }

        idx++;
    }

    m_menu->updateLayout();
}

CCMenu* TabMenu::getMainMenu() {
    return m_menu;
}

void TabMenu::addChild(CCNode* child, int zOrder, int tag) {
    if (!m_menu) {
        CCMenu::addChild(child, zOrder, tag);
        return;
    }
    m_menu->addChild(child, zOrder, tag);
}

void TabMenu::setContentSize(const CCSize& contentSize) {
    CCMenu::setContentSize(contentSize);
    updateUI();
}

void TabMenu::updateUI() {
    auto size = getContentSize();

    if (m_menu && m_navigationMenu) {
        m_pages.clear();

        m_menu->setPosition(size / 2.f);

        float availSpace = size.width;
        float navSpace = 20.f + m_prevButton->getScaledContentWidth() + m_nextButton->getScaledContentWidth();

        auto layout = static_cast<SimpleAxisLayout*>(m_menu->getLayout());
        layout->ignoreInvisibleChildren(false);
        m_menu->updateLayout();

        float menuWidth = m_menu->getContentWidth();

        layout->ignoreInvisibleChildren(true);

        if (size.width < menuWidth) {
            availSpace = size.width - navSpace;
        }

        float width = 0.f;
        int pageIdx = 0;

        m_pages.push_back(std::vector<CCNode*>());

        for (auto child : m_menu->getChildrenExt()) {
            auto childWidth = child->getScaledContentWidth() + 2.f;
            width += childWidth;

            if ((width - 2.f) > availSpace) {
                width = childWidth;
                m_pages.push_back(std::vector<CCNode*>());
                pageIdx++;
            }

            m_pages[pageIdx].push_back(child);
        }

        m_navigationMenu->setVisible(m_pages.size() > 1);
        m_navigationMenu->setContentSize(size);
        m_navigationMenu->setPosition(size / 2.f);
        m_navigationMenu->updateLayout();

        goToPage();
    }
}

}