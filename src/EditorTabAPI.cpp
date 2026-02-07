#define GEODE_DEFINE_EVENT_EXPORTS
#include "../include/EditorTabAPI.hpp"
#include "EditorUI.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace alpha::editor_tabs {

    void addTab(geode::ZStringView tabID, geode::ZStringView modeID, CreateTab&& createTab, CreateTabIcon&& createIcon, ToggleTab&& toggleTab, ReloadTab&& reloadTab) {
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
}