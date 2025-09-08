#include "pch.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "GameObject.h"
#include "Player.h"
#include "Animation.h"

static const std::string& enemyAtlasPath = "../assets/sprites/atlases/enemyatlas.png";

Enemies::Enemy::Enemy(int level, std::string enemyType) :level(level), enemyType(enemyType) {
	if (!atlasLoaded) {
		enemyAtlasTexture = TextureManager::getTexture(enemyAtlasPath);
		atlasLoaded = true;
	}
}

void Enemies::Enemy::init() {
	LoadInfoFromJson(enemyType);
	EnemyManager::getInstance().add(parent.lock()->shared_from_this());
	hitFlickerTimer.getEndEvent().subscribe(shared_from_this(), &Enemy::ResetHitFlicker);
	// upon adding enemymovement, this gameobject becomes an enemy.
	// since GameobjectManager only uses raw pointers, and is only meant to handle drawing and updating gameobjects
	// we need a seperate manager for enemies to store their shared_ptr's.


	// have to manually add, since enemy object was created with no sprite, 
	// it isn't added to renderlayers_ due to invalid drawable ptr.
	GameObjectManager::getInstance().addExternalRenderable(parent.lock()->getRenderable(), 1);


}

void Enemies::Enemy::Destroy() {
	auto& parentPTR = parent;
	EnemyManager::getInstance().remove(parentPTR, true);
	EnemyManager::removeHitboxVisual(parentPTR);

}

void Enemies::Enemy::SetPlayer(GameObject* player) {
	_playerObj = player; // called once near playerObj creation to assign a target.
	_playerComponent = player->getDerivativesOfComponent<Player>().get();
	playerPos = &_playerObj->getPosition();

}
void Enemies::Enemy::update(float deltatime) {
	auto parentS = parent.lock();
	sf::Vector2f newDirection = (*playerPos - parentS->getPosition());
	float newSqrMag = newDirection.lengthSquared();
	if (newSqrMag <= halfSize) { direction = { 0,0 }; }
	else if (newSqrMag > 0) {// avoids normalizing zero vector, which crashes.
		direction = newDirection.normalized();
		// "knockback" is handled in getspeed().
		parentS->move(direction * getSpeed() * deltatime);
	}

	// checks attack timer and range check on playerObj, and deals _damage if it should. 
	if (Player::isVulnrable()) {
		Attack(deltatime, _playerObj);
	}
	hitFlickerTimer.update(deltatime);



}

void Enemies::Enemy::Attack(float deltaTime, GameObject* playerObj) {
	if (attackTimer >= 0){ // if attack isn't ready, decrement timer and return;
		attackTimer -= deltaTime;
		return;
	}
	//otherwise, attack.

	auto parentS = parent.lock();

	// 32 is the radius of the playerObj hitbox. 
	constexpr int playerSize = 32 * 32; // squared playerObj size, because we use sqr magnitude.
	if ((playerObj->getPosition() - parentS->getPosition()).lengthSquared() - playerSize <= _size) {

#pragma region make hitbox sprite
		auto hitbox = EnemyManager::getHitboxPool().make<TimedDestroy>(0,0.125f);
		if (hitbox) {
			hitbox->setSprite("../assets/sprites/shapes/circle_32.png", sf::IntRect{ {0,0},{32,32} });
			hitbox->getSprite()->setColor(sf::Color(192, 0, 0, 128));
			hitbox->setPosition(parentS->getPosition());
			hitbox->setOrigin(16, 16);
			hitbox->setScale(_attackSize, _attackSize);
			GameObjectManager::getInstance().add(hitbox, 0);
		}
		else
		{
			std::cout << "\nerror making hitbox visual";
		}
#pragma endregion

		auto player = playerObj->getDerivativesOfComponent<Player>();
		player->takeDamage(_damage); // dmg playerObj (or subclass of playerObj)
		attackTimer = _attackSpeed; // reset attack timer.
	}

}

