#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
    #ifdef ALPHALANEOUS_EDITOR_TABS_API_EXPORTING
        #define EDITOR_TABS_API_DLL __declspec(dllexport)
    #else
        #define EDITOR_TABS_API_DLL __declspec(dllimport)
    #endif
    #else
        #define EDITOR_TABS_API_DLL __attribute__((visibility("default")))
#endif

struct EDITOR_TABS_API_DLL TabData {
    std::string id;
    std::function<EditButtonBar*(EditorUI*, CCMenuItemToggler*)> onCreate;
    std::function<void(EditorUI*, EditButtonBar*)> onToggle;
    int tabTag;
};

class EDITOR_TABS_API_DLL EditorTabs {

protected:
    static EditorTabs* instance;
    std::vector<TabData> m_tabs;

public:
    std::vector<TabData> getTabs();

    void setTag(std::string id, int tag);

    void registerTab(std::string id, std::function<EditButtonBar*(EditorUI*, CCMenuItemToggler*)>, std::function<void(EditorUI*, EditButtonBar*)> = nullptr);

    static EditorTabs* get(){

        if (!instance) {
            instance = new EditorTabs();
        };
        return instance;
    }
};

class EDITOR_TABS_API_DLL EditorTabUtils {
    public:
    
    static EditButtonBar* createEditButtonBar(CCArray* arr, EditorUI* ui){
        auto winSize = CCDirector::get()->getWinSize();
        auto winBottom = CCDirector::get()->getScreenBottom();
        auto offset = ccp(winSize.width / 2 - 5.f, winBottom + ui->m_toolbarHeight - 6.f);
        auto rows = GameManager::get()->getIntGameVariable("0050");
        auto cols = GameManager::get()->getIntGameVariable("0049");

        return EditButtonBar::create(arr, offset, 0, false, cols, rows);
    }

    static void setTabIcons(CCMenuItemToggler* toggler, CCNode* on, CCNode* off){

        auto offBtnSprite = toggler->m_offButton->getNormalImage();
        auto onBtnSprite = toggler->m_onButton->getNormalImage();

        offBtnSprite->removeAllChildren();
        onBtnSprite->removeAllChildren();

        onBtnSprite->addChildAtPosition(on, Anchor::Center, ccp(0, 0));

        if(auto rgba = typeinfo_cast<CCRGBAProtocol*>(off)){
            rgba->setOpacity(150); 
        }
        offBtnSprite->addChildAtPosition(off, Anchor::Center, ccp(0, 0));
    }
};