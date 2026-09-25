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

    void setContentSize(const CCSize& contentSize) override;
    void setItemless();

    void setTab(Tab* tab);
    Tab* getTab();

    bool hasNoItems();

    void addChild(CCNode* child, int zOrder, int tag) override;

    void setPageDotsClickable(bool clickable);
    bool arePageDotsClickable();

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
    CCSize getMinSize();

    CCMenuItemSpriteExtra* createDot(int page);

    Ref<BoomScrollLayer> m_scrollLayerRef;
    Ref<CCArray> m_buttonArrayRef;
    Ref<CCArray> m_pagesArrayRef;
    CCMenu* m_itemContainer;
    CCMenu* m_dotContainer;
    CCMenu* m_navigationMenu;
    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;
    std::vector<CCMenuItemSpriteExtra*> m_dots;
    Ref<CCNode> m_container;

    Tab* m_tab;

    bool m_initialized = false;

    bool m_noItems = false;
    bool m_overrideSize = false;

    int m_rows = 0;
    int m_columns = 0;

    int m_page = 0;
    int m_pageCount = 0;

    bool m_pageDotsClickable;

    friend class ::ETEditButtonBar;
};

}