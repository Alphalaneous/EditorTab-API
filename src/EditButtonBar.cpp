#include "EditButtonBar.hpp"
#include "EditorUI.hpp"
#include <Geode/ui/Button.hpp>

EditButtonBar* ETEditButtonBar::create(cocos2d::CCArray* objects, cocos2d::CCPoint position, int tab, bool hasCreateItems, int columns, int rows) {
    auto ret = EditButtonBar::create(objects, position, tab, hasCreateItems, columns, rows);
    static_cast<ETEditButtonBar*>(ret)->setupChanges(columns, rows);
    return ret;
}

void ETEditButtonBar::setupChanges(int c, int r) {
    if (getUserFlag("disable-rewrite"_spr)) return;

    auto editorUI = ETEditorUI::get();

    if (!editorUI || !editorUI->m_fields->m_initialized) {
        return;
    }

    auto spacerLeft = editorUI->getChildByID("spacer-line-left");
    auto spacerRight = editorUI->getChildByID("spacer-line-right");

    auto widthOffset = 180;

    if (spacerLeft && spacerRight) {
        widthOffset = spacerLeft->getPositionX() + (editorUI->getContentWidth() - spacerRight->getPositionX());
    }

    auto barScale = editorUI->m_toolbarHeight / 92;

    setContentSize({editorUI->getContentWidth() - widthOffset, editorUI->m_toolbarHeight});
    setAnchorPoint({0.5f, 0.f});
    setScale(1);

    m_scrollLayer->setContentSize(getContentSize());
    m_scrollLayer->ignoreAnchorPointForPosition(false);
    m_scrollLayer->setPosition(getContentSize() / 2);
    m_scrollLayer->unscheduleUpdate();
    m_scrollLayer->unschedule(schedule_selector(BoomScrollLayer::updateDots));

    auto clickableDotsMenu = m_scrollLayer->getChildByID("zilko.clickable_dots/buttons-menu");
    if (clickableDotsMenu) {
        clickableDotsMenu->setVisible(false);
    }
    
    auto fields = m_fields.self();

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

        menu->setAnchorPoint({0.5f, 1.f});
        menu->setPosition({getContentWidth() / 2, getContentHeight() - 7 * barScale});

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

        float scaleX = (getContentWidth() - 60 * barScale) / menu->getContentWidth();
        float scaleY = (getContentHeight() - 14 * barScale) / menu->getContentHeight();
        float scale = std::min(scaleX, scaleY);

        menu->setScale(scale);
        fields->m_menuHeight = menu->getScaledContentHeight();

        menu->setLayout(layout);
        menu->updateLayout();
    }

    auto batch = m_scrollLayer->getChildByType<CCSpriteBatchNode>(0);
    if (batch) batch->setVisible(false);

    auto count = m_pagesArray ? m_pagesArray->count() : 1;

    fields->m_dots = CCNode::create();
    fields->m_dots->setAnchorPoint({0.5f, 0.f});
    fields->m_dots->setContentSize({getContentWidth(), 5.f * barScale});
    fields->m_dots->setPosition({getContentWidth() / 2, 0});
    fields->m_dots->setID("dots"_spr);

    addChild(fields->m_dots);

    for (int i = 0; i < count; i++) {
        auto spr = CCSprite::create("smallDot.png");
        spr->setScale(0.5f * barScale);

        if (clickableDotsMenu) {
            auto btn = geode::Button::createWithNode(spr, [this] (auto sender) {
                goToPage(sender->getTag());
            });
            btn->setCascadeColorEnabled(true);
            btn->setCascadeOpacityEnabled(true);
            btn->setTag(i);
            btn->setZOrder(i);

            fields->m_dots->addChild(btn);
        }
        else {
            fields->m_dots->addChild(spr);
        }
    } 

    if (spacerLeft && spacerRight) {
        float x = (spacerLeft->getPositionX() + spacerRight->getPositionX()) / 2;
        setPosition({x, 0});
    }
    else {
        setPosition({getContentWidth() / 2, 0});
    }

    showPage();
}

void ETEditButtonBar::loadFromItems(CCArray* items, int c, int r, bool preserve) {
    auto editorUI = ETEditorUI::get();

    if (!editorUI->m_fields->m_initialized) {
        EditButtonBar::loadFromItems(items, c, r, preserve);
        return;
    }

    EditButtonBar::loadFromItems(items, c, r, preserve);
    setupChanges(c, r);
}

void ETEditButtonBar::updatePage() {
    auto fields = m_fields.self();
    if (!fields->m_dots) return;

    auto arr = fields->m_dots->getChildrenExt<CCNodeRGBA>();

    if (arr.empty()) return;
    auto editorUI = ETEditorUI::get();
    auto barScale = editorUI->m_toolbarHeight / 92;

    float gap = 12.f * barScale;
    float overallWidth = fields->m_dots->getContentWidth() - 15.f * barScale;

    float width = (gap + arr[0]->getScaledContentWidth()) * fields->m_dots->getChildrenCount() - gap;
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
    if (getUserFlag("disable-rewrite"_spr)) return;
    auto editorUI = ETEditorUI::get();
    auto barScale = editorUI->m_toolbarHeight / 92;

    m_scrollLayer->m_extendedLayer->setContentSize(getContentSize());
    m_scrollLayer->m_extendedLayer->ignoreAnchorPointForPosition(false);
    m_scrollLayer->m_extendedLayer->setPosition(getContentSize() / 2);

    auto fields = m_fields.self();

    auto menu = getChildByType<CCMenu>(0);
    if (menu) {
        menu->setContentSize({getContentSize().width, fields->m_menuHeight});
        menu->setAnchorPoint({0.5f, 1.f});
        menu->ignoreAnchorPointForPosition(false);
        menu->setPosition({getContentWidth() / 2, getContentHeight() - 7 * barScale});

        auto leftBtn = menu->getChildByType<CCMenuItemSpriteExtra*>(0);
        auto rightBtn = menu->getChildByType<CCMenuItemSpriteExtra*>(1);

        if (leftBtn) {
            leftBtn->setPosition({leftBtn->getScaledContentWidth() / 2 + 5 * barScale, menu->getContentHeight() / 2});
            leftBtn->setScale(barScale);
            leftBtn->m_baseScale = barScale;
        }
        if (rightBtn) {
            rightBtn->setPosition({getContentWidth() - leftBtn->getScaledContentWidth() / 2 - 5 * barScale, menu->getContentHeight() / 2});
            rightBtn->setScale(barScale);
            rightBtn->m_baseScale = barScale;
        }
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
    if (getUserFlag("disable-rewrite"_spr)) return;

    if (!visible) {
        for (auto page : m_scrollLayer->m_pages->asExt<ButtonPage>()) {
            page->removeFromParent();
        }
    }
    else {
        showPage();
    }
}