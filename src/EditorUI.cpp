#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/modify/EditButtonBar.hpp>
#include "../include/EditorTabs.hpp"

using namespace geode::prelude;

//so BetterEdit doesn't do me dirty as it likes to do

class $modify(MyEditButtonBar, EditButtonBar) {
    
    void loadFromItems(CCArray* items, int c, int r, bool unkBool) {

        if(CCInteger* rows = typeinfo_cast<CCInteger*>(getUserObject("force-rows"))) {
            r = rows->getValue();
        }
        if(CCInteger* cols = typeinfo_cast<CCInteger*>(getUserObject("force-columns"))) {
            c = cols->getValue();
        }

        EditButtonBar::loadFromItems(items, c, r, unkBool);
    }

    void reloadItemsA(int rowCount, int columnCount) {
		if (m_buttonArray) this->loadFromItems(m_buttonArray, rowCount, columnCount, false);
	}
};

class $modify(MyEditorUI, EditorUI) {

    static void onModify(auto& self) {
        (void) self.setHookPriority("EditorUI::init", INT_MIN/2);
    }

    struct Fields {
        Ref<CCArray> m_editTabsArray;
        Ref<CCArray> m_editButtonBars;
        CCMenu* m_editTabsMenu;
        int m_selectedEditTab;

        Ref<CCArray> m_deleteTabsArray;
        Ref<CCArray> m_deleteButtonBars;
        CCMenu* m_deleteTabsMenu;
        int m_selectedDeleteTab;
    };

    void toggleMode(CCObject* sender) {

        int prevMode = m_selectedMode;

        auto tag = sender->getTag();

        m_fields->m_editTabsMenu->setVisible(false);
        m_fields->m_deleteTabsMenu->setVisible(false);

        for (CCNode* node : CCArrayExt<CCNode*>(m_fields->m_editButtonBars)) {
            node->setVisible(false);
        }

        for (CCNode* node : CCArrayExt<CCNode*>(m_fields->m_deleteButtonBars)) {
            node->setVisible(false);
        }

        CCArray* buttonBars = nullptr;

        switch (tag) {
            case 3: {
                if (m_fields->m_editTabsMenu->getChildrenCount() > 1){
                    m_fields->m_editTabsMenu->setVisible(true);
                }
                typeinfo_cast<CCNode*>(m_fields->m_editButtonBars->objectAtIndex(m_fields->m_selectedEditTab))->setVisible(true);
                break;
            }
            case 1: {
                if (m_fields->m_deleteTabsMenu->getChildrenCount() > 1){
                    m_fields->m_deleteTabsMenu->setVisible(true);
                }
                typeinfo_cast<CCNode*>(m_fields->m_deleteButtonBars->objectAtIndex(m_fields->m_selectedDeleteTab))->setVisible(true);
                break;
            }
        }

        EditorUI::toggleMode(sender);

        if (prevMode != tag) {
            for (TabData data : EditorTabs::get()->getTabs()) {
                if (data.onToggle && data.tabTag != -1 && m_createButtonBars->count() > data.tabTag) {
                    CCNode* buttonBar;
                    switch (data.type) {
                        case TabType::BUILD: {
                            if (m_createButtonBars->count() > data.tabTag) {
                                buttonBar = typeinfo_cast<CCNode*>(m_createButtonBars->objectAtIndex(data.tabTag));
                                data.onToggle(this, tag == 2 && m_selectedTab == data.tabTag, buttonBar);
                            }
                            break;
                        }
                        case TabType::EDIT: {
                            if (m_fields->m_editButtonBars->count() > data.tabTag) {
                                buttonBar = typeinfo_cast<CCNode*>(m_fields->m_editButtonBars->objectAtIndex(data.tabTag));
                                data.onToggle(this, tag == 3 && m_fields->m_selectedEditTab == data.tabTag, buttonBar);
                            }
                            break;
                        }
                        case TabType::DELETE: {
                            if (m_fields->m_deleteButtonBars->count() > data.tabTag) {
                                buttonBar = typeinfo_cast<CCNode*>(m_fields->m_deleteButtonBars->objectAtIndex(data.tabTag));
                                data.onToggle(this, tag == 1 && m_fields->m_selectedDeleteTab == data.tabTag, buttonBar);
                            }
                            break;
                        }
                    }
                }
            }
        }
        if (tag == 3 && m_fields->m_selectedEditTab != 0) {
            if (CCNode* node = getChildByID("hjfod.betteredit/custom-move-menu")) {
                node->setVisible(false);
            }
            m_editButtonBar->setVisible(false);
        }
    }