bool Enemies::Enemy::takeDamage(int _damage) {
	int finalDamage = Player::getStats()->Damage(_damage);
	_curHp -= finalDamage;
	
	if (_curHp <= 0) {
		OnDeath();
		Destroy();
		return true;
	}

	hitFlickerTimer.start(true);
	parent.lock()->getSprite()->setColor(hitColour);
	return false;

}

void Enemies::Enemy::OnDeath() {
	_playerComponent->AddXP(xpValue);
}


void Enemies::Enemy::ResetHitFlicker() {
	parent.lock()->getSprite()->setColor(sf::Color::White);
}


const json& Enemies::Enemy::LoadInfoFromJson(std::string enemyType) {
	const auto& json = GameData::getEnemy(enemyType);
	auto parentS = parent.lock();
	float increasePerLevel = getStatIncreaseMultiplier();

	if (json.contains("hp")) {
		_maxHp = json["hp"]* increasePerLevel;
		_curHp = _maxHp;
	}
	else { std::cerr << "\nhp not found/defined in json for " << enemyType; }

	if (json.contains("damage")) {
		_damage = json["damage"] * increasePerLevel;
	}
	else { std::cerr << "\nDamage not found/defined in json for " << enemyType; }

	if (json.contains("attackSpeed")) {
		_attackSpeed = 1 / json["attackSpeed"].get<float>();
	}
	else { std::cerr << " not found/defined in json for " << enemyType; }

	if (json.contains("attackSize")) {
		_attackSize = json["attackSize"];
	}
	else { std::cerr << " \n\"attack size\"not found/defined in json for " << enemyType; }

	if (json.contains("speed")) {
		_speed = json["speed"];

	}
	else { std::cerr << "\n\"speed\" not found/defined in json for " << enemyType; }

	if (json.contains("size")) {
		_size = json["size"];
		halfSize = _size / 2.0f;

	}
	else { std::cerr << "\n\"size \" not found/defined in json for " << enemyType; }

	if (json.contains("xp")) {
		xpValue = json["xp"];
	}
	else { std::cerr << "\n\"xp \" not found/defined in json for " << enemyType; }

	
	if (json.contains("spriteData")) {
		const auto& spriteData = json["spriteData"];
		// if sprite data is defined, sprite size and text position MUST also be there.
		std::vector<int> rawSize= spriteData["spriteSize"].get<std::vector<int>>();
		sf::Vector2i size = { rawSize[0],rawSize[1] };
		
		std::vector<int> rawTextCoords = spriteData["textureStartPos"].get<std::vector<int>>();
		sf::Vector2i pos = { rawTextCoords[0],rawTextCoords[1] };
		
		spriteRect = { pos ,size };
		parentS->setSprite(enemyAtlasTexture,spriteRect); 

		if (spriteData.contains("animation data")) {
			SpriteAnimation animation{ spriteRect };
			animation.LoadFromJson(spriteData["animation data"]);
			parentS->addComponent<SpriteAnimator>(animation);
		}


		
	}
	else {
		std::cerr << "\nSprite path not defined for enemy: " << enemyType;
		parentS->setSprite("../assets/sprites/twig.png");
	}
	parentS->setOrigin(spriteRect.size.x / 2.0f, spriteRect.size.y / 2.0f);

	if (json.contains("audioData")) {
		LoadAudioDataFromJson(json["audioData"]);
	}
	else
		std::cout << "\nenemy \"" << enemyType << "\" contains no audio data.";

	return json;

}

void Enemies::Enemy::UpdateFacingDirection() {
	if (direction.x == 0) return; // dont flip if input released.
	bool newFacingDirection = direction.x < 0; // signbit returns true if number is negative.
	if (newFacingDirection != facingDirection) { // only flip if facing direction is not the same as before.
		facingDirection = newFacingDirection; // store new facing direction
		parent.lock()->scaleObject(-1, 1); // flip playerObj.
	}
}