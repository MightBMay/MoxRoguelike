#pragma once
#include "pch.h"
#include "Animation.h"
#include "Enemy.h"
#include "EnemyManager.h"


namespace Enemies {
	class ScissorEnemy : public Enemy {

		static inline FMOD::Studio::EventInstance* snipEvent = nullptr;

	public:
		ScissorEnemy(int level) : Enemy(level, "scissor enemy") {}


		virtual void init() override {
			Enemy::init();

			auto animator = parent.lock()->getComponent<SpriteAnimator>();

			animator->getAnimationLoopEvent().subscribe([this]() {
				snipEvent->setParameterByName("pitch", rng::getFloat(0, 1));
				snipEvent->start();
				}
			);

		}

		virtual void Destroy() {
			Enemy::Destroy();
			if(snipEvent!= nullptr) snipEvent->release();
		}


		virtual void LoadAudioDataFromJson(const json& audioData) override{
			if (audioData.contains("snip")) {
				snipEvent = Audio::CreateFMODEvent(audioData["snip"]);
			}
			else
				std::cout << "\nno \"snip\" element found in audio data for \"" << enemyType << "\".";




		}

	};
}