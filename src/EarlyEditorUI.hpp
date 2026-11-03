
#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(EarlyEditorUI, EditorUI) {
    struct Fields {
        bool m_initialized = false;
        ~Fields() {
            s_instance = nullptr;
        }
    };

    bool init(LevelEditorLayer* editorLayer) {
        s_instance = this;
        auto fields = m_fields.self();
        return EditorUI::init(editorLayer);
    }

    bool initialized() {
        return m_fields->m_initialized;
    }

    void setInitialized() {
        m_fields->m_initialized = true;
    }

    static EarlyEditorUI* s_instance;
    static EarlyEditorUI* get() {
        return s_instance;
    }
};

EarlyEditorUI* EarlyEditorUI::s_instance = nullptr;