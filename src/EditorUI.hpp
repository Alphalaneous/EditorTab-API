#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include <Geode/modify/EditButtonBar.hpp>
#include "../include/EditorTabs.hpp"

using namespace geode::prelude;

class $modify(MyEditButtonBar, EditButtonBar) {
    
    struct Fields {
        int m_cols = 0;
        int m_rows = 0;
    };

    void loadFromItems(CCArray* items, int c, int r, bool preserve) {

        auto fields = m_fields.self();

        if(CCInteger* rows = typeinfo_cast<CCInteger*>(getUserObject("force-rows"))) {
            r = rows->getValue();
        }
        if(CCInteger* cols = typeinfo_cast<CCInteger*>(getUserObject("force-columns"))) {
            c = cols->getValue();
        }

        EditButtonBar::loadFromItems(items, c, r, preserve);

        if (Mod* mod = Loader::get()->getLoadedMod("hjfod.betteredit")) {
            if (mod->getVersion() <= VersionInfo{6, 8, 0, VersionTag{VersionTag::Beta, 3}}) {
                if (std::string_view(getID()) == "hjfod.betteredit/view-tab") {
                    updateUI(preserve);
                }
            }
        }

        // do not update if no change is made to prevent lag
        if (fields->m_cols == c && fields->m_rows == r && !preserve) return;

        fields->m_cols = c;
        fields->m_rows = r;
        updateUI(preserve);
    }

    void updateUI(bool preserve) {

        auto fields = m_fields.self();

        if (!Loader::get()->isModLoaded("hjfod.betteredit")) {

            EditButtonBar::loadFromItems(m_buttonArray, fields->m_cols, fields->m_rows, preserve);

            if (auto ui = typeinfo_cast<EditorUI*>(getParent())) {
                // fix visible pages when opening editor
                for (auto barPages : CCArrayExt<CCNode*>(m_pagesArray)) {
                    barPages->setVisible(false);
                }
                int page = m_scrollLayer->m_page % m_scrollLayer->m_pages->count();
                if (CCNode* currentPage = typeinfo_cast<CCNode*>(m_pagesArray->objectAtIndex(page))){
                    currentPage->setVisible(true);
                }

                auto winSize = CCDirector::get()->getWinSize();

                setPositionX(winSize.width / 2);
                m_scrollLayer->setPositionX(-(winSize.width / 2));

                float height;

                if (!Loader::get()->isModLoaded("geode.node-ids")){
                    height = ui->m_toolbarHeight;
                }
                else {
                    height = ui->m_tabsMenu->getPositionY();
                }

                if (auto menu = this->getChildByType<CCMenu>(0)) {
                    menu->setVisible(false);
                
                    // easier to create a new menu than work with the old one
                    CCMenu* navMenu = CCMenu::create();

                    navMenu->setPosition({-winSize.width / 2, 0});
                    navMenu->setContentSize(menu->getContentSize());
                    navMenu->setScale(menu->getScale());

                    float xOffset = (winSize.width / getScale())/2 - 104;
                    float yOffset = 2;

                    CCSprite* prevSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
                    prevSpr->setScale(0.6f);
                    CCSprite* nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
                    nextSpr->setFlipX(true);
                    nextSpr->setScale(0.6f);

                    CCMenuItemSpriteExtra* prevButtonOld = menu->getChildByType<CCMenuItemSpriteExtra>(0);
                    CCMenuItemSpriteExtra* nextButtonOld = menu->getChildByType<CCMenuItemSpriteExtra>(1);

                    CCMenuItemSpriteExtra* prevButton = CCMenuItemSpriteExtra::create(prevSpr, this, prevButtonOld->m_pfnSelector);
                    CCMenuItemSpriteExtra* nextButton = CCMenuItemSpriteExtra::create(nextSpr, this, nextButtonOld->m_pfnSelector);

                    prevButton->setPositionX(menu->getContentWidth()/2 - xOffset);
                    prevButton->setPositionY((height/2 + yOffset) / getScale());
                    
                    nextButton->setPositionX(menu->getContentWidth()/2 + xOffset);
                    nextButton->setPositionY((height/2 + yOffset) / getScale());

                    navMenu->addChild(prevButton);
                    navMenu->addChild(nextButton);

                    addChild(navMenu);
                }

                // layout the pages and set their widths and heights according to the row and column counts, scale accordingly
                for (ButtonPage* page : CCArrayExt<ButtonPage*>(m_scrollLayer->m_pages)) {
                    if (CCMenu* buttonMenu = page->getChildByType<CCMenu>(0)) {
                        RowLayout* layout = RowLayout::create();
                        layout->setAxisAlignment(AxisAlignment::Start);
                        layout->setCrossAxisAlignment(AxisAlignment::End);
                        layout->setAutoScale(true);
                        layout->setGrowCrossAxis(true);
                        layout->setCrossAxisOverflow(false);
                        buttonMenu->setLayout(layout);

                        float menuWidth = (fields->m_cols * 40 + fields->m_cols * layout->getGap()) - layout->getGap();
                        float menuHeight = (fields->m_rows * 40 + fields->m_rows * layout->getGap()) - layout->getGap();

                        buttonMenu->setContentSize({menuWidth, menuHeight});
                        buttonMenu->setAnchorPoint({0.5, 1});
                        buttonMenu->setPositionY(height / getScale() - 5);
                        buttonMenu->updateLayout();

                        float outerWidth = (winSize.width / getScale()) - 235;
                        float outerHeight = (height / getScale()) - 15;
                        float scaleW = outerWidth / menuWidth;
                        float scaleH = outerHeight / menuHeight;

                        buttonMenu->setScale(std::min(scaleW, scaleH));
                    }
                }
            }
        }
    }
};

