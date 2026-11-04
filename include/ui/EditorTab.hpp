#pragma once

#include <Geode/loader/Dispatch.hpp>
#include <Geode/binding/EditButtonBar.hpp>
#include <Geode/ui/Layout.hpp>
#include "../api/Tab.hpp"
#include "../api/ZImpl.hpp"

#define MY_MOD_ID "alphalaneous.editortab_api"

class ETEditButtonBar;

namespace alpha::editor_tabs {

class EditorTab : public EditButtonBar {
public:
    using CCNode::addChild;

    /// Internal method to handle constructing this, you can ignore it :3
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, construct, (), (this));

    EditorTab() : EditButtonBar(geode::ZeroConstructor) {
        construct();
    }

    /// Create an EditorTab node to pass into a Tab
    /// @param items a CCArray of nodes for the tab to hold
    /// @param tab a tab index, specifically for build Mode tabs
    /// @param hasCreateItems if the tab only contains CreateMenuItems and is in the Build mode, this should be true
    GEODE_EVENT_EXPORT_MEMBER_STATIC(EditorTab*, create, (cocos2d::CCArray* items, int tab = 0, bool hasCreateItems = false), (items, tab, hasCreateItems));

    /// Create an EditorTab node to pass into a Tab
    /// @param items a span of node refs for the tab to hold
    /// @param tab a tab index, specifically for build Mode tabs
    /// @param hasCreateItems if the tab only contains CreateMenuItems and is in the Build mode, this should be true
    GEODE_EVENT_EXPORT_MEMBER_STATIC(EditorTab*, create, (std::span<geode::Ref<cocos2d::CCNode>> items, int tab = 0, bool hasCreateItems = false), (items, tab, hasCreateItems));
    
    /// Create an EditorTab node to pass into a Tab
    /// @param items a span of nodes for the tab to hold
    /// @param tab a tab index, specifically for build Mode tabs
    /// @param hasCreateItems if the tab only contains CreateMenuItems and is in the Build mode, this should be true
    GEODE_EVENT_EXPORT_MEMBER_STATIC(EditorTab*, create, (std::span<cocos2d::CCNode*> items, int tab = 0, bool hasCreateItems = false), (items, tab, hasCreateItems));

    /// Create an EditorTab node to pass into a Tab with no items nor paging, useful if you want to add children to it yourself
    GEODE_EVENT_EXPORT_MEMBER_STATIC(EditorTab*, create, (), ());

