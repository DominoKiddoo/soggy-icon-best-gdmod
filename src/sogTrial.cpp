#include <Geode/Geode.hpp>

#include <Geode/utils/file.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include "../include/globalVars.hpp"

using namespace geode::prelude;

bool ignoreNextChange = false;


$on_mod(Loaded) {
    auto mod = Mod::get();
    playingSoggyLevel = false;

    
    // download song. code adapted from Weebify
    auto MDM = MusicDownloadManager::sharedState();
    auto GLM = GameLevelManager::get();

	GLM->downloadLevel(62912799, false, false);
    #ifdef GEODE_IS_ANDROID
		std::filesystem::path p = MDM->pathForSong(383158).c_str();
		if (!std::filesystem::exists(p.parent_path() / "383158.mp3"_spr))
			std::filesystem::copy(Mod::get()->getResourcesDir() / "383158.mp3", p.parent_path() / "383158.mp3");
	#else
		if (!MDM->isSongDownloaded(383158)) 
			std::filesystem::copy(Mod::get()->getResourcesDir() / "383158.mp3", std::filesystem::path(MDM->pathForSong(383158).c_str()));
	#endif

    listenForSettingChanges<bool>("disablesog", [](bool value) {

        if (ignoreNextChange) {
            ignoreNextChange = false;
            return;
        }

        if (value) {
            Loader::get()->queueInMainThread([]() {
                auto GLM = GameLevelManager::sharedState();
                Mod::get()->setSettingValue("disablesog", false);

                // create le sog
                auto level = GJGameLevel::create();
                auto resDir = Mod::get()->getResourcesDir();
                auto filePath = resDir / "level.txt"; 

                auto levResult = utils::file::readString(filePath);

                if (levResult.isOk()) {
                    auto levString = levResult.unwrap();
                    
                    level->m_levelString = levString;
                    level->m_levelName = "Soggy Sog Trials";
                    level->m_songID = 383158;

                    playingSoggyLevel = true;

                    auto scene = PlayLayer::scene(level, false, false);
                    playingSoggyLevel = true;
                    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
                } else {
                    log::error("failed: {}", levResult.unwrapErr());
                }


            });
        }
    }, mod);
}


class $modify(SoggyEndLevelLayer, EndLevelLayer) {
    void customSetup() {
        EndLevelLayer::customSetup();

        if (!playingSoggyLevel) {
            return;
        }


        if (PlayLayer::get()->m_isPracticeMode) {
            auto dialogue = DialogObject::create(
                "soggy cat", 
                "dude<d050>.<d050>.<d050>.<d050> i admire your effort but you gotta beat it in <s260>NORMAL</s> mode.", 
                1, 
                1, 
                true, 
                {255, 255, 255}
            );
            auto dialogueArray = CCArray::create();
            dialogueArray->addObject(dialogue);

            auto layer = DialogLayer::createDialogLayer(nullptr, dialogueArray, 2);
            layer->animateInRandomSide();

            auto soggyPortrait = CCSprite::create("sog.png"_spr);
            soggyPortrait->setPosition(layer->m_characterSprite->getPosition());
            soggyPortrait->setScale(0.7f);
            layer->m_mainLayer->addChild(soggyPortrait);
            layer->m_characterSprite->setVisible(false);
            
            
            


            CCScene::get()->addChild(layer); // hello index staff. you do actually have to add it to the scene, but i've stopped hardcoding z order. DAMN IN HINDSIGHT THIS COMMENT LOOKS AI HELP 
            return;
        }

        ignoreNextChange = true;
        Mod::get()->setSettingValue("disablesog", true);
        

        
        log::info("Soggy end layer customSetup reached!");


        auto dialogue = DialogObject::create(
            "soggy cat", 
            "<cr><s260>ok, ok fine. </s></c><d150>it is <cy>clear</c> you are skilled with <d100><i100>the sog</i>.<d080> soggy icons has been disabled. <d100><s100><cb>i hope you are happy.</c></s>", 
            1, 
            1, 
            true, 
            {255, 255, 255}
        );


        auto dialogueArray = CCArray::create();
        dialogueArray->addObject(dialogue);

        auto layer = DialogLayer::createDialogLayer(nullptr, dialogueArray, 2);
        layer->animateInRandomSide();

        // blood, sweat and tears went into this please help
        auto soggyPortrait = CCSprite::create("sog.png"_spr);
        soggyPortrait->setPosition(layer->m_characterSprite->getPosition());
        soggyPortrait->setScale(0.7f);
        layer->m_mainLayer->addChild(soggyPortrait);
        layer->m_characterSprite->setVisible(false);
        
        
        


        CCScene::get()->addChild(layer, 100);

    }
    
};  

class $modify(GJBaseGameLayer) {
    bool shouldExitHackedLevel() {
        if (playingSoggyLevel) {
            return false; 
        }
        return GJBaseGameLayer::shouldExitHackedLevel();
    }
};