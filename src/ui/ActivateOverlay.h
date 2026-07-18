#include <Geode/Geode.hpp>

#define AGD_HORRIBLE_ID_OVERLAY "overlay"_spr;
#define AGD_HORRIBLE_ID_ACTIVATOR "activator"_spr;

namespace ids {
    inline constexpr auto overlay = AGD_HORRIBLE_ID_OVERLAY;
    inline constexpr auto activator = AGD_HORRIBLE_ID_ACTIVATOR;
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