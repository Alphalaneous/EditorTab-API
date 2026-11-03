#define GEODE_DEFINE_EVENT_EXPORTS
#include "../include/Optional.hpp"
#include "../include/Mode.hpp"
#include "../include/Tab.hpp"
#undef GEODE_DEFINE_EVENT_EXPORTS

#include <Geode/Geode.hpp>
#include "EarlyEditorUI.hpp"
#include "ModeHandler.hpp"
#include "TabToggler.hpp"
#include "EditorTab.hpp"

using namespace geode::prelude;

namespace alpha::editor_tabs {

struct Mode::Impl : public ZImpl<Mode::Impl> {
    std::string m_id;
    std::vector<std::shared_ptr<alpha::editor_tabs::Tab>> m_tabs;
    std::string m_currentTab;
};

void Mode::construct(geode::ZStringView ID) {
    m_impl = std::make_unique<Impl>();
    (**m_impl)->m_id = ID;
}

geode::ZStringView Mode::getID() {
    return (**m_impl)->m_id;
}

void Mode::show() {
    ModeHandler::get()->switchMode(this);
}

alpha::editor_tabs::Tab* Mode::createTab(geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon) {
    return createTab(ID, node, icon, 0);
}

alpha::editor_tabs::Tab* Mode::createTab(geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority) {
    auto tab = (**m_impl)->m_tabs.emplace_back(std::make_shared<Tab>(ID, node, icon, priority));
    static_cast<Tab*>(tab.get())->setMode(this);

    auto editor = EarlyEditorUI::get();

    node->setZOrder(10);
    node->setVisible(false);
    node->setID(fmt::format("{}-tab-bar", ID));

    auto toggle = static_cast<TabToggler*>(tab->getTabToggle());
    toggle->setID(fmt::format("{}-tab", ID));

    auto editorTab = typeinfo_cast<alpha::editor_tabs::EditorTab*>(node);
    if (editorTab) {
        editorTab->setTab(tab.get());
    }

    if (editor->initialized()) {
        if ((**m_impl)->m_tabs.size() == 1) {
            (**m_impl)->m_currentTab = ID;
        }

        editor->addChild(node);

        node->setContentSize(EditorTab::getMaxSize());
        tab->reloadItems();
        
        Tab::TabInitializedEvent(tab.get()).send();

        auto handler = ModeHandler::get();
        if (handler) {
            bool isActive = handler->getCurrentMode() == this;
            node->setVisible(isActive && (**m_impl)->m_currentTab == ID);
        }
    }

    return tab.get();
}

void Mode::removeTab(geode::ZStringView ID) {
    removeTab(getTab(ID));
}

void Mode::removeTab(alpha::editor_tabs::Tab* tab) {
    if (!tab) return;

    auto editor = EarlyEditorUI::get();

    if ((**m_impl)->m_id == alpha::editor_tabs::Build) {
        editor->m_createButtonBars->removeObject(tab->getNode());
    }

    tab->getNode()->removeFromParent();
    tab->getTabToggle()->removeFromParent();

    editor->m_tabsMenu->updateLayout();

    unsigned int prevIdx = 0;

    bool isCurrentTab = (**m_impl)->m_currentTab == tab->getID();

    if (isCurrentTab) {
        for (unsigned int i = 0; i < (**m_impl)->m_tabs.size(); i++) {
            if ((**m_impl)->m_tabs[i].get()->getID() == tab->getID()) {
                prevIdx = i;
                break;
            }
        }
    }

    std::erase_if((**m_impl)->m_tabs, [tab] (const std::shared_ptr<alpha::editor_tabs::Tab>& t) {
        return t.get() == tab;
    });

    if (isCurrentTab) {
        if ((**m_impl)->m_tabs.empty()) {
            switchTab("");
            return;
        }

        auto idx = std::clamp(prevIdx, 0u, static_cast<unsigned int>((**m_impl)->m_tabs.size()) - 1);
        switchTab((**m_impl)->m_tabs[idx].get());
    }
}

Tab* Mode::getTab(geode::ZStringView ID) {
    auto it = std::find_if((**m_impl)->m_tabs.begin(), (**m_impl)->m_tabs.end(), 
    [ID] (const std::shared_ptr<alpha::editor_tabs::Tab>& tab) {
        return tab && tab->getID() == ID;
    });

    return (it != (**m_impl)->m_tabs.end()) ? it->get() : nullptr;
}

Tab* Mode::getTabByNode(EditButtonBar* node) {
    auto it = std::find_if((**m_impl)->m_tabs.begin(), (**m_impl)->m_tabs.end(), 
    [node] (const std::shared_ptr<alpha::editor_tabs::Tab>& tab) {
        return tab && tab->getNode() == node;
    });

    return (it != (**m_impl)->m_tabs.end()) ? it->get() : nullptr;
}

Tab* Mode::getTabByIndex(unsigned int index) {
    if (index >= (**m_impl)->m_tabs.size()) return nullptr;
    return (**m_impl)->m_tabs[index].get();
}

Tab* Mode::getCurrentTab() {
    return getTab((**m_impl)->m_currentTab);
}

Result<unsigned int> Mode::getTabIndex(ZStringView ID) {
    return getTabIndex(getTab(ID));
}

Result<unsigned int> Mode::getTabIndex(Tab* tab) {
    unsigned int idx = 0;
    for (const auto& tabInner : (**m_impl)->m_tabs) {
        if (tabInner.get() == tab) {
            return Ok(idx);
        }
        idx++;
    }

    return Err("Index not found for tab");}

std::span<const std::shared_ptr<Tab>> Mode::getAllTabs() {
    return (**m_impl)->m_tabs;
}

void Mode::switchTab(ZStringView ID) {
    switchTab(getTab(ID));
}

void Mode::switchTab(unsigned int index) {
    switchTab(getTabByIndex(index));
}

void Mode::switchTab(alpha::editor_tabs::Tab* tab) {
    auto handler = ModeHandler::get();
    if (!handler) return;

    bool isActive = handler->getCurrentMode() == this;

    for (const auto& tab : (**m_impl)->m_tabs) {
        tab->getNode()->setVisible(false);
        static_cast<TabToggler*>(tab->getTabToggle())->toggleTab(false);
    }

    auto editor = EarlyEditorUI::get();
    auto oldTab = getCurrentTab();

    if (tab) {
        (**m_impl)->m_currentTab = tab->getID();

        if (isActive && oldTab != tab) {
            if (oldTab) {
                Tab::SwitchTabEvent(oldTab).send(false);
            }

            Tab::SwitchTabEvent(tab).send(true);
        }

        tab->getNode()->setVisible(true && isActive);
        static_cast<TabToggler*>(tab->getTabToggle())->toggleTab(true);

        if ((**m_impl)->m_id == alpha::editor_tabs::Build) {
            editor->m_createButtonBar = tab->getNode();
        }

        if (isActive) {
            editor->m_selectedTab = getTabIndex(tab).unwrapOrDefault();
        }
    }
    else {
        (**m_impl)->m_currentTab = "";
        if (isActive) {
            editor->m_selectedTab = 0;
        }
    }
}

void Mode::hideMode() {
    auto editor = EarlyEditorUI::get();

    for (const auto& tab : (**m_impl)->m_tabs) {
        tab->getNode()->setVisible(false);

        auto toggle = static_cast<TabToggler*>(tab->getTabToggle());
        toggle->toggleTab(false);
        toggle->removeFromParent();
        editor->m_tabsArray->removeObject(toggle);
    }

    editor->m_tabsMenu->updateLayout();
}

void Mode::showMode() {
    auto editor = EarlyEditorUI::get();

    for (const auto& tab : (**m_impl)->m_tabs) {
        tab->getNode()->setVisible(false);

        auto toggle = static_cast<TabToggler*>(tab->getTabToggle());
        toggle->toggleTab(false);
        toggle->removeFromParent();
        editor->m_tabsMenu->addChild(toggle);
        editor->m_tabsArray->addObject(toggle);
    }

    auto tab = getCurrentTab();

    if (tab) {
        tab->getNode()->setVisible(true);
        static_cast<TabToggler*>(tab->getTabToggle())->toggleTab(true);
    }

    editor->m_selectedTab = getTabIndex(tab).unwrapOrDefault();
    editor->m_tabsMenu->updateLayout();
    editor->m_tabsMenu->setVisible((**m_impl)->m_tabs.size() > 1);
}

void Mode::updateToggles() {
    for (const auto& tab : (**m_impl)->m_tabs) {
        static_cast<TabToggler*>(tab->getTabToggle())->toggleTab(false);
    }

    auto tab = getCurrentTab();
    if (tab) {
        static_cast<TabToggler*>(tab->getTabToggle())->toggleTab(true);
    }
}

void Mode::setupTabs() {
    auto editor = EarlyEditorUI::get();

    for (const auto& tab : (**m_impl)->m_tabs) {
        auto node = tab->getNode();
        node->setContentSize(EditorTab::getFullMaxSize());

        editor->addChild(node);

        tab->reloadItems();
        Tab::TabInitializedEvent(tab.get()).send();
    }
}

void Mode::removeAllTabs() {
    auto editor = EarlyEditorUI::get();
    
    for (const auto& tab : (**m_impl)->m_tabs) {
        tab->getNode()->removeFromParent();
        tab->getTabToggle()->removeFromParent();
    }

    (**m_impl)->m_tabs.clear();
}

void Mode::reloadAllTabs() {
    for (const auto& tab : (**m_impl)->m_tabs) {
        tab->reloadItems();
    }
}

void Mode::reloadAllTabSizes(int rows, int columns) {
    for (const auto& tab : (**m_impl)->m_tabs) {
        static_cast<EditorTab*>(tab->getNode())->updateItemSize(rows, columns);
    }
}

void Mode::removeSelf() {
    ModeHandler::get()->removeMode(this);
}

#define OnEditorTab(method, ...) \
return ([&]() -> decltype(auto) { \
    auto editorTab = typeinfo_cast<alpha::editor_tabs::EditorTab*>((**m_impl)->m_node.data()); \
    if (!editorTab) { \
        using Ret = decltype(editorTab->method(__VA_ARGS__)); \
        if constexpr (std::is_void_v<Ret>) return; \
        else return Ret{}; \
    } \
    return editorTab->method(__VA_ARGS__); \
}())

struct Tab::Impl : public ZImpl<Impl> {
    std::string m_id;
    Ref<EditButtonBar> m_node = nullptr;
    Ref<CCNode> m_icon;
    alpha::editor_tabs::Mode* m_mode;
    Ref<alpha::editor_tabs::TabToggler> m_toggler;

