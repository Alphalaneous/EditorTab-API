#pragma once

#include "../include/Tab.hpp"
#include "../include/Mode.hpp"
#include <Geode/Geode.hpp>
#include "TabToggler.hpp"

using namespace geode::prelude;
using namespace alpha::editor_tabs;

namespace internal {

class Tab : public alpha::editor_tabs::Tab {
public:
    void construct(ZStringView ID, EditButtonBar* node, cocos2d::CCNode* icon, int priority = 0);
    void destruct();

    geode::ZStringView getID();

    void show();
    EditButtonBar* getNode();

    void overrideSize(bool override, int rows, int columns);
    void overrideSize(int rows, int columns);

    int getRows();
    int getColumns();

    void setPriority(int priority);
    int getPriority();

    alpha::editor_tabs::Mode* getMode();
    TabToggler* getTabToggle();

    void setMode(alpha::editor_tabs::Mode* mode);

    void reloadItems();

    void setItemless();

    void removeSelf();

    void setPageDotsClickable(bool clickable);
    bool arePageDotsClickable();

protected:
    class Impl;
    Impl* impl();
};

}