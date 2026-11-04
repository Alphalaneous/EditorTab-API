#include "DummyBoom.hpp"

namespace alpha::editor_tabs {

DummyBoom* DummyBoom::create() {
    auto ret = new DummyBoom();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool DummyBoom::init() {
    if (!CCLayer::init()) return false;

    m_extendedLayerRef = ExtendedLayer::create();
    m_extendedLayer = m_extendedLayerRef;

    m_pagesRef = CCArray::create();
    m_pages = m_pagesRef;
    m_pages->addObject(CCNode::create());

    return true;
}

}