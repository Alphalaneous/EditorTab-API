#pragma once

#include <Geode/loader/Dispatch.hpp>
#include "../api/ZImpl.hpp"

#define MY_MOD_ID "alphalaneous.editortab_api"

namespace alpha::editor_tabs {

class Tab;

class TabToggler : public CCMenuItemToggler {
public:
    GEODE_EVENT_EXPORT_MEMBER(TabToggler, void, construct, (), (this));

    TabToggler() {
        construct();
    }

    GEODE_EVENT_EXPORT_MEMBER_STATIC(TabToggler*, create, (alpha::editor_tabs::Tab* tab, CCNode* icon), (tab, icon));

    GEODE_EVENT_EXPORT_MEMBER(TabToggler, void, toggleTab, (bool on), (this, on));
    GEODE_EVENT_EXPORT_MEMBER(TabToggler, void, onPress, (CCObject* sender), (this, sender));
    GEODE_EVENT_EXPORT_MEMBER(TabToggler, cocos2d::CCNode*, getIcon, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(TabToggler, void, setIcon, (cocos2d::CCNode* icon), (this, icon));

protected:
    GEODE_EVENT_EXPORT_MEMBER(TabToggler, bool, init, (alpha::editor_tabs::Tab* tab, CCNode* icon), (this, tab, icon));

    class Impl;
    std::unique_ptr<ZImpl<Impl>> m_impl;
};

}