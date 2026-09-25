#include "ModeHandler.hpp"
#include "EditorUI.hpp"
#include "Mode.hpp"

ModeHandler* ModeHandler::get() {
    auto editor = ETEditorUI::get();
    if (!editor) return nullptr;

    return editor->m_fields->m_modeHandler.get();
}

alpha::editor_tabs::Mode* ModeHandler::getMode(geode::ZStringView ID) {
    auto it = std::find_if(m_modes.begin(), m_modes.end(), 
    [ID](const std::shared_ptr<alpha::editor_tabs::Mode>& t) {
        auto internalMode = static_cast<::internal::Mode*>(t.get());
        return t && internalMode->getID() == ID;
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
        auto internalMode = static_cast<::internal::Mode*>(mode.get());
        internalMode->hideMode();
    }

    auto oldMode = static_cast<::internal::Mode*>(getCurrentMode());
    if (oldMode && oldMode != mode) {
        auto currentTab = oldMode->getCurrentTab();
        if (currentTab) {
            tab::SwitchTabEvent(currentTab).send(false);
        }
    }

    if (mode) {
        auto internalMode = static_cast<::internal::Mode*>(mode);
        m_currentMode = internalMode->getID();
        internalMode->showMode();

        if (mode != oldMode) {
            if (oldMode) {
                mode::SwitchModeEvent(oldMode).send(false);
            }

            auto currentTab = mode->getCurrentTab();
            if (currentTab) {
                tab::SwitchTabEvent(currentTab).send(true);
            }

            mode::SwitchModeEvent(mode).send(true);
        }

        auto editor = ETEditorUI::get();

        if (internalMode->getID() == alpha::editor_tabs::Build) {
            editor->m_selectedMode = 2;
        }
        else if (internalMode->getID() == alpha::editor_tabs::Edit) {
            editor->m_selectedMode = 3;
        }
        else if (internalMode->getID() == alpha::editor_tabs::Delete) {
            editor->m_selectedMode = 1;
        }
        else if (internalMode->getID() == alpha::editor_tabs::View) {
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

std::span<const std::shared_ptr<alpha::editor_tabs::Mode>> ModeHandler::getAllModes() {
    return m_modes;
}

void ModeHandler::setupModes() {
    for (const auto& mode : m_modes) {
        auto internalMode = static_cast<::internal::Mode*>(mode.get());
        internalMode->setupTabs();
    }
}

void ModeHandler::removeMode(geode::ZStringView ID) {
    removeMode(getMode(ID));
}

void ModeHandler::removeMode(alpha::editor_tabs::Mode* mode) {
    auto internalMode = static_cast<::internal::Mode*>(mode);
    internalMode->removeAllTabs();

    int prevIdx = 0;

    bool isCurrentMode = m_currentMode == internalMode->getID();

    if (isCurrentMode) {
        for (int i = 0; i < m_modes.size(); i++) {
            auto internalModeInner = static_cast<::internal::Mode*>(m_modes[i].get());
            if (internalModeInner->getID() == internalMode->getID()) {
                prevIdx = i;
                break;
            }
        }
    }

    std::erase_if(m_modes, [mode](const std::shared_ptr<alpha::editor_tabs::Mode>& m) {
        return m.get() == mode;
    });

    if (isCurrentMode) {
        if (m_modes.empty()) {
            switchMode("");
            return;
        }

        auto idx = std::clamp(prevIdx, 0, static_cast<int>(m_modes.size()) - 1);

        auto internalMode = static_cast<::internal::Mode*>(m_modes[idx].get());
        switchMode(internalMode);
    }
}

void ModeHandler::reloadAllModes() {
    for (const auto& mode : m_modes) {
        auto internalMode = static_cast<::internal::Mode*>(mode.get());
        internalMode->reloadAllTabs();
    }
}