#pragma once

#include <Geode/Result.hpp>
#include <Geode/binding/EditButtonBar.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/loader/Dispatch.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <span>

#define MY_MOD_ID "alphalaneous.editortab_api"

namespace alpha::editor_tabs {

/// Vanilla modes
static const std::string Build = "robtop.geometry-dash/build";
static const std::string Edit = "robtop.geometry-dash/edit";
static const std::string Delete = "robtop.geometry-dash/delete";
static const std::string View = "robtop.geometry-dash/view";

class Mode;
class Tab;
class EditorTab;

/// Get a Mode by ID
GEODE_EVENT_EXPORT(Mode*, getMode, (geode::ZStringView ID), (ID));

/// Get the current editor Mode
GEODE_EVENT_EXPORT(Mode*, getCurrentMode, (void), ());

/// Get the current editor Mode's ID
GEODE_EVENT_EXPORT(geode::ZStringView, getCurrentModeID, (void), ());

/// Get the current Tab (gets the current mode and then finds what tab is showing)
GEODE_EVENT_EXPORT(Tab*, getCurrentTab, (void), ());

/// Get the current Tab ID (gets the current mode and then finds what tab is showing)
GEODE_EVENT_EXPORT(geode::ZStringView, getCurrentTabID, (void), ());

/// Get all the available modes
GEODE_EVENT_EXPORT(std::span<const std::shared_ptr<Mode>>, getAllModes, (void), ());

/// Get all the available tabs
GEODE_EVENT_EXPORT(std::vector<std::shared_ptr<Tab>>, getAllTabs, (void), ());

/// Create a new Mode and assign it an ID
GEODE_EVENT_EXPORT(Mode*, createMode, (geode::ZStringView ID), (ID));

/// Get any Tab from any mode by ID (duplicate IDs will get the first found one)
GEODE_EVENT_EXPORT(Tab*, getTab, (geode::ZStringView ID), (ID));

/// Get any Tab from any mode by their EditorTab node
GEODE_EVENT_EXPORT(Tab*, getTabByNode, (alpha::editor_tabs::EditorTab* node), (node));

/// Remove a Mode by ID
GEODE_EVENT_EXPORT(void, removeMode, (geode::ZStringView ID), (ID));

/// Remove a Mode
GEODE_EVENT_EXPORT(void, removeMode, (Mode* mode), (mode));

/// Reload all modes (useful for refreshing the UI in some cases)
GEODE_EVENT_EXPORT(void, reloadAllModes, (void), ());

/// Switch to a mode
GEODE_EVENT_EXPORT(void, switchMode, (Mode* mode), (mode));

/// Switch to a mode by ID
GEODE_EVENT_EXPORT(void, switchMode, (geode::ZStringView ID), (ID));

/// Event sent when all tabs are finished initializing in EditorUI::init
class AllTabsInitializedEvent : public geode::Event<AllTabsInitializedEvent, bool()> {
    using Event::Event;
};

/// Event sent when the move menu is setup
class SetupMoveMenuEvent : public geode::Event<SetupMoveMenuEvent, bool(alpha::editor_tabs::Tab* editTab)> {
    using Event::Event;
};

}