#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/EditButtonBar.hpp>
#include <Geode/modify/BoomScrollLayer.hpp>

using namespace geode::prelude;

class $modify(ETEditButtonBar, EditButtonBar) {
    struct Fields {
        CCNode* m_dots;
    };

    static void onModify(auto& self) {
        (void) self.setHookPriority("EditButtonBar::loadFromItems", Priority::EarlyPost);
    }

    static EditButtonBar* create(cocos2d::CCArray* objects, cocos2d::CCPoint position, int tab, bool hasCreateItems, int columns, int rows);

    void setupChanges(int c, int r);
    void loadFromItems(CCArray* items, int c, int r, bool preserve);
    void goToPage(int page);
    void optimizedSetVisible(bool visible);

    void updatePage();
    void showPage();
    void onLeft(cocos2d::CCObject* sender);
    void onRight(cocos2d::CCObject* sender);
};