class $modify(MyEditorUI, EditorUI) {

    static void onModify(auto& self) {
        (void) self.setHookPriority("EditorUI::init", 1000); 
        (void) self.setHookPriority("EditorUI::toggleMode", 1000);
        (void) self.setHookPriority("EditorUI::onSelectBuildTab", 1000);
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

        std::vector<TabData> tabs;
    };

    void toggleMode(CCObject* sender) {
        
        auto fields = m_fields.self();
        int prevMode = m_selectedMode;

        auto tag = sender->getTag();

        fields->m_editTabsMenu->setVisible(false);
        fields->m_deleteTabsMenu->setVisible(false);

        for (CCNode* node : CCArrayExt<CCNode*>(m_createButtonBars)) {
            if (CCNode* real = typeinfo_cast<CCNode*>(node->getUserObject("real-node"_spr))) {
                real->setVisible(false);
            }
        }

        for (CCNode* node : CCArrayExt<CCNode*>(fields->m_editButtonBars)) {
            node->setVisible(false);
        }

        for (CCNode* node : CCArrayExt<CCNode*>(fields->m_deleteButtonBars)) {
            node->setVisible(false);
        }

        CCArray* buttonBars = nullptr;

        switch (tag) {
            case 3: {
                if (fields->m_editTabsMenu->getChildrenCount() > 1){
                    fields->m_editTabsMenu->setVisible(true);
                }
                typeinfo_cast<CCNode*>(fields->m_editButtonBars->objectAtIndex(fields->m_selectedEditTab))->setVisible(true);
                break;
            }
            case 2: {
                auto buttonBar = typeinfo_cast<CCNode*>(m_createButtonBars->objectAtIndex(m_selectedTab));
                if (CCNode* real = typeinfo_cast<CCNode*>(buttonBar->getUserObject("real-node"_spr))) {
                    real->setVisible(true);
                }
                break;
            }
            case 1: {
                if (fields->m_deleteTabsMenu->getChildrenCount() > 1){
                    fields->m_deleteTabsMenu->setVisible(true);
                }
                typeinfo_cast<CCNode*>(fields->m_deleteButtonBars->objectAtIndex(fields->m_selectedDeleteTab))->setVisible(true);
                break;
            }
        }

        EditorUI::toggleMode(sender);

        if (prevMode != tag) {
            for (TabData data : fields->tabs) {
                if (data.onToggle && data.tabTag != -1 && m_createButtonBars->count() > data.tabTag) {
                    CCNode* buttonBar;
                    switch (data.type) {
                        case TabType::BUILD: {
                            if (m_createButtonBars->count() > data.tabTag) {
                                buttonBar = typeinfo_cast<CCNode*>(m_createButtonBars->objectAtIndex(data.tabTag));
                                if (CCNode* node = typeinfo_cast<CCNode*>(buttonBar->getUserObject("real-node"_spr))) {
                                    data.onToggle(this, tag == 2 && m_selectedTab == data.tabTag, node);
                                }
                                else {
                                    data.onToggle(this, tag == 2 && m_selectedTab == data.tabTag, buttonBar);
                                }
                            }
                            break;
                        }
                        case TabType::EDIT: {
                            if (fields->m_editButtonBars->count() > data.tabTag) {
                                buttonBar = typeinfo_cast<CCNode*>(fields->m_editButtonBars->objectAtIndex(data.tabTag));
                                data.onToggle(this, tag == 3 && fields->m_selectedEditTab == data.tabTag, buttonBar);
                            }
                            break;
                        }
                        case TabType::DELETE: {
                            if (fields->m_deleteButtonBars->count() > data.tabTag) {
                                buttonBar = typeinfo_cast<CCNode*>(fields->m_deleteButtonBars->objectAtIndex(data.tabTag));
                                data.onToggle(this, tag == 1 && fields->m_selectedDeleteTab == data.tabTag, buttonBar);
                            }
                            break;
                        }
                    }
                }
            }
        }
        
    }

