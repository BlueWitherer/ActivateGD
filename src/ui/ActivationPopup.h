#include <Geode/Geode.hpp>

class ActivationPopup final : public geode::Popup, private FLAlertLayerProtocol {
private:
    class Impl;
    std::unique_ptr<Impl> m_impl;

    geode::Result<> validate();

protected:
    ActivationPopup();
    ~ActivationPopup();

    bool init();

public:
    static ActivationPopup* create();
};