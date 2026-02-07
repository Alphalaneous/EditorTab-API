#include "EditorUI.hpp"
#include "EditButtonBar.hpp"

ETEditorUI* ETEditorUI::s_instance = nullptr;

ETEditorUI* ETEditorUI::get() {
    return s_instance;
}

void ETEditorUI::onModify(auto& self) {
    (void) self.setHookPriority("EditorUI::init", Priority::EarlyPost);
}

bool ETEditorUI::init(LevelEditorLayer* editorLayer) {
    s_instance = this;

    if (!EditorUI::init(editorLayer)) return false;

    auto fields = m_fields.self();

    m_tabsMenu->removeAllChildren();

    m_tabsMenu->setAnchorPoint({0.5f, 0.f});
    m_tabsMenu->setPosition({getContentWidth() / 2, m_toolbarHeight - 1});

    setupTabs();

    fields->m_initialized = true;

    /*alpha::editor_tabs::addTab("delete-test"_spr, alpha::editor_tabs::DELETE, [] {
        auto label = CCLabelBMFont::create("Meow", "bigFont.fnt");

        auto btn = CCMenuItemExt::createSpriteExtra(label, [] (auto btn) {
            alpha::editor_tabs::removeTab("delete-test"_spr);
        });

        auto menu = CCMenu::create();
        menu->setContentSize(btn->getContentSize());
        menu->ignoreAnchorPointForPosition(false);
        menu->setAnchorPoint({0.5f, 0.5f});

        auto winSize = CCDirector::get()->getWinSize();
        menu->setPosition({winSize.width / 2, 50});

        btn->setPosition(menu->getContentSize() / 2);

        menu->addChild(btn);

        return menu;
    }, [] {
        return CCSprite::createWithSpriteFrameName("spike_01_001.png");
    }, [] (bool state, auto tab) {
        log::info("state: {}", state);
    }, [] (int rows, int cols, auto tab) {
        log::info("rows: {}, cols: {}", rows, cols);
    });

    alpha::editor_tabs::addTab("log-test"_spr, alpha::editor_tabs::EDIT, [] {
        auto label = CCLabelBMFont::create("Meow", "bigFont.fnt");

        auto btn = CCMenuItemExt::createSpriteExtra(label, [] (auto btn) {
            log::info("mode: {}", alpha::editor_tabs::getCurrentMode().unwrapOr(""));
            log::info("tab: {}", alpha::editor_tabs::getCurrentTab().unwrapOr(""));
            log::info("tab mode: {}", alpha::editor_tabs::getTabMode("log-test"_spr).unwrapOr(""));
            log::info("tab idx: {}", alpha::editor_tabs::getTabIndex("log-test"_spr).unwrapOr(-1));
            log::info("tab node: {}", alpha::editor_tabs::nodeForTab("log-test"_spr).unwrapOr(nullptr));

            alpha::editor_tabs::switchTab("new-mode-test-15"_spr);
        });

        auto menu = CCMenu::create();
        menu->setContentSize(btn->getContentSize());
        menu->ignoreAnchorPointForPosition(false);
        menu->setAnchorPoint({0.5f, 0.5f});

        auto winSize = CCDirector::get()->getWinSize();
        menu->setPosition({winSize.width / 2, 50});

        btn->setPosition(menu->getContentSize() / 2);

        menu->addChild(btn);

        return menu;
    }, [] {
        return CCSprite::createWithSpriteFrameName("spike_01_001.png");
    }, [] (bool state, auto tab) {
        log::info("state: {}", state);
    }, [] (int rows, int cols, auto tab) {
        log::info("rows: {}, cols: {}", rows, cols);
    });

    for (int i = 0; i < 30; i++) {
        alpha::editor_tabs::addTab(fmt::format("new-mode-test-{}"_spr, i), alpha::editor_tabs::BUILD, [i] {
            std::vector<Ref<CCNode>> nodes;
            auto label = CCLabelBMFont::create(fmt::format("test-{}", i).c_str(), "bigFont.fnt");
            nodes.push_back(label);

            auto buttonBar = alpha::editor_tabs::createEditButtonBar(nodes);

            return buttonBar;
        }, [i] {
            return CCLabelBMFont::create(fmt::format("{}", i).c_str(), "bigFont.fnt");
        });
    }*/

    return true;
}

