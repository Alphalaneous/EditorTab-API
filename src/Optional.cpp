#define GEODE_DEFINE_EVENT_EXPORTS
#include "../include/Optional.hpp"
#include "../include/Mode.hpp"
#include "../include/Tab.hpp"
#include "../include/EditorTab.hpp"

#include <Geode/Geode.hpp>
#include "ModeHandler.hpp"
#include "TabToggler.hpp"

using namespace geode::prelude;

namespace alpha::editor_tabs {

EditButtonBar* createTabBar(cocos2d::CCArray* items, int tab, bool hasCreateItems) {
    return EditorTab::create(items, tab, hasCreateItems);
}

EditButtonBar* createTabBar(std::span<Ref<CCNode>> items, int tab, bool hasCreateItems) {
    return EditorTab::create(items, tab, hasCreateItems);
}

EditButtonBar* createTabBar(std::span<CCNode*> items, int tab, bool hasCreateItems) {
    return EditorTab::create(items, tab, hasCreateItems);
}

EditButtonBar* createTabNode() {
    return EditorTab::create();
}

cocos2d::CCSize getMaxTabSize() {
    return EditorTab::getMaxSize();
}

Mode* getMode(geode::ZStringView ID) {
    OnHandler(getMode, ID);
}

Mode* getCurrentMode() {
    OnHandler(getCurrentMode);
}

ZStringView getCurrentModeID() {
    OnHandler(getCurrentModeID);
}

Tab* getCurrentTab() {
    OnHandler(getCurrentTab);
}

ZStringView getCurrentTabID() {
    OnHandler(getCurrentTabID);
}

Mode* createMode(geode::ZStringView ID) {
    OnHandler(createMode, ID);
}

Tab* getTab(geode::ZStringView ID) {
    OnHandler(getTab, ID);
}

Tab* getTabByNode(EditButtonBar* node) {
    OnHandler(getTabByNode, node);
}

void removeMode(geode::ZStringView ID) {
    OnHandler(removeMode, ID);
}

void removeMode(Mode* mode) {
    OnHandler(removeMode, mode);
}

std::span<const std::shared_ptr<Mode>> getAllModes() {
    OnHandler(getAllModes);
}

std::vector<std::shared_ptr<Tab>> getAllTabs() {
    OnHandler(getAllTabs);
}

void reloadAllModes() {
    OnHandler(reloadAllModes);
}

void switchMode(ZStringView ID) {
    OnHandler(switchMode, ID);
}

void switchMode(alpha::editor_tabs::Mode* mode) {
    OnHandler(switchMode, mode);
}

}