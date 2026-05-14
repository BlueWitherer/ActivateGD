#include "ui/ActivateOverlay.h"
#include "ui/ActivationPopup.h"

#include <ranges>

#include <Geode/Geode.hpp>

#include <Geode/ui/Button.hpp>

#include <Geode/modify/MoreOptionsLayer.hpp>

#include <horrible/OptionalAPI.hpp>

using namespace geode::prelude;
using namespace horrible;

static std::vector<std::weak_ptr<Hook>> s_activatorHooks;  // Array of registered hooks for activator button

$on_game(Loaded) {
    if (auto om = OverlayManager::get()) {
        // mfw i nullcheck static mem >:3
        if (auto overlay = ActivateOverlay::get()) om->addChild(overlay);
    };

    if (Loader::get()->isModLoaded("cubicstudios.horriblemenu")) {
        horrible::listenForHorribleOptionChanges(
            ids::overlay,
            [](HorribleOptionSave data) {
                if (auto activate = ActivateOverlay::get()) activate->setVisible(data.enabled);
            });

        horrible::listenForHorribleOptionChanges(
            ids::activator,
            [](HorribleOptionSave data) {
                for (auto const& hook : s_activatorHooks) {
                    if (auto h = hook.lock()) (void)h->toggle(data.enabled);
                };
            });

        constexpr auto category = "Activate GD";

        auto const optionOverlay = OptionV2(
            ids::overlay,
            "Activation Message",
            "Enable the 'Activate GD' overlay for your entire game.",
            category,
            SillyTier::Medium,
            true);

        auto const optionActivator = OptionV2(
            ids::activator,
            "Activator",
            "Enable the button in game settings to actually 'activate' Geometry Dash.\n<co>This will only appear if the overlay is also visible!</c>",
            category,
            SillyTier::Low,
            true);

        OptionManagerV2::registerOption(optionOverlay);
        OptionManagerV2::registerOption(optionActivator);
    };
};

class $modify(ActivateMoreOptionsLayer, MoreOptionsLayer) {
    static void onModify(auto& self) {
        if (Loader::get()->isModInstalled("cubicstudios.horriblemenu")) {
            utils::StringMap<std::shared_ptr<Hook>> const& hooks = self.m_hooks;  // intellisense my beloved
            auto enable = OptionManagerV2::isEnabled(ids::activator).unwrapOr(true);

            for (auto const& hook : hooks | std::views::values) {
                hook->setAutoEnable(enable);
                (void)hook->toggle(enable);

                s_activatorHooks.push_back(hook);
            };
        };
    };

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