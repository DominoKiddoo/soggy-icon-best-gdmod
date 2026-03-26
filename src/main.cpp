#include <Geode/Geode.hpp>


#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/binding/GJGameState.hpp>

using namespace geode::prelude;


class $modify(SoggyPlayLayer, PlayLayer) {
	struct Fields {
        CCSprite* m_sogIcon = nullptr;
        CCSprite* m_sogIconP2 = nullptr;
    };

	// there is a commment here to prove i am not a fuck map
	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
			return false;
		}

		auto iconSize = Mod::get()->getSettingValue<float>("icon-scale");


		m_fields->m_sogIcon = CCSprite::create("sog.png"_spr);
        m_fields->m_sogIcon->setID("sogIcon"_spr);
        m_fields->m_sogIcon->setScale(iconSize - 0.5f);
        m_fields->m_sogIcon->setZOrder(50667); // because it spells soggy hahahaha i am so funny index staff please laugh

        
        m_fields->m_sogIconP2 = CCSprite::create("soginverted.png"_spr);
        m_fields->m_sogIconP2->setID("sogIconP2"_spr);
        m_fields->m_sogIconP2->setScale(iconSize - 0.5f);
        m_fields->m_sogIconP2->setZOrder(50667);
        m_fields->m_sogIconP2->setVisible(false);

        this->m_objectLayer->addChild(m_fields->m_sogIcon);
        this->m_objectLayer->addChild(m_fields->m_sogIconP2);
		this->schedule(schedule_selector(SoggyPlayLayer::updSog));


		return true;
	}

	void updSog(float dt) {
		auto baseGameLayer = GJBaseGameLayer::get();

		auto sogIcon = m_fields->m_sogIcon;
		auto sogIconP2 = m_fields->m_sogIconP2;

		baseGameLayer->m_player1->setCascadeOpacityEnabled(true);
		baseGameLayer->m_player2->setCascadeOpacityEnabled(true);



		sogIcon->setPosition(baseGameLayer->m_player1->m_position);
		sogIconP2->setPosition(baseGameLayer->m_player2->m_position);


		// better way of hiding player #sigma
		if (baseGameLayer->m_player1->getOpacity() != 0) {
			baseGameLayer->m_player1->setOpacity(0);
		}

		if (baseGameLayer->m_player2->getOpacity() != 0) {
			baseGameLayer->m_player2->setOpacity(0);
		}

		if ((baseGameLayer->m_gameState.m_isDualMode)) { // w dank meme 01
			sogIconP2->setVisible(true);
		} else {
			sogIconP2->setVisible(false);
		}
		


	}
};

