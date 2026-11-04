#pragma once

#include <Geode/loader/Dispatch.hpp>
#include <Geode/cocos/menu_nodes/CCMenu.h>
#include "../api/ZImpl.hpp"

#define MY_MOD_ID "alphalaneous.editortab_api"

class ETEditButtonBar;

namespace alpha::editor_tabs {

class TabMenu : public cocos2d::CCMenu {
public:
    using CCNode::addChild;

    /// Internal method to handle constructing this, you can ignore it :3
    GEODE_EVENT_EXPORT_MEMBER(TabMenu, void, construct, (), (this));

    TabMenu() {
        construct();
    }

    GEODE_EVENT_EXPORT_MEMBER_STATIC(TabMenu*, create, (), ());

    GEODE_EVENT_EXPORT_MEMBER(TabMenu, cocos2d::CCMenu*, getMainMenu, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(TabMenu, void, updateUI, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(TabMenu, void, updatePage, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(TabMenu, void, goToPage, (unsigned int page), (this, page));
    GEODE_EVENT_EXPORT_MEMBER(TabMenu, unsigned int, getPage, (), (this));
    GEODE_EVENT_EXPORT_MEMBER(TabMenu, unsigned int, getPageCount, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(TabMenu, void, onLeft, (cocos2d::CCObject* sender), (this, sender));
    GEODE_EVENT_EXPORT_MEMBER(TabMenu, void, onRight, (cocos2d::CCObject* sender), (this, sender));

    GEODE_EVENT_EXPORT_MEMBER(TabMenu, void, setContentSize, (const cocos2d::CCSize& contentSize), (this, contentSize));
    GEODE_EVENT_EXPORT_MEMBER(TabMenu, void, addChild, (cocos2d::CCNode* child, int zOrder, int tag), (this, child, zOrder, tag));
protected:
    GEODE_EVENT_EXPORT_MEMBER(TabMenu, bool, init, (), (this));

    class Impl;
    std::unique_ptr<ZImpl<Impl>> m_impl;
};

}