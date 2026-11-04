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

static const std::string Build = "robtop.geometry-dash/build";
static const std::string Edit = "robtop.geometry-dash/edit";
static const std::string Delete = "robtop.geometry-dash/delete";
static const std::string View = "robtop.geometry-dash/view";

class Mode;
class Tab;
class EditorTab;

GEODE_EVENT_EXPORT(Mode*, getMode, (geode::ZStringView ID), (ID));

GEODE_EVENT_EXPORT(Mode*, getCurrentMode, (void), ());

GEODE_EVENT_EXPORT(geode::ZStringView, getCurrentModeID, (void), ());

GEODE_EVENT_EXPORT(Tab*, getCurrentTab, (void), ());

GEODE_EVENT_EXPORT(geode::ZStringView, getCurrentTabID, (void), ());

GEODE_EVENT_EXPORT(std::span<const std::shared_ptr<Mode>>, getAllModes, (void), ());

GEODE_EVENT_EXPORT(std::vector<std::shared_ptr<Tab>>, getAllTabs, (void), ());

GEODE_EVENT_EXPORT(Mode*, createMode, (geode::ZStringView ID), (ID));

GEODE_EVENT_EXPORT(Tab*, getTab, (geode::ZStringView ID), (ID));

GEODE_EVENT_EXPORT(Tab*, getTabByNode, (alpha::editor_tabs::EditorTab* node), (node));

GEODE_EVENT_EXPORT(void, removeMode, (geode::ZStringView ID), (ID));

GEODE_EVENT_EXPORT(void, removeMode, (Mode* mode), (mode));

GEODE_EVENT_EXPORT(void, reloadAllModes, (void), ());

GEODE_EVENT_EXPORT(void, switchMode, (Mode* mode), (mode));

GEODE_EVENT_EXPORT(void, switchMode, (geode::ZStringView ID), (ID));

class AllTabsInitializedEvent : public geode::Event<AllTabsInitializedEvent, bool()> {
    using Event::Event;
};

}