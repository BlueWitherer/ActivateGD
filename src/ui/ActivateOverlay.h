#include <Geode/Geode.hpp>

namespace ids {
    inline constexpr auto overlay = "overlay"_spr;
    inline constexpr auto activator = "activator"_spr;
};

class ActivateOverlay final : public cocos2d::CCNode {
private:
    cocos2d::CCSize const m_size = cocos2d::CCDirector::sharedDirector()->getWinSize();

    cocos2d::CCLabelBMFont* m_activate = nullptr;
    cocos2d::CCLabelBMFont* m_settings = nullptr;

protected:
    bool init() override;

    static ActivateOverlay* create();

public:
    static ActivateOverlay* get();
};