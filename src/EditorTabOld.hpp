/*#pragma once

#include <Geode/Geode.hpp>
#include "../include/Tab.hpp"

using namespace geode::prelude;

class ETEditButtonBar;

namespace alpha::editor_tabs {

class EditorTab : public EditButtonBar {
public:
    using CCNode::addChild;

    EditorTab();

    static EditorTab* create(CCArray* items, int tab, bool hasCreateItems);
    static EditorTab* create(std::span<Ref<CCNode>> items, int tab, bool hasCreateItems);
    static EditorTab* create(std::span<CCNode*> items, int tab, bool hasCreateItems);
    static EditorTab* create();

    void overrideSize(bool override, int rows = 0, int columns = 0);
    void overrideSize(int rows, int columns);

    int getRows();
    int getColumns();

    void updateTabUIEvent();

    void setContentSize(const CCSize& contentSize) override;
    void setPosition(const CCPoint& position) override;
    void setVisible(bool visible) override;

    void setTab(Tab* tab);
    Tab* getTab();

    void setItemless(bool itemless);
    bool isItemless();

    void setAutoScale(bool enabled);
    bool hasAutoScale();

    void disableSizeOverrideIfExceeded(bool disable);
    bool isDisableSizeOverrideIfExceeded();

    CCArray* getButtons();
    CCArray* getAllButtons();

    template<class T>
    requires std::convertible_to<T, CCNode*>
    void addItems(std::span<T> items) {
        insertItems(items, getButtons()->count());
    }

    template<class T>
    requires std::convertible_to<T, CCNode*>
    void removeItems(std::span<T> items) {
        if (items.empty()) return;

        for (CCNode* item : items) {
            getButtons()->removeObject(item);
        }

        reloadItems();
    }

    template<class T>
    requires std::convertible_to<T, CCNode*>
    void insertItems(std::span<T> items, unsigned int index) {
        if (items.empty()) return;

        index = std::min(index, getButtons()->count());

        bool hadNonCreateItem = false;
        bool inserted = false;

        for (CCNode* item : items) {
            if (!item) continue;

            if (m_hasCreateItems && !typeinfo_cast<CreateMenuItem*>(item)) {
                hadNonCreateItem = true;
                continue;
            }

            inserted = true;
            getButtons()->insertObject(item, index);
            index++;
        }

        if (hadNonCreateItem) {
            log::error("Cannot put non CreateMenuItems in a tab that requires them");
        }

        if (inserted) {
            reloadItems();
        }
    }

    template<class T>
    requires std::convertible_to<T, CCNode*>
    void insertItemsAfter(std::span<T> items, cocos2d::CCNode* after) {
        unsigned int idx = getButtons()->indexOfObject(after);
        if (idx == UINT_MAX) {
            idx = getButtons()->count();
        } 
        else {
            idx++;
        }

        insertItems(items, idx);
    }

    template<class T>
    requires std::convertible_to<T, CCNode*>
    void insertItemsBefore(std::span<T> items, cocos2d::CCNode* before) {
        unsigned int idx = getButtons()->indexOfObject(before);
        if (idx == UINT_MAX) {
            idx = getButtons()->count();
        }

        insertItems(items, idx);
    }

    template<class T>
    requires std::convertible_to<T, CreateMenuItem*>
    void insertItemsAfter(std::span<T> items, int objectID) {
        unsigned int idx = getButtons()->count();
        for (auto node : getButtons()->asExt<CreateMenuItem>()) {
            if (node->m_objectID == objectID) {
                idx = getButtons()->indexOfObject(node) + 1;
                break;
            }
        }

        insertItems(items, idx);
    }

    template<class T>
    requires std::convertible_to<T, CreateMenuItem*>
    void insertItemsBefore(std::span<T> items, int objectID) {
        unsigned int idx = getButtons()->count();
        for (auto node : getButtons()->asExt<CreateMenuItem>()) {
            if (node->m_objectID == objectID) {
                idx = getButtons()->indexOfObject(node);
                break;
            }
        }

        insertItems(items, idx);
    }

    void addItem(cocos2d::CCNode* item);

    void removeItem(cocos2d::CCNode* item);
    void removeItemByID(geode::ZStringView ID);
    void removeItemByTag(int tag);
    void removeItemByObjectID(int objectID);

    void insertItem(cocos2d::CCNode* item, unsigned int index);
    void insertItemAfter(cocos2d::CCNode* item, cocos2d::CCNode* after);
    void insertItemBefore(cocos2d::CCNode* item, cocos2d::CCNode* before);
    void insertItemAfter(CreateMenuItem* item, int objectID);
    void insertItemBefore(CreateMenuItem* item, int objectID);

    cocos2d::CCNode* findItemByID(geode::ZStringView ID);
    cocos2d::CCNode* findItemByTag(int tag);
    CreateMenuItem* findItemByObjectID(int objectID);

    void setTabAlignment(alpha::editor_tabs::Tab::TabAlignment alignment);
    alpha::editor_tabs::Tab::TabAlignment getTabAlignment();

    void setHorizontalButtonAlignment(geode::AxisAlignment alignment);
    geode::AxisAlignment getHorizontalButtonAlignment();

    void setVerticalButtonAlignment(geode::AxisAlignment alignment);
    geode::AxisAlignment getVerticalButtonAlignment();

    void addChild(CCNode* child, int zOrder, int tag) override;

    void reloadItems();
    void updateItemSize(int rows, int columns);

    static CCSize getMaxSize();
    static CCSize getFullMaxSize();

protected:
    bool init(CCArray* items, int tab, bool hasCreateItems);
    bool init() override;

    void updateNavMenu();
    void updateDots();
    void updateUI();
    void updatePages();
    float getRealWidth();

    void goToPage_(int page);
    void onLeft_(CCObject* sender);
    void onRight_(CCObject* sender);
    void loadFromItems_(CCArray* items, int columns, int rows, bool preserve);

    CCMenuItemSpriteExtra* createDot(int page);

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

    int m_rows = 0;
    int m_columns = 0;

    int m_page = 0;
    int m_pageCount = 0;

    friend class ::ETEditButtonBar;
};

}*/