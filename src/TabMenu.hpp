#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace alpha::editor_tabs {

class TabMenu : public CCMenu {
public:
    using CCNode::addChild;

    static TabMenu* create();

    void setContentSize(const CCSize& contentSize) override;
    void addChild(CCNode* child, int zOrder, int tag) override;

    CCMenu* getMainMenu();
    void updateUI();

    void goToPage();
    void onLeft(CCObject* sender);
    void onRight(CCObject* sender);

protected:
    bool init() override;

    CCMenu* m_menu;
    CCMenu* m_navigationMenu;

    CCMenuItemSpriteExtra* m_prevButton;
    CCMenuItemSpriteExtra* m_nextButton;

    std::vector<std::vector<CCNode*>> m_pages;
    int m_page;
};

}