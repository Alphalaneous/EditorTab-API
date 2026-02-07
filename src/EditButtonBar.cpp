#include "EditButtonBar.hpp"
#include "EditorUI.hpp"
#include <Geode/binding/EditButtonBar.hpp>

void ETEditButtonBar::loadFromItems(CCArray* items, int c, int r, bool preserve) {
    EditButtonBar::loadFromItems(items, c, r, preserve);
    auto editorUI = ETEditorUI::get();

    setContentSize({editorUI->getContentWidth() - 180, editorUI->m_toolbarHeight});
    setAnchorPoint({0.5f, 0.f});
    m_scrollLayer->setContentSize(getContentSize());
    m_scrollLayer->ignoreAnchorPointForPosition(false);
    m_scrollLayer->setPosition(getContentSize() / 2);
    m_scrollLayer->unscheduleUpdate();
    m_scrollLayer->unschedule(schedule_selector(BoomScrollLayer::updateDots));

    for (auto page : m_scrollLayer->m_pages->asExt<ButtonPage>()) {
        page->setContentSize(getContentSize());
        page->ignoreAnchorPointForPosition(false);
        page->setPosition(getContentSize() / 2);
        page->setAnchorPoint({0.5f, 0.5f});

        auto menu = page->getChildByType<CCMenu*>(0);
        if (!menu) continue;

        for (auto item : menu->getChildrenExt<CCMenuItemSpriteExtra>()) {
            item->setScale(1.f);
            item->m_baseScale = 1.f;
        }

        menu->setAnchorPoint({0.5f, 0.5f});
        menu->setPosition(getContentSize() / 2);

        auto layout = RowLayout::create();
        layout->setGrowCrossAxis(true);
        layout->setCrossAxisOverflow(false);
        layout->setAutoScale(false);
        layout->setAxisAlignment(AxisAlignment::Start);
        layout->setCrossAxisAlignment(AxisAlignment::End);

        float gap = 5.f;

        float width = c * (40.f + gap) - gap; 
        float height = r * (40.f + gap) - gap; 

        menu->setContentSize({width, height});

        float scaleX = (getContentWidth() - 40) / menu->getContentWidth();
        float scaleY = (getContentHeight() - 14) / menu->getContentHeight();
        float scale = std::min(scaleX, scaleY);

        menu->setScale(scale);

        menu->setLayout(layout);
        menu->updateLayout();
    }

    auto fields = m_fields.self();
    fields->m_dots = m_scrollLayer->getChildByType<CCSpriteBatchNode>(0);
    fields->m_dots->setAnchorPoint({0.5f, 0.f});
    fields->m_dots->setContentSize({getContentWidth(), 5.f});
    fields->m_dots->setPositionX(getContentWidth() / 2);

    runAction(CallFuncExt::create([this, editorUI, fields] {
        auto spacerLeft = editorUI->getChildByID("spacer-line-left");
        auto spacerRight = editorUI->getChildByID("spacer-line-right");

        if (spacerLeft && spacerRight) {
            float x = (spacerLeft->getPositionX() + spacerRight->getPositionX()) / 2;
            setPosition({x, 0});
        }
        else {
            setPosition({getContentWidth() / 2, 0});
        }
    }));

    showPage();
}

void ETEditButtonBar::updatePage() {
    auto fields = m_fields.self();
    if (!fields->m_dots) return;

    auto arr = fields->m_dots->getChildrenExt<CCSprite>();

    float gap = 12.f;
    float overallWidth = fields->m_dots->getContentWidth() - 15.f;

    float width = (gap + 3.2f) * fields->m_dots->getChildrenCount() - gap;
    float start = fields->m_dots->getContentWidth() / 2 - width / 2;

    float factor = 1.f;

    if (width > overallWidth) {
        factor = overallWidth / width;
        width = overallWidth;
        start = fields->m_dots->getContentWidth() / 2 - width / 2;
    }

    int idx = 0;
    for (auto dot : arr) {
        dot->setColor({125, 125, 125});
        float x = dot->getScaledContentWidth()/2 + start + ((dot->getScaledContentWidth() + gap) * factor) * idx;
        dot->setPosition({x, fields->m_dots->getContentHeight() / 2});
        idx++;
    }

    arr[getPage()]->setColor({255, 255, 255});
}

void ETEditButtonBar::goToPage(int page) {
    EditButtonBar::goToPage(page);
    showPage();
}

void ETEditButtonBar::showPage() {
    m_scrollLayer->m_extendedLayer->setContentSize(getContentSize());
    m_scrollLayer->m_extendedLayer->ignoreAnchorPointForPosition(false);
    m_scrollLayer->m_extendedLayer->setPosition(getContentSize() / 2);

    auto menu = getChildByType<CCMenu>(0);
    if (menu) {
        menu->setContentSize(getContentSize());
        menu->setAnchorPoint({0.5f, 0.5f});
        menu->ignoreAnchorPointForPosition(false);
        menu->setPosition(getContentSize() / 2);

        auto leftBtn = menu->getChildByType<CCMenuItemSpriteExtra*>(0);
        auto rightBtn = menu->getChildByType<CCMenuItemSpriteExtra*>(1);

        leftBtn->setPosition({leftBtn->getContentWidth() / 2 + 5, getContentHeight() / 2});
        rightBtn->setPosition({getContentWidth() - leftBtn->getContentWidth() / 2 - 5, getContentHeight() / 2});
    }

    for (auto page : m_scrollLayer->m_pages->asExt<ButtonPage>()) {
        page->removeFromParent();
        page->setContentSize(getContentSize());
        page->ignoreAnchorPointForPosition(false);
        page->setPosition(getContentSize() / 2);
        page->setAnchorPoint({0.5f, 0.5f});
    }

    m_scrollLayer->m_extendedLayer->addChild(m_scrollLayer->m_pages->asExt<ButtonPage>()[getPage()]);
    updatePage();
}

void ETEditButtonBar::onLeft(CCObject* sender) {
    EditButtonBar::onLeft(sender);
    showPage();
}

void ETEditButtonBar::onRight(CCObject* sender) {
    EditButtonBar::onRight(sender);
    showPage();
}

void ETEditButtonBar::optimizedSetVisible(bool visible) {
    setVisible(visible);
    if (!visible) {
        for (auto page : m_scrollLayer->m_pages->asExt<ButtonPage>()) {
            page->removeFromParent();
        }
    }
    else {
        showPage();
    }
}

void ETBoomScrollLayer::updateDots(float dt) {
    if (typeinfo_cast<EditButtonBar*>(getParent())) return;
    BoomScrollLayer::updateDots(dt);
}