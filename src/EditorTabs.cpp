
#include <Geode/Geode.hpp>
#include "../include/EditorTabs.hpp"

EditorTabs* EditorTabs::instance = nullptr;

void EditorTabs::registerTab(TabType type, std::string id, std::function<cocos2d::CCNode*(EditorUI*, CCMenuItemToggler*)> onCreate, std::function<void(EditorUI*, bool, cocos2d::CCNode*)> onToggle){
    TabData data {type, id, onCreate, onToggle, -1};
    m_tabs.push_back(data);
}

void EditorTabs::setTag(std::string id, int tag){
    for(TabData &data : m_tabs){
        if(data.id == id){
            data.tabTag = tag;
            break;
        }
    }
}

std::vector<TabData> EditorTabs::getTabs(){
    return m_tabs;
}