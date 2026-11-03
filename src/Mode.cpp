#define GEODE_DEFINE_EVENT_EXPORTS
#include "../include/Mode.hpp"
#include "../include/Optional.hpp"
#include "../include/Tab.hpp"
#include "../include/EditorTab.hpp"

#include <Geode/Geode.hpp>
#include "ModeHandler.hpp"
#include "TabToggler.hpp"
#include "EditorUI.hpp"

using namespace geode::prelude;

namespace alpha::editor_tabs {

struct Mode::Impl : public ZImpl<Mode::Impl> {
    std::string m_id;
    std::vector<std::shared_ptr<alpha::editor_tabs::Tab>> m_tabs;
    std::string m_currentTab;
};

void Mode::construct(geode::ZStringView ID) {
    m_impl = std::make_unique<Impl>();
    (**m_impl)->m_id = ID;
}

geode::ZStringView Mode::getID() {
    return (**m_impl)->m_id;
}

void Mode::show() {
    ModeHandler::get()->switchMode(this);
}

alpha::editor_tabs::Tab* Mode::createTab(geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon) {
    return createTab(ID, node, icon, 0);
}

alpha::editor_tabs::Tab* Mode::createTab(geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority) {
    auto tab = (**m_impl)->m_tabs.emplace_back(std::make_shared<Tab>(ID, node, icon, priority));
    static_cast<Tab*>(tab.get())->setMode(this);

    auto editor = ETEditorUI::get();

    node->setZOrder(10);
    node->setVisible(false);
    node->setID(fmt::format("{}-tab-bar", ID));

    auto toggle = static_cast<TabToggler*>(tab->getTabToggle());
    toggle->setID(fmt::format("{}-tab", ID));

    auto editorTab = typeinfo_cast<alpha::editor_tabs::EditorTab*>(node);
    if (editorTab) {
        editorTab->setTab(tab.get());
    }

    if (editor->initialized()) {
        if ((**m_impl)->m_tabs.size() == 1) {
            (**m_impl)->m_currentTab = ID;
        }

        editor->addChild(node);

        node->setContentSize(alpha::editor_tabs::EditorTab::getMaxSize());
        tab->reloadItems();
        
        Tab::TabInitializedEvent(tab.get()).send();

        auto handler = ModeHandler::get();
        if (handler) {
            bool isActive = handler->getCurrentMode() == this;
            node->setVisible(isActive && (**m_impl)->m_currentTab == ID && !editor->m_uiHidden);
        }
    }

    return tab.get();
}

void Mode::removeTab(geode::ZStringView ID) {
    removeTab(getTab(ID));
}

void Mode::removeTab(alpha::editor_tabs::Tab* tab) {
    if (!tab) return;

    auto editor = ETEditorUI::get();

    if ((**m_impl)->m_id == alpha::editor_tabs::Build) {
        editor->m_createButtonBars->removeObject(tab->getNode());
    }

    tab->getNode()->removeFromParent();
    tab->getTabToggle()->removeFromParent();

    editor->m_tabsMenu->updateLayout();

    unsigned int prevIdx = 0;

    bool isCurrentTab = (**m_impl)->m_currentTab == tab->getID();

    if (isCurrentTab) {
        for (unsigned int i = 0; i < (**m_impl)->m_tabs.size(); i++) {
            if ((**m_impl)->m_tabs[i].get()->getID() == tab->getID()) {
                prevIdx = i;
                break;
            }
        }
    }

    std::erase_if((**m_impl)->m_tabs, [tab] (const std::shared_ptr<alpha::editor_tabs::Tab>& t) {
        return t.get() == tab;
    });

    if (isCurrentTab) {
        if ((**m_impl)->m_tabs.empty()) {
            switchTab("");
            return;
        }

        auto idx = std::clamp(prevIdx, 0u, static_cast<unsigned int>((**m_impl)->m_tabs.size()) - 1);
        switchTab((**m_impl)->m_tabs[idx].get());
    }
}

Tab* Mode::getTab(geode::ZStringView ID) {
    auto it = std::find_if((**m_impl)->m_tabs.begin(), (**m_impl)->m_tabs.end(), 
    [ID] (const std::shared_ptr<alpha::editor_tabs::Tab>& tab) {
        return tab && tab->getID() == ID;
    });

    return (it != (**m_impl)->m_tabs.end()) ? it->get() : nullptr;
}

Tab* Mode::getTabByNode(EditButtonBar* node) {
    auto it = std::find_if((**m_impl)->m_tabs.begin(), (**m_impl)->m_tabs.end(), 
    [node] (const std::shared_ptr<alpha::editor_tabs::Tab>& tab) {
        return tab && tab->getNode() == node;
    });

    return (it != (**m_impl)->m_tabs.end()) ? it->get() : nullptr;
}

Tab* Mode::getTabByIndex(unsigned int index) {
    if (index >= (**m_impl)->m_tabs.size()) return nullptr;
    return (**m_impl)->m_tabs[index].get();
}

Tab* Mode::getCurrentTab() {
    return getTab((**m_impl)->m_currentTab);
}

Result<unsigned int> Mode::getTabIndex(ZStringView ID) {
    return getTabIndex(getTab(ID));
}

Result<unsigned int> Mode::getTabIndex(Tab* tab) {
    unsigned int idx = 0;
    for (const auto& tabInner : (**m_impl)->m_tabs) {
        if (tabInner.get() == tab) {
            return Ok(idx);
        }
        idx++;
    }

    return Err("Index not found for tab");}

std::span<const std::shared_ptr<Tab>> Mode::getAllTabs() {
    return (**m_impl)->m_tabs;
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
    auto editor = ETEditorUI::get();

    bool isActive = handler->getCurrentMode() == this && !editor->m_uiHidden;

    for (const auto& tab : (**m_impl)->m_tabs) {
        tab->getNode()->setVisible(false);
        static_cast<TabToggler*>(tab->getTabToggle())->toggleTab(false);
    }

    auto oldTab = getCurrentTab();

    if (tab) {
        (**m_impl)->m_currentTab = tab->getID();

        if (isActive && oldTab != tab) {
            if (oldTab) {
                Tab::SwitchTabEvent(oldTab).send(false);
            }

            Tab::SwitchTabEvent(tab).send(true);
        }

        tab->getNode()->setVisible(isActive);
        static_cast<TabToggler*>(tab->getTabToggle())->toggleTab(true);

        if ((**m_impl)->m_id == alpha::editor_tabs::Build) {
            editor->m_createButtonBar = tab->getNode();
        }

        if (isActive) {
            editor->m_selectedTab = getTabIndex(tab).unwrapOrDefault();
        }
    }
    else {
        (**m_impl)->m_currentTab = "";
        if (isActive) {
            editor->m_selectedTab = 0;
        }
    }
}

void Mode::hideMode() {
    auto editor = ETEditorUI::get();

    for (const auto& tab : (**m_impl)->m_tabs) {
        tab->getNode()->setVisible(false);

        auto toggle = static_cast<TabToggler*>(tab->getTabToggle());
        toggle->toggleTab(false);
        toggle->removeFromParent();
        editor->m_tabsArray->removeObject(toggle);
    }

    editor->m_tabsMenu->updateLayout();
}

void Mode::showMode() {
    auto editor = ETEditorUI::get();

    for (const auto& tab : (**m_impl)->m_tabs) {
        tab->getNode()->setVisible(false);

        auto toggle = static_cast<TabToggler*>(tab->getTabToggle());
        toggle->toggleTab(false);
        toggle->removeFromParent();
        editor->m_tabsMenu->addChild(toggle);
        editor->m_tabsArray->addObject(toggle);
    }

    auto tab = getCurrentTab();

    if (tab) {
        tab->getNode()->setVisible(!editor->m_uiHidden);
        static_cast<TabToggler*>(tab->getTabToggle())->toggleTab(true);
    }

    editor->m_selectedTab = getTabIndex(tab).unwrapOrDefault();
    editor->m_tabsMenu->updateLayout();
    editor->m_tabsMenu->setVisible((**m_impl)->m_tabs.size() > 1 && !editor->m_uiHidden);
}

void Mode::updateToggles() {
    for (const auto& tab : (**m_impl)->m_tabs) {
        static_cast<TabToggler*>(tab->getTabToggle())->toggleTab(false);
    }

    auto tab = getCurrentTab();
    if (tab) {
        static_cast<TabToggler*>(tab->getTabToggle())->toggleTab(true);
    }
}

void Mode::setupTabs() {
    auto editor = ETEditorUI::get();

    for (const auto& tab : (**m_impl)->m_tabs) {
        auto node = tab->getNode();
        node->setContentSize(alpha::editor_tabs::EditorTab::getFullMaxSize());

        editor->addChild(node);

        tab->reloadItems();
        Tab::TabInitializedEvent(tab.get()).send();
    }
}

void Mode::removeAllTabs() {
    auto editor = ETEditorUI::get();
    
    for (const auto& tab : (**m_impl)->m_tabs) {
        tab->getNode()->removeFromParent();
        tab->getTabToggle()->removeFromParent();
    }

    (**m_impl)->m_tabs.clear();
}

void Mode::reloadAllTabs() {
    for (const auto& tab : (**m_impl)->m_tabs) {
        tab->reloadItems();
    }
}

void Mode::reloadAllTabSizes(int rows, int columns) {
    for (const auto& tab : (**m_impl)->m_tabs) {
        static_cast<EditorTab*>(tab->getNode())->updateItemSize(rows, columns);
    }
}

void Mode::removeSelf() {
    ModeHandler::get()->removeMode(this);
}

}