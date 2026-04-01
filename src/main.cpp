#include "ui/ActivateOverlay.h"
#include "ui/ActivationPopup.h"

#include <Geode/Geode.hpp>

#include <Geode/ui/Button.hpp>

#include <Geode/modify/MoreOptionsLayer.hpp>

using namespace geode::prelude;

$on_game(Loaded) {
    if (auto overlay = OverlayManager::get()) {
        // mfw i nullcheck static memory :D
        if (auto activate = ActivateOverlay::get()) overlay->addChild(activate);
    };
};

class $modify(ActivateMoreOptionsLayer, MoreOptionsLayer) {
    bool init() {
        if (!MoreOptionsLayer::init()) return false;

        if (auto activate = ActivateOverlay::get()) {
            if (!activate->isVisible()) return true;

            if (auto layer = m_mainLayer->getChildByID("gameplay-options-layer-1")) {
                auto btnSprite = CircleButtonSprite::createWithSpriteFrameName(
                    "GJ_lock_open_001.png");
                btnSprite->setTopOffset({0.f, 2.5f});

                auto btn = Button::createWithNode(
                    btnSprite,
                    [](auto) {
                        if (auto popup = ActivationPopup::create()) popup->show();
                    });
                btn->setID("activate-btn"_spr);
                btn->setScale(0.875);
                btn->setPosition({(layer->getScaledContentWidth() / 2.f) + 172.5f, (layer->getScaledContentHeight() / 2.f) - 112.5f});
                btn->setTouchPriority(-510);  // i do this a few times, touch in this layer is so fucked

                layer->addChild(btn, 9);

                auto label = CCLabelBMFont::create("Activate", "bigFont.fnt");
                label->setID("activate-label"_spr);
                label->setScale(0.3f);
                label->setAnchorPoint({0.5, 0.5});
                label->setPosition({btn->getPositionX(), btn->getPositionY() + (btn->getScaledContentHeight() * 0.625f)});

                layer->addChild(label, 9);
            };
        };

        return true;
    };
};