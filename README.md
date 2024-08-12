# EditorTab API

An API mod for adding editor tabs easily. 

To use, inside $execute, you want to register an Editor tab, You must pass in an ID. There is one required lambda, which is for creating the EditButtonBar, and an optional one for when you leave that tab. 

`EditorTabUtils::setTabIcons(toggler, onNode, offNode)` is for settings the tab icons so you don't have to do it manually to the toggler passed in

`EditorTabUtils::createEditButtonBar(nodeArray, editorUI)` is for creating a button bar with vanilla values, you can always create your own EditButtonBar with custom values if needed.

Example:

```c++
$execute {
    EditorTabs::get()->registerTab("rawr"_spr, [](EditorUI* ui, CCMenuItemToggler* toggler) -> EditButtonBar* { //create

        auto arr = CCArray::create();
        auto label = CCLabelBMFont::create("meow", "bigFont.fnt");
        arr->addObject(label);

        CCLabelBMFont* textLabelOn = CCLabelBMFont::create("uwu", "bigFont.fnt");
        textLabelOn->setScale(0.4f);
        CCLabelBMFont* textLabelOff = CCLabelBMFont::create("owo", "bigFont.fnt");
        textLabelOff->setScale(0.4f);

        EditorTabUtils::setTabIcons(toggler, textLabelOn, textLabelOff);

        return EditorTabUtils::createEditButtonBar(arr, ui);
        
    }, [](EditorUI*, EditButtonBar*) { //left the tab

        log::info(":3");

    });
}
```