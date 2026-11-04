#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace alpha::editor_tabs {

class DummyBoom : public BoomScrollLayer {
public: 
    static DummyBoom* create();
protected:
    bool init();

    Ref<ExtendedLayer> m_extendedLayerRef = nullptr;
    Ref<CCArray> m_pagesRef = nullptr;
};

};