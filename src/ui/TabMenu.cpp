#define GEODE_DEFINE_EVENT_EXPORTS
#include "../../include/ui/TabMenu.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

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

struct TabMenu::Impl : public ZImpl<TabMenu::Impl> {
    CCMenu* m_menu = nullptr;
    CCMenu* m_navigationMenu = nullptr;

    CCMenuItemSpriteExtra* m_prevButton = nullptr;
    CCMenuItemSpriteExtra* m_nextButton = nullptr;

    std::vector<std::vector<CCNode*>> m_pages{};
    int m_page = 0;
};

void TabMenu::construct() {
    m_impl = std::make_unique<Impl>();
}

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
    m_impl->get()->m_prevButton = CCMenuItemSpriteExtra::create(prevArrow, this, menu_selector(TabMenu::onLeft));
    m_impl->get()->m_prevButton->setID("prev-button"_spr);

    navMenu->addChild(m_impl->get()->m_prevButton);

    auto nextArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    nextArrow->setScale(0.4f);
    nextArrow->setFlipX(true);
    m_impl->get()->m_nextButton = CCMenuItemSpriteExtra::create(nextArrow, this, menu_selector(TabMenu::onRight));
    m_impl->get()->m_nextButton->setID("next-button"_spr);

    navMenu->addChild(m_impl->get()->m_nextButton);

    setLayout(MenuUpdate::create());

    addChild(menu);
    addChild(navMenu);

    m_impl->get()->m_menu = menu;
    m_impl->get()->m_navigationMenu = navMenu;

    return true;
}

void TabMenu::onLeft(CCObject* sender) {
    m_impl->get()->m_page--;
    updatePage();
}

void TabMenu::onRight(CCObject* sender) {
    m_impl->get()->m_page++;
    updatePage();
}

void TabMenu::goToPage(unsigned int page) {
    m_impl->get()->m_page = page;
    updatePage();
}

void TabMenu::updatePage() {
    if (m_impl->get()->m_page < 0) {
        m_impl->get()->m_page = m_impl->get()->m_pages.size() - 1;
    }
    if (m_impl->get()->m_page >= m_impl->get()->m_pages.size()) {
        m_impl->get()->m_page = 0;
    }
    
    int idx = 0;
    for (const auto& page : m_impl->get()->m_pages) {
        for (auto node : page) {
            node->setVisible(idx == m_impl->get()->m_page);
        }

        idx++;
    }

    m_impl->get()->m_menu->updateLayout();
}

unsigned int TabMenu::getPage() {
    return m_impl->get()->m_page;
}

unsigned int TabMenu::getPageCount() {
    return m_impl->get()->m_pages.size();
}

CCMenu* TabMenu::getMainMenu() {
    return m_impl->get()->m_menu;
}

void TabMenu::addChild(CCNode* child, int zOrder, int tag) {
    if (!m_impl->get()->m_menu) {
        CCMenu::addChild(child, zOrder, tag);
        return;
    }
    m_impl->get()->m_menu->addChild(child, zOrder, tag);
}

void TabMenu::setContentSize(const CCSize& contentSize) {
    CCMenu::setContentSize(contentSize);
    updateUI();
}

void TabMenu::updateUI() {
    auto size = getContentSize();

    if (m_impl->get()->m_menu && m_impl->get()->m_navigationMenu) {
        m_impl->get()->m_pages.clear();

        m_impl->get()->m_menu->setPosition(size / 2.f);

        float availSpace = size.width;
        float navSpace = 20.f + m_impl->get()->m_prevButton->getScaledContentWidth() + m_impl->get()->m_nextButton->getScaledContentWidth();

        auto layout = static_cast<SimpleAxisLayout*>(m_impl->get()->m_menu->getLayout());
        layout->ignoreInvisibleChildren(false);
        m_impl->get()->m_menu->updateLayout();

        float menuWidth = m_impl->get()->m_menu->getContentWidth();

        layout->ignoreInvisibleChildren(true);

        if (size.width < menuWidth) {
            availSpace = size.width - navSpace;
        }

        float width = 0.f;
        int pageIdx = 0;

        m_impl->get()->m_pages.push_back(std::vector<CCNode*>());

        for (auto child : m_impl->get()->m_menu->getChildrenExt()) {
            auto childWidth = child->getScaledContentWidth() + 2.f;
            width += childWidth;

            if ((width - 2.f) > availSpace) {
                width = childWidth;
                m_impl->get()->m_pages.push_back(std::vector<CCNode*>());
                pageIdx++;
            }

            m_impl->get()->m_pages[pageIdx].push_back(child);
        }

        m_impl->get()->m_navigationMenu->setVisible(m_impl->get()->m_pages.size() > 1);
        m_impl->get()->m_navigationMenu->setContentSize(size);
        m_impl->get()->m_navigationMenu->setPosition(size / 2.f);
        m_impl->get()->m_navigationMenu->updateLayout();

        updatePage();
    }
}

}