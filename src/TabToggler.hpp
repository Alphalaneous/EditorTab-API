#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace internal {
    class Tab;
}

namespace alpha::editor_tabs {

class TabToggler : public CCMenuItemToggler {
public:
    static TabToggler* create(::internal::Tab* tab, CCNode* icon);

    void toggleTab(bool on);
    void onPress(CCObject* sender);
protected:
    bool init(::internal::Tab* tab, CCNode* icon);

    CCNode* m_icon;
    ::internal::Tab* m_tab;
};

}