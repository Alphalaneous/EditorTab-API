
#include <Geode/Geode.hpp>
#include "../include/EditorTabs.hpp"

EditorTabs* EditorTabs::instance = nullptr;

void EditorTabs::registerTab(std::string id, std::function<EditButtonBar*(EditorUI*, CCMenuItemToggler*)> onCreate, std::function<void(EditorUI*, EditButtonBar*)> onToggle){
    TabData data {id, onCreate, onToggle, -1};
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

std::list<TabData> EditorTabs::getTabs(){
    return m_tabs;
}