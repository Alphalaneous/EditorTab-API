#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "ModeHandler.hpp"

using namespace geode::prelude;

class $modify(ETEditorUI, EditorUI) {

    static void onModify(auto& self) {
        (void) self.setHookPriority("EditorUI::toggleMode", Priority::Replace);
        (void) self.setHookPriority("EditorUI::selectBuildTab", Priority::Replace);
    }

    struct Fields {
        std::shared_ptr<ModeHandler> m_modeHandler;
        bool m_initialized;
        std::vector<CCMenuItemToggler*> m_modeToggles;

        ~Fields() {
            s_instance = nullptr;
        }
    };

    static ETEditorUI* s_instance;

    static ETEditorUI* get();

    bool init(LevelEditorLayer* editorLayer);
    bool initialized();

    CCMenuItemToggler* createModeToggle(ZStringView ID, int tag, ZStringView sprite);

    void toggleMode(cocos2d::CCObject* sender);
    void updateModeToggles(int mode);

    void setupCreateMenu();
    void updateCreateMenu(bool selectTab);
    void createMoveMenu();
    void setupDeleteMenu();
    void selectBuildTab(int tab);
    void showUI(bool show);
    void updateButtons();
    void updateSpecialTabVisibility();

    CCNode* iconForIdx(int idx);
    Result<ZStringView> idForBuildTabIndex(unsigned int index);
    Result<int> indexForBuildTabID(ZStringView id);

    void setupBuildMode();
    void setupEditMode();
    void setupDeleteMode();
    void setupViewMode();

    static constexpr std::array<std::string, 14> TabIDs {
        "block",
        "outline",
        "slope",
        "hazard",
        "3d",
        "portal",
        "monster",
        "pixel",
        "collectible",
        "icon",
        "deco",
        "sawblade",
        "trigger",
        "custom"
    };
};