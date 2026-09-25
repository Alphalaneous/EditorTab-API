#include "EditorUI.hpp"
#include "EditorTab.hpp"
#include "ModeHandler.hpp"
#include "Mode.hpp"
#include "Tab.hpp"
#include "TabMenu.hpp"

ETEditorUI* ETEditorUI::s_instance = nullptr;

bool ETEditorUI::init(LevelEditorLayer* editorLayer) {
    s_instance = this;

    auto fields = m_fields.self();
    fields->m_modeHandler = std::make_shared<ModeHandler>();

    if (!EditorUI::init(editorLayer)) return false;

    auto spacerLeft = getChildByID("spacer-line-left");
    spacerLeft->setPositionX(96.f);

    auto categoriesMenu = getChildByID("toolbar-categories-menu");
    categoriesMenu->setContentSize({90.f, 90.f});
    categoriesMenu->setPositionX(47.f);
    auto layout = static_cast<AxisLayout*>(categoriesMenu->getLayout());
    layout->setAxis(Axis::Row);
    layout->setGrowCrossAxis(true);
    layout->setAxisReverse(false);

    categoriesMenu->removeAllChildren();

    auto buildToggle = createModeToggle(alpha::editor_tabs::Build, 2, "build.png"_spr);
    categoriesMenu->addChild(buildToggle);
    m_uiItems->addObject(buildToggle);

    auto editToggle = createModeToggle(alpha::editor_tabs::Edit, 3, "edit.png"_spr);
    categoriesMenu->addChild(editToggle);
    m_uiItems->addObject(editToggle);

    auto deleteToggle = createModeToggle(alpha::editor_tabs::Delete, 1, "delete.png"_spr);
    categoriesMenu->addChild(deleteToggle);
    m_uiItems->addObject(deleteToggle);

    auto viewToggle = createModeToggle(alpha::editor_tabs::View, 4, "view.png"_spr);
    categoriesMenu->addChild(viewToggle);
    m_uiItems->addObject(viewToggle);

    categoriesMenu->updateLayout();

    buildToggle->toggle(true);

    auto winSize = CCDirector::get()->getWinSize();

    auto newTabMenu = TabMenu::create();
    newTabMenu->setPosition({winSize.width / 2.f, m_tabsMenu->getPositionY()});
    newTabMenu->setContentSize(m_tabsMenu->getContentSize());
    newTabMenu->setScale(m_tabsMenu->getScale());
    newTabMenu->setZOrder(m_tabsMenu->getZOrder());
    newTabMenu->setID(m_tabsMenu->getID());

    m_tabsMenu->removeFromParent();
    m_tabsMenu = newTabMenu;

    addChild(m_tabsMenu);
    m_uiItems->addObject(m_tabsMenu);

    setupBuildMode();
    setupEditMode();
    setupDeleteMode();
    setupViewMode();

    ModeHandler::get()->switchMode(alpha::editor_tabs::Build);

    return true;
}

ETEditorUI* ETEditorUI::get() {
    return s_instance;
}

CCMenuItemToggler* ETEditorUI::createModeToggle(ZStringView ID, int tag, ZStringView sprite) {
    auto spr = CCSprite::create(sprite.c_str());
    
    auto sprOn = ButtonSprite::create(spr, 40, true, 40.f, "GJ_button_02.png", 1.f);
    auto sprOff = ButtonSprite::create(spr, 40, true, 40.f, "GJ_button_04.png", 1.f);
    
    sprOn->setContentSize({40.f, 40.f});
    sprOff->setContentSize({40.f, 40.f});

    sprOn->updateSpriteOffset({0.f, -1.5f});
    sprOff->updateSpriteOffset({0.f, -1.5f});

    auto toggler = CCMenuItemToggler::create(sprOff, sprOn, this, menu_selector(EditorUI::toggleMode));
    toggler->setTag(tag);
    toggler->m_notClickable = true;

    m_fields->m_modeToggles.push_back(toggler);

    return toggler;
}

void ETEditorUI::setupCreateMenu() {
    EditorUI::setupCreateMenu();

    m_tabsMenu->removeAllChildren();
    m_tabsArray->removeAllObjects();

    auto fields = m_fields.self();

    fields->m_modeHandler->setupModes();
    fields->m_initialized = true;
}

void ETEditorUI::updateModeToggles(int mode) {
    auto fields = m_fields.self();

    for (auto toggler : fields->m_modeToggles) {
        toggler->toggle(mode == toggler->getTag());
    }
}

