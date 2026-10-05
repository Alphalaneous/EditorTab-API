#pragma once

#include "Optional.hpp"
#include <Geode/binding/EditButtonBar.hpp>
#include <Geode/utils/ZStringView.hpp>

namespace alpha::editor_tabs {

class Mode {
public:
    geode::ZStringView getID() {
        return alpha::editor_tabs::mode::getID(this);
    }

    void show() {
        alpha::editor_tabs::mode::show(this);
    }

    Tab* createTab(geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority = 0) {
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

    Tab* getTabByNode(EditButtonBar* node) {
        return alpha::editor_tabs::mode::getTabByNode(this, node);
    }
    
    Tab* getTabByIndex(unsigned int index) {
        return alpha::editor_tabs::mode::getTabByIndex(this, index);
    }

    Tab* getCurrentTab() {
        return alpha::editor_tabs::mode::getCurrentTab(this);
    }

    geode::Result<unsigned int> getTabIndex(geode::ZStringView ID) {
        return alpha::editor_tabs::mode::getTabIndex(this, ID);
    }

    geode::Result<unsigned int> getTabIndex(Tab* tab) {
        return alpha::editor_tabs::mode::getTabIndex(this, tab);
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