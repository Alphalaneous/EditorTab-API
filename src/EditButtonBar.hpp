#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/EditButtonBar.hpp>

using namespace geode::prelude;

class $modify(ETEditButtonBar, EditButtonBar) {
    void loadFromItems(CCArray* items, int columns, int rows, bool preserve);
    void goToPage(int page);
    void onLeft(CCObject* sender);
    void onRight(CCObject* sender);
};