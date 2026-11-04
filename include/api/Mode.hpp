#pragma once

#include <Geode/binding/EditButtonBar.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <Geode/loader/Dispatch.hpp>

#include "ZImpl.hpp"

#define MY_MOD_ID "alphalaneous.editortab_api"

class ETEditorUI;

namespace alpha::editor_tabs {

class Tab;
class EditorTab;

class Mode {
public:

    /// Event sent whenever your Mode is switched
    class SwitchModeEvent : public geode::GlobalEvent<SwitchModeEvent, bool(bool show), Mode*> {
        using GlobalEvent::GlobalEvent;
    };

    /// Internal method to handle constructing this, you can ignore it :3
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, construct, (geode::ZStringView ID), (this, ID));

    Mode(geode::ZStringView ID) {
        construct(ID);
    }

    /// Gets the Mode's ID
    GEODE_EVENT_EXPORT_MEMBER(Mode, geode::ZStringView, getID, (), (this));

    /// Shows the Mode, setting the current one to this
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, show, (), (this));

    /// Switches the Tab by ID
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, switchTab, (geode::ZStringView ID), (this, ID));

    /// Switches the Tab by index
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, switchTab, (unsigned int index), (this, index));

    /// Switches the Tab
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, switchTab, (Tab* tab), (this, tab));

    /// Gets the currently active Tab
    GEODE_EVENT_EXPORT_MEMBER(Mode, Tab*, getCurrentTab, (), (this));

    /// Gets a Tab by ID
    GEODE_EVENT_EXPORT_MEMBER(Mode, Tab*, getTab, (geode::ZStringView ID), (this, ID));

    /// Gets a Tab by EditorTab node
    GEODE_EVENT_EXPORT_MEMBER(Mode, Tab*, getTabByNode, (alpha::editor_tabs::EditorTab* node), (this, node));

    /// Gets a Tab by index
    GEODE_EVENT_EXPORT_MEMBER(Mode, Tab*, getTabByIndex, (unsigned int index), (this, index));

    /// Gets a Tab's index by ID
    GEODE_EVENT_EXPORT_MEMBER(Mode, geode::Result<unsigned int>, getTabIndex, (geode::ZStringView ID), (this, ID));

    /// Gets a Tab's index
    GEODE_EVENT_EXPORT_MEMBER(Mode, geode::Result<unsigned int>, getTabIndex, (Tab* tab), (this, tab));

    /// Gets all Tabs within the Mode
    GEODE_EVENT_EXPORT_MEMBER(Mode, std::span<const std::shared_ptr<Tab>>, getAllTabs, (), (this));

    /// Create a Tab with an ID (should be unique and prefixed, such as "epic-tab"_spr)
    /// @param icon The icon that shows on the toggler to switch to the Tab
    /// @param priority The priority the tab will have, change this to change the order of the Tab's togglers
    GEODE_EVENT_EXPORT_MEMBER(Mode, Tab*, createTab, (geode::ZStringView ID, alpha::editor_tabs::EditorTab* node, cocos2d::CCNode* icon, int priority = 0), (this, ID, node, icon, priority));

    /// Remove a Tab by ID
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, removeTab, (geode::ZStringView ID), (this, ID));

    /// Remove a Tab
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, removeTab, (Tab* tab), (this, tab));

    /// Remove all Tabs
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, removeAllTabs, (), (this));

    /// Reload all Tabs (useful for refreshing the UI in some cases)
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, reloadAllTabs, (), (this));

    /// Remove this Mode from the editor
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, removeSelf, (), (this));

protected:
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, updateToggles, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, setupTabs, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, showMode, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, hideMode, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(Mode, void, updateTabSizes, (int rows, int columns), (this, rows, columns));

    friend class ::ETEditorUI;

    class Impl;
    std::unique_ptr<ZImpl<Impl>> m_impl;
};

}