void ETEditorUI::toggleMode(cocos2d::CCObject* sender) {
    int mode = sender->getTag();

    if (m_selectedMode != mode) {
        auto fields = m_fields.self();

        updateModeToggles(sender->getTag());

        std::string modeID;
        switch (sender->getTag()) {
            case 1: {
                modeID = alpha::editor_tabs::Delete;
                break;
            }
            case 2: {
                modeID = alpha::editor_tabs::Build;
                break;
            }
            case 3: {
                modeID = alpha::editor_tabs::Edit;
                break;
            }
            case 4: {
                modeID = alpha::editor_tabs::View;
                break;
            }
        }

        ModeHandler::get()->switchMode(modeID);
        resetUI();
    }

    updateSpecialTabVisibility();
}

void ETEditorUI::showUI(bool show) {
    EditorUI::showUI(show);
    
    if (show) {
        updateSpecialTabVisibility();
    }

    auto mode = static_cast<::internal::Mode*>(ModeHandler::get()->getCurrentMode());
    if (!mode) return;

    if (show) {
        mode->showMode();
    }
    else {
        mode->hideMode();
    }
}

void ETEditorUI::updateSpecialTabVisibility() {
    auto mode = static_cast<::internal::Mode*>(ModeHandler::get()->getCurrentMode());
    if (!mode) return;

    if (mode->getID() == alpha::editor_tabs::Delete) {
        auto tab = static_cast<::internal::Tab*>(mode->getCurrentTab());
        m_deleteMenu->setVisible(tab->getID() == "delete");
    }
    if (mode->getID() == alpha::editor_tabs::Edit) {
        auto tab = static_cast<::internal::Tab*>(mode->getCurrentTab());
        m_editButtonBar->setVisible(tab->getID() == "edit");
    }
}

void ETEditorUI::updateButtons() {
    EditorUI::updateButtons();
    updateSpecialTabVisibility();
}

bool ETEditorUI::initialized() {
    return m_fields->m_initialized;
}

void ETEditorUI::selectBuildTab(int tab) {
    EditorUI::selectBuildTab(tab);

    auto mode = static_cast<::internal::Mode*>(ModeHandler::get()->getCurrentMode());
    if (mode && mode->getID() == alpha::editor_tabs::Build) {
        mode->switchTab(std::string(idForBuildTabIndex(tab).unwrapOrDefault()));
    }
}

void ETEditorUI::updateCreateMenu(bool selectTab) {
    if (m_selectedMode != 2) return;
    
    for (auto item : CCArrayExt<CreateMenuItem, false>(m_createButtonArray)) {
        enableButton(item);
    }

    for (auto item : CCArrayExt<CreateMenuItem, false>(m_customObjectButtonArray)) {
        enableButton(item);
    }

    for (auto item : CCArrayExt<CreateMenuItem, false>(m_createButtonArray)) {
        if (item->m_objectID == m_selectedObjectIndex) {
            disableButton(item);
            if (!selectTab) return;
            
            selectBuildTab(item->m_tabIndex);
            m_createButtonBar->goToPage(item->m_pageIndex);
            return;
        }
    }

    for (auto item : CCArrayExt<CreateMenuItem, false>(m_customObjectButtonArray)) {
        if (item->m_objectID == m_selectedObjectIndex) {
            disableButton(item);
            if (!selectTab) return;
            
            selectBuildTab(item->m_tabIndex);
            m_createButtonBar->goToPage(item->m_pageIndex);
            return;
        }
    }
}

void ETEditorUI::createMoveMenu() {
    EditorUI::createMoveMenu();
}

void ETEditorUI::setupDeleteMenu() {
    EditorUI::setupDeleteMenu();
}

CCNode* ETEditorUI::iconForIdx(int idx) {
    if (idx == 13) {
        auto container = CCNodeRGBA::create();
        container->setCascadeColorEnabled(true);
        container->setCascadeOpacityEnabled(true);
        container->setAnchorPoint({0.5f, 0.5f});

        auto label = CCLabelBMFont::create("C", "bigFont.fnt");
        container->setContentSize(label->getContentSize());

        label->setScale(1.25f);
        label->setPosition(container->getContentSize() / 2.f + CCPoint{0.f, 2.5f});

        container->addChild(label);
        return container;
    }

    static constexpr std::array<std::string_view, 13> TabIcons {
        "square_01_001.png",
        "blockOutline_01_001.png",
        "triangle_a_02_001.png",
        "spike_01_001.png",
        "persp_outline_01_001.png",
        "ring_01_001.png",
        "GJBeast01_01_001.png",
        "pixelb_03_01_001.png",
        "pixelitem_001_001.png",
        "particle_01_001.png",
        "d_spikes_01_001.png",
        "sawblade_02_001.png",
        "edit_eTintCol01Btn_001.png"
    };

    return CCSprite::createWithSpriteFrameName(std::string(TabIcons[idx]).c_str());
}

