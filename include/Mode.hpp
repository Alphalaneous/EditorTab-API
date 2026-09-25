#pragma once

#include "API.hpp"
#include <Geode/utils/ZStringView.hpp>

namespace alpha::editor_tabs {

class Mode {
public:
    Mode(geode::ZStringView ID) {
        alpha::editor_tabs::mode::construct(this, ID);
    }

    ~Mode() {
        alpha::editor_tabs::mode::destruct(this);
    }

    geode::ZStringView getID() {
        return alpha::editor_tabs::mode::getID(this);
    }

    void show() {
        alpha::editor_tabs::mode::show(this);
    }

    Tab* createTab(geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority) {
        return alpha::editor_tabs::mode::createTab(this, ID, node, icon, priority);
    }

    void removeTab(geode::ZStringView ID) {
        alpha::editor_tabs::mode::removeTab(this, ID);
    }

    void removeTab(Tab* tab) {
        alpha::editor_tabs::mode::removeTab(this, tab);
    }

    Tab* getTab(geode::ZStringView ID) {
        return alpha::editor_tabs::mode::getTab(this, ID);
    }

    Tab* getCurrentTab() {
        return alpha::editor_tabs::mode::getCurrentTab(this);
    }

    const std::span<const std::shared_ptr<Tab>> getAllTabs() {
        return alpha::editor_tabs::mode::getAllTabs(this);
    }

    void switchTab(geode::ZStringView ID) {
        alpha::editor_tabs::mode::switchTab(this, ID);
    }

    void switchTab(Tab* tab) {
        alpha::editor_tabs::mode::switchTab(this, tab);
    }

    void reloadAllTabs() {
        alpha::editor_tabs::mode::reloadAllTabs(this);
    }

    void removeSelf() {
        alpha::editor_tabs::mode::removeSelf(this);
    }
    
protected:
    void* m_impl; 
};

}