    void updateButtons() {
        EditorUI::updateButtons();
        queueInMainThread([self = Ref(this)] {
            auto fields = self->m_fields.self();
            if (self->m_selectedMode == 3) {
                self->selectEditTab(fields->m_selectedEditTab);
            }
            if (self->m_selectedMode == 1) {
                self->selectDeleteTab(fields->m_selectedDeleteTab);
            }
        });
    }


    void toggleAll(TabType type, int tag){

        auto fields = m_fields.self();

        for (TabData data : fields->tabs) {

            bool isTab = data.type == type && data.tabTag == tag;

            switch (data.type) {
                case TabType::BUILD: {
                    if (CCNode* buttonBar = typeinfo_cast<CCNode*>(m_createButtonBars->objectAtIndex(data.tabTag))) {
                        if (CCNode* node = typeinfo_cast<CCNode*>(buttonBar->getUserObject("real-node"_spr))) {
                            if (isTab) node->setVisible(true);
                            if (data.onToggle) data.onToggle(this, isTab, node);
                        }
                        else {
                            if (data.onToggle) data.onToggle(this, isTab, buttonBar);
                        }
                    }
                    break;
                }
                case TabType::EDIT: {
                    if (CCNode* buttonBar = typeinfo_cast<CCNode*>(fields->m_editButtonBars->objectAtIndex(data.tabTag))) {
                        if (isTab) buttonBar->setVisible(true);
                        if (data.onToggle) data.onToggle(this, isTab, buttonBar);
                    }
                    break;
                }
                case TabType::DELETE: {
                    if (CCNode* buttonBar = typeinfo_cast<CCNode*>(fields->m_deleteButtonBars->objectAtIndex(data.tabTag))) {
                        if (isTab) buttonBar->setVisible(true);
                        if (data.onToggle) data.onToggle(this, isTab, buttonBar);
                    }
                    break;
                }
            }
        }
    }

