#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace alpha::editor_tabs {

class Tab;

class TabToggler : public CCMenuItemToggler {
public:
    static TabToggler* create(alpha::editor_tabs::Tab* tab, CCNode* icon);

    void toggleTab(bool on);
    void onPress(CCObject* sender);
protected:
    bool init(alpha::editor_tabs::Tab* tab, CCNode* icon);

    CCNode* m_icon;
    alpha::editor_tabs::Tab* m_tab;
};

}