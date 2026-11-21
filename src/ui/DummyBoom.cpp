#include "DummyBoom.hpp"
#include <Geode/binding/BoomScrollLayer.hpp>

namespace alpha::editor_tabs {

DummyBoom::DummyBoom() : BoomScrollLayer(geode::ZeroConstructor) {
    m_uReference = 1;

    auto director = CCDirector::get();
    m_pActionManager = director->getActionManager();
    m_pActionManager->retain();
    m_pScheduler = director->getScheduler();
    m_pScheduler->retain();

    auto pEngine = CCScriptEngineManager::sharedManager()->getScriptEngine();
    m_eScriptType = pEngine != nullptr ? pEngine->getScriptType() : kScriptTypeNone;

    m_fScaleX = 1.f;
    m_fScaleY = 1.f;

    m_nTag = -1;
    m_sAdditionalTransform = CCAffineTransformMakeIdentity();
    m_bTransformDirty = true;
    m_bInverseDirty = true;
    m_bVisible = true;

    m_pComponentContainer = new CCComponentContainer(ZeroConstructor);
    m_pComponentContainer->m_pComponents = nullptr;
    m_pComponentContainer->m_pOwner = this;

    m_bIgnoreAnchorPointForPosition = true;
    setAnchorPoint({0.5f, 0.5f});
}

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