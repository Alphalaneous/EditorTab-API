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

namespace mode {

GEODE_EVENT_EXPORT(geode::ZStringView, getID, (Mode* mode), (mode));

GEODE_EVENT_EXPORT(void, show, (Mode* mode), (mode));

GEODE_EVENT_EXPORT(void, switchTab, (Mode* mode, geode::ZStringView ID), (mode, ID));

GEODE_EVENT_EXPORT(void, switchTab, (Mode* mode, Tab* tab), (mode, tab));

GEODE_EVENT_EXPORT(Tab*, getCurrentTab, (Mode* mode), (mode));

GEODE_EVENT_EXPORT(Tab*, getTab, (Mode* mode, geode::ZStringView ID), (mode, ID));

GEODE_EVENT_EXPORT(Tab*, getTabByNode, (Mode* mode, EditButtonBar* node), (mode, node));

GEODE_EVENT_EXPORT(Tab*, getTabByIndex, (Mode* mode, unsigned int index), (mode, index));

GEODE_EVENT_EXPORT(geode::Result<unsigned int>, getTabIndex, (Mode* mode, geode::ZStringView ID), (mode, ID));

GEODE_EVENT_EXPORT(geode::Result<unsigned int>, getTabIndex, (Mode* mode, Tab* tab), (mode, tab));

GEODE_EVENT_EXPORT(std::span<const std::shared_ptr<Tab>>, getAllTabs, (Mode* mode), (mode));

GEODE_EVENT_EXPORT(Tab*, createTab, (Mode* mode, geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority = 0), (mode, ID, node, icon, priority));

GEODE_EVENT_EXPORT(void, removeTab, (Mode* mode, geode::ZStringView ID), (mode, ID));

GEODE_EVENT_EXPORT(void, removeTab, (Mode* mode, Tab* tab), (mode, tab));

GEODE_EVENT_EXPORT(void, reloadAllTabs, (Mode* mode), (mode));

GEODE_EVENT_EXPORT(void, removeSelf, (Mode* mode), (mode));

class SwitchModeEvent : public geode::GlobalEvent<SwitchModeEvent, bool(bool show), Mode*> {
    using GlobalEvent::GlobalEvent;
};

}

namespace tab {

GEODE_EVENT_EXPORT(geode::ZStringView, getID, (Tab* tab), (tab));

GEODE_EVENT_EXPORT(void, show, (Tab* tab), (tab));

GEODE_EVENT_EXPORT(EditButtonBar*, getNode, (Tab* tab), (tab));

GEODE_EVENT_EXPORT(cocos2d::CCNode*, getIcon, (Tab* tab), (tab));

GEODE_EVENT_EXPORT(void, setPriority, (Tab* tab, int priority), (tab, priority));

GEODE_EVENT_EXPORT(int, getPriority, (Tab* tab), (tab));

GEODE_EVENT_EXPORT(geode::Result<unsigned int>, getIndex, (Tab* tab), (tab));

GEODE_EVENT_EXPORT(void, overrideSize, (Tab* tab, int rows, int columns), (tab, rows, columns));

GEODE_EVENT_EXPORT(void, overrideSize, (Tab* tab, bool override, int rows, int columns), (tab, override, rows, columns));

GEODE_EVENT_EXPORT(int, getRows, (Tab* tab), (tab));

GEODE_EVENT_EXPORT(int, getColumns, (Tab* tab), (tab));

GEODE_EVENT_EXPORT(Mode*, getMode, (Tab* tab), (tab));

GEODE_EVENT_EXPORT(CCMenuItemToggler*, getTabToggle, (Tab* tab), (tab));

GEODE_EVENT_EXPORT(void, reloadItems, (Tab* tab), (tab));

GEODE_EVENT_EXPORT(void, setItemless, (Tab* tab, bool itemless), (tab, itemless));

GEODE_EVENT_EXPORT(bool, isItemless, (Tab* tab), (tab));

GEODE_EVENT_EXPORT(void, setAutoScale, (Tab* tab, bool enabled), (tab, enabled));

GEODE_EVENT_EXPORT(bool, hasAutoScale, (Tab* tab), (tab));

GEODE_EVENT_EXPORT(void, removeSelf, (Tab* tab), (tab));

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

}

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