#pragma once

#include <Geode/Geode.hpp>

#ifdef GEODE_IS_WINDOWS
    #ifdef ALPHALANEOUS_EDITOR_TABS_API_EXPORTING
        #define EDITOR_TABS_API_DLL __declspec(dllexport)
    #else
        #define EDITOR_TABS_API_DLL __declspec(dllimport)
    #endif
    #else
        #define EDITOR_TABS_API_DLL __attribute__((visibility("default")))
#endif

#undef DELETE

enum TabType {
    BUILD,
    EDIT,
    DELETE
};

struct TabData {
    TabType type;
    std::string id;
    std::function<cocos2d::CCNode*(EditorUI*, CCMenuItemToggler*)> onCreate;
    std::function<void(EditorUI*, bool, cocos2d::CCNode*)> onToggle;
    int tabTag;
};

#define create_tab_callback(method) std::bind(&method, this, std::placeholders::_1, std::placeholders::_2)
#define toggle_tab_callback(method) std::bind(&method, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

class EDITOR_TABS_API_DLL EditorTabs { 

protected:
    static EditorTabs* instance;
    std::vector<TabData> m_tabs;

public:
    std::vector<TabData> getRegisteredTabs();

    void registerTab(TabType type, std::string id, std::function<cocos2d::CCNode*(EditorUI*, CCMenuItemToggler*)>, std::function<void(EditorUI*, bool, cocos2d::CCNode*)> = nullptr);
    static void addTab(EditorUI* ui, TabType type, std::string id, std::function<cocos2d::CCNode*(EditorUI*, CCMenuItemToggler*)>, std::function<void(EditorUI*, bool, cocos2d::CCNode*)> = nullptr);
    static void addTab(EditorUI* ui, TabData data);

    static EditorTabs* get(){

        if (!instance) {
            instance = new EditorTabs();
        };
        return instance;
    }
};

class EDITOR_TABS_API_DLL EditorTabUtils {
    public:
    
    static EditButtonBar* createDummyBar(){

        EditButtonBar* bar = EditButtonBar::create(cocos2d::CCArray::create(), {0, 0}, -1, false, 0, 0);
        bar->removeAllChildren();
        bar->addChild(cocos2d::CCNode::create()); //fix betteredit crash
        return bar;
    }


    static EditButtonBar* createEditButtonBar(cocos2d::CCArray* arr, EditorUI* ui){
        auto winSize = cocos2d::CCDirector::get()->getWinSize();
        auto winBottom = cocos2d::CCDirector::get()->getScreenBottom();
        auto offset = cocos2d::CCPoint(winSize.width / 2 - 5.f, winBottom + 92 - 6.f);
        auto rows = GameManager::get()->getIntGameVariable("0050");
        auto cols = GameManager::get()->getIntGameVariable("0049");

        return EditButtonBar::create(arr, offset, -1, false, cols, rows);
    }

    static void setTabIcons(CCMenuItemToggler* toggler, cocos2d::CCNode* on, cocos2d::CCNode* off){

        auto offBtnSprite = toggler->m_offButton->getNormalImage();
        auto onBtnSprite = toggler->m_onButton->getNormalImage();

        offBtnSprite->removeAllChildren();
        onBtnSprite->removeAllChildren();

        onBtnSprite->addChildAtPosition(on, geode::Anchor::Center, ccp(0, -1));

        if(auto rgba = geode::cast::typeinfo_cast<cocos2d::CCRGBAProtocol*>(off)){
            rgba->setOpacity(150); 
        }
        offBtnSprite->addChildAtPosition(off, geode::Anchor::Center, ccp(0, -1));
    }

    static void setTabIcon(CCMenuItemToggler* toggler, cocos2d::CCNode* icon){
        setTabIcons(toggler, icon, icon);
    }
};