void ETEditorUI::showUI(bool show) {
    EditorUI::showUI(show);
    auto fields = m_fields.self();
    fields->m_uiVisible = show;

    auto& currentTabs = fields->m_tabs[fields->m_currentMode];

    m_tabsMenu->setVisible(currentTabs.size() > 1 && show);

    if (show) switchMode(fields->m_currentMode);
    else {
        for (const auto& [k, v] : fields->m_tabs) {
            for (const auto& tabData : v) {
                setTabVisible(tabData.tab, false);
            }
        }
    }
}

void ETEditorUI::onPause(CCObject* sender) {
    auto fields = m_fields.self();
    auto currentTab = fields->m_currentTab;

    EditorUI::onPause(sender);

    switchTab(currentTab);
}

void ETEditorUI::setupTabs() {

    auto fields = m_fields.self();

    auto& createTab = fields->m_tabs[alpha::editor_tabs::BUILD];
    auto arr =  m_createButtonBars->asExt<CCNode>();
    
    auto buildMode = alpha::editor_tabs::BUILD;

    createTab.push_back(tabWithSpriteFrame("block", buildMode, arr[0], "square_01_001.png"));
    createTab.push_back(tabWithSpriteFrame("outline", buildMode, arr[1], "blockOutline_01_001.png"));
    createTab.push_back(tabWithSpriteFrame("slope", buildMode, arr[2], "triangle_a_02_001.png"));
    createTab.push_back(tabWithSpriteFrame("hazard", buildMode, arr[3], "spike_01_001.png"));
    createTab.push_back(tabWithSpriteFrame("3d", buildMode, arr[4], "persp_outline_01_001.png"));
    createTab.push_back(tabWithSpriteFrame("portal", buildMode, arr[5], "ring_01_001.png"));
    createTab.push_back(tabWithSpriteFrame("monster", buildMode, arr[6], "GJBeast01_01_001.png"));
    createTab.push_back(tabWithSpriteFrame("pixel", buildMode, arr[7], "pixelb_03_01_001.png"));
    createTab.push_back(tabWithSpriteFrame("collectible", buildMode, arr[8], "pixelitem_001_001.png"));
    createTab.push_back(tabWithSpriteFrame("icon", buildMode, arr[9], "particle_01_001.png"));
    createTab.push_back(tabWithSpriteFrame("deco", buildMode, arr[10], "d_spikes_01_001.png"));
    createTab.push_back(tabWithSpriteFrame("sawblade", buildMode, arr[11], "sawblade_02_001.png"));
    createTab.push_back(tabWithSpriteFrame("trigger", buildMode, arr[12], "edit_eTintCol01Btn_001.png"));
    createTab.push_back(tabWithNodeCallback("custom", buildMode, arr[13], [this] {
        auto container = CCNodeRGBA::create();
        container->setCascadeColorEnabled(true);
        container->setCascadeOpacityEnabled(true);
        container->setAnchorPoint({0.5f, 0.5f});

        auto label = CCLabelBMFont::create("C", "bigFont.fnt");
        container->setContentSize(label->getContentSize());

        label->setScale(1.2f);
        label->setPosition(container->getContentSize()/2 + CCPoint{0, 1});

        container->addChild(label);

        return container;
    }));

    fields->m_tabs[alpha::editor_tabs::EDIT].push_back(tabWithSpriteFrame("edit", alpha::editor_tabs::EDIT, m_editButtonBar, "GJ_hammerIcon_001.png"));
    fields->m_tabs[alpha::editor_tabs::DELETE].push_back(tabWithSpriteFrame("delete", alpha::editor_tabs::DELETE, m_deleteMenu, "edit_delBtn_001.png"));

    setupButtons();
    switchMode(alpha::editor_tabs::BUILD);
}

InternalTabData ETEditorUI::tabWithSpriteFrame(ZStringView id, ZStringView mode, CCNode* tab, ZStringView frameName) {
    auto sprOn = CCSprite::createWithSpriteFrameName(frameName.c_str());

    auto sprOff = CCSprite::createWithSpriteFrameName(frameName.c_str());
    sprOff->setOpacity(150);
    
    return {id, mode, tab, sprOn, sprOff};
}

InternalTabData ETEditorUI::tabWithNodeCallback(ZStringView id, ZStringView mode, CCNode* tab, std::function<CCNode*()>&& callback) {
    auto sprOn = callback();

    auto sprOff = callback();
    if (auto rgba = typeinfo_cast<CCRGBAProtocol*>(sprOff)) {
        rgba->setOpacity(150);
    }
    
    return {id, mode, tab, sprOn, sprOff};
}