    void toggleAll(TabType type, int tag){

        for (TabData data : EditorTabs::get()->getTabs()) {

            bool isTab = data.type == type && data.tabTag == tag;

            switch (data.type) {
                case TabType::BUILD: {
                    if (CCNode* buttonBar = typeinfo_cast<CCNode*>(m_createButtonBars->objectAtIndex(data.tabTag))) {
                        if (data.onToggle) data.onToggle(this, isTab, buttonBar);
                    }
                    break;
                }
                case TabType::EDIT: {
                    if (CCNode* buttonBar = typeinfo_cast<CCNode*>(m_fields->m_editButtonBars->objectAtIndex(data.tabTag))) {
                        if (isTab) buttonBar->setVisible(true);
                        if (data.onToggle) data.onToggle(this, isTab, buttonBar);
                    }
                    break;
                }
                case TabType::DELETE: {
                    if (CCNode* buttonBar = typeinfo_cast<CCNode*>(m_fields->m_deleteButtonBars->objectAtIndex(data.tabTag))) {
                        if (isTab) buttonBar->setVisible(true);
                        if (data.onToggle) data.onToggle(this, isTab, buttonBar);
                    }
                    break;
                }
            }
        }
    }

    void onSelectBuildTab(CCObject* sender) {
        if (m_selectedMode == 2) {
            auto tag = sender->getTag();
            toggleAll(TabType::BUILD, tag);
            EditorUI::onSelectBuildTab(sender);
        }
    }

    void onSelectEditTab(CCObject* sender) {
        if(m_selectedMode == 3){
    
            for (CCMenuItemToggler* node : CCArrayExt<CCMenuItemToggler*>(m_fields->m_editTabsArray)) {
                node->toggle(false);
            }
            static_cast<CCMenuItemToggler*>(sender)->toggle(true);

            for (CCNode* node : CCArrayExt<CCNode*>(m_fields->m_editButtonBars)) {
                node->setVisible(false);
            }

            auto tag = sender->getTag();
            m_fields->m_selectedEditTab = tag;
            toggleAll(TabType::EDIT, tag);

            EditorUI::onSelectBuildTab(sender);
        }
    }

    void onSelectDeleteTab(CCObject* sender) {
        if(m_selectedMode == 1){

            for (CCMenuItemToggler* node : CCArrayExt<CCMenuItemToggler*>(m_fields->m_deleteTabsArray)) {
                node->toggle(false);
            }
            static_cast<CCMenuItemToggler*>(sender)->toggle(true);

            for (CCNode* node : CCArrayExt<CCNode*>(m_fields->m_deleteButtonBars)) {
                node->setVisible(false);
            }

            auto tag = sender->getTag();
            m_fields->m_selectedDeleteTab = tag;
            toggleAll(TabType::DELETE, tag);

            EditorUI::onSelectBuildTab(sender);
        }
    }

    RowLayout* createTabsLayout(){
        RowLayout* layout = RowLayout::create();
        layout->setAxisAlignment(AxisAlignment::Center);
        layout->setCrossAxisAlignment(AxisAlignment::Center);
        layout->setCrossAxisLineAlignment(AxisAlignment::Start);
        layout->setGap(2);
        layout->setAutoScale(true);
        layout->setGrowCrossAxis(false);
        layout->setCrossAxisOverflow(true);

        return layout;
    }

