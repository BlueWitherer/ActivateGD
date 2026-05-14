#include <Geode/Geode.hpp>

class ActivationPopup final : public geode::Popup {
private:
    std::string m_code;

    geode::TextInput* m_codeInput = nullptr;

    geode::Result<> validate();

protected:
    bool init();

public:
    static ActivationPopup* create();
};