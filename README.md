# EditorTab API

An API mod for adding editor tabs easily. 

To use, preferably hook EditorUI::init and use EditorTabs::get()->addTab() to add a tab. You must pass in an ID. There is one required lambda, which is for creating the CCNode, and an optional one for when you toggle that tab, state being true if visible, false if not. 

`EditorTabUtils::setTabIcons(toggler, onNode, offNode)` is for settings the tab icons so you don't have to do it manually to the toggler passed in

`EditorTabUtils::createEditButtonBar(nodeArray, editorUI)` is for creating a button bar with vanilla values, you can always create your own EditButtonBar with custom values if needed.

You must define a tab type to specifiy if you want it in the BUILD, EDIT, or DELETE tab

Example:

```c++
class $modify(EditorUI) {

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        EditorTabs::get()->addTab(this /*The EditorUI*/, TabType::EDIT, "rawr"_spr, [](EditorUI* ui, CCMenuItemToggler* toggler) -> CCNode* { //create the tab

            auto arr = CCArray::create();
            auto label = CCLabelBMFont::create("meow", "bigFont.fnt");
            arr->addObject(label);

            CCLabelBMFont* textLabelOn = CCLabelBMFont::create("uwu", "bigFont.fnt");
            textLabelOn->setScale(0.4f);
            CCLabelBMFont* textLabelOff = CCLabelBMFont::create("owo", "bigFont.fnt");
            textLabelOff->setScale(0.4f);

            EditorTabUtils::setTabIcons(toggler, textLabelOn, textLabelOff);

            return EditorTabUtils::createEditButtonBar(arr, ui);
            
        }, [](EditorUI*, bool state, CCNode*) { //toggled the tab (activates on every tab click)
            log::info(":3");
        });

        return true;
    }
};
```

Alternatively you can use the callback macros rather than a lambda

```c++
class $modify(MyEditorUI, EditorUI) {

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        EditorTabs::get()->addTab(this, TabType::EDIT, "rawr"_spr, create_tab_callback(MyEditorUI::createMeowBar), toggle_tab_callback(MyEditorUI::toggleMeowBar));

        return true;
    }

    void toggleMeowBar(EditorUI* ui, bool state, cocos2d::CCNode* bar) {
        log::info(":3");
    }

	CCNode* createMeowBar(EditorUI* ui, CCMenuItemToggler* toggler) {
        auto arr = CCArray::create();
        auto label = CCLabelBMFont::create("meow", "bigFont.fnt");
        arr->addObject(label);

        CCLabelBMFont* textLabelOn = CCLabelBMFont::create("uwu", "bigFont.fnt");
        textLabelOn->setScale(0.4f);
        CCLabelBMFont* textLabelOff = CCLabelBMFont::create("owo", "bigFont.fnt");
        textLabelOff->setScale(0.4f);

        EditorTabUtils::setTabIcons(toggler, textLabelOn, textLabelOff);

        return EditorTabUtils::createEditButtonBar(arr, ui);
    }

};
```

Optionally, you can register it at the start of the game using EditorTabs::get()->registerTab(), this does not require you to pass in the EditorUI, but it cannot be as easily changed during runtime.

Example:

```c++
$execute {
    EditorTabs::get()->registerTab(TabType::BUILD, "rawr"_spr, [](EditorUI* ui, CCMenuItemToggler* toggler) -> CCNode* { //create

        auto arr = CCArray::create();
        auto label = CCLabelBMFont::create("meow", "bigFont.fnt");
        arr->addObject(label);

        CCLabelBMFont* textLabelOn = CCLabelBMFont::create("uwu", "bigFont.fnt");
        textLabelOn->setScale(0.4f);
        CCLabelBMFont* textLabelOff = CCLabelBMFont::create("owo", "bigFont.fnt");
        textLabelOff->setScale(0.4f);

        EditorTabUtils::setTabIcons(toggler, textLabelOn, textLabelOff);

        return EditorTabUtils::createEditButtonBar(arr, ui);
        
    }, [](EditorUI*, bool state, CCNode*) { //toggled the tab (activates on every tab click)

        log::info(":3");

    });
}
```