void ETEditorUI::fitNode(CCNode* node, const CCSize& size) {
    float scaleX = size.width / node->getContentWidth();
    float scaleY = size.height / node->getContentHeight();
    float scale = std::min(scaleX, scaleY);

    node->setScale(scale);
}

CCMenuItemToggler* ETEditorUI::createToggler(const InternalTabData& tabData) {

    auto tabOn = CCSprite::createWithSpriteFrameName("GJ_tabOn_001.png");
    auto tabOff = CCSprite::createWithSpriteFrameName("GJ_tabOff_001.png");
    tabOff->setOpacity(150);

    fitNode(tabData.buttonTopOn, {tabOn->getContentWidth() - 7, tabOn->getContentHeight() - 5});
    fitNode(tabData.buttonTopOff, {tabOff->getContentWidth() - 7, tabOff->getContentHeight() - 5});

    tabData.buttonTopOn->setPosition({tabOn->getContentWidth() / 2, tabOn->getContentHeight() / 2 - 1});
    tabData.buttonTopOff->setPosition({tabOff->getContentWidth() / 2, tabOff->getContentHeight() / 2 - 1});

    tabOn->addChild(tabData.buttonTopOn);
    tabOff->addChild(tabData.buttonTopOff);

    auto toggler = CCMenuItemExt::createToggler(tabOn, tabOff, [this](CCMenuItemToggler* toggler) {
        auto fields = m_fields.self();

        for (const auto& tab : fields->m_tabs[fields->m_currentMode]) {
            if (tab.toggler != toggler) {
                setTabVisible(tab.tab, false);
                tab.toggler->toggle(false);
                tab.toggler->setClickable(true);
            }
        }
        
        auto& tabData = fields->m_tabs[fields->m_currentMode][toggler->getTag()];
        switchTab(tabData);
    });

    toggler->setID(fmt::format("{}-tab", tabData.id));

    return toggler;
}

void ETEditorUI::setupButton(InternalTabData& tabData) {
    auto fields = m_fields.self();

    auto toggler = createToggler(tabData);
    int idx = fields->m_tabs[tabData.mode].size();

    toggler->setTag(idx);
    tabData.idx = idx;
    toggler->setVisible(false);

    m_tabsMenu->addChild(toggler);
    tabData.toggler = toggler;

    m_tabsMenu->updateLayout();
}

void ETEditorUI::setupButtons() {
    auto fields = m_fields.self();

    m_tabsMenu->setContentWidth(getContentWidth() / m_tabsMenu->getScale());

    auto tabWidth = m_tabsMenu->getContentWidth() - 36;
    fields->m_maxTabs = (tabWidth - 2) / 34;

    fields->m_arrowMenu = CCMenu::create();
    fields->m_arrowMenu->setContentSize(m_tabsMenu->getContentSize());
    fields->m_arrowMenu->ignoreAnchorPointForPosition(false);
    fields->m_arrowMenu->setPosition(m_tabsMenu->getPosition());
    fields->m_arrowMenu->setAnchorPoint(m_tabsMenu->getAnchorPoint());
    fields->m_arrowMenu->setID("tabs-navigation-menu"_spr);
    
    auto prevButton = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_arrow_02_001.png", 0.4f, [this, fields] (auto btn) {
        auto currentPage = fields->m_tabPage[fields->m_currentMode];
        auto maxPages = fields->m_tabs[fields->m_currentMode].size() / fields->m_maxTabs;

        currentPage--;
        if (currentPage < 0) {
            currentPage = maxPages - 1;
        }

        goToPage(currentPage);
    });

    auto nextBtn = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_arrow_02_001.png", 0.4f, [this, fields] (auto btn) {
        auto currentPage = fields->m_tabPage[fields->m_currentMode];
        auto maxPages = fields->m_tabs[fields->m_currentMode].size() / fields->m_maxTabs;

        currentPage++;
        if (currentPage > maxPages - 1) {
            currentPage = 0;
        }

        goToPage(currentPage);
    });
    nextBtn->getChildByType<CCSprite>(0)->setFlipX(true);

    prevButton->setPosition({15, fields->m_arrowMenu->getContentHeight() / 2});
    nextBtn->setPosition({fields->m_arrowMenu->getContentWidth() - 15, fields->m_arrowMenu->getContentHeight() / 2});

    fields->m_arrowMenu->addChild(prevButton);
    fields->m_arrowMenu->addChild(nextBtn);

    addChild(fields->m_arrowMenu);

    for (auto& [k, v] : fields->m_tabs) {
        int idx = 0;
        for (auto& tab : v) {
            tab.idx = idx;
            auto toggler = createToggler(tab);
            toggler->setTag(idx);
            toggler->setVisible(false);
            tab.toggler = toggler;

            m_tabsMenu->addChild(toggler);

            idx++;
        }
    }

    m_tabsMenu->updateLayout();
}