    bool init(LevelEditorLayer* editorLayer) {

        m_fields->m_editTabsMenu = CCMenu::create();
        m_fields->m_editTabsMenu->setLayout(createTabsLayout());
        m_fields->m_editTabsMenu->setID("edit-tabs-menu");
        m_fields->m_editTabsMenu->ignoreAnchorPointForPosition(false);
        m_fields->m_deleteTabsMenu = CCMenu::create();
        m_fields->m_deleteTabsMenu->setLayout(createTabsLayout());
        m_fields->m_deleteTabsMenu->setID("delete-tabs-menu");
        m_fields->m_deleteTabsMenu->ignoreAnchorPointForPosition(false);

        m_fields->m_editTabsArray = CCArray::create();
        m_fields->m_deleteTabsArray = CCArray::create();

        m_fields->m_editButtonBars = CCArray::create();
        m_fields->m_deleteButtonBars = CCArray::create();

        if (!EditorUI::init(editorLayer)) return false;

        CCSize winSize = CCDirector::get()->getWinSize();

        CCNode* modeMenu = m_swipeBtn->getParent();

        m_fields->m_editTabsMenu->setPosition({winSize.width/2, modeMenu->getScaledContentSize().height});
        m_fields->m_editTabsMenu->setAnchorPoint({0.5, 0});
        m_fields->m_editTabsMenu->setContentSize(m_tabsMenu->getContentSize());
        m_fields->m_editTabsMenu->setVisible(false);

        m_fields->m_deleteTabsMenu->setPosition({winSize.width/2, modeMenu->getScaledContentSize().height});
        m_fields->m_deleteTabsMenu->setAnchorPoint({0.5, 0});
        m_fields->m_deleteTabsMenu->setContentSize(m_tabsMenu->getContentSize());
        m_fields->m_deleteTabsMenu->setVisible(false);

        addChild(m_fields->m_editTabsMenu);
        addChild(m_fields->m_deleteTabsMenu);

        for(TabData data : EditorTabs::get()->getTabs()){
            
            int tab = 0;

            switch (data.type) {
                case TabType::BUILD: {
                    tab = m_tabsArray->count();
                    break;
                }
                case TabType::EDIT: {
                    tab = m_fields->m_editTabsArray->count();
                    break;
                }
                case TabType::DELETE: {
                    tab = m_fields->m_deleteTabsArray->count();
                    break;
                }
            }

            std::string id = data.id;
            EditorTabs::get()->setTag(id, tab);

            auto onBg = CCSprite::createWithSpriteFrameName("GJ_tabOn_001.png");
            auto offBg = CCSprite::createWithSpriteFrameName("GJ_tabOff_001.png");
            offBg->setOpacity(150);

            auto tabToggler = CCMenuItemExt::createToggler(
                offBg, onBg, [this, data](CCObject* sender) {
                    switch (data.type) {
                        case TabType::BUILD: {
                            this->onSelectBuildTab(sender);
                            break;
                        }
                        case TabType::EDIT: {
                            this->onSelectEditTab(sender);
                            break;
                        }
                        case TabType::DELETE: {
                            this->onSelectDeleteTab(sender);
                            break;
                        }
                    }
                    static_cast<CCMenuItemToggler*>(sender)->setClickable(false);
                }
            );
            
            tabToggler->setID(id);
            tabToggler->setTag(tab);

            CCNode* bar = data.onCreate(this, tabToggler);
            bar->setID(fmt::format("{}-bar", id));
            bar->setZOrder(10);
            bar->setVisible(false);

            switch (data.type) {
                case TabType::BUILD: {
                    m_tabsMenu->addChild(tabToggler);
                    m_tabsMenu->updateLayout();
                    m_tabsArray->addObject(tabToggler);
                    m_createButtonBars->addObject(bar);
                    break;
                }
                case TabType::EDIT: {
                    m_fields->m_editTabsMenu->addChild(tabToggler);
                    m_fields->m_editTabsMenu->updateLayout();
                    m_fields->m_editTabsArray->addObject(tabToggler);
                    m_fields->m_editButtonBars->addObject(bar);
                    bar->setVisible(false);
                    break;
                }
                case TabType::DELETE: {
                    m_fields->m_deleteTabsMenu->addChild(tabToggler);
                    m_fields->m_deleteTabsMenu->updateLayout();
                    m_fields->m_deleteTabsArray->addObject(tabToggler);
                    m_fields->m_deleteButtonBars->addObject(bar);
                    bar->setVisible(false);
                    break;
                }
            }

            if (m_fields->m_editTabsArray->count() > 0) {
                static_cast<CCMenuItemToggler*>(m_fields->m_editTabsArray->objectAtIndex(0))->toggle(true);
            }

            if (m_fields->m_deleteTabsArray->count() > 0) {
                static_cast<CCMenuItemToggler*>(m_fields->m_deleteTabsArray->objectAtIndex(0))->toggle(true);
            }
           
            if (m_fields->m_editTabsMenu->getChildrenCount() == 1) {
                m_fields->m_editTabsMenu->setVisible(false);
            }

            if (m_fields->m_deleteTabsMenu->getChildrenCount() == 1) {
                m_fields->m_deleteTabsMenu->setVisible(false);
            }

            if(!typeinfo_cast<EditButtonBar*>(bar)){
                float scale = m_createButtonBar->getScale();
                bar->setScale(scale);
                bar->setPositionY(bar->getPositionY() * scale);
            }

            float scale = m_tabsMenu->getScale();
            m_fields->m_editTabsMenu->setScale(scale);
            m_fields->m_deleteTabsMenu->setScale(scale);


            this->addChild(bar);
        }

        this->centerBuildTabs();
        for (auto c : CCArrayExt<CCNode*>(this->getChildren())) {
            if (auto bar = typeinfo_cast<EditButtonBar*>(c)) {
                static_cast<MyEditButtonBar*>(bar)->reloadItemsA(
                    GameManager::get()->getIntGameVariable("0049"),
                    GameManager::get()->getIntGameVariable("0050")
                );
            }
        }
        this->centerBuildTabs();

        return true;
    }

