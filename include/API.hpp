#pragma once

#include <Geode/cocos/base_nodes/CCNode.h>
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

        inline void construct(Mode* mode, geode::ZStringView ID)
        GEODE_EVENT_EXPORT_NORES(&construct, (mode, ID));

        inline void destruct(Mode* mode)
        GEODE_EVENT_EXPORT_NORES(&destruct, (mode));

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

        inline void construct(Tab* tab, geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority = 0)
        GEODE_EVENT_EXPORT_NORES(&construct, (tab, ID, node, icon, priority));

        inline void destruct(Tab* tab)
        GEODE_EVENT_EXPORT_NORES(&destruct, (tab));

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

        inline void setItemless(Tab* tab)
        GEODE_EVENT_EXPORT_NORES(&setItemless, (tab));

        inline void removeSelf(Tab* tab)
        GEODE_EVENT_EXPORT_NORES(&removeSelf, (tab));

        inline void setPageDotsClickable(Tab* tab, bool clickable)
        GEODE_EVENT_EXPORT_NORES(&setPageDotsClickable, (tab, clickable));

        inline bool arePageDotsClickable(Tab* tab)
        GEODE_EVENT_EXPORT_NORES(&arePageDotsClickable, (tab));

        class ResizeTabEvent : public geode::GlobalEvent<ResizeTabEvent, bool(const cocos2d::CCSize& size, float scale), Tab*> {
            using GlobalEvent::GlobalEvent;
        };

        class SwitchTabEvent : public geode::GlobalEvent<SwitchTabEvent, bool(bool show), Tab*> {
            using GlobalEvent::GlobalEvent;
        };
    }

    inline Mode* getMode(geode::ZStringView ID)
    GEODE_EVENT_EXPORT_NORES(&getMode, (ID));

    inline Mode* getCurrentMode()
    GEODE_EVENT_EXPORT_NORES(&getCurrentMode, ());

    inline geode::ZStringView getCurrentModeID()
    GEODE_EVENT_EXPORT_NORES(&getCurrentModeID, ());

    inline std::span<const std::shared_ptr<Mode>> getAllModes()
    GEODE_EVENT_EXPORT_NORES(&getAllModes, ());

    inline Mode* createMode(geode::ZStringView ID)
    GEODE_EVENT_EXPORT_NORES(&createMode, (ID));

    inline Tab* getTab(geode::ZStringView ID)
    GEODE_EVENT_EXPORT_NORES(&getTab, (ID));

    inline Tab* getTabByNode(EditButtonBar* node)
    GEODE_EVENT_EXPORT_NORES(&getTabByNode, (node));

    inline EditButtonBar* createTabBar(cocos2d::CCArray* items, int tab, bool hasCreateItems)
    GEODE_EVENT_EXPORT_ID_NORES(
        static_cast<EditButtonBar*(*)(cocos2d::CCArray*, int, bool)>(&createTabBar),
        (items, tab, hasCreateItems),
        MY_MOD_ID "/createTabBar1"
    );

    inline EditButtonBar* createTabBar(std::span<geode::Ref<cocos2d::CCNode>> items, int tab, bool hasCreateItems)
    GEODE_EVENT_EXPORT_ID_NORES(
        static_cast<EditButtonBar*(*)(std::span<geode::Ref<cocos2d::CCNode>>, int, bool)>(&createTabBar),
        (items, tab, hasCreateItems),
        MY_MOD_ID "/createTabBar2"
    );

    inline EditButtonBar* createTabNode()
    GEODE_EVENT_EXPORT_NORES(&createTabNode, ());

    inline geode::Result<std::string_view> idForBuildTabIndex(unsigned int index)
    GEODE_EVENT_EXPORT(&idForBuildTabIndex, (index));
    
    inline geode::Result<int> indexForBuildTabID(std::string_view id)
    GEODE_EVENT_EXPORT(&indexForBuildTabID, (id));

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

