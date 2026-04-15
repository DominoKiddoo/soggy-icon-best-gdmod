#include <Geode/Geode.hpp>
using namespace geode::prelude;


$on_mod(Loaded) {
    auto mod = Mod::get();


    if (Mod::get()->getSettingValue<bool>("soggyreaction")) {
        auto oldOverlay = OverlayManager::get()->getChildByID("soggyreactionoverlay"_spr);
        if (oldOverlay) {
            OverlayManager::get()->removeChild(oldOverlay);
        }
        auto overlay = CCSprite::create("livesoggyreaction.png"_spr);
        overlay->setScale(0.5f);

        auto winSize = CCDirector::get()->getWinSize();
        overlay->setAnchorPoint({0.0f, 0.0f});
        overlay->setID("soggyreactionoverlay"_spr);
        overlay->setOpacity(Mod::get()->getSettingValue<float>("reactionopacity") * 255);
        OverlayManager::get()->addChild(overlay);
    }

    listenForSettingChanges<bool>("soggyreaction", [](bool value) {
        Loader::get()->queueInMainThread([value]() {
            if (value) {
                auto oldOverlay = OverlayManager::get()->getChildByID("soggyreactionoverlay"_spr);
                if (oldOverlay) {
                    OverlayManager::get()->removeChild(oldOverlay);
                }
                auto overlay = CCSprite::create("livesoggyreaction.png"_spr);
                overlay->setScale(0.5f);
                overlay->setOpacity(Mod::get()->getSettingValue<float>("reactionopacity") * 255);
                auto winSize = CCDirector::get()->getWinSize();
                overlay->setAnchorPoint({0.0f, 0.0f});
                overlay->setID("soggyreactionoverlay"_spr);

                OverlayManager::get()->addChild(overlay);
            } else {
                auto oldOverlay = OverlayManager::get()->getChildByID("soggyreactionoverlay"_spr);
                if (oldOverlay) {
                    OverlayManager::get()->removeChild(oldOverlay);
                }
            }
        });
    });

    listenForSettingChanges<float>("reactionopacity", [](float value) {
        Loader::get()->queueInMainThread([value]() {
            CCSprite* overlay = static_cast<CCSprite*>(OverlayManager::get()->getChildByID("soggyreactionoverlay"_spr));
            if (overlay) {
                overlay->setOpacity(value * 255);
            }
        });
    });
}