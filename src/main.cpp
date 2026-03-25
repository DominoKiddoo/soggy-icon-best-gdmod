#include <Geode/Geode.hpp>


#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/binding/GJGameState.hpp>

using namespace geode::prelude;


class $modify(SoggyPlayLayer, PlayLayer) {
	struct Fields {
		float lastP2X = 0.0f;
	};

	// there is a commment here to prove i am not a fuck map
	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
			return false;
		}


		auto sogIcon = CCSprite::create("sog.png"_spr);
		sogIcon->setID("dominodev.soggy-icon/sogIcon");

		auto iconSize = Mod::get()->getSettingValue<float>("icon-scale");
		
		sogIcon->setScale(iconSize - 0.5f);

		auto sogIconP2 = CCSprite::create("soginverted.png"_spr);
		sogIconP2->setID("dominodev.soggy-icon/sogIconP2");
		
		sogIconP2->setScale(iconSize - 0.5f);

		sogIconP2->setPositionX(-4000);
		

		GJBaseGameLayer::get()->m_objectLayer->addChild(sogIcon);
		GJBaseGameLayer::get()->m_objectLayer->addChild(sogIconP2);

		sogIcon->setZOrder(50667); // because it spells soggy hahahaha i am so funny index staff please laugh
		sogIconP2->setZOrder(50667);
		this->schedule(schedule_selector(SoggyPlayLayer::updSog));


		return true;
	}

	void updSog(float dt) {
		auto baseGameLayer = GJBaseGameLayer::get();

		auto sogIcon = baseGameLayer->m_objectLayer->getChildByID("dominodev.soggy-icon/sogIcon");
		auto sogIconP2 = baseGameLayer->m_objectLayer->getChildByID("dominodev.soggy-icon/sogIconP2");

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

