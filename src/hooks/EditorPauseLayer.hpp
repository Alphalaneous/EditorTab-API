#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyEditorPauseLayer, EditorPauseLayer) {

    void onResume(CCObject* sender);

};