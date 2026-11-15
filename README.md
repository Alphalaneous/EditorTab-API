# Editor Tab API

An API mod for creating and managing editor tabs easily. 

*For Developers*

Check the headers for documentation

Include `API.hpp` for all headers to be included.

When adding modes, you have to provide a way to access them, such as your own button.
Please give each tab a unique ID for best results

## Example

```cpp
#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <alphalaneous.editortab_api/API.hpp>

using namespace geode::prelude;

class $modify(MyEditorUI, EditorUI) {

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        auto editMode = alpha::editor_tabs::getMode(alpha::editor_tabs::Edit);
        auto viewMode = alpha::editor_tabs::getMode(alpha::editor_tabs::View);

        auto btn = alpha::editor_tabs::createEditButton(
            CCSprite::createWithSpriteFrameName("hideBtn_001.png"),
            1, // Button Type ("GJ_button_0{}.png")
            [this] (CCMenuItemSpriteExtra* sender) {
                onShowUIBtn(sender);
            }
        );

        auto editTab = editMode->getTab("edit");
        editTab->getNode()->addItem(btn);

        auto meowTabNode = alpha::editor_tabs::EditorTab::create();
        meowTabNode->addChild(geode::Label::create("Meow", "bigFont.fnt"));

        auto meowTab = viewMode->createTab("meow-tab"_spr, meowTabNode, geode::Label::create("M", "bigFont.fnt"));

        meowTabNode->addEventListener(alpha::editor_tabs::Tab::SwitchTabEvent(meowTab), [] (bool show) {
            log::info("showing tab: {}", show);
        });

        return true;
    }
};


```