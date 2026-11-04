#define GEODE_DEFINE_EVENT_EXPORTS
#include "../../include/api/Tab.hpp"
#include "../../include/api/Editor.hpp"
#include "../../include/api/Mode.hpp"
#include "../../include/ui/EditorTab.hpp"
#include "../../include/ui/TabToggler.hpp"

#include <Geode/Geode.hpp>
#include "../hooks/EditorUI.hpp"

using namespace geode::prelude;

namespace alpha::editor_tabs {

struct Tab::Impl : public ZImpl<Impl> {
    std::string m_id = "";
    Ref<EditorTab> m_node = nullptr;
    Ref<CCNode> m_icon = nullptr;
    alpha::editor_tabs::Mode* m_mode = nullptr;
    Ref<alpha::editor_tabs::TabToggler> m_toggler = nullptr;

    int m_priority = 0;
};

void Tab::construct(geode::ZStringView ID, alpha::editor_tabs::EditorTab* node, cocos2d::CCNode* icon, int priority) {
    m_impl = std::make_unique<Impl>();
    m_impl->get()->m_id = ID;
    m_impl->get()->m_node = node;
    m_impl->get()->m_icon = icon;

    m_impl->get()->m_toggler = TabToggler::create(this, icon);
    m_impl->get()->m_toggler->setTag(node->m_tabIndex);
    m_impl->get()->m_toggler->setZOrder(priority);

    m_impl->get()->m_priority = priority;
}

geode::ZStringView Tab::getID() {
    return m_impl->get()->m_id;
}

void Tab::show() {
    m_impl->get()->m_mode->switchTab(this);
}

alpha::editor_tabs::EditorTab* Tab::getNode() {
    return m_impl->get()->m_node;
}

CCNode* Tab::getIcon() {
    return m_impl->get()->m_icon;
}

void Tab::setPriority(int priority) {
    m_impl->get()->m_priority = priority;
    m_impl->get()->m_toggler->setZOrder(priority);

    auto editor = ETEditorUI::get();
    editor->m_tabsMenu->updateLayout();}

int Tab::getPriority() {
    return m_impl->get()->m_priority;
}

Result<unsigned int> Tab::getIndex() {
    return m_impl->get()->m_mode->getTabIndex(this);
}

alpha::editor_tabs::Mode* Tab::getMode() {
    return m_impl->get()->m_mode;
}

CCMenuItemToggler* Tab::getTabToggle() {
    return m_impl->get()->m_toggler;
}

void Tab::removeSelf() {
    m_impl->get()->m_mode->removeTab(this);
}

void Tab::setMode(alpha::editor_tabs::Mode* mode) {
    m_impl->get()->m_mode = mode;
}

}