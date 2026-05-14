#include "../ActivateOverlay.h"

#include <horrible/OptionalAPI.hpp>

#include <Geode/Geode.hpp>

using namespace geode::prelude;

bool ActivateOverlay::init() {
    if (!CCNode::init()) return false;

    setID(ids::overlay);
    setContentSize(m_size);
    setAnchorPoint({0.5, 0.5});
    setPosition(m_size / 2.f);

    m_activate = CCLabelBMFont::create("Activate GD", "bigFont.fnt");
    m_activate->setID("activate-title");
    m_activate->setAnchorPoint({0, 0.5});
    m_activate->setPosition({m_size.width - 200.f, 60.f});
    m_activate->setScale(0.625f);
    m_activate->setOpacity(125);

    addChild(m_activate);

    m_settings = CCLabelBMFont::create("Go to settings to activate Geometry Dash.", "chatFont.fnt");
    m_settings->setID("activate-text");
    m_settings->setAnchorPoint({0, 0.5});
    m_settings->setPosition({m_size.width - 200.f, m_activate->getPositionY() - 20.f});
    m_settings->setScale(0.625f);
    m_settings->setOpacity(125);

    addChild(m_settings);

    return true;
};

ActivateOverlay* ActivateOverlay::create() {
    auto ret = new ActivateOverlay();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    };

    delete ret;
    return nullptr;
};

ActivateOverlay* ActivateOverlay::get() {
    static auto inst = ActivateOverlay::create();
    return inst;
};