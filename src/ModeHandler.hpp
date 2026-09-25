#pragma once

#include "../include/Mode.hpp"

class ModeHandler {
public:
    static ModeHandler* get();

    alpha::editor_tabs::Mode* getMode(geode::ZStringView ID);
    alpha::editor_tabs::Mode* createMode(geode::ZStringView ID);
    void switchMode(geode::ZStringView ID);
    void switchMode(alpha::editor_tabs::Mode* mode);

    geode::ZStringView getCurrentModeID();
    alpha::editor_tabs::Mode* getCurrentMode();
    std::span<const std::shared_ptr<alpha::editor_tabs::Mode>> getAllModes();

    void setupModes();

    void removeMode(geode::ZStringView ID);
    void removeMode(alpha::editor_tabs::Mode* mode);

    void reloadAllModes();

protected:
    std::vector<std::shared_ptr<alpha::editor_tabs::Mode>> m_modes;
    std::string m_currentMode;
};