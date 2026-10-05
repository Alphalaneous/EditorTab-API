#include <memory>
#define GEODE_DEFINE_EVENT_EXPORTS
#include <Geode/Geode.hpp>
#include "../include/Optional.hpp"
#include "../include/Mode.hpp"
#include "../include/Tab.hpp"
#include "EditorTab.hpp"
#include "ModeHandler.hpp"
#include "Mode.hpp"
#include "Tab.hpp"

using namespace geode::prelude;

namespace alpha::editor_tabs {

namespace mode {

::internal::Mode* shadow(Mode* mode) {
    return reinterpret_cast<::internal::Mode*>(mode);
}

geode::ZStringView getID(Mode* mode) {
    return shadow(mode)->getID();
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

Tab* getTabByNode(Mode* mode, EditButtonBar* node) {
    return shadow(mode)->getTabByNode(node);
}

Tab* getTabByIndex(Mode* mode, unsigned int index) {
    return shadow(mode)->getTabByIndex(index);
}

Tab* getCurrentTab(Mode* mode) {
    return shadow(mode)->getCurrentTab();
}

Result<unsigned int> getTabIndex(Mode* mode, ZStringView ID) {
    return shadow(mode)->getTabIndex(ID);
}

Result<unsigned int> getTabIndex(Mode* mode, Tab* tab) {
    return shadow(mode)->getTabIndex(tab);
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
    return reinterpret_cast<::internal::Tab*>(tab);
}

geode::ZStringView getID(Tab* tab) {
    return shadow(tab)->getID();
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

Result<unsigned int> getIndex(Tab* tab) {
    return shadow(tab)->getIndex();
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
    shadow(tab)->reloadItems();
}

void setItemless(Tab* tab, bool itemless) {
    shadow(tab)->setItemless(itemless);
}

bool isItemless(Tab* tab) {
    return shadow(tab)->isItemless();
}

void setAutoScale(Tab* tab, bool enabled) {
    shadow(tab)->setAutoScale(enabled);
}

bool hasAutoScale(Tab* tab) {
    return shadow(tab)->hasAutoScale();
}

void removeSelf(Tab* tab) {
    shadow(tab)->removeSelf();
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

Tab* getCurrentTab() {
    auto handler = ModeHandler::get();
    if (!handler) return nullptr;

    return handler->getCurrentTab();
}

ZStringView getCurrentTabID() {
    auto handler = ModeHandler::get();
    if (!handler) return nullptr;

    return handler->getCurrentTabID();
}

Mode* createMode(geode::ZStringView ID) {
    auto handler = ModeHandler::get();
    if (!handler) return nullptr;

    return handler->createMode(ID);
}

Tab* getTab(geode::ZStringView ID) {
    auto handler = ModeHandler::get();
    if (!handler) return nullptr;

    return handler->getTab(ID);
}

Tab* getTabByNode(EditButtonBar* node) {
    auto handler = ModeHandler::get();
    if (!handler) return nullptr;

    return handler->getTabByNode(node);
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

std::vector<std::shared_ptr<Tab>> getAllTabs() {
    auto handler = ModeHandler::get();
    if (!handler) return {};

    return handler->getAllTabs();
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

cocos2d::CCSize getMaxTabSize() {
    return EditorTab::getMaxSize();
}

}