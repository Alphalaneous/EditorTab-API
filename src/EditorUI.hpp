#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "../include/EditorTabAPI.hpp"

using namespace geode::prelude;

struct InternalTabData {
    std::string id;
    std::string mode;
    Ref<CCNode> tab;
    Ref<CCNode> buttonTopOn;
    Ref<CCNode> buttonTopOff;
    std::function<void(bool, CCNode*)> onTab;
    std::function<void(int, int, CCNode*)> onReload;

    Ref<CCMenuItemToggler> toggler;
    int idx;

    bool operator==(const InternalTabData& data) {
        return this->id == data.id;
    }
};

using namespace alpha::editor_tabs;

class $modify(ETEditorUI, EditorUI) {
private:
    static ETEditorUI* s_instance;
public:

    struct Fields {
        StringMap<std::vector<InternalTabData>> m_tabs;
        StringMap<int> m_tabIndex;
        StringMap<int> m_tabPage;
        std::string m_currentMode;
        bool m_initialized = false;
        bool m_uiVisible = true;
        int m_maxTabs = 14;
        InternalTabData m_currentTab;
        CCMenu* m_arrowMenu;
        ~Fields() {
            s_instance = nullptr;
        }
    };

    static void onModify(auto& self) {
        (void) self.setHookPriority("EditorUI::init", Priority::EarlyPost);
    }

    static ETEditorUI* get();

    bool init(LevelEditorLayer* editorLayer);
    void setupTabs();
    void showUI(bool show);
    void onPause(CCObject* sender);
    InternalTabData tabWithSpriteFrame(ZStringView id, ZStringView mode, CCNode* tab, ZStringView frameName);
    InternalTabData tabWithNodeCallback(ZStringView id, ZStringView mode, CCNode* tab, std::function<CCNode*()>&& callback);
    void fitNode(CCNode* node, const CCSize& size);
    CCMenuItemToggler* createToggler(const InternalTabData& tabData);
    void setupButtons();
    void setupButton(InternalTabData& tabData);
    void switchTab(const InternalTabData& tabData);
    void switchTab(ZStringView id);
    void switchMode(ZStringView mode);
    void toggleModeInternal();
    void toggleMode(CCObject* sender);
    void reloadEditTabs();
    void setTabVisible(CCNode* tab, bool visible);
    void updateCreateMenu(bool selectTab);

    void goToPage(int page);

    void addTab(geode::ZStringView tabID, geode::ZStringView modeID, CreateTab&& createTab, CreateTabIcon&& createIcon, ToggleTab&& toggleTab, ReloadTab&& reloadTab);
    void removeTab(geode::ZStringView tabID);
    Result<const InternalTabData&> getTab(geode::ZStringView tabID);
};