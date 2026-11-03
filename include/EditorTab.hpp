#pragma once

#include <Geode/loader/Dispatch.hpp>
#include <Geode/binding/EditButtonBar.hpp>
#include "Tab.hpp"
#include "ZImpl.hpp"

#define MY_MOD_ID "alphalaneous.editortab_api"

class ETEditButtonBar;

namespace alpha::editor_tabs {

class EditorTab : public EditButtonBar {
public:
    using CCNode::addChild;

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, construct, (), (this));

    EditorTab() : EditButtonBar(geode::ZeroConstructor) {
        construct();
    }

    GEODE_EVENT_EXPORT_MEMBER_STATIC(EditorTab*, create, (cocos2d::CCArray* items, int tab, bool hasCreateItems), (items, tab, hasCreateItems));
    GEODE_EVENT_EXPORT_MEMBER_STATIC(EditorTab*, create, (std::span<geode::Ref<cocos2d::CCNode>> items, int tab, bool hasCreateItems), (items, tab, hasCreateItems));
    GEODE_EVENT_EXPORT_MEMBER_STATIC(EditorTab*, create, (std::span<cocos2d::CCNode*> items, int tab, bool hasCreateItems), (items, tab, hasCreateItems));
    GEODE_EVENT_EXPORT_MEMBER_STATIC(EditorTab*, create, (), ());

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, overrideSize, (bool override, int rows = 0, int columns = 0), (this, override, rows, columns));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, overrideSize, (int rows, int columns), (this, rows, columns));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, int, getRows, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, int, getColumns, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, updateTabUIEvent, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setContentSize, (const cocos2d::CCSize& contentSize), (this, contentSize));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setPosition, (const cocos2d::CCPoint& position), (this, position));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setVisible, (bool visible), (this, visible));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, addChild, (CCNode* child, int zOrder, int tag), (this, child, zOrder, tag));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setTab, (Tab* tab), (this, tab));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, Tab*, getTab, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setItemless, (bool itemless), (this, itemless));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, bool, isItemless, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setAutoScale, (bool enabled), (this, enabled));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, bool, hasAutoScale, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, disableSizeOverrideIfExceeded, (bool disable), (this, disable));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, bool, isDisableSizeOverrideIfExceeded, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, cocos2d::CCArray*, getButtons, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, cocos2d::CCArray*, getAllButtons, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, addItem, (cocos2d::CCNode* item), (this, item));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, removeItem, (cocos2d::CCNode* item), (this, item));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, removeItemByID, (geode::ZStringView ID), (this, ID));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, removeItemByTag, (int tag), (this, tag));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, removeItemByObjectID, (int objectID), (this, objectID));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, insertItem, (cocos2d::CCNode* item, unsigned int index), (this, item, index));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, insertItemAfter, (cocos2d::CCNode* item, cocos2d::CCNode* after), (this, item, after));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, insertItemBefore, (cocos2d::CCNode* item, cocos2d::CCNode* before), (this, item, before));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, insertItemAfter, (CreateMenuItem* item, int objectID), (this, item, objectID));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, insertItemBefore, (CreateMenuItem* item, int objectID), (this, item, objectID));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, cocos2d::CCNode*, findItemByID, (geode::ZStringView ID), (this, ID));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, cocos2d::CCNode*, findItemByTag, (int tag), (this, tag));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, CreateMenuItem*, findItemByObjectID, (int objectID), (this, objectID));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setTabAlignment, (alpha::editor_tabs::Tab::TabAlignment alignment), (this, alignment));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, alpha::editor_tabs::Tab::TabAlignment, getTabAlignment, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setHorizontalButtonAlignment, (geode::AxisAlignment alignment), (this, alignment));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, geode::AxisAlignment, getHorizontalButtonAlignment, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setVerticalButtonAlignment, (geode::AxisAlignment alignment), (this, alignment));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, geode::AxisAlignment, getVerticalButtonAlignment, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, reloadItems, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, updateItemSize, (int rows, int columns), (this, rows, columns));

    GEODE_EVENT_EXPORT_MEMBER_STATIC(cocos2d::CCSize, getMaxSize, (), ());
    GEODE_EVENT_EXPORT_MEMBER_STATIC(cocos2d::CCSize, getFullMaxSize, (), ());

    template<class T>
    requires std::convertible_to<T, cocos2d::CCNode*>
    void addItems(std::span<T> items) {
        insertItems(items, getButtons()->count());
    }

    template<class T>
    requires std::convertible_to<T, cocos2d::CCNode*>
    void removeItems(std::span<T> items) {
        if (items.empty()) return;

        for (cocos2d::CCNode* item : items) {
            getButtons()->removeObject(item);
        }

        reloadItems();
    }

    template<class T>
    requires std::convertible_to<T, cocos2d::CCNode*>
    void insertItems(std::span<T> items, unsigned int index) {
        if (items.empty()) return;

        index = std::min(index, getButtons()->count());

        bool hadNonCreateItem = false;
        bool inserted = false;

        for (cocos2d::CCNode* item : items) {
            if (!item) continue;

            if (m_hasCreateItems && !geode::cast::typeinfo_cast<CreateMenuItem*>(item)) {
                hadNonCreateItem = true;
                continue;
            }

            inserted = true;
            getButtons()->insertObject(item, index);
            index++;
        }

        if (hadNonCreateItem) {
            geode::log::error("Cannot put non CreateMenuItems in a tab that requires them");
        }

        if (inserted) {
            reloadItems();
        }
    }

    template<class T>
    requires std::convertible_to<T, cocos2d::CCNode*>
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
    requires std::convertible_to<T, cocos2d::CCNode*>
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

protected:
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, bool, init, (cocos2d::CCArray* items, int tab, bool hasCreateItems), (this, items, tab, hasCreateItems));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, bool, init, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, updateNavMenu, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, updateDots, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, updateUI, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, updatePages, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, float, getRealWidth, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, goToPage_, (int page), (this, page));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, onLeft_, (cocos2d::CCObject* sender), (this, sender));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, onRight_, (cocos2d::CCObject* sender), (this, sender));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, loadFromItems_, (cocos2d::CCArray* items, int columns, int rows, bool preserve), (this, items, columns, rows, preserve));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, CCMenuItemSpriteExtra*, createDot, (int page), (this, page));

    friend class ::ETEditButtonBar;

    class Impl;
    std::unique_ptr<ZImpl<Impl>> m_impl;
};

}