/*inline void addTab(geode::ZStringView tabID, geode::ZStringView modeID, CreateTab createTab, CreateTabIcon createIcon, ToggleTab toggleTab = nullptr, ReloadTab reloadTab = nullptr) 
GEODE_EVENT_EXPORT_NORES(&addTab, (tabID, modeID, std::move(createTab), std::move(createIcon), std::move(toggleTab), std::move(reloadTab)));*/



    /*static const std::string BUILD = "robtop.geometry-dash/build";
    static const std::string EDIT = "robtop.geometry-dash/edit";
    static const std::string DELETE = "robtop.geometry-dash/delete";

    using CreateTab = std::function<cocos2d::CCNode*()>;
    using CreateTabIcon = std::function<cocos2d::CCNode*()>;
    using ToggleTab = std::function<void(bool, cocos2d::CCNode*)>;
    using ReloadTab = std::function<void(int, int, cocos2d::CCNode*)>;

    inline void addTab(geode::ZStringView tabID, geode::ZStringView modeID, CreateTab&& createTab, CreateTabIcon&& createIcon, ToggleTab&& toggleTab = nullptr, ReloadTab&& reloadTab = nullptr) 
    GEODE_EVENT_EXPORT_NORES(&addTab, (tabID, modeID, std::move(createTab), std::move(createIcon), std::move(toggleTab), std::move(reloadTab)));

    inline void switchMode(geode::ZStringView modeID)
    GEODE_EVENT_EXPORT_NORES(&switchMode, (modeID));

    inline void switchTab(geode::ZStringView tabID)
    GEODE_EVENT_EXPORT_NORES(&switchTab, (tabID));

    inline void changeModeSprites(bool enabled)
    GEODE_EVENT_EXPORT_NORES(&changeModeSprites, (enabled));

    inline void addModeSwitchCallback(geode::Function<void(geode::ZStringView id)>&& callback, geode::Mod* mod = geode::Mod::get())
    GEODE_EVENT_EXPORT_NORES(&addModeSwitchCallback, (std::move(callback), mod));

    inline void addTabSwitchCallback(geode::Function<void(geode::ZStringView id)>&& callback, geode::Mod* mod = geode::Mod::get())
    GEODE_EVENT_EXPORT_NORES(&addTabSwitchCallback, (std::move(callback), mod));

    inline geode::Result<std::vector<cocos2d::CCNode*>> getAllTabs()
    GEODE_EVENT_EXPORT(&getAllTabs, ());

    inline geode::Result<geode::ZStringView> getCurrentMode()
    GEODE_EVENT_EXPORT(&getCurrentMode, ());

    inline geode::Result<geode::ZStringView> getCurrentTab()
    GEODE_EVENT_EXPORT(&getCurrentTab, ());

    inline geode::Result<int> getTabIndex(geode::ZStringView tabID)
    GEODE_EVENT_EXPORT(&getTabIndex, (tabID));

    inline geode::Result<geode::ZStringView> getTabMode(geode::ZStringView tabID)
    GEODE_EVENT_EXPORT(&getTabMode, (tabID));

    inline geode::Result<geode::Ref<cocos2d::CCNode>> nodeForTab(geode::ZStringView tabID)
    GEODE_EVENT_EXPORT(&nodeForTab, (tabID));

    inline geode::Result<int> indexForTab(cocos2d::CCNode* tab)
    GEODE_EVENT_EXPORT(&indexForTab, (tab));

    inline geode::Result<geode::Ref<cocos2d::CCNode>> tabForIndex(int index, geode::ZStringView modeID)
    GEODE_EVENT_EXPORT(&tabForIndex, (index, modeID));

    inline geode::Result<geode::ZStringView> idForTab(cocos2d::CCNode* tab)
    GEODE_EVENT_EXPORT(&idForTab, (tab));

    inline geode::Result<geode::ZStringView> idForTabIndex(int index, geode::ZStringView modeID)
    GEODE_EVENT_EXPORT(&idForTabIndex, (index, modeID));

    inline geode::Result<geode::Ref<CCMenuItemToggler>> togglerForTab(geode::ZStringView tabID)
    GEODE_EVENT_EXPORT(&togglerForTab, (tabID));

    inline EditButtonBar* createEditButtonBar(std::span<geode::Ref<cocos2d::CCNode>> nodes)
    GEODE_EVENT_EXPORT_NORES(&createEditButtonBar, (nodes));

    inline void removeTab(geode::ZStringView tabID)
    GEODE_EVENT_EXPORT_NORES(&removeTab, (tabID));

    inline void updateTabMenu()
    GEODE_EVENT_EXPORT_NORES(&updateTabMenu, ());*/
};