Result<int> ETEditorUI::indexForBuildTabID(std::string_view id) {
    for (int i = 0; i < TabIDs.size(); i++) {
        if (TabIDs[i] == id) {
            return Ok(i);
        }
    }
    return Err("Tab with ID doesn't exist");
}

Result<std::string_view> ETEditorUI::idForBuildTabIndex(unsigned int index) {
    if (index >= TabIDs.size()) return Err("Index too high");
    return Ok(TabIDs[index]);
}

void ETEditorUI::setupBuildMode() {
    auto mode = ModeHandler::get()->createMode(alpha::editor_tabs::Build);
    auto internalMode = static_cast<::internal::Mode*>(mode);

    auto newBars = CCArray::create();

    int prio = -1000 * m_createButtonBars->count();

    for (auto bar : m_createButtonBars->asExt<EditButtonBar>()) {
        bar->removeFromParent();

        auto tabID = std::string(idForBuildTabIndex(bar->m_tabIndex).unwrapOrDefault());

        for (auto item : bar->m_buttonArray->asExt<CCNode>()) {
            item->removeFromParent();
        }

        auto newBar = alpha::editor_tabs::EditorTab::create(bar->m_buttonArray, bar->m_tabIndex, bar->m_hasCreateItems);
        internalMode->createTab(tabID, newBar, iconForIdx(bar->m_tabIndex), prio);
        
        newBars->addObject(newBar);

        prio += 1000;
    }

    m_createButtonBars->removeAllObjects();
    m_createButtonBars->addObjectsFromArray(newBars);

    m_createButtonBar = internalMode->getTab("block")->getNode();
}

void ETEditorUI::setupEditMode() {
    auto mode = ModeHandler::get()->createMode(alpha::editor_tabs::Edit);
    auto internalMode = static_cast<::internal::Mode*>(mode);

    Ref<EditButtonBar> bar = m_editButtonBar;
    bar->removeFromParent();

    for (auto item : bar->m_buttonArray->asExt<CCNode>()) {
        item->removeFromParent();
    }

    m_editButtonBar = alpha::editor_tabs::EditorTab::create(bar->m_buttonArray, bar->m_tabIndex, bar->m_hasCreateItems);
    internalMode->createTab("edit", m_editButtonBar, CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png"), -1000);
}

void ETEditorUI::setupDeleteMode() {
    auto mode = ModeHandler::get()->createMode(alpha::editor_tabs::Delete);
    auto internalMode = static_cast<::internal::Mode*>(mode);

    auto deleteTab = alpha::editor_tabs::EditorTab::create();

    m_deleteMenu->ignoreAnchorPointForPosition(false);
    m_deleteMenu->setContentSize({0.f, 0.f});

    auto tab = internalMode->createTab("delete", deleteTab, CCSprite::create("delete-tab.png"_spr), -1000);

    auto centerWorld = deleteTab->convertToWorldSpace(deleteTab->getContentSize() / 2.f);
    m_deleteMenu->setPosition(centerWorld);

    addEventListener(tab::SwitchTabEvent(tab), [this] (bool show) {
        m_deleteMenu->setVisible(show);
    });

    addEventListener(tab::ResizeTabEvent(tab), [this, deleteTab] (const CCSize& size, float scale) {
        m_deleteMenu->setScale(scale);
        auto centerWorld = deleteTab->convertToWorldSpace(size / 2.f);
        m_deleteMenu->setPosition(centerWorld);
    });
}

void ETEditorUI::setupViewMode() {
    auto mode = ModeHandler::get()->createMode(alpha::editor_tabs::View);
    auto internalMode = static_cast<::internal::Mode*>(mode);

    auto viewTab = alpha::editor_tabs::EditorTab::create();

    internalMode->createTab("view", viewTab, CCSprite::create("view-tab.png"_spr), -1000);
}