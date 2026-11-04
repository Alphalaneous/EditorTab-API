#pragma once

#include <Geode/loader/Dispatch.hpp>
#include "../api/ZImpl.hpp"

#define MY_MOD_ID "alphalaneous.editortab_api"

namespace alpha::editor_tabs {

class Tab;

class TabToggler : public CCMenuItemToggler {
public:
    /// Internal method to handle constructing this, you can ignore it :3
    GEODE_EVENT_EXPORT_MEMBER(TabToggler, void, construct, (), (this));

    TabToggler() {
        construct();
    }

    GEODE_EVENT_EXPORT_MEMBER_STATIC(TabToggler*, create, (alpha::editor_tabs::Tab* tab, CCNode* icon), (tab, icon));
    GEODE_EVENT_EXPORT_MEMBER(TabToggler, void, onPress, (CCObject* sender), (this, sender));

    /// Get the icon on the toggler
    GEODE_EVENT_EXPORT_MEMBER(TabToggler, cocos2d::CCNode*, getIcon, (), (this));

    /// Set and replace the icon on the toggler
    GEODE_EVENT_EXPORT_MEMBER(TabToggler, void, setIcon, (cocos2d::CCNode* icon), (this, icon));

    /// Get the toggler's associated Tab
    GEODE_EVENT_EXPORT_MEMBER(TabToggler, alpha::editor_tabs::Tab*, getTab, (), (this));

protected:
    GEODE_EVENT_EXPORT_MEMBER(TabToggler, bool, init, (alpha::editor_tabs::Tab* tab, CCNode* icon), (this, tab, icon));
    GEODE_EVENT_EXPORT_MEMBER(TabToggler, void, toggleTab, (bool on), (this, on));

    friend class Mode;

    class Impl;
    std::unique_ptr<ZImpl<Impl>> m_impl;
};

}