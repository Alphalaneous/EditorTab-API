#pragma once

#include <Geode/Result.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/CreateMenuItem.hpp>
#include <Geode/binding/EditButtonBar.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <Geode/loader/Dispatch.hpp>
#include "ZImpl.hpp"

#define MY_MOD_ID "alphalaneous.editortab_api"

namespace alpha::editor_tabs {

class Mode;
class EditorTab;

class Tab {
public:
    enum class TabAlignment {
        Top,
        Center
    };

    class UpdateTabUIEvent : public geode::GlobalEvent<UpdateTabUIEvent, bool(alpha::editor_tabs::EditorTab* node), Tab*> {
        using GlobalEvent::GlobalEvent;
    };
    
    class UpdateTabSizeEvent : public geode::GlobalEvent<UpdateTabSizeEvent, bool(alpha::editor_tabs::EditorTab* node, const cocos2d::CCSize& size), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    class UpdateTabVisibilityEvent : public geode::GlobalEvent<UpdateTabVisibilityEvent, bool(alpha::editor_tabs::EditorTab* node, bool visible), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    class SwitchTabEvent : public geode::GlobalEvent<SwitchTabEvent, bool(bool show), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    class SwitchPageEvent : public geode::GlobalEvent<SwitchPageEvent, bool(alpha::editor_tabs::EditorTab* node, int page), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    class TabLoadedEvent : public geode::GlobalEvent<TabLoadedEvent, bool(alpha::editor_tabs::EditorTab* node), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    class TabInitializedEvent : public geode::GlobalEvent<TabInitializedEvent, bool(), Tab*> {
        using GlobalEvent::GlobalEvent;
    };

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, construct, (geode::ZStringView ID, alpha::editor_tabs::EditorTab* node, cocos2d::CCNode* icon, int priority), (this, ID, node, icon, priority));

    Tab(geode::ZStringView ID, alpha::editor_tabs::EditorTab* node, cocos2d::CCNode* icon, int priority) {
        construct(ID, node, icon, priority);
    }

    GEODE_EVENT_EXPORT_MEMBER(Tab, geode::ZStringView, getID, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, show, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, alpha::editor_tabs::EditorTab*, getNode, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, cocos2d::CCNode*, getIcon, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, setPriority, (int priority), (this, priority));

    GEODE_EVENT_EXPORT_MEMBER(Tab, int, getPriority, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, geode::Result<unsigned int>, getIndex, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, Mode*, getMode, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, CCMenuItemToggler*, getTabToggle, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, removeSelf, (), (this));

    GEODE_EVENT_EXPORT_MEMBER(Tab, void, setMode, (alpha::editor_tabs::Mode* mode), (this, mode));

protected:
    class Impl;
    std::unique_ptr<ZImpl<Impl>> m_impl;
};

}