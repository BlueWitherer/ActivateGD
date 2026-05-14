#include <Geode/Geode.hpp>

namespace ids {
    inline constexpr auto overlay = "overlay"_spr;
    inline constexpr auto activator = "activator"_spr;
};

class ActivateOverlay final : public cocos2d::CCNode {
private:
    class Impl;
    std::unique_ptr<Impl> m_impl;

protected:
    ActivateOverlay();
    ~ActivateOverlay();

    bool init() override;

    static ActivateOverlay* create();

public:
    static ActivateOverlay* get();
};