#include "Mode.hpp"

#include <Geode/Geode.hpp>
#include "EditorUI.hpp"
#include "Tab.hpp"
#include "ModeHandler.hpp"
#include "EditorTab.hpp"

using namespace geode::prelude;
using namespace alpha::editor_tabs;

namespace internal {

struct Mode::Impl {
    std::string m_id;
    std::vector<std::shared_ptr<alpha::editor_tabs::Tab>> m_tabs;
    std::string m_currentTab;
};

Mode::Impl* Mode::impl() {
    return static_cast<Mode::Impl*>(m_impl);
}

Mode::Mode(ZStringView ID) {
    m_impl = new Mode::Impl();
    impl()->m_id = ID;
}

Mode::~Mode() {
    delete impl();
}

geode::ZStringView Mode::getID() {
    return impl()->m_id;
}

void Mode::show() {
    ModeHandler::get()->switchMode(this);
}

alpha::editor_tabs::Tab* Mode::createTab(geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority) {
    auto tab = impl()->m_tabs.emplace_back(std::make_shared<::internal::Tab>(ID, node, icon, priority));

    auto internalTab = static_cast<::internal::Tab*>(tab.get());
    internalTab->setMode(this);

    auto editor = ETEditorUI::get();

    node->setZOrder(10);
    node->setVisible(false);
    node->setID(fmt::format("{}-tab-bar", ID));

    auto toggle = internalTab->getTabToggle();
    toggle->setID(fmt::format("{}-tab", ID));

    if (editor->initialized()) {
        if (impl()->m_tabs.size() == 1) {
            impl()->m_currentTab = ID;
        }

        editor->addChild(node);

        if (impl()->m_id == alpha::editor_tabs::Build) {
            editor->m_createButtonBars->addObject(node);
        }

        internalTab->reloadItems();
        tab::TabInitializedEvent(internalTab).send();
    }

    auto editorTab = typeinfo_cast<alpha::editor_tabs::EditorTab*>(node);
    if (editorTab) {
        editorTab->setTab(tab.get());
    }

    return tab.get();
}

void Mode::removeTab(ZStringView ID) {
    removeTab(getTab(ID));
}

void Mode::removeTab(alpha::editor_tabs::Tab* tab) {
    if (!tab) return;

    auto editor = ETEditorUI::get();
    auto internalTab = static_cast<::internal::Tab*>(tab);

    if (impl()->m_id == alpha::editor_tabs::Build) {
        editor->m_createButtonBars->removeObject(internalTab->getNode());
    }

    internalTab->getNode()->removeFromParent();
    internalTab->getTabToggle()->removeFromParent();

    editor->m_tabsMenu->updateLayout();

    unsigned int prevIdx = 0;

    bool isCurrentTab = impl()->m_currentTab == internalTab->getID();

    if (isCurrentTab) {
        for (unsigned int i = 0; i < impl()->m_tabs.size(); i++) {
            auto internalTabInner = static_cast<::internal::Tab*>(impl()->m_tabs[i].get());
            if (internalTabInner->getID() == internalTab->getID()) {
                prevIdx = i;
                break;
            }
        }
    }

    std::erase_if(impl()->m_tabs, [tab](const std::shared_ptr<alpha::editor_tabs::Tab>& t) {
        return t.get() == tab;
    });

    if (isCurrentTab) {
        if (impl()->m_tabs.empty()) {
            switchTab("");
            return;
        }

        auto idx = std::clamp(prevIdx, 0u, static_cast<unsigned int>(impl()->m_tabs.size()) - 1);

        auto internalTab = static_cast<::internal::Tab*>(impl()->m_tabs[idx].get());
        switchTab(internalTab);
    }
}

alpha::editor_tabs::Tab* Mode::getTab(geode::ZStringView ID) {
    auto it = std::find_if(impl()->m_tabs.begin(), impl()->m_tabs.end(), 
    [ID](const std::shared_ptr<alpha::editor_tabs::Tab>& t) {
        auto internalTab = static_cast<::internal::Tab*>(t.get());
        return t && internalTab->getID() == ID;
    });

    return (it != impl()->m_tabs.end()) ? it->get() : nullptr;
}

alpha::editor_tabs::Tab* Mode::getTabByNode(EditButtonBar* node) {
    auto it = std::find_if(impl()->m_tabs.begin(), impl()->m_tabs.end(), 
    [node](const std::shared_ptr<alpha::editor_tabs::Tab>& t) {
        auto internalTab = static_cast<::internal::Tab*>(t.get());
        return t && internalTab->getNode() == node;
    });

    return (it != impl()->m_tabs.end()) ? it->get() : nullptr;
}

alpha::editor_tabs::Tab* Mode::getTabByIndex(unsigned int index) {
    if (index >= impl()->m_tabs.size()) return nullptr;
    return impl()->m_tabs[index].get();
}

alpha::editor_tabs::Tab* Mode::getCurrentTab() {
    return getTab(impl()->m_currentTab);
}

std::span<const std::shared_ptr<alpha::editor_tabs::Tab>> Mode::getAllTabs() {
    return impl()->m_tabs;
}

void Mode::switchTab(ZStringView ID) {
    switchTab(getTab(ID));
}

void Mode::switchTab(unsigned int index) {
    switchTab(getTabByIndex(index));
}

void Mode::switchTab(alpha::editor_tabs::Tab* tab) {
    auto handler = ModeHandler::get();
    if (!handler) return;

    bool isActive = handler->getCurrentMode() == this;

    for (const auto& tab : impl()->m_tabs) {
        auto internalTab = static_cast<::internal::Tab*>(tab.get());
        internalTab->getNode()->setVisible(false);
        internalTab->getTabToggle()->toggleTab(false);
    }

    auto editor = ETEditorUI::get();
    auto oldTab = getCurrentTab();

    if (tab) {
        auto internalTab = static_cast<::internal::Tab*>(tab);
        impl()->m_currentTab = internalTab->getID();

        if (isActive && oldTab != tab) {
            if (oldTab) {
                tab::SwitchTabEvent(oldTab).send(false);
            }

            tab::SwitchTabEvent(tab).send(true);
        }

        internalTab->getNode()->setVisible(true && isActive);
        internalTab->getTabToggle()->toggleTab(true);

        if (impl()->m_id == alpha::editor_tabs::Build) {
            auto editor = ETEditorUI::get();
            editor->m_createButtonBar = internalTab->getNode();
        }

        if (isActive) {
            editor->m_selectedTab = getTabIndex(internalTab).unwrapOrDefault();
        }
    }
    else {
        impl()->m_currentTab = "";
        if (isActive) {
            editor->m_selectedTab = 0;
        }
    }
}

geode::Result<unsigned int> Mode::getTabIndex(geode::ZStringView ID) {
    return getTabIndex(getTab(ID));
}

geode::Result<unsigned int> Mode::getTabIndex(alpha::editor_tabs::Tab* tab) {
    unsigned int idx = 0;
    for (const auto& tabInner : impl()->m_tabs) {
        if (tabInner.get() == tab) {
            return Ok(idx);
        }
        idx++;
    }

    return Err("Index not found for tab");
}

void Mode::hideMode() {
    auto editor = ETEditorUI::get();

    for (const auto& tab : impl()->m_tabs) {
        auto internalTab = static_cast<::internal::Tab*>(tab.get());
        internalTab->getNode()->setVisible(false);

        auto toggle = internalTab->getTabToggle();
        toggle->toggleTab(false);
        toggle->removeFromParent();
        editor->m_tabsArray->removeObject(toggle);
    }

    editor->m_tabsMenu->updateLayout();
}

void Mode::showMode() {
    auto editor = ETEditorUI::get();

    for (const auto& tab : impl()->m_tabs) {
        auto internalTabInner = static_cast<::internal::Tab*>(tab.get());
        internalTabInner->getNode()->setVisible(false);

        auto toggle = internalTabInner->getTabToggle();
        toggle->toggleTab(false);
        toggle->removeFromParent();
        editor->m_tabsMenu->addChild(toggle);
        editor->m_tabsArray->addObject(toggle);
    }

    auto internalTab = static_cast<::internal::Tab*>(getCurrentTab());

    if (internalTab) {
        internalTab->getNode()->setVisible(true);
        internalTab->getTabToggle()->toggleTab(true);
    }

    editor->m_selectedTab = getTabIndex(internalTab).unwrapOrDefault();
    editor->m_tabsMenu->updateLayout();
    editor->m_tabsMenu->setVisible(impl()->m_tabs.size() > 1);
}

void Mode::updateToggles() {
    for (const auto& tab : impl()->m_tabs) {
        auto internalTab = static_cast<::internal::Tab*>(tab.get());
        internalTab->getTabToggle()->toggleTab(false);
    }

    auto internalTab = static_cast<::internal::Tab*>(getCurrentTab());

    if (internalTab) {
        internalTab->getTabToggle()->toggleTab(true);
    }
}

void Mode::setupTabs() {
    auto editor = ETEditorUI::get();

    for (const auto& tab : impl()->m_tabs) {
        auto internalTab = static_cast<::internal::Tab*>(tab.get());
        auto node = internalTab->getNode();
        node->setContentSize(EditorTab::getMaxSize());

        editor->addChild(node);

        if (impl()->m_id == alpha::editor_tabs::Build) {
            editor->m_createButtonBars->addObject(node);
        }

        internalTab->reloadItems();
        tab::TabInitializedEvent(internalTab).send();
    }
}

void Mode::removeAllTabs() {
    auto editor = ETEditorUI::get();
    
    for (const auto& tab : impl()->m_tabs) {
        auto internalTab = static_cast<::internal::Tab*>(tab.get());

        internalTab->getNode()->removeFromParent();
        internalTab->getTabToggle()->removeFromParent();
    }

    impl()->m_tabs.clear();
}

void Mode::reloadAllTabs() {
    for (const auto& tab : impl()->m_tabs) {
        auto internalTab = static_cast<::internal::Tab*>(tab.get());
        internalTab->reloadItems();
    }
}

void Mode::removeSelf() {
    ModeHandler::get()->removeMode(this);
}

}