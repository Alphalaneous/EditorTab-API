#define GEODE_DEFINE_EVENT_EXPORTS
#include <Geode/Geode.hpp>
#include "../include/API.hpp"
#include "../include/Mode.hpp"
#include "../include/Tab.hpp"
#include "EditorUI.hpp"
#include "EditorTab.hpp"
#include "ModeHandler.hpp"
#include "Mode.hpp"
#include "Tab.hpp"

using namespace geode::prelude;

namespace alpha::editor_tabs {

    namespace mode {
        ::internal::Mode* shadow(Mode* mode) {
            return std::bit_cast<::internal::Mode*>(mode);
        }

        geode::ZStringView getID(Mode* mode) {
            return shadow(mode)->getID();
        }

        void construct(Mode* mode, ZStringView ID) {
            shadow(mode)->construct(ID);
        }

        void destruct(Mode* mode) {
            shadow(mode)->destruct();
        }

        void show(Mode* mode) {
            shadow(mode)->show();
        }

        alpha::editor_tabs::Tab* createTab(Mode* mode, geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority)  {
            return shadow(mode)->createTab(ID, node, icon, priority);
        }

        void removeTab(Mode* mode, geode::ZStringView ID) {
            shadow(mode)->removeTab(ID);
        }

        void removeTab(Mode* mode, alpha::editor_tabs::Tab* tab) {
            shadow(mode)->removeTab(tab);
        }

        Tab* getTab(Mode* mode, geode::ZStringView ID) {
            return shadow(mode)->getTab(ID);
        }

        Tab* getCurrentTab(Mode* mode) {
            return shadow(mode)->getCurrentTab();
        }

        std::span<const std::shared_ptr<Tab>> getAllTabs(Mode* mode) {
            return shadow(mode)->getAllTabs();
        }

        void switchTab(Mode* mode, ZStringView ID) {
            shadow(mode)->switchTab(ID);
        }

        void switchTab(Mode* mode, alpha::editor_tabs::Tab* tab) {
            shadow(mode)->switchTab(tab);
        }

        void reloadAllTabs(Mode* mode) {
            shadow(mode)->reloadAllTabs();
        }

        void removeSelf(Mode* mode) {
            shadow(mode)->removeSelf();
        }
    }

    namespace tab {
        ::internal::Tab* shadow(Tab* tab) {
            return std::bit_cast<::internal::Tab*>(tab);
        }

        geode::ZStringView getID(Tab* tab) {
            return shadow(tab)->getID();
        }

        void construct(Tab* tab, ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority) {
            shadow(tab)->construct(ID, node, icon, priority);
        }

        void destruct(Tab* tab) {
            shadow(tab)->destruct();
        }

        void show(Tab* tab) {
            shadow(tab)->show();
        }

        EditButtonBar* getNode(Tab* tab) {
            return shadow(tab)->getNode();
        }

        CCNode* getIcon(Tab* tab) {
            return shadow(tab)->getIcon();
        }

        void setPriority(Tab* tab, int priority) {
            shadow(tab)->setPriority(priority);
        }

        int getPriority(Tab* tab) {
            return shadow(tab)->getPriority();
        }

        void overrideSize(Tab* tab, bool override, int rows, int columns) {
            shadow(tab)->overrideSize(override, rows, columns);
        }

        void overrideSize(Tab* tab, int rows, int columns) {
            shadow(tab)->overrideSize(rows, columns);
        }

        int getRows(Tab* tab) {
            return shadow(tab)->getRows();
        }

        int getColumns(Tab* tab) {
            return shadow(tab)->getColumns();
        }

        alpha::editor_tabs::Mode* getMode(Tab* tab) {
            return shadow(tab)->getMode();
        }

        CCMenuItemToggler* getTabToggle(Tab* tab) {
            return shadow(tab)->getTabToggle();
        }

        void reloadItems(Tab* tab) {
            return shadow(tab)->reloadItems();
        }

