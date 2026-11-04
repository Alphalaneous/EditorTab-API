#pragma once

#include <Geode/Result.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/CreateMenuItem.hpp>
#include <Geode/binding/EditButtonBar.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <Geode/loader/Dispatch.hpp>
#include "ZImpl.hpp"

#define MY_MOD_ID "alphalaneous.editortab_api"

namespace alpha::editor_tabs {

class Mode;
class EditorTab;
class TabToggler;

class Tab {
public:
    enum class TabAlignment {
        Top,
        Center
    };

    /// Event sent whenever the Tab's UI is updated such as when its content size changes and position changes
    class UpdateTabUIEvent : public geode::GlobalEvent<UpdateTabUIEvent, bool(alpha::editor_tabs::EditorTab* node), Tab*> {
        using GlobalEvent::GlobalEvent;
    };
    
    /// Event sent whenever the Tab's content size changes
    class UpdateTabSizeEvent : public geode::GlobalEvent<UpdateTabSizeEvent, bool(alpha::editor_tabs::EditorTab* node, const cocos2d::CCSize& size), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    /// Event sent whenever the Tab's visibility changes
    class UpdateTabVisibilityEvent : public geode::GlobalEvent<UpdateTabVisibilityEvent, bool(alpha::editor_tabs::EditorTab* node, bool visible), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    /// Event sent whenever a tab is switched to and off of
    class SwitchTabEvent : public geode::GlobalEvent<SwitchTabEvent, bool(bool show), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    /// Event sent whenever a tab's pages are switched
    class SwitchPageEvent : public geode::GlobalEvent<SwitchPageEvent, bool(alpha::editor_tabs::EditorTab* node, int page), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    /// Event sent when the tab is loaded or reloaded
    class TabLoadedEvent : public geode::GlobalEvent<TabLoadedEvent, bool(alpha::editor_tabs::EditorTab* node), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    /// Event sent when the tab is initialized
    class TabInitializedEvent : public geode::GlobalEvent<TabInitializedEvent, bool(), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    /// Internal method to handle constructing this, you can ignore it :3
    GEODE_EVENT_EXPORT_MEMBER(Tab, void, construct, (geode::ZStringView ID, alpha::editor_tabs::EditorTab* node, cocos2d::CCNode* icon, int priority), (this, ID, node, icon, priority));

    Tab(geode::ZStringView ID, alpha::editor_tabs::EditorTab* node, cocos2d::CCNode* icon, int priority) {
        construct(ID, node, icon, priority);
    }

    /// Get the Tab's ID
    GEODE_EVENT_EXPORT_MEMBER(Tab, geode::ZStringView, getID, (), (this));

    /// Show the Tab within it's mode (will not visibly switch to if the mode is not visible)
    GEODE_EVENT_EXPORT_MEMBER(Tab, void, show, (), (this));

    /// Get the Tab's EditorTab node
    GEODE_EVENT_EXPORT_MEMBER(Tab, alpha::editor_tabs::EditorTab*, getNode, (), (this));

    /// Set the Tab's priority, change this to change the order of the Tab's togglers
    GEODE_EVENT_EXPORT_MEMBER(Tab, void, setPriority, (int priority), (this, priority));

    /// Get the Tab's priority
    GEODE_EVENT_EXPORT_MEMBER(Tab, int, getPriority, (), (this));

    /// Get the Tab's index within the Mode
    GEODE_EVENT_EXPORT_MEMBER(Tab, geode::Result<unsigned int>, getIndex, (), (this));

    /// Get the Mode the Tab is in
    GEODE_EVENT_EXPORT_MEMBER(Tab, Mode*, getMode, (), (this));

    /// Get the TabToggler the Tab is associated with
    GEODE_EVENT_EXPORT_MEMBER(Tab, alpha::editor_tabs::TabToggler*, getTabToggler, (), (this));

    /// Removes this Tab from its Mode
    GEODE_EVENT_EXPORT_MEMBER(Tab, void, removeSelf, (), (this));

protected:
    GEODE_EVENT_EXPORT_MEMBER(Tab, void, setMode, (alpha::editor_tabs::Mode* mode), (this, mode));

    friend class Mode;

    class Impl;
    std::unique_ptr<ZImpl<Impl>> m_impl;
};

}