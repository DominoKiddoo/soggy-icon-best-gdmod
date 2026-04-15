#include <Geode/Geode.hpp>


#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/binding/GJGameState.hpp>
#include "../include/globalVars.hpp"

using namespace geode::prelude;


class $modify(SoggyPlayLayer, PlayLayer) {
	struct Fields {
        CCSprite* m_sogIcon = nullptr;
        CCSprite* m_sogIconP2 = nullptr;
		bool m_isTwoPlayerMode = false;

		float m_sogIconP1ActualScaleY = 1.0f;
		float m_sogIconP2ActualScaleY = 1.0f;

		bool isHolding = false;

		bool isHoldingP1 = false;
		bool isHoldingP2 = false;

		// "nothing is too long", Tyegurr 2026
    };

	// there is a commment here to prove i am not a fuck map
	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
			return false;
		}


		m_fields->m_isTwoPlayerMode = level->m_twoPlayerMode;

		m_fields->m_sogIcon = nullptr;
   		m_fields->m_sogIconP2 = nullptr;

		if (!Mod::get()->getSettingValue<bool>("disablesog")) {
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


			float baseScale = iconSize - 0.5f;
            m_fields->m_sogIcon->setScale(baseScale);
            m_fields->m_sogIconP2->setScale(baseScale);

            m_fields->m_sogIconP1ActualScaleY = m_fields->m_sogIcon->getScaleY();
            m_fields->m_sogIconP2ActualScaleY = m_fields->m_sogIconP2->getScaleY();


			this->m_objectLayer->addChild(m_fields->m_sogIcon);
			this->m_objectLayer->addChild(m_fields->m_sogIconP2);


			// squish :3c

            if (Mod::get()->getSettingValue<bool>("squish")) {
                m_fields->m_sogIcon->setScaleY(m_fields->m_sogIconP1ActualScaleY * 0.8f);
                m_fields->m_sogIconP2->setScaleY(m_fields->m_sogIconP2ActualScaleY * 0.8f);

                m_fields->m_sogIconP1ActualScaleY = m_fields->m_sogIcon->getScaleY();
                m_fields->m_sogIconP2ActualScaleY = m_fields->m_sogIconP2->getScaleY();
            }

			this->schedule(schedule_selector(SoggyPlayLayer::updSog));
		}

		return true;
	}

	void updSog(float dt) {

		if (!m_fields->m_sogIcon || !m_fields->m_sogIconP2) {
        	return; 
    	}
		auto baseGameLayer = GJBaseGameLayer::get();

		auto sogIcon = m_fields->m_sogIcon;
		auto sogIconP2 = m_fields->m_sogIconP2;

		baseGameLayer->m_player1->setCascadeOpacityEnabled(true);
		baseGameLayer->m_player2->setCascadeOpacityEnabled(true);


		// ooh cool squish thing
		if (!m_fields->m_isTwoPlayerMode) {
			if (!m_fields->isHolding) {
				sogIcon->setScaleY(sogIcon->getScaleY() + (m_fields->m_sogIconP1ActualScaleY - sogIcon->getScaleY()) * std::min(1.0f, dt * 15.0f));
				sogIconP2->setScaleY(sogIconP2->getScaleY() + (m_fields->m_sogIconP2ActualScaleY - sogIconP2->getScaleY()) * std::min(1.0f, dt * 15.0f));

			} else {
				sogIcon->setScaleY(sogIcon->getScaleY() + (m_fields->m_sogIconP1ActualScaleY * 1.8f - sogIcon->getScaleY()) * std::min(1.0f, dt * 15.0f));
				sogIconP2->setScaleY(sogIconP2->getScaleY() + (m_fields->m_sogIconP2ActualScaleY * 1.8f - sogIconP2->getScaleY()) * std::min(1.0f, dt * 15.0f));
			}
		} else {
			if (!m_fields->isHoldingP1) {
				sogIcon->setScaleY(sogIcon->getScaleY() + (m_fields->m_sogIconP1ActualScaleY - sogIcon->getScaleY()) * std::min(1.0f, dt * 15.0f));

			} else {
				sogIcon->setScaleY(sogIcon->getScaleY() + (m_fields->m_sogIconP1ActualScaleY * 1.8f - sogIcon->getScaleY()) * std::min(1.0f, dt * 15.0f));
			}

			if (!m_fields->isHoldingP2) {
				sogIconP2->setScaleY(sogIconP2->getScaleY() + (m_fields->m_sogIconP2ActualScaleY - sogIconP2->getScaleY()) * std::min(1.0f, dt * 15.0f));

			} else {
				sogIconP2->setScaleY(sogIconP2->getScaleY() + (m_fields->m_sogIconP2ActualScaleY * 1.8f - sogIconP2->getScaleY()) * std::min(1.0f, dt * 15.0f));
			}
		}

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

	void onQuit() {
		PlayLayer::onQuit();

		if (playingSoggyLevel) {
			playingSoggyLevel = false;
			
			if (Mod::get()->getSettingValue<bool>("disablesog")) {
				return;
			}
			
			Loader::get()->queueInMainThread([this]() {
				auto dialogue = DialogObject::create(
					"soggy cat", 
					"<cr>HAHA! YOU FAILED MY CHALLENGE!! TRY AGAIN WHEN YOU HAVE THE SKILL, THEN YOU MAY DISABLE ME!</c> (btw if you really do need to disable the icons, use the <cy>geode toggle</c> i wont judge)", 
					1, 
					0.7f, 
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
				OverlayManager::get()->addChild(layer);

			});
			
			
		}
	

	}

};


class $modify(BJHookLayer, GJBaseGameLayer) {
	void handleButton(bool down, int button, bool isPlayer1) {
		GJBaseGameLayer::handleButton(down, button, isPlayer1);
		
		if (m_isEditor || Mod::get()->getSettingValue<bool>("disablesog")) {
			return;
		}

		auto sogIconP1 = static_cast<SoggyPlayLayer*>(PlayLayer::get())->m_fields->m_sogIcon;
		auto sogIconP2 = static_cast<SoggyPlayLayer*>(PlayLayer::get())->m_fields->m_sogIconP2;

		auto fields = static_cast<SoggyPlayLayer*>(PlayLayer::get())->m_fields.self();
		log::info("button: {}, down: {}, isPlayer1: {}", button, down, isPlayer1);

		
		
		if (button != 1) {
			return;
		}
		
		if (fields->m_isTwoPlayerMode) {
			if (down && isPlayer1) {
				fields->isHoldingP1 = true;
			} else if (!down && isPlayer1) {
				fields->isHoldingP1 = false;
			}

			if (down && !isPlayer1) {
				fields->isHoldingP2 = true;
			} else if (!down && !isPlayer1) {
				fields->isHoldingP2 = false;
			}

		} else {
			if (down) {
				fields->isHolding = true;
				// sogIconP1->setScaleY(sogIconP1->getScaleY() * 1.8f);
				// sogIconP2->setScaleY(sogIconP2->getScaleY() * 1.8f);
			} else {
				fields->isHolding = false;
			}
		}
	}
};