    /// Override the row and column counts of the EditorTab
    /// @param override toggling whether the override is active or not
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, overrideSize, (bool override, int rows = 0, int columns = 0), (this, override, rows, columns));

    /// Override the row and column counts of the EditorTab
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, overrideSize, (int rows, int columns), (this, rows, columns));

    /// Get the row count
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, int, getRows, (), (this));

    /// Get the column count
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, int, getColumns, (), (this));

    /// Get the Tab this EditorTab is associated with
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, Tab*, getTab, (), (this));

    /// Set whether this has items or not, if false, the node loses the paging and does not follow the items array, and instead is used to add children to
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setItemless, (bool itemless), (this, itemless));

    /// Returns whether this is itemless or not
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, bool, isItemless, (), (this));

    /// Sets whether this should auto scale based on content size or not
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setAutoScale, (bool enabled), (this, enabled));

    /// Returns whether this auto scales based on content size
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, bool, hasAutoScale, (), (this));

    /// Set if this should not override row and column counts when the item count is greater than rows * columns
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, disableSizeOverrideIfExceeded, (bool disable), (this, disable));

    /// Return true if this should not override row and column counts when the item count is greater than rows * columns
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, bool, isDisableSizeOverrideIfExceeded, (), (this));

    /// Get the items CCArray
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, cocos2d::CCArray*, getItems, (), (this));

    /// Add an item
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, addItem, (cocos2d::CCNode* item), (this, item));

    /// Remove an item
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, removeItem, (cocos2d::CCNode* item), (this, item));

    /// Remove an item by its Node ID
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, removeItemByID, (geode::ZStringView ID), (this, ID));

    /// Remove an item by its Tag
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, removeItemByTag, (int tag), (this, tag));

    /// Remove an item if it is a CreateMenuItem with an object ID
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, removeItemByObjectID, (int objectID), (this, objectID));

    /// Insert an item at an index
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, insertItem, (cocos2d::CCNode* item, unsigned int index), (this, item, index));

    /// Insert an item after another item
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, insertItemAfter, (cocos2d::CCNode* item, cocos2d::CCNode* after), (this, item, after));

    /// Insert an item before another item
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, insertItemBefore, (cocos2d::CCNode* item, cocos2d::CCNode* before), (this, item, before));

    /// Insert an item after an object ID if it is a CreateMenuItem
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, insertItemAfter, (CreateMenuItem* item, int objectID), (this, item, objectID));

    /// Insert an item before an object ID if it is a CreateMenuItem
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, insertItemBefore, (CreateMenuItem* item, int objectID), (this, item, objectID));

    /// Find an item by its Node ID
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, cocos2d::CCNode*, findItemByID, (geode::ZStringView ID), (this, ID));

    /// Find an item by its tag
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, cocos2d::CCNode*, findItemByTag, (int tag), (this, tag));

    /// Find an item by its object ID if it is a CreateMenuItem
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, CreateMenuItem*, findItemByObjectID, (int objectID), (this, objectID));

    /// Set the tab alignment, whether it should be vertically aligned to the top or center of the toolbar
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setTabAlignment, (alpha::editor_tabs::Tab::TabAlignment alignment), (this, alignment));

    /// Get the tab alignment
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, alpha::editor_tabs::Tab::TabAlignment, getTabAlignment, (), (this));

    /// Set the horizontal axis alignment
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setHorizontalItemAlignment, (geode::AxisAlignment alignment), (this, alignment));

    /// Get the horizontal axis alignment
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, geode::AxisAlignment, getHorizontalItemAlignment, (), (this));

    /// Set the vertical axis alignment
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setVerticalItemAlignment, (geode::AxisAlignment alignment), (this, alignment));

    /// Get the vertical axis alignment
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, geode::AxisAlignment, getVerticalItemAlignment, (), (this));

    /// Reload all Items (useful for refreshing the UI in some cases)
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, reloadAllItems, (), (this));

    /// Set the padding for this within the toolbar (default is 8.f left and 8.f right)
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setPadding, (geode::Padding padding), (this, padding));

    /// Get the padding
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, geode::Padding, getPadding, (), (this));

    /// Get the size of this with the padding distances removed
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, cocos2d::CCSize, getPaddedSize, (), (this));

    /// Get the total available size of this with the padding distances removed
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, cocos2d::CCSize, getAvailablePaddedSize, (), (this));

    /// Set the page dot scale
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setDotScale, (float scale), (this, scale));

    /// Get the page dot scale
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, float, getDotScale, (), (this));

    /// Get the total available size in the editor for any EditorTab
    GEODE_EVENT_EXPORT_MEMBER_STATIC(cocos2d::CCSize, getAvailableSize, (), ());

    /// Add items
    template<class T>
    requires std::convertible_to<T, cocos2d::CCNode*>
    void addItems(std::span<T> items) {
        insertItems(items, getItems()->count());
    }

    /// Remove items
    template<class T>
    requires std::convertible_to<T, cocos2d::CCNode*>
    void removeItems(std::span<T> items) {
        if (items.empty()) return;

        for (cocos2d::CCNode* item : items) {
            getItems()->removeObject(item);
        }

        reloadAllItems();
    }

    /// Insert items at index
    template<class T>
    requires std::convertible_to<T, cocos2d::CCNode*>
    void insertItems(std::span<T> items, unsigned int index) {
        if (items.empty()) return;

        index = std::min(index, getItems()->count());

        bool hadNonCreateItem = false;
        bool inserted = false;

        for (cocos2d::CCNode* item : items) {
            if (!item) continue;

            if (m_hasCreateItems && !geode::cast::typeinfo_cast<CreateMenuItem*>(item)) {
                hadNonCreateItem = true;
                continue;
            }

            inserted = true;
            getItems()->insertObject(item, index);
            index++;
        }

        if (hadNonCreateItem) {
            geode::log::error("Cannot put non CreateMenuItems in a tab that requires them");
        }

        if (inserted) {
            reloadAllItems();
        }
    }

    /// Insert items at after an item
    template<class T>
    requires std::convertible_to<T, cocos2d::CCNode*>
    void insertItemsAfter(std::span<T> items, cocos2d::CCNode* after) {
        unsigned int idx = getItems()->indexOfObject(after);
        if (idx == UINT_MAX) {
            idx = getItems()->count();
        } 
        else {
            idx++;
        }

        insertItems(items, idx);
    }

    /// Insert items at before an item
    template<class T>
    requires std::convertible_to<T, cocos2d::CCNode*>
    void insertItemsBefore(std::span<T> items, cocos2d::CCNode* before) {
        unsigned int idx = getItems()->indexOfObject(before);
        if (idx == UINT_MAX) {
            idx = getItems()->count();
        }

        insertItems(items, idx);
    }

    /// Insert items at after an object ID if they are CreateMenuItems
    template<class T>
    requires std::convertible_to<T, CreateMenuItem*>
    void insertItemsAfter(std::span<T> items, int objectID) {
        unsigned int idx = getItems()->count();
        for (auto node : getItems()->asExt<CreateMenuItem>()) {
            if (node->m_objectID == objectID) {
                idx = getItems()->indexOfObject(node) + 1;
                break;
            }
        }

        insertItems(items, idx);
    }

    /// Insert items at before an object ID if they are CreateMenuItems
    template<class T>
    requires std::convertible_to<T, CreateMenuItem*>
    void insertItemsBefore(std::span<T> items, int objectID) {
        unsigned int idx = getItems()->count();
        for (auto node : getItems()->asExt<CreateMenuItem>()) {
            if (node->m_objectID == objectID) {
                idx = getItems()->indexOfObject(node);
                break;
            }
        }

        insertItems(items, idx);
    }

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setContentSize, (const cocos2d::CCSize& contentSize), (this, contentSize));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setPosition, (const cocos2d::CCPoint& position), (this, position));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setVisible, (bool visible), (this, visible));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, addChild, (CCNode* child, int zOrder, int tag), (this, child, zOrder, tag));

protected:
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, bool, init, (cocos2d::CCArray* items, int tab, bool hasCreateItems), (this, items, tab, hasCreateItems));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, bool, init, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, setTab, (Tab* tab), (this, tab));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, updateDots, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, updateUI, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, updatePages, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, updateTabSize, (int rows, int columns), (this, rows, columns));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, goToPage_, (int page), (this, page));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, onLeft_, (cocos2d::CCObject* sender), (this, sender));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, onRight_, (cocos2d::CCObject* sender), (this, sender));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, CCMenuItemSpriteExtra*, createDot, (int page), (this, page));

    GEODE_EVENT_EXPORT_MEMBER(EditorTab, void, loadFromItems_, (cocos2d::CCArray* items, int columns, int rows, bool preserve), (this, items, columns, rows, preserve));
    GEODE_EVENT_EXPORT_MEMBER(EditorTab, cocos2d::CCArray*, getAllItems, (), (this));

    friend class ::ETEditButtonBar;
    friend class Mode;

    class Impl;
    std::unique_ptr<ZImpl<Impl>> m_impl;
};

}