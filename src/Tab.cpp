#include <Geode/Geode.hpp>
#include "EditorTab.hpp"
#include "EditorUI.hpp"
#include "Mode.hpp"
#include "Tab.hpp"

using namespace geode::prelude;
using namespace alpha::editor_tabs;

namespace internal {

struct Tab::Impl {
    std::string m_id;
    Ref<EditButtonBar> m_node;
    Ref<CCNode> m_icon;
    alpha::editor_tabs::Mode* m_mode;
    Ref<alpha::editor_tabs::TabToggler> m_toggler;

    int m_priority;
};

Tab::Impl* Tab::impl() {
    return static_cast<Tab::Impl*>(m_impl);
}

void Tab::construct(ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority) {
    m_impl = new Tab::Impl();
    impl()->m_id = ID;
    impl()->m_node = node;
    impl()->m_icon = icon;

    impl()->m_toggler = TabToggler::create(this, icon);
    impl()->m_toggler->setTag(node->m_tabIndex);
    impl()->m_toggler->setZOrder(priority);

    impl()->m_priority = priority;
}

void Tab::destruct() {
    delete impl();
}

geode::ZStringView Tab::getID() {
    return impl()->m_id;
}

void Tab::show() {
    auto internalMode = static_cast<::internal::Mode*>(impl()->m_mode);
    internalMode->switchTab(this);
}

EditButtonBar* Tab::getNode() {
    return impl()->m_node;
}

void Tab::overrideSize(bool override, int rows, int columns) {
    auto editorTab = typeinfo_cast<EditorTab*>(getNode());
    if (!editorTab) return;

    editorTab->overrideSize(override, rows, columns);
}

void Tab::overrideSize(int rows, int columns) {
    auto editorTab = typeinfo_cast<EditorTab*>(getNode());
    if (!editorTab) return;

    editorTab->overrideSize(rows, columns);
}

int Tab::getRows() {
    auto editorTab = typeinfo_cast<EditorTab*>(getNode());
    if (!editorTab) return 0;

    return editorTab->getRows();
}

int Tab::getColumns() {
    auto editorTab = typeinfo_cast<EditorTab*>(getNode());
    if (!editorTab) return 0;
    
    return editorTab->getColumns();
}

void Tab::setPriority(int priority) {
    impl()->m_priority = priority;
    impl()->m_toggler->setZOrder(priority);

    auto editor = ETEditorUI::get();
    editor->m_tabsMenu->updateLayout();
}

int Tab::getPriority() {
    return impl()->m_priority;
}

alpha::editor_tabs::Mode* Tab::getMode() {
    return impl()->m_mode;
}

void Tab::setMode(alpha::editor_tabs::Mode* mode) {
    impl()->m_mode = mode;
}

TabToggler* Tab::getTabToggle() {
    return impl()->m_toggler;
}

void Tab::reloadItems() {
    auto rows = GameManager::get()->getIntGameVariable(GameVar::EditorButtonRows);
    auto cols = GameManager::get()->getIntGameVariable(GameVar::EditorButtonsPerRow);

    getNode()->reloadItems(cols, rows);
}

void Tab::setItemless() {
    auto editorTab = typeinfo_cast<alpha::editor_tabs::EditorTab*>(getNode());
    if (!editorTab) return;

    editorTab->setItemless();
}

void Tab::removeSelf() {
    auto internalMode = static_cast<::internal::Mode*>(impl()->m_mode);
    internalMode->removeTab(this);
}

void Tab::setPageDotsClickable(bool clickable) {
    auto editorTab = typeinfo_cast<EditorTab*>(getNode());
    if (!editorTab) return;

    editorTab->setPageDotsClickable(clickable);
}

bool Tab::arePageDotsClickable() {
    auto editorTab = typeinfo_cast<EditorTab*>(getNode());
    if (!editorTab) return false;

    return editorTab->arePageDotsClickable();
}

}