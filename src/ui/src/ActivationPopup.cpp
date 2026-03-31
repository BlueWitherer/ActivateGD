#include "../ActivationPopup.h"

#include "../ActivateOverlay.h"

#include <asp/iter.hpp>
#include <asp/time.hpp>

#include <Geode/Geode.hpp>

#include <Geode/ui/Button.hpp>

using namespace geode::prelude;

// ez string
namespace str = utils::string;

static std::unordered_map<unsigned int, std::string_view> const s_numToLetter = {
    {2, "ABC"},
    {3, "DEF"},
    {4, "GHI"},
    {5, "JKL"},
    {6, "MNO"},
    {7, "PQRS"},
    {8, "TUV"},
    {9, "WXYZ"},
};

static constexpr auto s_allowedChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

class ActivationPopup::Impl final {
public:
    std::string code;

    TextInput* codeInput = nullptr;
};

ActivationPopup::ActivationPopup() : m_impl(std::make_unique<Impl>()) {};
ActivationPopup::~ActivationPopup() {};

bool ActivationPopup::init() {
    auto gen = utils::random::generateString;  // sry i couldnt think of any other way
    m_impl->code = fmt::format("{}-{}-{}-{}-{}", gen(5, s_allowedChars), gen(5, s_allowedChars), gen(5, s_allowedChars), gen(5, s_allowedChars), gen(5, s_allowedChars));

    if (!Popup::init({275.f, 140.f})) return false;

    setID("activation"_spr);
    setTitle("Activate your GD!");

    m_impl->codeInput = TextInput::create(250.f, "Activation code...");
    m_impl->codeInput->setID("code-input"_spr);
    m_impl->codeInput->setPosition(m_mainLayer->getScaledContentSize() / 2.f);
    m_impl->codeInput->setFilter("0123456789-");
    m_impl->codeInput->setMaxCharCount(29);

    auto inputLabel = CCLabelBMFont::create("Type in your activation key", "bigFont.fnt");
    inputLabel->setID("code-input-label"_spr);
    inputLabel->setAlignment(kCCTextAlignmentCenter);
    inputLabel->setAnchorPoint({0.5, 0.5});
    inputLabel->setPosition({m_impl->codeInput->getPositionX(), m_impl->codeInput->getPositionY() + 25.f});
    inputLabel->setScale(0.375f);

    m_mainLayer->addChild(m_impl->codeInput, 9);
    m_mainLayer->addChild(inputLabel, 1);

    auto codeCopyBtnSpr = CCLabelBMFont::create(m_impl->code.c_str(), "chatFont.fnt");
    codeCopyBtnSpr->setAnchorPoint({0.5, 0.5});
    codeCopyBtnSpr->setAlignment(kCCTextAlignmentCenter);
    codeCopyBtnSpr->setColor({225, 225, 125});

    auto codeCopyBtn = Button::createWithNode(
        codeCopyBtnSpr,
        [this](auto) {
            utils::clipboard::write(m_impl->code);
            if (auto notif = Notification::create("Copied to clipboard", NotificationIcon::Info, 1.25f)) notif->show();
        });
    codeCopyBtn->setID("copy-code-btn"_spr);
    codeCopyBtn->setPosition({m_impl->codeInput->getPositionX(), m_impl->codeInput->getPositionY() - 30.f});
    codeCopyBtn->setScale(0.875f);

    m_mainLayer->addChild(codeCopyBtn, 1);

    auto codeHintLabel = CCLabelBMFont::create("Replace each letter with its corresponding number.", "chatFont.fnt");
    codeHintLabel->setID("code-hint-label"_spr);
    codeHintLabel->setAlignment(kCCTextAlignmentCenter);
    codeHintLabel->setAnchorPoint({0.5, 0.5});
    codeHintLabel->setPosition({m_impl->codeInput->getPositionX(), m_impl->codeInput->getPositionY() - 45.f});
    codeHintLabel->setScale(0.5f);

    m_mainLayer->addChild(codeHintLabel);

    auto infoBtn = Button::createWithSpriteFrameName(
        "GJ_infoIcon_001.png",
        [this](auto) {
            if (auto popup = FLAlertLayer::create(
                    this,
                    "Help",
                    "To <cy>activate GD</c>, you must type <cg>the provided auto-generated code</c> into the text field with each letter <cy>substituted by its corresponding number on a phone keypad</c>.\n\nFor example, \"<cl>GEODE</c>\" should be written instead as <cj>43633</c>.",
                    "OK",
                    nullptr,
                    300.f)) popup->show();
        });
    infoBtn->setID("info-btn");
    infoBtn->setScale(0.75f);
    infoBtn->setPosition(m_mainLayer->getScaledContentSize() - 15.f);

    m_mainLayer->addChild(infoBtn, 9);

    auto activateBtn = Button::createWithNode(
        ButtonSprite::create(
            "Activate",
            "bigFont.fnt",
            "GJ_button_01.png",
            0.75f),
        [this](Button* btn) {
            auto res = validate();
            if (res.isOk()) {
                log::info("activated gd");

                createQuickPopup(
                    "GD Activated",
                    "Your installation of Geometry Dash has been <cg>activated</c>.\n\nWhat for...?",
                    "OK",
                    nullptr,
                    [this, btn](auto, auto) {
                        if (auto overlay = ActivateOverlay::get()) overlay->setVisible(false);
                        onClose(btn);
                    });
            } else if (res.isErr()) {
                log::error("couldn't activate gd - {}", res.unwrapErr());
                if (auto notif = Notification::create(res.unwrapErr(), NotificationIcon::Error, 2.5f)) notif->show();
            };
        });
    activateBtn->setID("activate-btn");
    activateBtn->setScale(0.75f);
    activateBtn->setPosition({m_mainLayer->getScaledContentWidth() / 2.f, 1.25f});

    m_mainLayer->addChild(activateBtn, 1);

    setTouchPriority(-511);  // its fine u can thank rob

    return true;
};

Result<> ActivationPopup::validate() {
    if (m_impl->codeInput) {
        auto const code = str::filter(m_impl->code, s_allowedChars);
        auto const input = str::filter(m_impl->codeInput->getString(), "0123456789");

        if (input.size() <= 0) return Err("Code is missing");
        if (code.size() != input.size()) return Err("Code format doesn't match");

        using namespace asp;

        auto valid = iter::from(code)
                         .zip(iter::from(input))
                         .all([](auto pair) {
                             char letter = pair.first;      // generated code
                             char digitChar = pair.second;  // user input code

                             auto num = utils::numFromString<unsigned int>(std::string{digitChar});
                             if (!num.isOk()) return false;

                             unsigned int digit = std::move(num).unwrap();

                             log::trace("comparing int {} with char {}", digit, letter);

                             auto it = s_numToLetter.find(digit);
                             if (it == s_numToLetter.end()) return false;

                             return it->second.find(letter) != std::string_view::npos;
                         });

        if (valid) return Ok();
        return Err("Invalid code");
    };

    return Err("Failed to access text input");
};

ActivationPopup* ActivationPopup::create() {
    auto ret = new ActivationPopup();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    };

    delete ret;
    return nullptr;
};
