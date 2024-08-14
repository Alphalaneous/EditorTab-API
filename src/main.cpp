#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "../include/EditorTabs.hpp"

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {

    void toggleMode(CCObject* sender) {
        auto tag = sender->getTag();

        if (tag != 1){
            for(TabData data : EditorTabs::get()->getTabs()){
                if(data.onToggle && data.tabTag != -1 && m_createButtonBars->count() > data.tabTag){
                    if(EditButtonBar* buttonBar = typeinfo_cast<EditButtonBar*>(m_createButtonBars->objectAtIndex(data.tabTag))) {
                        data.onToggle(this, buttonBar);
                    }
                }
            }
        }

        EditorUI::toggleMode(sender);
    }

    void onSelectBuildTab(CCObject* sender) {
        auto tag = sender->getTag();

        for(TabData data : EditorTabs::get()->getTabs()){
            if(data.tabTag != tag && data.onToggle){
                if(EditButtonBar* buttonBar = typeinfo_cast<EditButtonBar*>(m_createButtonBars->objectAtIndex(data.tabTag))) {
                    data.onToggle(this, buttonBar);
                }
                break;
            }
        }

        EditorUI::onSelectBuildTab(sender);
    }

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        for(TabData data : EditorTabs::get()->getTabs()){
            
            int tab = m_tabsArray->count();
            std::string id = data.id;

            EditorTabs::get()->setTag(id, tab);

            auto onBg = CCSprite::createWithSpriteFrameName("GJ_tabOn_001.png");
            auto offBg = CCSprite::createWithSpriteFrameName("GJ_tabOff_001.png");
            offBg->setOpacity(150);

            auto tabToggler = CCMenuItemExt::createToggler(
                onBg, offBg, [this](CCObject* sender) {
                    this->onSelectBuildTab(sender);
                    static_cast<CCMenuItemToggler*>(sender)->setClickable(false);
                }
            );
            
            tabToggler->setID(id);
            tabToggler->setTag(tab);

            m_tabsMenu->addChild(tabToggler);
            m_tabsMenu->updateLayout();

            EditButtonBar* bar = data.onCreate(this, tabToggler);
            bar->setID(fmt::format("{}-bar", id));
            bar->setZOrder(10);
            bar->setVisible(false);

            m_tabsArray->addObject(tabToggler);
            m_createButtonBars->addObject(bar);

            this->addChild(bar);
        }

        return true;
    }
};