void ETEditorUI::switchMode(ZStringView mode) {
    auto fields = m_fields.self();
    
    fields->m_currentMode = mode;

    auto& currentTabs = fields->m_tabs[mode];
    auto idx = fields->m_tabIndex[mode];

    for (const auto& [k, v] : fields->m_tabs) {
        for (const auto& tabData : v) {
            setTabVisible(tabData.tab, false);
            tabData.toggler->setVisible(false);
        }
    }

    auto& currentTab = currentTabs[idx];

    for (const auto& tabData : currentTabs) {
        tabData.toggler->setVisible(true);

        if (tabData.toggler != currentTab.toggler) {
            tabData.toggler->toggle(false);
        }
    }

    auto currentPage = fields->m_tabPage[fields->m_currentMode];
    switchTab(currentTab);
    goToPage(currentPage);

    m_tabsMenu->setVisible(currentTabs.size() > 1 && fields->m_uiVisible);
    m_tabsMenu->updateLayout();

    fields->m_arrowMenu->setVisible(currentTabs.size() > fields->m_maxTabs);

    auto dummy = CCNode::create();
    dummy->setTag(-1);
    toggleMode(dummy);
}

void ETEditorUI::goToPage(int page) {
    auto fields = m_fields.self();
    
    fields->m_tabPage[fields->m_currentMode] = page;
    
    auto& currentTabs = fields->m_tabs[fields->m_currentMode];

    for (const auto& tabData : currentTabs) {
        int tabPage = tabData.idx / fields->m_maxTabs;
        tabData.toggler->setVisible(page == tabPage);
    }

    m_tabsMenu->updateLayout();
}

void ETEditorUI::switchTab(ZStringView id) {
    auto fields = m_fields.self();

    for (const auto& [k, v] : fields->m_tabs) {
        for (const auto& tabData : v) {
            if (tabData.id != id) continue;
            fields->m_currentTab = tabData;
            fields->m_tabIndex[tabData.mode] = tabData.idx;

            switchMode(tabData.mode);

            int page = tabData.idx / fields->m_maxTabs;
            goToPage(page);
            break;
        }
    }
}

void ETEditorUI::switchTab(const InternalTabData& tabData) {
    auto fields = m_fields.self();

    setTabVisible(tabData.tab, true);
    fields->m_tabIndex[fields->m_currentMode] = tabData.idx;

    for (const auto& [k, v] : fields->m_tabs) {
        for (const auto& tabData : v) {
            tabData.toggler->toggle(false);
        }
    }

    tabData.toggler->setClickable(false);
    tabData.toggler->toggle(true);

    auto& currentTab = fields->m_currentTab;

    if (tabData.toggler != currentTab.toggler) {
        if (tabData.onTab) tabData.onTab(true, tabData.tab);
        if (currentTab.onTab) currentTab.onTab(false, currentTab.tab);
    }

    fields->m_currentTab = tabData;

    int page = tabData.idx / fields->m_maxTabs;
    goToPage(page);
}

void ETEditorUI::toggleMode(CCObject* sender) {
    auto fields = m_fields.self();
    bool skip = false;
    if (sender && sender->getTag() == -1) {
        int tag = 0;
        if (fields->m_currentMode == alpha::editor_tabs::BUILD) tag = 2;
        else if (fields->m_currentMode == alpha::editor_tabs::EDIT) tag = 3;
        else if (fields->m_currentMode == alpha::editor_tabs::DELETE) tag = 1;
        sender->setTag(tag);
        skip = true;
        if (tag == 0) {
            m_buildModeBtn->setSprite(CCSprite::createWithSpriteFrameName("edit_buildBtn_001.png"));
            m_editModeBtn->setSprite(CCSprite::createWithSpriteFrameName("edit_editBtn_001.png"));
            m_deleteModeBtn->setSprite(CCSprite::createWithSpriteFrameName("edit_deleteBtn_001.png"));
            m_selectedMode = -1;
            return;
        }
    }

    EditorUI::toggleMode(sender);

    if (!fields->m_initialized || skip) return;

    switch (sender->getTag()) {
        case 3: {
            switchMode(alpha::editor_tabs::EDIT);
            break;
        }
        case 2: {
            switchMode(alpha::editor_tabs::BUILD);
            break;
        }
        case 1: {
            switchMode(alpha::editor_tabs::DELETE);
            break;
        }
        default: {
            break;
        }
    }
}