    int m_priority;
};

void Tab::construct(geode::ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority) {
    m_impl = std::make_unique<Impl>();
    (**m_impl)->m_id = ID;
    (**m_impl)->m_node = node;
    (**m_impl)->m_icon = icon;

    (**m_impl)->m_toggler = TabToggler::create(this, icon);
    (**m_impl)->m_toggler->setTag(node->m_tabIndex);
    (**m_impl)->m_toggler->setZOrder(priority);

    (**m_impl)->m_priority = priority;
}

geode::ZStringView Tab::getID() {
    return (**m_impl)->m_id;
}

void Tab::show() {
    (**m_impl)->m_mode->switchTab(this);
}

EditButtonBar* Tab::getNode() {
    return (**m_impl)->m_node;
}

CCNode* Tab::getIcon() {
    return (**m_impl)->m_icon;
}

void Tab::setPriority(int priority) {
    (**m_impl)->m_priority = priority;
    (**m_impl)->m_toggler->setZOrder(priority);

    auto editor = EarlyEditorUI::get();
    editor->m_tabsMenu->updateLayout();}

int Tab::getPriority() {
    return (**m_impl)->m_priority;
}

Result<unsigned int> Tab::getIndex() {
    return (**m_impl)->m_mode->getTabIndex(this);
}

alpha::editor_tabs::Mode* Tab::getMode() {
    return (**m_impl)->m_mode;
}

CCMenuItemToggler* Tab::getTabToggle() {
    return (**m_impl)->m_toggler;
}

void Tab::removeSelf() {
    (**m_impl)->m_mode->removeTab(this);
}

void Tab::overrideSize(bool override, int rows, int columns) {
    OnEditorTab(overrideSize, override, rows, columns);
}

void Tab::overrideSize(int rows, int columns) {
    OnEditorTab(overrideSize, rows, columns);
}

int Tab::getRows() {
    OnEditorTab(getRows);
}

int Tab::getColumns() {    
    OnEditorTab(getColumns);
}

void Tab::reloadItems() {
    OnEditorTab(reloadItems);
}

void Tab::setItemless(bool itemless) {
    OnEditorTab(setItemless, itemless);
}

bool Tab::isItemless() {
    OnEditorTab(isItemless);
}

void Tab::setAutoScale(bool enabled) {
    OnEditorTab(setAutoScale, enabled);
}

bool Tab::hasAutoScale() {
    OnEditorTab(hasAutoScale);
}

void Tab::addItem(cocos2d::CCNode* item) {
    OnEditorTab(addItem, item);
}

void Tab::addItems(std::span<cocos2d::CCNode*> items) {
    OnEditorTab(addItems, items);
}

void Tab::addItems(std::span<geode::Ref<cocos2d::CCNode>> items) {
    OnEditorTab(addItems, items);
}

void Tab::removeItem(cocos2d::CCNode* item) {
    OnEditorTab(removeItem, item);
}

void Tab::removeItems(std::span<cocos2d::CCNode*> items) {
    OnEditorTab(removeItems, items);
}

void Tab::removeItems(std::span<geode::Ref<cocos2d::CCNode>> items) {
    OnEditorTab(removeItems, items);
}

void Tab::removeItemByID(geode::ZStringView ID) {
    OnEditorTab(removeItemByID, ID);
}

void Tab::removeItemByTag(int tag) {
    OnEditorTab(removeItemByTag, tag);
}

void Tab::removeItemByObjectID(int objectID) {
    OnEditorTab(removeItemByObjectID, objectID);
}

void Tab::insertItem(cocos2d::CCNode* item, unsigned int index) {
    OnEditorTab(insertItem, item, index);
}

void Tab::insertItems(std::span<cocos2d::CCNode*> items, unsigned int index) {
    OnEditorTab(insertItems, items, index);
}

void Tab::insertItems(std::span<geode::Ref<cocos2d::CCNode>> items, unsigned int index) {
    OnEditorTab(insertItems, items, index);
}

void Tab::insertItemAfter(cocos2d::CCNode* item, cocos2d::CCNode* after) {
    OnEditorTab(insertItemAfter, item, after);
}

void Tab::insertItemsAfter(std::span<cocos2d::CCNode*> items, cocos2d::CCNode* after) {
    OnEditorTab(insertItemsAfter, items, after);
}

void Tab::insertItemsAfter(std::span<geode::Ref<cocos2d::CCNode>> items, cocos2d::CCNode* after) {
    OnEditorTab(insertItemsAfter, items, after);
}

void Tab::insertItemBefore(cocos2d::CCNode* item, cocos2d::CCNode* before) {
    OnEditorTab(insertItemBefore, item, before);
}

void Tab::insertItemsBefore(std::span<cocos2d::CCNode*> items, cocos2d::CCNode* before) {
    OnEditorTab(insertItemsBefore, items, before);
}

void Tab::insertItemsBefore(std::span<geode::Ref<cocos2d::CCNode>> items, cocos2d::CCNode* before) {
    OnEditorTab(insertItemsBefore, items, before);
}

void Tab::insertItemAfter(CreateMenuItem* item, int objectID) {
    OnEditorTab(insertItemAfter, item, objectID);
}

void Tab::insertItemsAfter(std::span<CreateMenuItem*> items, int objectID) {
    OnEditorTab(insertItemsAfter, items, objectID);
}

void Tab::insertItemsAfter(std::span<geode::Ref<CreateMenuItem>> items, int objectID) {
    OnEditorTab(insertItemsAfter, items, objectID);
}

void Tab::insertItemBefore(CreateMenuItem* item, int objectID) {
    OnEditorTab(insertItemBefore, item, objectID);
}

void Tab::insertItemsBefore(std::span<CreateMenuItem*> items, int objectID) {
    OnEditorTab(insertItemsBefore, items, objectID);
}

void Tab::insertItemsBefore(std::span<geode::Ref<CreateMenuItem>> items, int objectID) {
    OnEditorTab(insertItemsBefore, items, objectID);
}

cocos2d::CCNode* Tab::findItemByID(geode::ZStringView ID) {
    OnEditorTab(findItemByID, ID);
}

cocos2d::CCNode* Tab::findItemByTag(int tag) {
    OnEditorTab(findItemByTag, tag);
}

CreateMenuItem* Tab::findItemByObjectID(int objectID) {
    OnEditorTab(findItemByObjectID, objectID);
}

void Tab::setTabAlignment(alpha::editor_tabs::TabAlignment alignment) {
    OnEditorTab(setTabAlignment, alignment);
}

alpha::editor_tabs::TabAlignment Tab::getTabAlignment() {
    OnEditorTab(getTabAlignment);
}

void Tab::disableSizeOverrideIfExceeded(bool disable) {
    OnEditorTab(disableSizeOverrideIfExceeded, disable);
}

bool Tab::isDisableSizeOverrideIfExceeded() {
    OnEditorTab(isDisableSizeOverrideIfExceeded);
}

void Tab::setHorizontalButtonAlignment(geode::AxisAlignment alignment) {
    OnEditorTab(setHorizontalButtonAlignment, alignment);
}

geode::AxisAlignment Tab::getHorizontalButtonAlignment() {
    OnEditorTab(getHorizontalButtonAlignment);
}

void Tab::setVerticalButtonAlignment(geode::AxisAlignment alignment) {
    OnEditorTab(setVerticalButtonAlignment, alignment);
}

geode::AxisAlignment Tab::getVerticalButtonAlignment() {
    OnEditorTab(getVerticalButtonAlignment);
}

EditButtonBar* createTabBar(cocos2d::CCArray* items, int tab, bool hasCreateItems) {
    return EditorTab::create(items, tab, hasCreateItems);
}

EditButtonBar* createTabBar(std::span<Ref<CCNode>> items, int tab, bool hasCreateItems) {
    return EditorTab::create(items, tab, hasCreateItems);
}

EditButtonBar* createTabBar(std::span<CCNode*> items, int tab, bool hasCreateItems) {
    return EditorTab::create(items, tab, hasCreateItems);
}

EditButtonBar* createTabNode() {
    return EditorTab::create();
}

cocos2d::CCSize getMaxTabSize() {
    return EditorTab::getMaxSize();
}

Mode* getMode(geode::ZStringView ID) {
    OnHandler(getMode, ID);
}

Mode* getCurrentMode() {
    OnHandler(getCurrentMode);
}

ZStringView getCurrentModeID() {
    OnHandler(getCurrentModeID);
}

Tab* getCurrentTab() {
    OnHandler(getCurrentTab);
}

ZStringView getCurrentTabID() {
    OnHandler(getCurrentTabID);
}

Mode* createMode(geode::ZStringView ID) {
    OnHandler(createMode, ID);
}

Tab* getTab(geode::ZStringView ID) {
    OnHandler(getTab, ID);
}

Tab* getTabByNode(EditButtonBar* node) {
    OnHandler(getTabByNode, node);
}

void removeMode(geode::ZStringView ID) {
    OnHandler(removeMode, ID);
}

void removeMode(Mode* mode) {
    OnHandler(removeMode, mode);
}

std::span<const std::shared_ptr<Mode>> getAllModes() {
    OnHandler(getAllModes);
}

std::vector<std::shared_ptr<Tab>> getAllTabs() {
    OnHandler(getAllTabs);
}

void reloadAllModes() {
    OnHandler(reloadAllModes);
}

void switchMode(ZStringView ID) {
    OnHandler(switchMode, ID);
}

void switchMode(alpha::editor_tabs::Mode* mode) {
    OnHandler(switchMode, mode);
}

}