        void setItemless(Tab* tab) {
            return shadow(tab)->setItemless();
        }

        void removeSelf(Tab* tab) {
            return shadow(tab)->removeSelf();
        }

        void setPageDotsClickable(Tab* tab, bool clickable) {
            shadow(tab)->setPageDotsClickable(clickable);
        }

        bool arePageDotsClickable(Tab* tab) {
            return shadow(tab)->arePageDotsClickable();
        }
    }

    Mode* getMode(geode::ZStringView ID) {
        auto handler = ModeHandler::get();
        if (!handler) return nullptr;

        return handler->getMode(ID);
    }

    Mode* getCurrentMode() {
        auto handler = ModeHandler::get();
        if (!handler) return nullptr;

        return handler->getCurrentMode();
    }

    ZStringView getCurrentModeID() {
        auto handler = ModeHandler::get();
        if (!handler) return nullptr;

        return handler->getCurrentModeID();
    }

    Mode* createMode(geode::ZStringView ID) {
        auto handler = ModeHandler::get();
        if (!handler) return nullptr;

        return handler->createMode(ID);
    }

    Tab* getTab(geode::ZStringView ID) {
        return nullptr;
    }

    Tab* getTabByNode(EditButtonBar* node) {
        return nullptr;
    }

    EditButtonBar* createTabBar(cocos2d::CCArray* items, int tab, bool hasCreateItems) {
        return EditorTab::create(items, tab, hasCreateItems);
    }

    EditButtonBar* createTabBar(std::span<Ref<CCNode>> items, int tab, bool hasCreateItems) {
        return EditorTab::create(items, tab, hasCreateItems);
    }

    EditButtonBar* createTabNode() {
        return EditorTab::create();
    }

    geode::Result<std::string_view> idForBuildTabIndex(unsigned int index) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return Err("Not in Editor");
        
