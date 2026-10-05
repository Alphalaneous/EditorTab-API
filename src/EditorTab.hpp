#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ETEditButtonBar;

namespace alpha::editor_tabs {

class Tab;

class EditorTab : public EditButtonBar {
public:
    using CCNode::addChild;

    EditorTab();

    static EditorTab* create(CCArray* items, int tab, bool hasCreateItems);
    static EditorTab* create(std::span<Ref<CCNode>> items, int tab, bool hasCreateItems);
    static EditorTab* create();

    void overrideSize(bool override, int rows = 0, int columns = 0);
    void overrideSize(int rows, int columns);

    int getRows();
    int getColumns();

    void updateTabUIEvent();

    void setContentSize(const CCSize& contentSize) override;
    void setPosition(const CCPoint& position) override;

    void setTab(Tab* tab);
    Tab* getTab();

    void setItemless(bool itemless);
    bool isItemless();

    void setAutoScale(bool enabled);
    bool hasAutoScale();

    void addChild(CCNode* child, int zOrder, int tag) override;

    void setPageDotsClickable(bool clickable);
    bool arePageDotsClickable();

    static CCSize getMaxSize();

protected:
    bool init(CCArray* items, int tab, bool hasCreateItems);
    bool init() override;

    void updateNavMenu();
    void updateDots();
    void updateUI();
    void updatePages();

    void loadFromItems_(CCArray* items, int columns, int rows, bool preserve);
    void goToPage_(int page);
    void onLeft_(CCObject* sender);
    void onRight_(CCObject* sender);

    CCMenuItemSpriteExtra* createDot(int page);

    Ref<BoomScrollLayer> m_scrollLayerRef;
    Ref<CCArray> m_buttonArrayRef;
    Ref<CCArray> m_pagesArrayRef;
    Ref<CCMenu> m_itemContainer;
    Ref<CCMenu> m_dotContainer;
    Ref<CCMenu> m_navigationMenu;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    std::vector<Ref<CCMenuItemSpriteExtra>> m_dots;
    Ref<CCNode> m_container;

    Tab* m_tab;

    bool m_initialized = false;

    bool m_itemless = false;
    bool m_overrideSize = false;
    bool m_hasAutoScale = true;

    int m_rows = 0;
    int m_columns = 0;

    int m_page = 0;
    int m_pageCount = 0;

    bool m_pageDotsClickable;

    friend class ::ETEditButtonBar;
};

}