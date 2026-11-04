#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "../../include/api/Tab.hpp"
#include "../../include/api/Mode.hpp"

using namespace geode::prelude;

class $modify(ETEditorUI, EditorUI) {

    static void onModify(auto& self) {
        (void) self.setHookPriority("EditorUI::toggleMode", Priority::Replace);
        (void) self.setHookPriority("EditorUI::updateCreateMenu", Priority::Replace);
        (void) self.setHookPriorityPost("EditorUI::init", Priority::LatePost);
    }

    struct Fields {
        alpha::editor_tabs::Mode* m_buildMode = nullptr;
        alpha::editor_tabs::Mode* m_editMode = nullptr;
        alpha::editor_tabs::Mode* m_deleteMode = nullptr;
        alpha::editor_tabs::Mode* m_viewMode = nullptr;

        Ref<CCArray> m_editButtons{};
        Ref<CCDictionary> m_editButtonDict{};

        geode::ScrollLayer* m_editScrollLayer = nullptr;

        bool m_initialized = false;

        std::vector<std::shared_ptr<alpha::editor_tabs::Mode>> m_modes{};
        std::string m_currentMode = "";

        ~Fields() {
            s_instance = nullptr;
        }
    };

    static ETEditorUI* s_instance;
    static ETEditorUI* get();

    bool init(LevelEditorLayer* editorLayer);
    bool initialized();

    void populateEditButtons();
    void addItemToEditButtons(CCNode* item);

    CCArray* getEditButtons();

    void toggleMode(cocos2d::CCObject* sender);
    void updateModeToggles(int mode);
    void readjustDpadMenu(alpha::editor_tabs::EditorTab* node, float availableWidth);
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

    alpha::editor_tabs::Mode* getMode(geode::ZStringView ID);
    alpha::editor_tabs::Mode* createMode(geode::ZStringView ID);
    void switchMode(geode::ZStringView ID);
    void switchMode(alpha::editor_tabs::Mode* mode);

    geode::ZStringView getCurrentModeID();
    alpha::editor_tabs::Mode* getCurrentMode();
    std::span<const std::shared_ptr<alpha::editor_tabs::Mode>> getAllModes();
    std::vector<std::shared_ptr<alpha::editor_tabs::Tab>> getAllTabs();

    geode::ZStringView getCurrentTabID();
    alpha::editor_tabs::Tab* getCurrentTab();

    void setupModes();

    void removeMode(geode::ZStringView ID);
    void removeMode(alpha::editor_tabs::Mode* mode);

    alpha::editor_tabs::Tab* getTab(geode::ZStringView ID);
    alpha::editor_tabs::Tab* getTabByNode(alpha::editor_tabs::EditorTab* node);

    void reloadAllModes();
    void reloadAllModeSizes(int rows, int columns);

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