#include "ui/ActivateOverlay.h"
#include "ui/ActivationPopup.h"

#include <Geode/Geode.hpp>

#include <Geode/ui/Button.hpp>

#include <Geode/modify/MoreOptionsLayer.hpp>

using namespace geode::prelude;

$on_game(Loaded) {
    if (auto overlay = OverlayManager::get()) {
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
                btn->setPosition({m_mainLayer->getScaledContentWidth() - 110.f, 47.5f});
                btn->setTouchPriority(-510);

                m_mainLayer->addChild(btn, 9);
            };
        };

        return true;
    };
};