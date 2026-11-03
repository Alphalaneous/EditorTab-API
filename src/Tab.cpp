#define GEODE_DEFINE_EVENT_EXPORTS
#include "../include/Tab.hpp"
#include "../include/Optional.hpp"
#include "../include/Mode.hpp"
#include "../include/EditorTab.hpp"

#include <Geode/Geode.hpp>
#include "ModeHandler.hpp"
#include "TabToggler.hpp"
#include "EditorUI.hpp"

using namespace geode::prelude;

namespace alpha::editor_tabs {

#define OnEditorTab(method, ...) \
return ([&]() -> decltype(auto) { \
    auto editorTab = typeinfo_cast<alpha::editor_tabs::EditorTab*>((**m_impl)->m_node.data()); \
    if (!editorTab) { \
        using Ret = decltype(editorTab->method(__VA_ARGS__)); \
        if constexpr (std::is_void_v<Ret>) return; \
        else return Ret{}; \
    } \
    return editorTab->method(__VA_ARGS__); \
}())

struct Tab::Impl : public ZImpl<Impl> {
    std::string m_id;
    Ref<EditButtonBar> m_node = nullptr;
    Ref<CCNode> m_icon;
    alpha::editor_tabs::Mode* m_mode;
    Ref<alpha::editor_tabs::TabToggler> m_toggler;

