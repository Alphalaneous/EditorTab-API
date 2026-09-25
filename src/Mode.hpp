#pragma once

#include "../include/Mode.hpp"
#include "../include/Tab.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace alpha::editor_tabs;

namespace internal {

class Mode : public alpha::editor_tabs::Mode {
public:
    void construct(ZStringView ID);
    void destruct();

    geode::ZStringView getID();
    void show();

    alpha::editor_tabs::Tab* createTab(geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority = 0);

    void removeTab(ZStringView ID);
    void removeTab(alpha::editor_tabs::Tab* tab);

    alpha::editor_tabs::Tab* getTab(geode::ZStringView ID);
    alpha::editor_tabs::Tab* getCurrentTab();
    std::span<const std::shared_ptr<alpha::editor_tabs::Tab>> getAllTabs();

    void switchTab(ZStringView ID);
    void switchTab(alpha::editor_tabs::Tab* tab);

    void hideMode();
    void showMode();

    void updateToggles();
    void setupTabs();

    void removeAllTabs();
    void reloadAllTabs();

    void removeSelf();
    
protected:
    class Impl;
    Impl* impl();
};

}