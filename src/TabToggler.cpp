#include "TabToggler.hpp"
#include "Tab.hpp"
#include "Mode.hpp"

namespace alpha::editor_tabs {

TabToggler* TabToggler::create(::internal::Tab* tab, CCNode* icon) {
    auto ret = new TabToggler();
    if (ret->init(tab, icon)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool TabToggler::init(::internal::Tab* tab, CCNode* icon) {
    auto tabOff = CCSprite::createWithSpriteFrameName("GJ_tabOff_001.png");
    auto tabOn = CCSprite::createWithSpriteFrameName("GJ_tabOn_001.png");

    if (!CCMenuItemToggler::init(tabOff, tabOn, this, menu_selector(TabToggler::onPress))) return false;

    m_notClickable = true;

    m_icon = icon;
    m_tab = tab;

    tabOff->setOpacity(150);

    auto center = tabOff->convertToNodeSpace({0, 0});
    auto tabSize = tabOff->getContentSize();

    auto pos = center + CCPoint{0, -1};

    icon->setPosition(pos);

    auto iconSize = icon->getContentSize();

    float scaleX = (tabSize.width  - 8.f) / iconSize.width;
    float scaleY = (tabSize.height - 4.f) / iconSize.height;

    float scale = std::min(scaleX, scaleY);

    icon->setScale(scale);
    icon->setZOrder(1);

    auto rgba = typeinfo_cast<CCRGBAProtocol*>(icon);
    if (rgba) {
        rgba->setOpacity(150);
    }
    m_offButton->addChild(icon);

    return true;
}

void TabToggler::onPress(CCObject* sender) {
    auto internalMode = static_cast<::internal::Mode*>(m_tab->getMode());
    internalMode->switchTab(m_tab);
}

void TabToggler::toggleTab(bool on) {
    toggle(on);

    m_icon->removeFromParent();

    auto rgba = typeinfo_cast<CCRGBAProtocol*>(m_icon);
    if (rgba) {
        rgba->setOpacity(on ? 255 : 150);
    }
    
    auto node = on ? m_onButton : m_offButton;

    node->addChild(m_icon);
}

}