    void selectBuildTab(int tab) {
        EditorUI::selectBuildTab(tab);

        auto fields = m_fields.self();

        if (m_selectedMode == 2) {
            for (CCNode* node : CCArrayExt<CCNode*>(m_createButtonBars)) {
                if (CCNode* real = typeinfo_cast<CCNode*>(node->getUserObject("real-node"_spr))) {
                    real->setVisible(false);
                }
            }
            toggleAll(TabType::BUILD, tab);
        }
        else {
            bool isNext = false;
            if ((tab == 0 && m_selectedTab == m_tabsArray->count()-1) || (!(m_selectedTab == 0 && tab == m_tabsArray->count()-1) && tab > m_selectedTab)) {
                isNext = true;
            }

            if (m_selectedMode == 3) {
                if (isNext) fields->m_selectedEditTab++;
                else fields->m_selectedEditTab--;

                if (fields->m_selectedEditTab < 0) {
                    fields->m_selectedEditTab = fields->m_editTabsArray->count()-1;
                }
                if (fields->m_selectedEditTab > fields->m_editTabsArray->count()-1) {
                    fields->m_selectedEditTab = 0;
                }

                selectEditTab(fields->m_selectedEditTab);
            }
            if (m_selectedMode == 1) {
                if (isNext) fields->m_selectedDeleteTab++;
                else fields->m_selectedDeleteTab--;

                if (fields->m_selectedDeleteTab < 0) {
                    fields->m_selectedDeleteTab = fields->m_deleteTabsArray->count()-1;
                }
                if (fields->m_selectedDeleteTab > fields->m_deleteTabsArray->count()-1) {
                    fields->m_selectedDeleteTab = 0;
                }

                selectDeleteTab(fields->m_selectedDeleteTab);
            }
        }
    }

    void setTabIcons(CCArray* togglers) {
        for (auto toggler : CCArrayExt<CCMenuItemToggler*>(togglers)) {
            if (!toggler) return;
            auto offBtnSprite = toggler->m_offButton->getNormalImage();
            auto onBtnSprite = toggler->m_onButton->getNormalImage();

            if (!offBtnSprite || !onBtnSprite) return;

            auto offBtnChild = offBtnSprite->getChildren()->objectAtIndex(0);
            auto onBtnChild = onBtnSprite->getChildren()->objectAtIndex(0);

            if (!offBtnChild || !onBtnChild) return;

            if (offBtnChild == onBtnChild) {
                if(auto rgba = geode::cast::typeinfo_cast<cocos2d::CCRGBAProtocol*>(onBtnChild)){
                    if (toggler->isOn()) {
                        rgba->setOpacity(255); 
                    }
                    else {
                        rgba->setOpacity(150); 
                    }
                }
            }
        }
    }

    //editor sounds compat
    void onSelectBuildTab(CCObject* sender) {
        if (m_selectedMode == 2) {
            EditorUI::onSelectBuildTab(sender);
            setTabIcons(m_tabsArray);
        }
    }

    void onSelectEditTab(CCObject* sender) {
        if (m_selectedMode == 3) {
            selectEditTab(sender->getTag());
            EditorUI::onSelectBuildTab(sender);
            setTabIcons(m_fields->m_editTabsArray);
        }
    }

    void onSelectDeleteTab(CCObject* sender) {
        if (m_selectedMode == 1) {
            selectDeleteTab(sender->getTag());
            EditorUI::onSelectBuildTab(sender);
            setTabIcons(m_fields->m_deleteTabsArray);
        }
    }

    void selectEditTab(int tab) {

        auto fields = m_fields.self();

        for (CCMenuItemToggler* node : CCArrayExt<CCMenuItemToggler*>(fields->m_editTabsArray)) {
            node->toggle(false);
        }
        static_cast<CCMenuItemToggler*>(fields->m_editTabsArray->objectAtIndex(tab))->toggle(true);
        for (CCNode* node : CCArrayExt<CCNode*>(fields->m_editButtonBars)) {
            node->setVisible(false);
        }

        fields->m_selectedEditTab = tab;
        toggleAll(TabType::EDIT, tab);
    }

