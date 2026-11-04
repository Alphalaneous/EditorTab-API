#include "EditorPauseLayer.hpp"
#include "EditorUI.hpp"

void MyEditorPauseLayer::onResume(CCObject* sender) {
    auto editorUI = m_editorLayer->m_editorUI;

    bool reload = editorUI->m_reloadItems;
    editorUI->m_reloadItems = false;

    if (reload) {
        if (editorUI->m_editButtonBar) {
            editorUI->m_editButtonBar->removeFromParent();
            editorUI->m_editButtonBar = nullptr;
        }
        if (editorUI->m_dpadMoveNode) {
            editorUI->m_dpadMoveNode->removeFromParent();
            editorUI->m_dpadMoveNode = nullptr;
        }
    }

    EditorPauseLayer::onResume(sender);

    if (reload) {
        editorUI->createMoveMenu();
        auto rows = GameManager::get()->getIntGameVariable(GameVar::EditorButtonRows);
        auto cols = GameManager::get()->getIntGameVariable(GameVar::EditorButtonsPerRow);
        ETEditorUI::get()->reloadAllModeSizes(rows, cols);
        editorUI->updateButtons();
    }
}
