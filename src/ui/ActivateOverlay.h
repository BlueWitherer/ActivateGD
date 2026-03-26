#include <Geode/Geode.hpp>

class ActivateOverlay final : public cocos2d::CCNode {
private:
    class Impl;
    std::unique_ptr<Impl> m_impl;

protected:
    ActivateOverlay();
    ~ActivateOverlay();

    bool init() override;

public:
    static ActivateOverlay* create();
    static ActivateOverlay* get();
};