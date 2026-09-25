#include "EditButtonBar.hpp"
#include "EditorTab.hpp"

void ETEditButtonBar::loadFromItems(CCArray* items, int columns, int rows, bool preserve) {
    auto editorTab = typeinfo_cast<alpha::editor_tabs::EditorTab*>(this);
    if (!editorTab) {
        EditButtonBar::loadFromItems(items, columns, rows, preserve);
        return;
    }

    editorTab->loadFromItems_(items, columns, rows, preserve);
}

void ETEditButtonBar::goToPage(int page) {
    auto editorTab = typeinfo_cast<alpha::editor_tabs::EditorTab*>(this);
    if (!editorTab) {
        EditButtonBar::goToPage(page);
        return;
    }
    editorTab->goToPage_(page);
}

void ETEditButtonBar::onLeft(CCObject* sender) {
    auto editorTab = typeinfo_cast<alpha::editor_tabs::EditorTab*>(this);
    if (!editorTab) {
        EditButtonBar::onLeft(sender);
        return;
    }
    editorTab->onLeft_(sender);
}

void ETEditButtonBar::onRight(CCObject* sender) {
    auto editorTab = typeinfo_cast<alpha::editor_tabs::EditorTab*>(this);
    if (!editorTab) {
        EditButtonBar::onRight(sender);
        return;
    }
    editorTab->onRight_(sender);
}