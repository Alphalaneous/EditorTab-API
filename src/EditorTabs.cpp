
#include <Geode/Geode.hpp>
#include "../include/EditorTabs.hpp"
#include "EditorUI.hpp"

using namespace geode::prelude;

EditorTabs* EditorTabs::instance = nullptr;

void EditorTabs::registerTab(TabType type, std::string id, std::function<cocos2d::CCNode*(EditorUI*, CCMenuItemToggler*)> onCreate, std::function<void(EditorUI*, bool, cocos2d::CCNode*)> onToggle){
    TabData data {type, id, onCreate, onToggle, -1};
    m_tabs.push_back(data);
}

void EditorTabs::addTab(EditorUI* ui, TabData data){
    MyEditorUI* myEditorUI = static_cast<MyEditorUI*>(ui);

    int tab = 0;

    switch (data.type) {
        case TabType::BUILD: {
            tab = myEditorUI->m_tabsArray->count();
            break;
        }
        case TabType::EDIT: {
            tab = myEditorUI->m_fields->m_editTabsArray->count();
            break;
        }
        case TabType::DELETE: {
            tab = myEditorUI->m_fields->m_deleteTabsArray->count();
            break;
        }
    }

    std::string id = data.id;
    data.tabTag = tab;

    myEditorUI->m_fields->tabs.push_back(data);

    auto onBg = CCSprite::createWithSpriteFrameName("GJ_tabOn_001.png");
    auto offBg = CCSprite::createWithSpriteFrameName("GJ_tabOff_001.png");
    offBg->setOpacity(150);

    auto tabToggler = CCMenuItemExt::createToggler(
        offBg, onBg, [myEditorUI, data](CCObject* sender) {
            switch (data.type) {
                case TabType::BUILD: {
                    myEditorUI->onSelectBuildTab(sender);
                    break;
                }
                case TabType::EDIT: {
                    myEditorUI->onSelectEditTab(sender);
                    break;
                }
                case TabType::DELETE: {
                    myEditorUI->onSelectDeleteTab(sender);
                    break;
                }
            }
            static_cast<CCMenuItemToggler*>(sender)->setClickable(false);
        }
    );
    
    tabToggler->setID(id);
    tabToggler->setTag(tab);

    CCNode* bar = data.onCreate(myEditorUI, tabToggler);
    bar->setID(fmt::format("{}-bar", id));
    bar->setZOrder(10);
    bar->setVisible(false);

    switch (data.type) {
        case TabType::BUILD: {
            myEditorUI->m_tabsMenu->addChild(tabToggler);
            myEditorUI->m_tabsMenu->updateLayout();
            myEditorUI->m_tabsArray->addObject(tabToggler);
            myEditorUI->m_createButtonBars->addObject(bar);
            break;
        }
        case TabType::EDIT: {
            myEditorUI->m_fields->m_editTabsMenu->addChild(tabToggler);
            myEditorUI->m_fields->m_editTabsMenu->updateLayout();
            myEditorUI->m_fields->m_editTabsArray->addObject(tabToggler);
            myEditorUI->m_fields->m_editButtonBars->addObject(bar);
            bar->setVisible(false);
            break;
        }
        case TabType::DELETE: {
            myEditorUI->m_fields->m_deleteTabsMenu->addChild(tabToggler);
            myEditorUI->m_fields->m_deleteTabsMenu->updateLayout();
            myEditorUI->m_fields->m_deleteTabsArray->addObject(tabToggler);
            myEditorUI->m_fields->m_deleteButtonBars->addObject(bar);
            bar->setVisible(false);
            break;
        }
    }

    if (myEditorUI->m_fields->m_editTabsArray->count() > 0) {
        static_cast<CCMenuItemToggler*>(myEditorUI->m_fields->m_editTabsArray->objectAtIndex(0))->toggle(true);
    }

    if (myEditorUI->m_fields->m_deleteTabsArray->count() > 0) {
        static_cast<CCMenuItemToggler*>(myEditorUI->m_fields->m_deleteTabsArray->objectAtIndex(0))->toggle(true);
    }
    
    if (myEditorUI->m_fields->m_editTabsMenu->getChildrenCount() == 1) {
        myEditorUI->m_fields->m_editTabsMenu->setVisible(false);
    }

    if (myEditorUI->m_fields->m_deleteTabsMenu->getChildrenCount() == 1) {
        myEditorUI->m_fields->m_deleteTabsMenu->setVisible(false);
    }

    float scaleBar = myEditorUI->m_createButtonBar->getScale();
    bar->setScale(scaleBar);
    bar->setPositionY(bar->getPositionY() * scaleBar);

    float scale = myEditorUI->m_tabsMenu->getScale();
    myEditorUI->m_fields->m_editTabsMenu->setScale(scale);
    myEditorUI->m_fields->m_deleteTabsMenu->setScale(scale);

    myEditorUI->addChild(bar);
}

void EditorTabs::addTab(EditorUI* ui, TabType type, std::string id, std::function<cocos2d::CCNode*(EditorUI*, CCMenuItemToggler*)> onCreate, std::function<void(EditorUI*, bool, cocos2d::CCNode*)> onToggle){
    TabData data {type, id, onCreate, onToggle, -1};
    addTab(ui, data);
}

std::vector<TabData> EditorTabs::getRegisteredTabs(){
    return m_tabs;
}