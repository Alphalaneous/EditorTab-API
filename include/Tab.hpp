#pragma once

#include "Optional.hpp"
#include <Geode/Result.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/EditButtonBar.hpp>
#include <Geode/utils/ZStringView.hpp>

namespace alpha::editor_tabs {

class Tab {
public:
    geode::ZStringView getID() {
        return alpha::editor_tabs::tab::getID(this);
    }

    void show() {
        alpha::editor_tabs::tab::show(this);
    }

    EditButtonBar* getNode() {
        return alpha::editor_tabs::tab::getNode(this);
    }

    cocos2d::CCNode* getIcon() {
        return alpha::editor_tabs::tab::getIcon(this);
    }

    void overrideSize(bool override, int rows, int columns) {
        alpha::editor_tabs::tab::overrideSize(this, override, rows, columns);
    }

    void overrideSize(int rows, int columns) {
        alpha::editor_tabs::tab::overrideSize(this, rows, columns);
    }
    
    int getRows() {
        return alpha::editor_tabs::tab::getRows(this);
    }

    int getColumns() {
        return alpha::editor_tabs::tab::getColumns(this);
    }

    void setPriority(int priority) {
        alpha::editor_tabs::tab::setPriority(this, priority);
    }

    int getPriority() {
        return alpha::editor_tabs::tab::getPriority(this);
    }

    void setPageDotsClickable(bool clickable) {
        alpha::editor_tabs::tab::setPageDotsClickable(this, clickable);
    }

    bool arePageDotsClickable() {
        return alpha::editor_tabs::tab::arePageDotsClickable(this);
    }

    Mode* getMode() {
        return alpha::editor_tabs::tab::getMode(this);
    }

    geode::Result<unsigned int> getIndex() {
        return alpha::editor_tabs::tab::getIndex(this);
    }

    CCMenuItemToggler* getTabToggle() {
        return alpha::editor_tabs::tab::getTabToggle(this);
    }

    void reloadItems() {
        alpha::editor_tabs::tab::reloadItems(this);
    }

    void setItemless(bool itemless) {
        alpha::editor_tabs::tab::setItemless(this, itemless);
    }

    bool isItemless() {
        return alpha::editor_tabs::tab::isItemless(this);
    }

    void setAutoScale(bool enabled) {
        alpha::editor_tabs::tab::setAutoScale(this, enabled);
    }

    bool hasAutoScale() {
        return alpha::editor_tabs::tab::hasAutoScale(this);
    }

    void removeSelf() {
        alpha::editor_tabs::tab::removeSelf(this);
    }

protected:
    void* m_impl; 
};

}