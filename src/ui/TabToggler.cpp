#define GEODE_DEFINE_EVENT_EXPORTS
#include "../../include/ui/TabToggler.hpp"
#include "../../include/api/Mode.hpp"
#include "../../include/api/Tab.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace alpha::editor_tabs {

struct TabToggler::Impl : public ZImpl<TabToggler::Impl> {
    CCNode* m_icon = nullptr;
    alpha::editor_tabs::Tab* m_tab = nullptr;
    CCSprite* m_tabOff = nullptr;
    CCSprite* m_tabOn = nullptr;
};

void TabToggler::construct() {
    m_impl = std::make_unique<Impl>();
}

TabToggler* TabToggler::create(alpha::editor_tabs::Tab* tab, CCNode* icon) {
    auto ret = new TabToggler();
    if (ret->init(tab, icon)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool TabToggler::init(alpha::editor_tabs::Tab* tab, CCNode* icon) {
    m_impl->get()->m_tabOff = CCSprite::createWithSpriteFrameName("GJ_tabOff_001.png");
    m_impl->get()->m_tabOn = CCSprite::createWithSpriteFrameName("GJ_tabOn_001.png");

    if (!CCMenuItemToggler::init(m_impl->get()->m_tabOff, m_impl->get()->m_tabOn, this, menu_selector(TabToggler::onPress))) return false;

    m_notClickable = true;

    m_impl->get()->m_icon = icon;
    m_impl->get()->m_tab = tab;

    m_impl->get()->m_tabOff->setOpacity(150);

    auto center = m_impl->get()->m_tabOff->convertToNodeSpace({0.f, 0.f});
    auto tabSize = m_impl->get()->m_tabOff->getContentSize();

    auto pos = center + CCPoint{0.f, -1.f};

    icon->setPosition(pos);

    auto iconSize = icon->getContentSize();

    float scaleX = (tabSize.width - 8.f) / iconSize.width;
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

CCNode* TabToggler::getIcon() {
    return m_impl->get()->m_icon;
}

void TabToggler::setIcon(CCNode* icon) {
    m_impl->get()->m_icon->removeFromParent();
    m_impl->get()->m_icon = icon;

    auto center = m_impl->get()->m_tabOff->convertToNodeSpace({0.f, 0.f});
    auto tabSize = m_impl->get()->m_tabOff->getContentSize();

    auto pos = center + CCPoint{0.f, -1.f};

    icon->setPosition(pos);

    auto iconSize = icon->getContentSize();

    float scaleX = (tabSize.width - 8.f) / iconSize.width;
    float scaleY = (tabSize.height - 4.f) / iconSize.height;

    float scale = std::min(scaleX, scaleY);

    icon->setScale(scale);
    icon->setZOrder(1);

    auto rgba = typeinfo_cast<CCRGBAProtocol*>(m_impl->get()->m_icon);
    if (rgba) {
        rgba->setOpacity(isToggled() ? 255 : 150);
    }
    
    auto node = isToggled() ? m_onButton : m_offButton;
    node->addChild(m_impl->get()->m_icon);
}

void TabToggler::onPress(CCObject* sender) {
    auto oldTab = EditorUI::get()->m_selectedTab;

    m_impl->get()->m_tab->getMode()->switchTab(m_impl->get()->m_tab);

    if (oldTab != m_impl->get()->m_tab->getMode()->getTabIndex(m_impl->get()->m_tab).unwrapOrDefault()) {
        EditorSFXManager::sharedState()->playAction(164);
    }
}

void TabToggler::toggleTab(bool on) {
    toggle(on);

    m_impl->get()->m_icon->removeFromParent();

    auto rgba = typeinfo_cast<CCRGBAProtocol*>(m_impl->get()->m_icon);
    if (rgba) {
        rgba->setOpacity(on ? 255 : 150);
    }
    
    auto node = on ? m_onButton : m_offButton;

    node->addChild(m_impl->get()->m_icon);
}

}