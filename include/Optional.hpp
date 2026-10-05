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

inline geode::ZStringView getID(Mode* mode)
GEODE_EVENT_EXPORT_NORES(&getID, (mode));

inline void show(Mode* mode)
GEODE_EVENT_EXPORT_NORES(&show, (mode));

inline void switchTab(Mode* mode, geode::ZStringView ID)
GEODE_EVENT_EXPORT_ID_NORES(
    static_cast<void(*)(Mode*, geode::ZStringView)>(&switchTab),
    (mode, ID),
    MY_MOD_ID "/switchTab1"
);

inline void switchTab(Mode* mode, Tab* tab)
GEODE_EVENT_EXPORT_ID_NORES(
    static_cast<void(*)(Mode*, Tab*)>(&switchTab),
    (mode, tab),
    MY_MOD_ID "/switchTab2"
);

inline Tab* getCurrentTab(Mode* mode)
GEODE_EVENT_EXPORT_NORES(&getCurrentTab, (mode));

inline Tab* getTab(Mode* mode, geode::ZStringView ID)
GEODE_EVENT_EXPORT_NORES(&getTab, (mode, ID));

inline Tab* getTabByNode(Mode* mode, EditButtonBar* node)
GEODE_EVENT_EXPORT_NORES(&getTabByNode, (mode, node));

inline Tab* getTabByIndex(Mode* mode, unsigned int index)
GEODE_EVENT_EXPORT_NORES(&getTabByIndex, (mode, index));

inline geode::Result<unsigned int> getTabIndex(Mode* mode, geode::ZStringView ID)
GEODE_EVENT_EXPORT_ID(
    static_cast<geode::Result<unsigned int>(*)(Mode*, geode::ZStringView)>(&getTabIndex),
    (mode, ID),
    MY_MOD_ID "/getTabIndex1"
);

inline geode::Result<unsigned int> getTabIndex(Mode* mode, Tab* tab)
GEODE_EVENT_EXPORT_ID(
    static_cast<geode::Result<unsigned int>(*)(Mode*, Tab*)>(&getTabIndex),
    (mode, tab),
    MY_MOD_ID "/getTabIndex2"
);

inline std::span<const std::shared_ptr<Tab>> getAllTabs(Mode* mode)
GEODE_EVENT_EXPORT_NORES(&getAllTabs, (mode));

inline Tab* createTab(Mode* mode, geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority = 0)
GEODE_EVENT_EXPORT_NORES(&createTab, (mode, ID, node, icon, priority));

inline void removeTab(Mode* mode, geode::ZStringView ID)
GEODE_EVENT_EXPORT_ID_NORES(
    static_cast<void(*)(Mode*, geode::ZStringView)>(&removeTab),
    (mode, ID),
    MY_MOD_ID "/removeTab1"
);

inline void removeTab(Mode* mode, Tab* tab)
GEODE_EVENT_EXPORT_ID_NORES(
    static_cast<void(*)(Mode*, Tab*)>(&removeTab),
    (mode, tab),
    MY_MOD_ID "/removeTab2"
);

inline void reloadAllTabs(Mode* mode)
GEODE_EVENT_EXPORT_NORES(&reloadAllTabs, (mode));

inline void removeSelf(Mode* mode)
GEODE_EVENT_EXPORT_NORES(&removeSelf, (mode));

class SwitchModeEvent : public geode::GlobalEvent<SwitchModeEvent, bool(bool show), Mode*> {
    using GlobalEvent::GlobalEvent;
};

}

