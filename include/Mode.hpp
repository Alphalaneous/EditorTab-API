#pragma once

#include <Geode/binding/EditButtonBar.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <Geode/loader/Dispatch.hpp>

#include "ZImpl.hpp"

#define MY_MOD_ID "alphalaneous.editortab_api"

namespace alpha::editor_tabs {

class Tab;

class Mode {
public:
    class SwitchModeEvent : public geode::GlobalEvent<SwitchModeEvent, bool(bool show), Mode*> {
        using GlobalEvent::GlobalEvent;
    };

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, construct, (geode::ZStringView ID), (this, ID));

    Mode(geode::ZStringView ID) {
        construct(ID);
    }

    GEODE_EVENT_EXPORT_MEMBER(Mode, geode::ZStringView, getID, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, show, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, switchTab, (geode::ZStringView ID), (this, ID));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, switchTab, (unsigned int index), (this, index));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, switchTab, (Tab* tab), (this, tab));

    GEODE_EVENT_EXPORT_MEMBER(Mode, Tab*, getCurrentTab, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Mode, Tab*, getTab, (geode::ZStringView ID), (this, ID));

    GEODE_EVENT_EXPORT_MEMBER(Mode, Tab*, getTabByNode, (EditButtonBar* node), (this, node));

    GEODE_EVENT_EXPORT_MEMBER(Mode, Tab*, getTabByIndex, (unsigned int index), (this, index));

    GEODE_EVENT_EXPORT_MEMBER(Mode, geode::Result<unsigned int>, getTabIndex, (geode::ZStringView ID), (this, ID));

    GEODE_EVENT_EXPORT_MEMBER(Mode, geode::Result<unsigned int>, getTabIndex, (Tab* tab), (this, tab));

    GEODE_EVENT_EXPORT_MEMBER(Mode, std::span<const std::shared_ptr<Tab>>, getAllTabs, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Mode, Tab*, createTab, (geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon), (this, ID, node, icon));

    GEODE_EVENT_EXPORT_MEMBER(Mode, Tab*, createTab, (geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority), (this, ID, node, icon, priority));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, removeTab, (geode::ZStringView ID), (this, ID));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, removeTab, (Tab* tab), (this, tab));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, removeAllTabs, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, reloadAllTabs, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, reloadAllTabSizes, (int rows, int columns), (this, rows, columns));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, removeSelf, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, setupTabs, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, updateToggles, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, showMode, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Mode, void, hideMode, (), (this));

protected:
    class Impl;
    std::unique_ptr<ZImpl<Impl>> m_impl;
};

}