#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/EditButtonBar.hpp>
#include <Geode/modify/BoomScrollLayer.hpp>

using namespace geode::prelude;

class $modify(ETEditButtonBar, EditButtonBar) {
    
#ifndef GEODE_IS_MACOS
    struct Fields {
        CCSpriteBatchNode* m_dots;
    };
#endif

    static void onModify(auto& self) {
        (void) self.setHookPriority("EditButtonBar::loadFromItems", Priority::EarlyPost);
    }

    void loadFromItems(CCArray* items, int c, int r, bool preserve);
    void goToPage(int page);
    void optimizedSetVisible(bool visible);

    void updatePage();
    void showPage();
    void onLeft(cocos2d::CCObject* sender);
    void onRight(cocos2d::CCObject* sender);
};

#ifndef GEODE_IS_MACOS
class $modify(ETBoomScrollLayer, BoomScrollLayer) {
    void updateDots(float dt);
};
#endif