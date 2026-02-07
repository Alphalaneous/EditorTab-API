# EditorTab API

An API mod for adding editor tabs easily. 

To use, hook EditorUI::init and use alpha::editor_tabs::addTab() after calling original to add a tab. You must pass in an ID, the edit mode, a create node callback, and a create icon callback. There are two optional callbacks, one fot whent the tab is entered and exited, and one for when the tab is reloaded (row and column count changes). 

When adding a tab, it must be in a specific mode, the three vanilla ones are BUILD, EDIT, and DELETE, but you can assign your own too, though you are responsible for handling any buttons to switch to said mode.

You can use any node as the return in the create node callback. This API also provides an easy way to create an EditButtonBar (`alpha::editor_tabs::createEditButtonBar`) which will be initialized with the correct size and positioning.

<cr>Tab IDs should be unique</c>

```cpp
class $modify(MyEditorUI, EditorUI) {

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        alpha::editor_tabs::addTab("rawr-tab"_spr, alpha::editor_tabs::BUILD, 
        [] { // Create the tab
            std::vector<Ref<CCNode>> nodes;
            auto label = CCLabelBMFont::create("Rawr", "bigFont.fnt");
            nodes.push_back(label);

            return alpha::editor_tabs::createEditButtonBar(nodes);
        }, 
        [] { // create the tab icon
            return CCSprite::createWithSpriteFrameName("spike_01_001.png");
        }, 
        [] (bool state, auto tab) { // do something when the tab is entered and exited
            log::info("rawr-tab state: {}", state);
        }, 
        [] (int rows, int cols, auto tab) { // do something when the tab is reloaded
            log::info("rawr tab rows: {}, cols: {}", rows, cols);
        });

        return true;
    }
};

```

This API also provides ways to get information on what tab or mode you are on, as well as getting stuff from a tab ID itself.

```cpp
// current mode (BUILD, EDIT, DELETE, or a custom string)
geode::Result<geode::ZStringView> getCurrentMode()

// current tab as the ID
geode::Result<geode::ZStringView> getCurrentTab()

// get a tab's index by ID
geode::Result<int> getTabIndex(geode::ZStringView tabID)

// get a tab's mode by ID
geode::Result<geode::ZStringView> getTabMode(geode::ZStringView tabID)

// get a tab's node by ID
geode::Result<geode::Ref<cocos2d::CCNode>> nodeForTab(geode::ZStringView tabID)

// get a tab's toggler by ID
geode::Result<geode::Ref<CCMenuItemToggler>> togglerForTab(geode::ZStringView tabID)
```

You can also remove a tab if needed with just the tab ID.

```cpp
void removeTab(geode::ZStringView tabID)
```