    void selectDeleteTab(int tab) {

        auto fields = m_fields.self();

        for (CCMenuItemToggler* node : CCArrayExt<CCMenuItemToggler*>(fields->m_deleteTabsArray)) {
            node->toggle(false);
        }
        static_cast<CCMenuItemToggler*>(fields->m_deleteTabsArray->objectAtIndex(tab))->toggle(true);
        for (CCNode* node : CCArrayExt<CCNode*>(fields->m_deleteButtonBars)) {
            node->setVisible(false);
        }

        fields->m_selectedDeleteTab = tab;
        toggleAll(TabType::DELETE, tab);
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

        auto fields = m_fields.self();

        fields->m_editTabsMenu = CCMenu::create();
        fields->m_editTabsMenu->setLayout(createTabsLayout());
        fields->m_editTabsMenu->setID("edit-tabs-menu");
        fields->m_editTabsMenu->ignoreAnchorPointForPosition(false);
        fields->m_deleteTabsMenu = CCMenu::create();
        fields->m_deleteTabsMenu->setLayout(createTabsLayout());
        fields->m_deleteTabsMenu->setID("delete-tabs-menu");
        fields->m_deleteTabsMenu->ignoreAnchorPointForPosition(false);

        fields->m_editTabsArray = CCArray::create();
        fields->m_deleteTabsArray = CCArray::create();

        fields->m_editButtonBars = CCArray::create();
        fields->m_deleteButtonBars = CCArray::create();

        if (!EditorUI::init(editorLayer)) return false;

        if (!Loader::get()->isModLoaded("geode.node-ids")){
            m_tabsMenu->setLayout(createTabsLayout());
        }

        addChild(fields->m_editTabsMenu);
        addChild(fields->m_deleteTabsMenu);

        EditorTabs::addTab(this, TabType::EDIT, "edit-tab", [](EditorUI* ui, CCMenuItemToggler* toggler) -> CCNode* {
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

        EditorTabs::addTab(this, TabType::DELETE, "delete-tab", [](EditorUI* ui, CCMenuItemToggler* toggler) -> CCNode* {
            
            CCSprite* sprOn = CCSprite::createWithSpriteFrameName("edit_delBtn_001.png");
            sprOn->setScale(0.5f);
            CCSprite* sprOff = CCSprite::createWithSpriteFrameName("edit_delBtn_001.png");
            sprOff->setScale(0.5f);

            EditorTabUtils::setTabIcons(toggler, sprOn, sprOff);
            
            return EditorTabUtils::createDummyBar();
        }, [](EditorUI* ui, bool state, CCNode* bar) {
            ui->m_deleteMenu->setVisible(state);
        });

        for(TabData data : EditorTabs::get()->getRegisteredTabs()){
            EditorTabs::addTab(this, data);
        }

        return true;
    }

    void showUI(bool show){
        EditorUI::showUI(show);

        auto fields = m_fields.self();

        if (show) {
            switch (m_selectedMode) {
                case 3: {
                    if (fields->m_editTabsMenu->getChildrenCount() > 1){
                        fields->m_editTabsMenu->setVisible(true);
                    }

                    if (fields->m_selectedEditTab != 0) {
                        if (CCNode* node = getChildByID("hjfod.betteredit/custom-move-menu")) {
                            node->setVisible(false);
                        }
                        m_editButtonBar->setVisible(false);
                    }

                    typeinfo_cast<CCNode*>(fields->m_editButtonBars->objectAtIndex(fields->m_selectedEditTab))->setVisible(true);
                    break;
                }
                case 1: {
                    if (fields->m_deleteTabsMenu->getChildrenCount() > 1){
                        fields->m_deleteTabsMenu->setVisible(true);
                    }

                    if (fields->m_selectedDeleteTab != 0) {
                        m_deleteMenu->setVisible(false);
                    }

                    typeinfo_cast<CCNode*>(fields->m_deleteButtonBars->objectAtIndex(fields->m_selectedDeleteTab))->setVisible(true);
                    break;
                }
            }
        }
        else {
            fields->m_editTabsMenu->setVisible(false);
            fields->m_deleteTabsMenu->setVisible(false);

            for (CCNode* node : CCArrayExt<CCNode*>(fields->m_editButtonBars)) {
                node->setVisible(false);
            }
            for (CCNode* node : CCArrayExt<CCNode*>(fields->m_deleteButtonBars)) {
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
};

class $modify(LateEditorUI, EditorUI) {

    static void onModify(auto& self) {
        (void) self.setHookPriority("EditorUI::init", -1000);
        (void) self.setHookPriority("EditorUI::toggleMode", -1000);
        (void) self.setHookPriority("EditorUI::onPause", -1000);
    }

    bool init(LevelEditorLayer* editorLayer) {

        if (!EditorUI::init(editorLayer)) return false;
        

        EditorUI* editorUI = static_cast<EditorUI*>(this);
        MyEditorUI* myEditorUI = static_cast<MyEditorUI*>(editorUI);

        auto fields = myEditorUI->m_fields.self();

        CCSize winSize = CCDirector::get()->getWinSize();
        float height = 90;

        if (Loader::get()->isModLoaded("geode.node-ids")){
            if (CCNode* menu = getChildByID("toolbar-toggles-menu")){
                height = menu->getScaledContentSize().height;
            }
        }

        m_tabsMenu->setAnchorPoint({0.5, 0});
        m_tabsMenu->setPosition({winSize.width/2, height});

        fields->m_editTabsMenu->setPosition({winSize.width/2, height});
        fields->m_editTabsMenu->setAnchorPoint(m_tabsMenu->getAnchorPoint());
        fields->m_editTabsMenu->setContentSize(m_tabsMenu->getContentSize());
        fields->m_editTabsMenu->setScale(m_tabsMenu->getScale());
        fields->m_editTabsMenu->setVisible(false);
        fields->m_editTabsMenu->updateLayout();

        fields->m_deleteTabsMenu->setPosition({winSize.width/2, height});
        fields->m_deleteTabsMenu->setAnchorPoint(m_tabsMenu->getAnchorPoint());
        fields->m_deleteTabsMenu->setContentSize(m_tabsMenu->getContentSize());
        fields->m_deleteTabsMenu->setScale(m_tabsMenu->getScale());
        fields->m_deleteTabsMenu->setVisible(false);
        fields->m_deleteTabsMenu->updateLayout();

        for (auto c : CCArrayExt<CCNode*>(this->getChildren())) {
            if (auto bar = typeinfo_cast<EditButtonBar*>(c)) {
                static_cast<MyEditButtonBar*>(bar)->updateUI(true);
            }
        }

        float scaleBar = myEditorUI->m_createButtonBar->getScale();

        for (CCNode* node : CCArrayExt<CCNode*>(m_createButtonBars)) {
            if (CCNode* real = typeinfo_cast<CCNode*>(node->getUserObject("real-node"_spr))) {
                real->setScale(scaleBar);
                real->setPositionY(real->getPositionY() * scaleBar);
            }
        }

        for (CCNode* node : CCArrayExt<CCNode*>(fields->m_editButtonBars)) {
            if (!typeinfo_cast<EditButtonBar*>(node)){
                node->setScale(scaleBar);
                node->setPositionY(node->getPositionY() * scaleBar);
            }
        }

        for (CCNode* node : CCArrayExt<CCNode*>(fields->m_deleteButtonBars)) {
            if (!typeinfo_cast<EditButtonBar*>(node)){
                node->setScale(scaleBar);
                node->setPositionY(node->getPositionY() * scaleBar);
            }
        }

        return true;
    }

    void toggleMode(CCObject* sender) {
        EditorUI::toggleMode(sender);
        resolveTabIssue();
    }

    void onPause(CCObject* sender) {
        EditorUI::onPause(sender);
        resolveTabIssue();
    }

    void resolveTabIssue() {

        EditorUI* editorUI = static_cast<EditorUI*>(this);
        MyEditorUI* myEditorUI = static_cast<MyEditorUI*>(editorUI);

        if (m_selectedMode == 3 && myEditorUI->m_fields->m_selectedEditTab != 0) {
            if (CCNode* node = getChildByID("hjfod.betteredit/custom-move-menu")) {
                node->setVisible(false);
            }
            m_editButtonBar->setVisible(false);
        }
    }

};

class $modify(EditorPauseLayer) {
    
    void onResume(CCObject* pSender) {
        EditorPauseLayer::onResume(pSender);

        MyEditorUI* ui = static_cast<MyEditorUI*>(EditorUI::get());

        if (ui->m_selectedMode == 3 && ui->m_fields->m_selectedEditTab != 0) {
            if (CCNode* node = ui->getChildByID("hjfod.betteredit/custom-move-menu")) {
                node->setVisible(false);
            }
        }
    }
};