    int m_priority;
};

void Tab::construct(geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority) {
    m_impl = std::make_unique<Impl>();
    (**m_impl)->m_id = ID;
    (**m_impl)->m_node = node;
    (**m_impl)->m_icon = icon;

    (**m_impl)->m_toggler = TabToggler::create(this, icon);
    (**m_impl)->m_toggler->setTag(node->m_tabIndex);
    (**m_impl)->m_toggler->setZOrder(priority);

    (**m_impl)->m_priority = priority;
}

geode::ZStringView Tab::getID() {
    return (**m_impl)->m_id;
}

void Tab::show() {
    (**m_impl)->m_mode->switchTab(this);
}

EditButtonBar* Tab::getNode() {
    return (**m_impl)->m_node;
}

CCNode* Tab::getIcon() {
    return (**m_impl)->m_icon;
}

void Tab::setPriority(int priority) {
    (**m_impl)->m_priority = priority;
    (**m_impl)->m_toggler->setZOrder(priority);

    auto editor = ETEditorUI::get();
    editor->m_tabsMenu->updateLayout();}

int Tab::getPriority() {
    return (**m_impl)->m_priority;
}

Result<unsigned int> Tab::getIndex() {
    return (**m_impl)->m_mode->getTabIndex(this);
}

alpha::editor_tabs::Mode* Tab::getMode() {
    return (**m_impl)->m_mode;
}

CCMenuItemToggler* Tab::getTabToggle() {
    return (**m_impl)->m_toggler;
}

void Tab::removeSelf() {
    (**m_impl)->m_mode->removeTab(this);
}

void Tab::overrideSize(bool override, int rows, int columns) {
    OnEditorTab(overrideSize, override, rows, columns);
}

void Tab::overrideSize(int rows, int columns) {
    OnEditorTab(overrideSize, rows, columns);
}

int Tab::getRows() {
    OnEditorTab(getRows);
}

int Tab::getColumns() {    
    OnEditorTab(getColumns);
}

void Tab::reloadItems() {
    OnEditorTab(reloadItems);
}

void Tab::setItemless(bool itemless) {
    OnEditorTab(setItemless, itemless);
}

bool Tab::isItemless() {
    OnEditorTab(isItemless);
}

void Tab::setAutoScale(bool enabled) {
    OnEditorTab(setAutoScale, enabled);
}

bool Tab::hasAutoScale() {
    OnEditorTab(hasAutoScale);
}

void Tab::addItem(cocos2d::CCNode* item) {
    OnEditorTab(addItem, item);
}

void Tab::addItems(std::span<cocos2d::CCNode*> items) {
    OnEditorTab(addItems, items);
}

void Tab::addItems(std::span<geode::Ref<cocos2d::CCNode>> items) {
    OnEditorTab(addItems, items);
}

void Tab::removeItem(cocos2d::CCNode* item) {
    OnEditorTab(removeItem, item);
}

void Tab::removeItems(std::span<cocos2d::CCNode*> items) {
    OnEditorTab(removeItems, items);
}

void Tab::removeItems(std::span<geode::Ref<cocos2d::CCNode>> items) {
    OnEditorTab(removeItems, items);
}

void Tab::removeItemByID(geode::ZStringView ID) {
    OnEditorTab(removeItemByID, ID);
}

void Tab::removeItemByTag(int tag) {
    OnEditorTab(removeItemByTag, tag);
}

void Tab::removeItemByObjectID(int objectID) {
    OnEditorTab(removeItemByObjectID, objectID);
}

void Tab::insertItem(cocos2d::CCNode* item, unsigned int index) {
    OnEditorTab(insertItem, item, index);
}

void Tab::insertItems(std::span<cocos2d::CCNode*> items, unsigned int index) {
    OnEditorTab(insertItems, items, index);
}

void Tab::insertItems(std::span<geode::Ref<cocos2d::CCNode>> items, unsigned int index) {
    OnEditorTab(insertItems, items, index);
}

void Tab::insertItemAfter(cocos2d::CCNode* item, cocos2d::CCNode* after) {
    OnEditorTab(insertItemAfter, item, after);
}

void Tab::insertItemsAfter(std::span<cocos2d::CCNode*> items, cocos2d::CCNode* after) {
    OnEditorTab(insertItemsAfter, items, after);
}

void Tab::insertItemsAfter(std::span<geode::Ref<cocos2d::CCNode>> items, cocos2d::CCNode* after) {
    OnEditorTab(insertItemsAfter, items, after);
}

void Tab::insertItemBefore(cocos2d::CCNode* item, cocos2d::CCNode* before) {
    OnEditorTab(insertItemBefore, item, before);
}

void Tab::insertItemsBefore(std::span<cocos2d::CCNode*> items, cocos2d::CCNode* before) {
    OnEditorTab(insertItemsBefore, items, before);
}

void Tab::insertItemsBefore(std::span<geode::Ref<cocos2d::CCNode>> items, cocos2d::CCNode* before) {
    OnEditorTab(insertItemsBefore, items, before);
}

void Tab::insertItemAfter(CreateMenuItem* item, int objectID) {
    OnEditorTab(insertItemAfter, item, objectID);
}

void Tab::insertItemsAfter(std::span<CreateMenuItem*> items, int objectID) {
    OnEditorTab(insertItemsAfter, items, objectID);
}

void Tab::insertItemsAfter(std::span<geode::Ref<CreateMenuItem>> items, int objectID) {
    OnEditorTab(insertItemsAfter, items, objectID);
}

void Tab::insertItemBefore(CreateMenuItem* item, int objectID) {
    OnEditorTab(insertItemBefore, item, objectID);
}

void Tab::insertItemsBefore(std::span<CreateMenuItem*> items, int objectID) {
    OnEditorTab(insertItemsBefore, items, objectID);
}

void Tab::insertItemsBefore(std::span<geode::Ref<CreateMenuItem>> items, int objectID) {
    OnEditorTab(insertItemsBefore, items, objectID);
}

cocos2d::CCNode* Tab::findItemByID(geode::ZStringView ID) {
    OnEditorTab(findItemByID, ID);
}

cocos2d::CCNode* Tab::findItemByTag(int tag) {
    OnEditorTab(findItemByTag, tag);
}

CreateMenuItem* Tab::findItemByObjectID(int objectID) {
    OnEditorTab(findItemByObjectID, objectID);
}

void Tab::setTabAlignment(alpha::editor_tabs::Tab::TabAlignment alignment) {
    OnEditorTab(setTabAlignment, alignment);
}

alpha::editor_tabs::Tab::TabAlignment Tab::getTabAlignment() {
    OnEditorTab(getTabAlignment);
}

void Tab::disableSizeOverrideIfExceeded(bool disable) {
    OnEditorTab(disableSizeOverrideIfExceeded, disable);
}

bool Tab::isDisableSizeOverrideIfExceeded() {
    OnEditorTab(isDisableSizeOverrideIfExceeded);
}

void Tab::setHorizontalButtonAlignment(geode::AxisAlignment alignment) {
    OnEditorTab(setHorizontalButtonAlignment, alignment);
}

geode::AxisAlignment Tab::getHorizontalButtonAlignment() {
    OnEditorTab(getHorizontalButtonAlignment);
}

void Tab::setVerticalButtonAlignment(geode::AxisAlignment alignment) {
    OnEditorTab(setVerticalButtonAlignment, alignment);
}

geode::AxisAlignment Tab::getVerticalButtonAlignment() {
    OnEditorTab(getVerticalButtonAlignment);
}

void Tab::setMode(alpha::editor_tabs::Mode* mode) {
    (**m_impl)->m_mode = mode;
}

}