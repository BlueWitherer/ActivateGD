#include "../ActivateOverlay.h"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ActivateOverlay::Impl final {
public:
    CCSize const size = CCDirector::sharedDirector()->getWinSize();

    CCLabelBMFont* activate = nullptr;
    CCLabelBMFont* settings = nullptr;

    bool forceShow = Mod::get()->getSettingValue<bool>("force-show");

    bool isActivated = Mod::get()->getSavedValue<bool>("activated", false);
};

ActivateOverlay::ActivateOverlay() : m_impl(std::make_unique<Impl>()) {};
ActivateOverlay::~ActivateOverlay() {};

bool ActivateOverlay::init() {
    if (!CCNode::init()) return false;

    setID("overlay"_spr);
    setContentSize(m_impl->size);
    setAnchorPoint({0.5, 0.5});
    setPosition(m_impl->size / 2.f);
    setVisible(!m_impl->isActivated);

    m_impl->activate = CCLabelBMFont::create("Activate GD", "bigFont.fnt");
    m_impl->activate->setID("activate-title");
    m_impl->activate->setAnchorPoint({0, 0.5});
    m_impl->activate->setPosition({m_impl->size.width - 200.f, 60.f});
    m_impl->activate->setScale(0.625f);
    m_impl->activate->setOpacity(125);

    addChild(m_impl->activate);

    m_impl->settings = CCLabelBMFont::create("Go to settings to activate Geometry Dash.", "chatFont.fnt");
    m_impl->settings->setID("activate-text");
    m_impl->settings->setAnchorPoint({0, 0.5});
    m_impl->settings->setPosition({m_impl->size.width - 200.f, m_impl->activate->getPositionY() - 20.f});
    m_impl->settings->setScale(0.625f);
    m_impl->settings->setOpacity(125);

    addChild(m_impl->settings);

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