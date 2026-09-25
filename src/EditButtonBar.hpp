#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/EditButtonBar.hpp>

using namespace geode::prelude;

class $modify(ETEditButtonBar, EditButtonBar) {

    static void onModify(auto& self) {
        (void) self.setHookPriority("EditButtonBar::loadFromItems", Priority::Replace);
        (void) self.setHookPriority("EditButtonBar::goToPage", Priority::Replace);
        (void) self.setHookPriority("EditButtonBar::onLeft", Priority::Replace);
        (void) self.setHookPriority("EditButtonBar::onRight", Priority::Replace);
    }

    void loadFromItems(CCArray* items, int columns, int rows, bool preserve);
    void goToPage(int page);
    void onLeft(CCObject* sender);
    void onRight(CCObject* sender);
};