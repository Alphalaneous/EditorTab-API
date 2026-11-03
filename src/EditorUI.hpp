#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "ModeHandler.hpp"

using namespace geode::prelude;

class $modify(ETEditorUI, EditorUI) {

    static void onModify(auto& self) {
        (void) self.setHookPriority("EditorUI::toggleMode", Priority::Replace);
        (void) self.setHookPriority("EditorUI::updateCreateMenu", Priority::Replace);
        (void) self.setHookPriorityPost("EditorUI::init", Priority::LatePost);
    }

    struct Fields {
        std::shared_ptr<ModeHandler> m_modeHandler = nullptr;

        alpha::editor_tabs::Mode* m_buildMode = nullptr;
        alpha::editor_tabs::Mode* m_editMode = nullptr;
        alpha::editor_tabs::Mode* m_deleteMode = nullptr;
        alpha::editor_tabs::Mode* m_viewMode = nullptr;

        Ref<CCArray> m_editButtons{};
        Ref<CCDictionary> m_editButtonDict{};

        geode::ScrollLayer* m_editScrollLayer = nullptr;

        bool m_uiVisible = true;
    };

    bool init(LevelEditorLayer* editorLayer);

    void populateEditButtons();
    void addItemToEditButtons(CCNode* item);

    CCArray* getEditButtons();

    void toggleMode(cocos2d::CCObject* sender);
    void updateModeToggles(int mode);
    void readjustDpadMenu(EditButtonBar* node, float availableWidth);
    void setDpadStep(int p0);

    void fixButtonWidths();

    CreateMenuItem* getCreateBtn(int id, int bg);
    void setupCreateMenu();
    void createMoveMenu();
    void updateCreateMenu(bool selectTab);
    void selectBuildTab(int tab);
    void showUI(bool show);
    void updateButtons();
    void updateSpecialTabVisibility();
    void onPause(cocos2d::CCObject* sender);

    void updateDpadMenuButtons();

    CCNode* iconForIdx(int idx);
    Result<std::string_view> idForBuildTabIndex(unsigned int index);

    void setupBuildMode();
    void setupEditMode();
    void setupDeleteMode();
    void setupViewMode();
    bool scrollWheel(float y, float x);

    static constexpr std::array<std::string_view, 14> TabIDs {
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