void ETEditorUI::reloadEditTabs() {
    auto fields = m_fields.self();

    auto rows = GameManager::get()->getIntGameVariable("0049");
    auto cols = GameManager::get()->getIntGameVariable("0050");

    for (const auto& [k, v] : fields->m_tabs) {
        for (const auto& tabData : v) {
            auto bar = typeinfo_cast<EditButtonBar*>(tabData.tab.data());
            if (bar) bar->reloadItems(rows, cols);

            if (tabData.onReload) tabData.onReload(rows, cols, tabData.tab);
        }
    }
}

void ETEditorUI::setTabVisible(CCNode* tab, bool visible) {
    if (auto bar = static_cast<ETEditButtonBar*>(typeinfo_cast<EditButtonBar*>(tab))) {
        bar->optimizedSetVisible(visible);
        return;
    }
    tab->setVisible(visible);
}

void ETEditorUI::updateCreateMenu(bool selectTab) {
    EditorUI::updateCreateMenu(selectTab);
    auto fields = m_fields.self();

    if (selectTab) {
        int page = m_selectedTab / fields->m_maxTabs;
        fields->m_tabIndex[fields->m_currentMode] = m_selectedTab;
        switchMode(fields->m_currentMode);
        goToPage(page);
        return;
    }

    if (fields->m_initialized) {
        auto idx = fields->m_tabIndex[fields->m_currentMode];
        fields->m_tabIndex[fields->m_currentMode] = idx;
        switchMode(fields->m_currentMode);
    }
}

void ETEditorUI::addTab(geode::ZStringView tabID, geode::ZStringView modeID, CreateTab&& createTab, CreateTabIcon&& createIcon, ToggleTab&& toggleTab, ReloadTab&& reloadTab) {
    auto fields = m_fields.self();
    
    auto sprOn = createIcon();

    auto sprOff = createIcon();
    if (auto rgba = typeinfo_cast<CCRGBAProtocol*>(sprOff)) {
        rgba->setOpacity(150);
    }

    auto tab = createTab();
    tab->setID(fmt::format("{}-tab-bar", tabID));
    setTabVisible(tab, false);

    tab->setZOrder(10);
    addChild(tab);
    
    auto tabData = InternalTabData{tabID, modeID, tab, sprOn, sprOff, std::move(toggleTab), std::move(reloadTab)};
    setupButton(tabData);

    if (modeID == fields->m_currentMode) {
        tabData.toggler->setVisible(true);
        m_tabsMenu->updateLayout();
    }

    fields->m_tabs[modeID].push_back(std::move(tabData));

    switchMode(fields->m_currentMode);
}

void ETEditorUI::removeTab(geode::ZStringView tabID) {
    auto fields = m_fields.self();

    for (auto& [k, v] : fields->m_tabs) {
        for (const auto& tabData : v) {
            if (tabData.id != tabID) continue;
            auto& tabIndex = fields->m_tabIndex[fields->m_currentMode];

            tabData.toggler->removeFromParent();
            tabData.tab->removeFromParent();

            if (tabData.mode == fields->m_currentMode) {
                m_tabsMenu->setVisible(v.size() > 2);
                m_tabsMenu->updateLayout();

                if (tabData.idx == tabIndex) {
                    tabIndex = std::max(tabIndex - 1, 0);
                }
            }
            
            v.erase(std::find(v.begin(), v.end(), tabData));

            if (v.size() == 0) {
                switchMode(alpha::editor_tabs::BUILD);
                break;
            }

            switchTab(v[tabIndex]);
            break;
        }
    }
}

Result<const InternalTabData&> ETEditorUI::getTab(geode::ZStringView tabID) {
    auto fields = m_fields.self();

    for (auto& [k, v] : fields->m_tabs) {
        for (const auto& tabData : v) {
            if (tabData.id == tabID) return geode::Ok(tabData);
        }
    }

    return geode::Err("Tab not found");
}