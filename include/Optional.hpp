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

struct Mode;
struct Tab;

GEODE_EVENT_EXPORT(Mode*, getMode, (geode::ZStringView ID), (ID));

GEODE_EVENT_EXPORT(Mode*, getCurrentMode, (void), ());

GEODE_EVENT_EXPORT(geode::ZStringView, getCurrentModeID, (void), ());

GEODE_EVENT_EXPORT(Tab*, getCurrentTab, (void), ());

GEODE_EVENT_EXPORT(geode::ZStringView, getCurrentTabID, (void), ());

GEODE_EVENT_EXPORT(std::span<const std::shared_ptr<Mode>>, getAllModes, (void), ());

GEODE_EVENT_EXPORT(std::vector<std::shared_ptr<Tab>>, getAllTabs, (void), ());

GEODE_EVENT_EXPORT(Mode*, createMode, (geode::ZStringView ID), (ID));

GEODE_EVENT_EXPORT(Tab*, getTab, (geode::ZStringView ID), (ID));

GEODE_EVENT_EXPORT(Tab*, getTabByNode, (EditButtonBar* node), (node));

GEODE_EVENT_EXPORT(EditButtonBar*, createTabBar, (cocos2d::CCArray* items, int tab = 0, bool hasCreateItems = false), (items, tab, hasCreateItems));

GEODE_EVENT_EXPORT(EditButtonBar*, createTabBar, (std::span<geode::Ref<cocos2d::CCNode>> items, int tab = 0, bool hasCreateItems = false), (items, tab, hasCreateItems));

GEODE_EVENT_EXPORT(EditButtonBar*, createTabBar, (std::span<cocos2d::CCNode*> items, int tab = 0, bool hasCreateItems = false), (items, tab, hasCreateItems));

GEODE_EVENT_EXPORT(EditButtonBar*, createTabNode, (void), ());

GEODE_EVENT_EXPORT(void, removeMode, (geode::ZStringView ID), (ID));

GEODE_EVENT_EXPORT(void, removeMode, (Mode* mode), (mode));

GEODE_EVENT_EXPORT(void, reloadAllModes, (void), ());

GEODE_EVENT_EXPORT(void, switchMode, (Mode* mode), (mode));

GEODE_EVENT_EXPORT(void, switchMode, (geode::ZStringView ID), (ID));

GEODE_EVENT_EXPORT(cocos2d::CCSize, getMaxTabSize, (void), ());

class AllTabsInitializedEvent : public geode::Event<AllTabsInitializedEvent, bool()> {
    using Event::Event;
};

}