namespace tab {

inline geode::ZStringView getID(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&getID, (tab));

inline void show(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&show, (tab));

inline EditButtonBar* getNode(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&getNode, (tab));

inline cocos2d::CCNode* getIcon(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&getIcon, (tab));

inline void setPriority(Tab* tab, int priority)
GEODE_EVENT_EXPORT_NORES(&setPriority, (tab, priority));

inline int getPriority(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&getPriority, (tab));

inline geode::Result<unsigned int> getIndex(Tab* tab)
GEODE_EVENT_EXPORT(&getIndex, (tab));

inline void overrideSize(Tab* tab, int rows, int columns)
GEODE_EVENT_EXPORT_ID_NORES(
    static_cast<void(*)(Tab*, int, int)>(&overrideSize),
    (tab, rows, columns),
    MY_MOD_ID "/overrideSize1"
);

inline void overrideSize(Tab* tab, bool override, int rows, int columns)
GEODE_EVENT_EXPORT_ID_NORES(
    static_cast<void(*)(Tab*, bool, int, int)>(&overrideSize),
    (tab, override, rows, columns),
    MY_MOD_ID "/overrideSize2"
);

inline int getRows(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&getRows, (tab));

inline int getColumns(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&getColumns, (tab));

inline Mode* getMode(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&getMode, (tab));

inline CCMenuItemToggler* getTabToggle(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&getTabToggle, (tab));

inline void reloadItems(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&reloadItems, (tab));

inline void setItemless(Tab* tab, bool itemless)
GEODE_EVENT_EXPORT_NORES(&setItemless, (tab, itemless));

inline bool isItemless(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&isItemless, (tab));

inline void setAutoScale(Tab* tab, bool enabled)
GEODE_EVENT_EXPORT_NORES(&setAutoScale, (tab, enabled));

inline bool hasAutoScale(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&hasAutoScale, (tab));

inline void removeSelf(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&removeSelf, (tab));

inline void setPageDotsClickable(Tab* tab, bool clickable)
GEODE_EVENT_EXPORT_NORES(&setPageDotsClickable, (tab, clickable));

inline bool arePageDotsClickable(Tab* tab)
GEODE_EVENT_EXPORT_NORES(&arePageDotsClickable, (tab));

class UpdateTabUIEvent : public geode::GlobalEvent<UpdateTabUIEvent, bool(EditButtonBar* node), Tab*> {
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

inline Mode* getMode(geode::ZStringView ID)
GEODE_EVENT_EXPORT_NORES(&getMode, (ID));

inline Mode* getCurrentMode()
GEODE_EVENT_EXPORT_NORES(&getCurrentMode, ());

inline geode::ZStringView getCurrentModeID()
GEODE_EVENT_EXPORT_NORES(&getCurrentModeID, ());

inline Tab* getCurrentTab()
GEODE_EVENT_EXPORT_NORES(&getCurrentTab, ());

inline geode::ZStringView getCurrentTabID()
GEODE_EVENT_EXPORT_NORES(&getCurrentTabID, ());

inline std::span<const std::shared_ptr<Mode>> getAllModes()
GEODE_EVENT_EXPORT_NORES(&getAllModes, ());

inline std::vector<std::shared_ptr<Tab>> getAllTabs()
GEODE_EVENT_EXPORT_NORES(&getAllTabs, ());

inline Mode* createMode(geode::ZStringView ID)
GEODE_EVENT_EXPORT_NORES(&createMode, (ID));

inline Tab* getTab(geode::ZStringView ID)
GEODE_EVENT_EXPORT_NORES(&getTab, (ID));

inline Tab* getTabByNode(EditButtonBar* node)
GEODE_EVENT_EXPORT_NORES(&getTabByNode, (node));

inline EditButtonBar* createTabBar(cocos2d::CCArray* items, int tab = 0, bool hasCreateItems = false)
GEODE_EVENT_EXPORT_ID_NORES(
    static_cast<EditButtonBar*(*)(cocos2d::CCArray*, int, bool)>(&createTabBar),
    (items, tab, hasCreateItems),
    MY_MOD_ID "/createTabBar1"
);

inline EditButtonBar* createTabBar(std::span<geode::Ref<cocos2d::CCNode>> items, int tab = 0, bool hasCreateItems = false)
GEODE_EVENT_EXPORT_ID_NORES(
    static_cast<EditButtonBar*(*)(std::span<geode::Ref<cocos2d::CCNode>>, int, bool)>(&createTabBar),
    (items, tab, hasCreateItems),
    MY_MOD_ID "/createTabBar2"
);

inline EditButtonBar* createTabNode()
GEODE_EVENT_EXPORT_NORES(&createTabNode, ());

inline void removeMode(geode::ZStringView ID)
GEODE_EVENT_EXPORT_ID_NORES(
    static_cast<void(*)(geode::ZStringView)>(&removeMode),
    (ID),
    MY_MOD_ID "/removeMode1"
);

inline void removeMode(Mode* mode)
GEODE_EVENT_EXPORT_ID_NORES(
    static_cast<void(*)(Mode*)>(&removeMode),
    (mode),
    MY_MOD_ID "/removeMode2"
);

inline void reloadAllModes()
GEODE_EVENT_EXPORT_NORES(&reloadAllModes, ());

inline void switchMode(Mode* mode)
GEODE_EVENT_EXPORT_ID_NORES(
    static_cast<void(*)(Mode*)>(&switchMode),
    (mode),
    MY_MOD_ID "/switchMode1"
);

inline void switchMode(geode::ZStringView ID)
GEODE_EVENT_EXPORT_ID_NORES(
    static_cast<void(*)(geode::ZStringView)>(&switchMode),
    (ID),
    MY_MOD_ID "/switchMode2"
);

inline cocos2d::CCSize getMaxTabSize()
GEODE_EVENT_EXPORT_NORES(&getMaxTabSize, ());

class AllTabsInitializedEvent : public geode::Event<AllTabsInitializedEvent, bool()> {
    using Event::Event;
};

}