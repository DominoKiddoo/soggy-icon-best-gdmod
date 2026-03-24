#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayLayer.hpp>



class $modify(SoggyPlayLayer, PlayLayer) {



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


		GJBaseGameLayer::get()->m_objectLayer->addChild(sogIcon);
		GJBaseGameLayer::get()->m_objectLayer->addChild(sogIconP2);

		sogIcon->setZOrder(50667); // because it spells soggy hahahaha i am so funny index staff please laugh
		this->schedule(schedule_selector(SoggyPlayLayer::updSog));


		return true;
	}

	void updSog(float dt) {
		auto baseGameLayer = GJBaseGameLayer::get();

		auto sogIcon = baseGameLayer->m_objectLayer->getChildByID("dominodev.soggy-icon/sogIcon");
		auto sogIconP2 = baseGameLayer->m_objectLayer->getChildByID("dominodev.soggy-icon/sogIconP2");

		auto player = PlayLayer::get()->getChildByID("main-node")->getChildByID("batch-layer")->getChildByID("PlayerObject");

		baseGameLayer->m_player1->setScale(0.0f); // ass fix to hide the player
		baseGameLayer->m_player2->setScale(0.0f); // ass fix to hide the player v2

		

		sogIcon->setPosition(baseGameLayer->m_player1->m_position);
		sogIconP2->setPosition(baseGameLayer->m_player2->m_position);


	}
};