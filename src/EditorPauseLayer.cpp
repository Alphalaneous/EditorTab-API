#include "EditorPauseLayer.hpp"
#include "EditorUI.hpp"

void MyEditorPauseLayer::onResume(CCObject* sender) {
    auto editorUI = static_cast<ETEditorUI*>(m_editorLayer->m_editorUI);
    bool reload = editorUI->m_reloadItems;
    editorUI->m_reloadItems = false;

    EditorPauseLayer::onResume(sender);

    if (reload) editorUI->reloadEditTabs();
}
