#pragma once

#include "Geode/cocos/menu_nodes/CCMenuItem.h"
#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/utils/ZStringView.hpp>
#include <Geode/binding/EditButtonBar.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/loader/Dispatch.hpp>

#define MY_MOD_ID "alphalaneous.editortab_api"
#undef DELETE

namespace alpha::editor_tabs {
    static const std::string BUILD = "robtop.geometry-dash/build";
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

    inline geode::Result<geode::Ref<CCMenuItemToggler>> togglerForTab(geode::ZStringView tabID)
    GEODE_EVENT_EXPORT(&togglerForTab, (tabID));

    inline EditButtonBar* createEditButtonBar(std::span<geode::Ref<cocos2d::CCNode>> nodes)
    GEODE_EVENT_EXPORT_NORES(&createEditButtonBar, (nodes));

    inline void removeTab(geode::ZStringView tabID)
    GEODE_EVENT_EXPORT_NORES(&removeTab, (tabID));
};