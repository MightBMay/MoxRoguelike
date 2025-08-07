#include "EnemyManager.h"
#include "Enemy.h"

#include"TestEnemy.h"



const map<int, function<void(shared_ptr<GameObject>, int)>> EnemyManager::EnemyList 
{
    {0,[](shared_ptr<GameObject> obj, int level) {obj->addComponent<TestEnemy>(level); }},
    {1,[](shared_ptr<GameObject> obj, int level) {obj->addComponent<TestEnemy>(level); }}
};


EnemyManager& EnemyManager::getInstance() {
	static EnemyManager instance;
	return instance;
}


void EnemyManager::SpawnEnemy(int index, int level, int quantity) {
    for (int i = 0; i < quantity; ++i)
        SpawnEnemy(index, level);
}
void EnemyManager::SpawnEnemy(int index, int level) {
    static GameObject* player = Enemy::GetPlayer();
    std::shared_ptr<GameObject> enemyObj = GameObject::Create(1);

    auto it = EnemyList.find(index); // search map for index
    //if index not found, return and error log.
    if (it == EnemyList.end()) { std::cerr << "Enemy index out of bounds"; return; }
    it->second(enemyObj, level); // calls function stored in enemylist which 
    // adds the enemy component.

    enemyObj->setPosition( // set position to be around the player, with random 2000x1600 variance. 
        player->getPosition() + sf::Vector2f{ rng::getFloat(-1000, 1000), rng::getFloat(-800, 800) }
    );



}

void EnemyManager::add(std::shared_ptr<GameObject> obj) {
    if (!obj) return;

    
    auto it = std::find_if( // check if object exists already
        enemyObjects_.begin(),
        enemyObjects_.end(),
        [&obj](const std::weak_ptr<GameObject>& weakObj) { // lambda to lock the weak ptr.
            auto locked = weakObj.lock();
            return locked && locked == obj;  
        }
    );

    if (it != enemyObjects_.end()) return;  // Already exists

    enemyObjects_.push_back(obj);  // Store as weak_ptr (implicit conversion)
}

void EnemyManager::remove(std::shared_ptr<GameObject>& obj) {
    if (!obj) return;

    // Remove from main list
    enemyObjects_.erase(
        std::remove_if(
            enemyObjects_.begin(),
            enemyObjects_.end(),
            [&obj](const std::weak_ptr<GameObject>& weakObj) {// Lambda that converts weak pointer 
                auto sharedObj = weakObj.lock();            // to shared
                return !sharedObj || sharedObj == obj; 
            }
        ),
        enemyObjects_.end()
    );



}

void EnemyManager::remove(std::shared_ptr<GameObject>& obj, bool DestroyObject) {
    if (!obj) return;

    // Remove from main list
    enemyObjects_.erase(
        std::remove_if(
            enemyObjects_.begin(),
            enemyObjects_.end(),
            [&obj](const std::weak_ptr<GameObject>& weakObj) { // Lambda that converts weak pointer 
                auto sharedObj = weakObj.lock();              // to shared
                return !sharedObj || sharedObj == obj; 
            }
        ),
        enemyObjects_.end()
    );


    if(DestroyObject)
        GameObjectManager::getInstance().remove(obj);
}



/// <summary>
/// Returns list of enemies within radius units^2 from position. REMEMBER RADIUS IS SQUARED
/// </summary>
/// <param name="position"></param>
/// <param name="radius"></param>
/// <returns></returns>
std::vector<std::shared_ptr<GameObject>> EnemyManager::getInRange(sf::Vector2f& position, float radius) {
    std::vector<std::shared_ptr<GameObject>> inRange;
    for (auto& enemy : enemyObjects_) {
        if ((position - enemy->getPosition()).lengthSquared() < radius + enemy->getDerivativesOfComponent<Enemy>()->_size)
            inRange.push_back(enemy);
    }
    return inRange;
}


std::vector<std::shared_ptr<GameObject>> 
EnemyManager::getInRange(sf::FloatRect& rect) {
    std::vector<std::shared_ptr<GameObject>> inRange;
    return inRange;
}


std::shared_ptr<GameObject> EnemyManager::getFirstInRange(sf::Vector2f& position, float radius) {
    for (auto& enemy : enemyObjects_) {
        if ((position - enemy->getPosition()).lengthSquared() < radius + enemy->getDerivativesOfComponent<Enemy>()->_size)
            return enemy;
    }
    return nullptr;
}

/// <summary>
/// Get the closest enemy object to the given position.
/// </summary>
/// <param name="position"> position to check distances from.</param>
/// <param name="cutoffRadius"> maximum distance from position to check for. Keep in mind distance check uses square magnitude.</param>
/// <returns> Closest enemy GameObject to given position. Keep in mind if you set a cutoff radius this CAN return nullptr.</returns>
std::shared_ptr<GameObject> EnemyManager::getClosest(const sf::Vector2f& position, const float cutoffRadiusSqr = std::numeric_limits<float>::infinity()) {
    float closestDist = std::numeric_limits<float>::infinity();
    std::shared_ptr<GameObject> closestObj = nullptr;
    for (const auto& enemy : enemyObjects_) {
        // get distance (squared)
        const auto distance = (position - enemy->getPosition()).lengthSquared();
        if (distance >= cutoffRadiusSqr) continue; // if outside radius, next enemy.

        if (distance < closestDist) { // if closer than current closest, update.
            closestDist = distance;
            closestObj = enemy;
        }

    }

    return closestObj;
}