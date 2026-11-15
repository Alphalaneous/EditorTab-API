#define GEODE_DEFINE_EVENT_EXPORTS
#include "../../include/api/Editor.hpp"
#include "../../include/api/Mode.hpp"
#include "../../include/api/Tab.hpp"
#include "../../include/ui/EditorTab.hpp"
#include "../../include/ui/TabToggler.hpp"

#include <Geode/Geode.hpp>
#include "../hooks/EditorUI.hpp"

using namespace geode::prelude;

namespace alpha::editor_tabs {

Mode* getMode(geode::ZStringView ID) {
    return ETEditorUI::get()->getMode(ID);
}

Mode* getCurrentMode() {
    return ETEditorUI::get()->getCurrentMode();
}

ZStringView getCurrentModeID() {
    return ETEditorUI::get()->getCurrentModeID();
}

Tab* getCurrentTab() {
    return ETEditorUI::get()->getCurrentTab();
}

ZStringView getCurrentTabID() {
    return ETEditorUI::get()->getCurrentTabID();
}

Mode* createMode(geode::ZStringView ID) {
    return ETEditorUI::get()->createMode(ID);
}

Tab* getTab(geode::ZStringView ID) {
    return ETEditorUI::get()->getTab(ID);
}

Tab* getTabByNode(alpha::editor_tabs::EditorTab* node) {
    return ETEditorUI::get()->getTabByNode(node);
}

void removeMode(geode::ZStringView ID) {
    ETEditorUI::get()->removeMode(ID);
}

void removeMode(Mode* mode) {
    ETEditorUI::get()->removeMode(mode);
}

std::span<const std::shared_ptr<Mode>> getAllModes() {
    return ETEditorUI::get()->getAllModes();
}

std::vector<std::shared_ptr<Tab>> getAllTabs() {
    return ETEditorUI::get()->getAllTabs();
}

void reloadAllModes() {
    ETEditorUI::get()->reloadAllModes();
}

void switchMode(ZStringView ID) {
    ETEditorUI::get()->switchMode(ID);
}

void switchMode(alpha::editor_tabs::Mode* mode) {
    ETEditorUI::get()->switchMode(mode);
}

CCMenuItemSpriteExtra* createEditButton(cocos2d::CCSprite* sprite, int bgType, geode::Function<void(CCMenuItemSpriteExtra* sender)> callback, cocos2d::CCPoint offset) {
    std::string background = fmt::format("GJ_button_0{}.png", bgType);

    CCSize size = sprite->getContentSize();
    float fit = std::min(std::min(32.f / size.width, 32.f / size.height), 1.f);

    auto buttonSprite = ButtonSprite::create(sprite, 40, 0, 40.f, fit, true, background.c_str(), false, 40.f);

    auto button = CCMenuItemExt::createSpriteExtra(buttonSprite, std::move(callback));
    button->setScale(0.9f);
    button->m_baseScale = 0.9f;

    buttonSprite->m_spritePosition = offset;
    buttonSprite->updateSpriteBGSize();

    return button;
}

}