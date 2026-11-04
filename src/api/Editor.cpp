#define GEODE_DEFINE_EVENT_EXPORTS
#include "../../include/api/Editor.hpp"
#include "../../include/api/Mode.hpp"
#include "../../include/api/Tab.hpp"
#include "../../include/ui/EditorTab.hpp"
#include "../../include/ui/TabToggler.hpp"

#include <Geode/Geode.hpp>
#include "../hooks/EditorUI.hpp"

using namespace geode::prelude;

namespace alpha::editor_tabs {

Mode* getMode(geode::ZStringView ID) {
    return ETEditorUI::get()->getMode(ID);
}

Mode* getCurrentMode() {
    return ETEditorUI::get()->getCurrentMode();
}

ZStringView getCurrentModeID() {
    return ETEditorUI::get()->getCurrentModeID();
}

Tab* getCurrentTab() {
    return ETEditorUI::get()->getCurrentTab();
}

ZStringView getCurrentTabID() {
    return ETEditorUI::get()->getCurrentTabID();
}

Mode* createMode(geode::ZStringView ID) {
    return ETEditorUI::get()->createMode(ID);
}

Tab* getTab(geode::ZStringView ID) {
    return ETEditorUI::get()->getTab(ID);
}

Tab* getTabByNode(alpha::editor_tabs::EditorTab* node) {
    return ETEditorUI::get()->getTabByNode(node);
}

void removeMode(geode::ZStringView ID) {
    ETEditorUI::get()->removeMode(ID);
}

void removeMode(Mode* mode) {
    ETEditorUI::get()->removeMode(mode);
}

std::span<const std::shared_ptr<Mode>> getAllModes() {
    return ETEditorUI::get()->getAllModes();
}

std::vector<std::shared_ptr<Tab>> getAllTabs() {
    return ETEditorUI::get()->getAllTabs();
}

void reloadAllModes() {
    ETEditorUI::get()->reloadAllModes();
}

void switchMode(ZStringView ID) {
    ETEditorUI::get()->switchMode(ID);
}

void switchMode(alpha::editor_tabs::Mode* mode) {
    ETEditorUI::get()->switchMode(mode);
}

}