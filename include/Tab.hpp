#pragma once

#include <Geode/Result.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/CreateMenuItem.hpp>
#include <Geode/binding/EditButtonBar.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <Geode/loader/Dispatch.hpp>
#include "ZImpl.hpp"
#include "TabAlignment.hpp"

#define MY_MOD_ID "alphalaneous.editortab_api"

namespace alpha::editor_tabs {

class Mode;

class Tab {
public:
    class UpdateTabUIEvent : public geode::GlobalEvent<UpdateTabUIEvent, bool(EditButtonBar* node), Tab*> {
        using GlobalEvent::GlobalEvent;
    };
    
    class UpdateTabSizeEvent : public geode::GlobalEvent<UpdateTabSizeEvent, bool(EditButtonBar* node, const cocos2d::CCSize& size), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    class UpdateTabVisibilityEvent : public geode::GlobalEvent<UpdateTabVisibilityEvent, bool(EditButtonBar* node, bool visible), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    class SwitchTabEvent : public geode::GlobalEvent<SwitchTabEvent, bool(bool show), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    class SwitchPageEvent : public geode::GlobalEvent<SwitchPageEvent, bool(EditButtonBar* node, int page), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    class TabLoadedEvent : public geode::GlobalEvent<TabLoadedEvent, bool(EditButtonBar* node), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    class TabInitializedEvent : public geode::GlobalEvent<TabInitializedEvent, bool(), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, construct, (geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority), (this, ID, node, icon, priority));

    Tab(geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority) {
        construct(ID, node, icon, priority);
    }

    GEODE_EVENT_EXPORT_MEMBER(Tab, geode::ZStringView, getID, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, show, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, EditButtonBar*, getNode, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, cocos2d::CCNode*, getIcon, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, setPriority, (int priority), (this, priority));

    GEODE_EVENT_EXPORT_MEMBER(Tab, int, getPriority, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, geode::Result<unsigned int>, getIndex, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, overrideSize, (int rows, int columns), (this, rows, columns));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, overrideSize, (bool override, int rows, int columns), (this, override, rows, columns));

    GEODE_EVENT_EXPORT_MEMBER(Tab, int, getRows, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, int, getColumns, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, Mode*, getMode, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, CCMenuItemToggler*, getTabToggle, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, reloadItems, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, setItemless, (bool itemless), (this, itemless));

    GEODE_EVENT_EXPORT_MEMBER(Tab, bool, isItemless, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, setAutoScale, (bool enabled), (this, enabled));

    GEODE_EVENT_EXPORT_MEMBER(Tab, bool, hasAutoScale, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, removeSelf, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, addItem, (cocos2d::CCNode* item), (this, item));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, addItems, (std::span<cocos2d::CCNode*> items), (this, items));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, addItems, (std::span<geode::Ref<cocos2d::CCNode>> items), (this, items));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, removeItem, (cocos2d::CCNode* item), (this, item));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, removeItems, (std::span<cocos2d::CCNode*> items), (this, items));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, removeItems, (std::span<geode::Ref<cocos2d::CCNode>> items), (this, items));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, removeItemByID, (geode::ZStringView ID), (this, ID));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, removeItemByTag, (int tag), (this, tag));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, removeItemByObjectID, (int objectID), (this, objectID));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItem, (cocos2d::CCNode* item, unsigned int index), (this, item, index));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItems, (std::span<cocos2d::CCNode*> items, unsigned int index), (this, items, index));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItems, (std::span<geode::Ref<cocos2d::CCNode>> items, unsigned int index), (this, items, index));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItemAfter, (cocos2d::CCNode* item, cocos2d::CCNode* after), (this, item, after));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItemsAfter, (std::span<cocos2d::CCNode*> items, cocos2d::CCNode* after), (this, items, after));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItemsAfter, (std::span<geode::Ref<cocos2d::CCNode>> items, cocos2d::CCNode* after), (this, items, after));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItemBefore, (cocos2d::CCNode* item, cocos2d::CCNode* before), (this, item, before));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItemsBefore, (std::span<cocos2d::CCNode*> items, cocos2d::CCNode* before), (this, items, before));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItemsBefore, (std::span<geode::Ref<cocos2d::CCNode>> items, cocos2d::CCNode* before), (this, items, before));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItemAfter, (CreateMenuItem* item, int objectID), (this, item, objectID));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItemsAfter, (std::span<CreateMenuItem*> items, int objectID), (this, items, objectID));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItemsAfter, (std::span<geode::Ref<CreateMenuItem>> items, int objectID), (this, items, objectID));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItemBefore, (CreateMenuItem* item, int objectID), (this, item, objectID));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItemsBefore, (std::span<CreateMenuItem*> items, int objectID), (this, items, objectID));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, insertItemsBefore, (std::span<geode::Ref<CreateMenuItem>> items, int objectID), (this, items, objectID));

    GEODE_EVENT_EXPORT_MEMBER(Tab, cocos2d::CCNode*, findItemByID, (geode::ZStringView ID), (this, ID));

    GEODE_EVENT_EXPORT_MEMBER(Tab, cocos2d::CCNode*, findItemByTag, (int tag), (this, tag));

    GEODE_EVENT_EXPORT_MEMBER(Tab, CreateMenuItem*, findItemByObjectID, (int objectID), (this, objectID));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, setTabAlignment, (alpha::editor_tabs::TabAlignment alignment), (this, alignment));

    GEODE_EVENT_EXPORT_MEMBER(Tab, alpha::editor_tabs::TabAlignment, getTabAlignment, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, disableSizeOverrideIfExceeded, (bool disable), (this, disable));

    GEODE_EVENT_EXPORT_MEMBER(Tab, bool, isDisableSizeOverrideIfExceeded, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, setHorizontalButtonAlignment, (geode::AxisAlignment alignment), (this, alignment));

    GEODE_EVENT_EXPORT_MEMBER(Tab, geode::AxisAlignment, getHorizontalButtonAlignment, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, setVerticalButtonAlignment, (geode::AxisAlignment alignment), (this, alignment));

    GEODE_EVENT_EXPORT_MEMBER(Tab, geode::AxisAlignment, getVerticalButtonAlignment, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, setMode, (alpha::editor_tabs::Mode* mode), (this, mode));

protected:
    class Impl;
    std::unique_ptr<ZImpl<Impl>> m_impl;
};

}