        return editorUI->idForBuildTabIndex(index);
    }

    geode::Result<int> indexForBuildTabID(std::string_view id) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return Err("Not in Editor");

        return editorUI->indexForBuildTabID(id);
    }

    void removeMode(geode::ZStringView ID) {
        auto handler = ModeHandler::get();
        if (!handler) return;

        handler->removeMode(ID);
    }

    void removeMode(Mode* mode) {
        auto handler = ModeHandler::get();
        if (!handler) return;

        handler->removeMode(mode);
    }

    std::span<const std::shared_ptr<Mode>> getAllModes() {
        auto handler = ModeHandler::get();
        if (!handler) return {};

        return handler->getAllModes();
    }

    void reloadAllModes() {
        auto handler = ModeHandler::get();
        if (!handler) return;

        handler->reloadAllModes();
    }

    void switchMode(ZStringView ID) {
        auto handler = ModeHandler::get();
        if (!handler) return;
        
        handler->switchMode(ID);
    }

    void switchMode(alpha::editor_tabs::Mode* mode) {
        auto handler = ModeHandler::get();
        if (!handler) return;

        handler->switchMode(mode);
    }

    /*void addTab(geode::ZStringView tabID, geode::ZStringView modeID, CreateTab&& createTab, CreateTabIcon&& createIcon, ToggleTab&& toggleTab, ReloadTab&& reloadTab) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return;

        editorUI->addTab(tabID, modeID, std::move(createTab), std::move(createIcon), std::move(toggleTab), std::move(reloadTab));
    }

    void switchMode(geode::ZStringView modeID) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return;

        editorUI->switchMode(modeID);
    }

    void switchTab(geode::ZStringView tabID) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return;

        editorUI->switchTab(tabID);
    }

    void addModeSwitchCallback(geode::Function<void(geode::ZStringView id)>&& callback, geode::Mod* mod) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return;

        editorUI->m_fields->m_modeCallbacks[mod->getID()].push_back(std::move(callback));
    }

    void addTabSwitchCallback(geode::Function<void(geode::ZStringView id)>&& callback, geode::Mod* mod) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return;

        editorUI->m_fields->m_tabCallbacks[mod->getID()].push_back(std::move(callback));
    }

    void changeModeSprites(bool enabled) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return;
        editorUI->m_fields->m_changeModeSprites = enabled;
    }

    geode::Result<std::vector<cocos2d::CCNode*>> getAllTabs() {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return geode::Err("Not in Editor");

        return geode::Ok(editorUI->getAllTabs());
    }

    geode::Result<geode::ZStringView> getCurrentMode() {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return geode::Err("Not in Editor");

        return geode::Ok(editorUI->m_fields->m_currentMode);
    }

    geode::Result<geode::ZStringView> getCurrentTab() {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return geode::Err("Not in Editor");

        return geode::Ok(editorUI->m_fields->m_currentTab.id);
    }

    geode::Result<int> getTabIndex(geode::ZStringView tabID) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return geode::Err("Not in Editor");

        const auto& res = editorUI->getTab(tabID);
        if (!res) return geode::Err("Tab doesn't exist");

        return geode::Ok(res.unwrap().idx);
    }

    geode::Result<geode::ZStringView> getTabMode(geode::ZStringView tabID) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return geode::Err("Not in Editor");

        const auto& res = editorUI->getTab(tabID);
        if (!res) return geode::Err("Tab doesn't exist");

        return geode::Ok(res.unwrap().mode);
    }

    EditButtonBar* createEditButtonBar(std::span<geode::Ref<cocos2d::CCNode>> nodes) {
        auto rows = GameManager::get()->getIntGameVariable("0050");
        auto cols = GameManager::get()->getIntGameVariable("0049");
        
        auto arr = CCArray::create();
        for (const auto& node : nodes) {
            arr->addObject(node);
        }

        return EditButtonBar::create(arr, {}, -1, false, cols, rows);
    }

    geode::Result<Ref<cocos2d::CCNode>> nodeForTab(geode::ZStringView tabID) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return geode::Err("Not in Editor");

        const auto& res = editorUI->getTab(tabID);
        if (!res) return geode::Err("Tab doesn't exist");

        return geode::Ok(res.unwrap().tab);
    }

    geode::Result<int> indexForTab(cocos2d::CCNode* tab){
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return geode::Err("Not in Editor");

        const auto& res = editorUI->getTabIndex(tab);
        if (!res) return geode::Err("Tab doesn't exist");

        return geode::Ok(res.unwrap());
    }

    geode::Result<geode::ZStringView> idForTab(cocos2d::CCNode* tab) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return geode::Err("Not in Editor");

        const auto& res = editorUI->getTabID(tab);
        if (!res) return geode::Err("Tab doesn't exist");

        return geode::Ok(res.unwrap());
    }

    geode::Result<geode::Ref<cocos2d::CCNode>> tabForIndex(int index, geode::ZStringView modeID) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return geode::Err("Not in Editor");

        const auto& res = editorUI->getTabByIndex(index, modeID);
        if (!res) return geode::Err("Tab doesn't exist");

        return geode::Ok(res.unwrap());
    }

    inline geode::Result<geode::ZStringView> idForTabIndex(int index, geode::ZStringView modeID) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return geode::Err("Not in Editor");

        const auto& res = editorUI->getTabIDByIndex(index, modeID);
        if (!res) return geode::Err("Tab doesn't exist");

        return geode::Ok(res.unwrap());
    }

    geode::Result<geode::Ref<CCMenuItemToggler>> togglerForTab(geode::ZStringView tabID) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return geode::Err("Not in Editor");

        const auto& res = editorUI->getTab(tabID);
        if (!res) return geode::Err("Tab doesn't exist");

        return geode::Ok(res.unwrap().toggler);
    }

    void removeTab(geode::ZStringView tabID) {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return;

        editorUI->removeTab(tabID);
    }

    void updateTabMenu() {
        auto editorUI = ETEditorUI::get();
        if (!editorUI) return;

        editorUI->resizeButtons();
    }*/
}