    void showUI(bool p0){
        EditorUI::showUI(p0);

        if (p0) {
            switch (m_selectedMode) {
                case 3: {
                    if (m_fields->m_editTabsMenu->getChildrenCount() > 1){
                        m_fields->m_editTabsMenu->setVisible(true);
                    }

                    if (m_fields->m_selectedEditTab != 0) {
                        if (CCNode* node = getChildByID("hjfod.betteredit/custom-move-menu")) {
                            node->setVisible(false);
                        }
                        m_editButtonBar->setVisible(false);
                    }

                    typeinfo_cast<CCNode*>(m_fields->m_editButtonBars->objectAtIndex(m_fields->m_selectedEditTab))->setVisible(true);
                    break;
                }
                case 1: {
                    if (m_fields->m_deleteTabsMenu->getChildrenCount() > 1){
                        m_fields->m_deleteTabsMenu->setVisible(true);
                    }

                    if (m_fields->m_selectedDeleteTab != 0) {
                        m_deleteMenu->setVisible(false);
                    }

                    typeinfo_cast<CCNode*>(m_fields->m_deleteButtonBars->objectAtIndex(m_fields->m_selectedDeleteTab))->setVisible(true);
                    break;
                }
            }
        }
        else {
            m_fields->m_editTabsMenu->setVisible(false);
            m_fields->m_deleteTabsMenu->setVisible(false);

            for (CCNode* node : CCArrayExt<CCNode*>(m_fields->m_editButtonBars)) {
                node->setVisible(false);
            }
            for (CCNode* node : CCArrayExt<CCNode*>(m_fields->m_deleteButtonBars)) {
                node->setVisible(false);
            }
        }
    }

    void updateDeleteButtons(){
        EditorUI::updateDeleteButtons();

        if (m_selectedMode == 1 && m_fields->m_selectedDeleteTab > 0) {
            m_deleteMenu->setVisible(false);
        }
    }

    void centerBuildTabs() {
        // This centers the build tab
        auto winSize = CCDirector::get()->getWinSize();
        for (auto c : CCArrayExt<CCNode*>(this->getChildren())) {
            if (auto bar = typeinfo_cast<EditButtonBar*>(c)) {
                if (bar->getChildrenCount() > 0) {
                    getChild(bar, 0)->setPositionX(-winSize.width / 2 + 5);
                    if (auto menu = getChildOfType<CCMenu>(bar, 0)) {
                        menu->setPositionX(winSize.width / 2 + 5);
                    }
                }
                bar->setPositionX(winSize.width / 2);
            }
        }
    }
};

class $modify(EditorPauseLayer) {
    
    void onResume(CCObject* pSender) {
        EditorPauseLayer::onResume(pSender);

        MyEditorUI* ui = static_cast<MyEditorUI*>(EditorUI::get());

        ui->centerBuildTabs();

        for (auto c : CCArrayExt<CCNode*>(ui->getChildren())) {
            if (auto bar = typeinfo_cast<EditButtonBar*>(c)) {
                static_cast<MyEditButtonBar*>(bar)->reloadItemsA(
                    GameManager::get()->getIntGameVariable("0049"),
                    GameManager::get()->getIntGameVariable("0050")
                );
            }
        }

        ui->centerBuildTabs();

        if (ui->m_selectedMode == 3 && ui->m_fields->m_selectedEditTab != 0) {
            if (CCNode* node = ui->getChildByID("hjfod.betteredit/custom-move-menu")) {
                node->setVisible(false);
            }
        }
    }
};



$execute {

    EditorTabs::get()->registerTab(TabType::EDIT, "edit-tab", [](EditorUI* ui, CCMenuItemToggler* toggler) -> CCNode* {
        EditButtonBar* editTabBar = ui->m_editButtonBar;
        editTabBar->removeFromParentAndCleanup(false);

        CCSprite* sprOn = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");
        sprOn->setScale(0.45f);
        CCSprite* sprOff = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");
        sprOff->setScale(0.45f);

        EditorTabUtils::setTabIcons(toggler, sprOn, sprOff);

        return editTabBar;
    }, [](EditorUI* ui, bool state, CCNode* bar) {
        if (CCNode* node = ui->getChildByID("hjfod.betteredit/custom-move-menu")) {
            node->setVisible(state);
            bar->setVisible(false);
        }
        else bar->setVisible(state);
    });

    EditorTabs::get()->registerTab(TabType::DELETE, "delete-tab", [](EditorUI* ui, CCMenuItemToggler* toggler) -> CCNode* {
        
        CCSprite* sprOn = CCSprite::createWithSpriteFrameName("edit_delBtn_001.png");
        sprOn->setScale(0.5f);
        CCSprite* sprOff = CCSprite::createWithSpriteFrameName("edit_delBtn_001.png");
        sprOff->setScale(0.5f);

        EditorTabUtils::setTabIcons(toggler, sprOn, sprOff);
        
        return EditorTabUtils::createDummyBar();
    }, [](EditorUI* ui, bool state, CCNode* bar) {
        ui->m_deleteMenu->setVisible(state);
    });
}