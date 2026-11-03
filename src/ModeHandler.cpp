#include "ModeHandler.hpp"
#include "EditorUI.hpp"
#include "EarlyEditorUI.hpp"
#include "../include/Tab.hpp"
#include "../include/Mode.hpp"
#include "../include/Optional.hpp"

ModeHandler* ModeHandler::get() {
    auto editor = modify_cast<ETEditorUI*>(EarlyEditorUI::get());
    if (!editor) return nullptr;

    return editor->m_fields->m_modeHandler.get();
}

alpha::editor_tabs::Mode* ModeHandler::getMode(geode::ZStringView ID) {
    auto it = std::find_if(m_modes.begin(), m_modes.end(), 
    [ID] (const std::shared_ptr<alpha::editor_tabs::Mode>& mode) {
        return mode && mode->getID() == ID;
    });

    return (it != get()->m_modes.end()) ? it->get() : nullptr;
}

alpha::editor_tabs::Mode* ModeHandler::createMode(ZStringView ID) {
    return m_modes.emplace_back(std::make_shared<alpha::editor_tabs::Mode>(ID)).get();
}

void ModeHandler::switchMode(geode::ZStringView ID) {
    switchMode(getMode(ID));
}

void ModeHandler::switchMode(alpha::editor_tabs::Mode* mode) {
    for (const auto& mode : m_modes) {
        mode.get()->hideMode();
    }

    auto oldMode = getCurrentMode();
    if (oldMode && oldMode != mode) {
        auto currentTab = oldMode->getCurrentTab();
        if (currentTab) {
            alpha::editor_tabs::Tab::SwitchTabEvent(currentTab).send(false);
        }
    }

    if (mode) {
        m_currentMode = mode->getID();
        mode->show();

        if (!mode->getCurrentTab() && mode->getAllTabs().size() > 0) {
            mode->switchTab(mode->getAllTabs()[0].get());
        }

        if (mode != oldMode) {
            if (oldMode) {
                alpha::editor_tabs::Mode::SwitchModeEvent(oldMode).send(false);
            }

            auto currentTab = mode->getCurrentTab();
            if (currentTab) {
                alpha::editor_tabs::Tab::SwitchTabEvent(currentTab).send(true);
            }

            alpha::editor_tabs::Mode::SwitchModeEvent(mode).send(true);
        }

        auto editor =  modify_cast<ETEditorUI*>(EarlyEditorUI::get());

        if (mode->getID() == alpha::editor_tabs::Build) {
            editor->m_selectedMode = 2;
        }
        else if (mode->getID() == alpha::editor_tabs::Edit) {
            editor->m_selectedMode = 3;
        }
        else if (mode->getID() == alpha::editor_tabs::Delete) {
            editor->m_selectedMode = 1;
        }
        else if (mode->getID() == alpha::editor_tabs::View) {
            editor->m_selectedMode = 4;
        }
        else {
            editor->m_selectedMode = -1;
        }

        editor->updateModeToggles(editor->m_selectedMode);
    }
    else {
        m_currentMode = "";
    }
}

geode::ZStringView ModeHandler::getCurrentModeID() {
    return m_currentMode;
}

alpha::editor_tabs::Mode* ModeHandler::getCurrentMode() {
    return getMode(m_currentMode);
}

geode::ZStringView ModeHandler::getCurrentTabID() {
    auto tab = getCurrentTab();
    if (!tab) return "";

    return tab->getID();
}

alpha::editor_tabs::Tab* ModeHandler::getCurrentTab() {
    auto mode = getCurrentMode();
    if (!mode) return nullptr;

    return mode->getCurrentTab();
}

std::span<const std::shared_ptr<alpha::editor_tabs::Mode>> ModeHandler::getAllModes() {
    return m_modes;
}

std::vector<std::shared_ptr<alpha::editor_tabs::Tab>> ModeHandler::getAllTabs() {
    std::vector<std::shared_ptr<alpha::editor_tabs::Tab>> tabs;

    for (const auto& mode : m_modes) {
        auto modeTabs = mode->getAllTabs();
        tabs.insert(tabs.begin(), modeTabs.begin(), modeTabs.end());
    }

    return tabs;
}

void ModeHandler::setupModes() {
    for (const auto& mode : m_modes) {
        mode.get()->setupTabs();
    }
}

void ModeHandler::removeMode(geode::ZStringView ID) {
    removeMode(getMode(ID));
}

void ModeHandler::removeMode(alpha::editor_tabs::Mode* mode) {
    mode->removeAllTabs();

    int prevIdx = 0;
    bool isCurrentMode = m_currentMode == mode->getID();

    if (isCurrentMode) {
        for (int i = 0; i < m_modes.size(); i++) {
            if (m_modes[i].get()->getID() == mode->getID()) {
                prevIdx = i;
                break;
            }
        }
    }

    std::erase_if(m_modes, [mode] (const std::shared_ptr<alpha::editor_tabs::Mode>& m) {
        return m.get() == mode;
    });

    if (isCurrentMode) {
        if (m_modes.empty()) {
            switchMode("");
            return;
        }

        auto idx = std::clamp(prevIdx, 0, static_cast<int>(m_modes.size()) - 1);
        switchMode(m_modes[idx].get());
    }
}

void ModeHandler::reloadAllModes() {
    for (const auto& mode : m_modes) {
        mode.get()->reloadAllTabs();
    }
    switchMode(m_currentMode);
}

void ModeHandler::reloadAllModeSizes(int rows, int columns) {
    for (const auto& mode : m_modes) {
        mode.get()->reloadAllTabSizes(rows, columns);
    }
    switchMode(m_currentMode);
}

alpha::editor_tabs::Tab* ModeHandler::getTab(geode::ZStringView ID) {
    for (const auto& mode : m_modes) {
        auto tab = mode->getTab(ID);
        if (tab) return tab;
    }

    return nullptr;
}

alpha::editor_tabs::Tab* ModeHandler::getTabByNode(EditButtonBar* node) {
    for (const auto& mode : m_modes) {
        auto tab = mode->getTabByNode(node);
        if (tab) return tab;
    }
    
    return nullptr;
}