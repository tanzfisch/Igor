// Igor game engine
// (c) Copyright 2012-2022 by Martin Loga
// see copyright notice in corresponding header file

#include "GameLayer.h"

GameLayer::GameLayer(iWindowPtr window)
    : iLayer(window, L"GameLayer"), _quadtree(iaRectangled(0, 0, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT))
{
}

iaVector2d GameLayer::getRandomDir()
{
    iaVector2d direction(0.0, 1.0);
    direction.rotateXY(iaRandom::getNextFloat() * M_PI * 2.0);
    return direction;
}

iEntity GameLayer::createPlayer()
{
    // init player
    iEntity entity = _entityScene.createEntity("player");

    auto position = entity.addComponent<PositionComponent>(iaVector2d(PLAYFIELD_WIDTH * 0.5, PLAYFIELD_HEIGHT * 0.5));
    auto size = entity.addComponent<SizeComponent>(STANDARD_UNIT_SIZE * 1.5);
    entity.addComponent<OrientationComponent>(iaVector2d(0.0, -1.0), false);
    entity.addComponent<VelocityComponent>(iaVector2d(1.0, 0.0), 0.5, true);
    entity.addComponent<PartyComponent>(FRIEND);
    entity.addComponent<DamageComponent>(0.0);
    entity.addComponent<HealthComponent>(100.0);
    entity.addComponent<ExperienceComponent>(0.0, 1.0);
    entity.addComponent<CoinsComponent>(0.0);
    entity.addComponent<ModifierComponent>(1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
    entity.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("wagiuA5.png"), true, true, iaTime::fromSeconds(iaRandom::getNextFloat()), true);
    auto &weapon = entity.addComponent<WeaponComponent>(_weapons["Knife"]);

    entity.addComponent<TargetComponent>(iInvalidEntityID, false, false);
    entity.addComponent<MovementControlComponent>();

    auto &object = entity.addComponent<QuadtreeObjectComponent>();
    object._object = std::make_shared<iQuadtreeObjectd>(iaCircled(position._position._x, position._position._y, size._size * 0.5), entity.getID());
    _quadtree.insert(object._object);

    iaVector2f a(-1.0, 0.0);
    iaVector2f b(1.0, 0.0);
    iaVector2f c(0.0, 1.0);
    iaVector2f d(0.0, -1.0);

    return entity;
}

iEntity GameLayer::createViewport(iEntityID targetID)
{
    iEntity target(targetID, _entityScene);
    const iaVector2d targetPosition = target.getComponent<PositionComponent>()._position;

    iEntity entity = _entityScene.createEntity("viewport");

    auto &viewportComp = entity.addComponent<ViewportComponent>();
    viewportComp._targetOffset.set(0.0, 0.0);
    viewportComp._targetID = targetID;
    viewportComp._viewport.setSize(PLAYFIELD_VIEWPORT_WIDTH, PLAYFIELD_VIEWPORT_HEIGHT);
    viewportComp._viewport.setCenter(targetPosition);

    return entity;
}

void GameLayer::createObject(const iaVector2d &pos, uint32 party, ObjectType objectType)
{
    iEntity entity = _entityScene.createEntity("object");
    entity.addComponent<PositionComponent>(pos);
    entity.addComponent<OrientationComponent>(iaVector2d(0.0, -1.0), false);
    auto size = entity.addComponent<SizeComponent>(COIN_SIZE);
    entity.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("coin.png"), true, true, iaTime::fromSeconds(iaRandom::getNextFloat()), true);

    entity.addComponent<PickupComponent>(true);
    entity.addComponent<ExperienceGainComponent>(0.0);
    entity.addComponent<CoinGainComponent>(objectType == ObjectType::Coin ? 1.0 : 0.0);
    entity.addComponent<DamageComponent>(0.0);
    entity.addComponent<HealComponent>(0.0);

    entity.addComponent<PartyComponent>(party);

    auto &object = entity.addComponent<QuadtreeObjectComponent>();
    object._object = std::make_shared<iQuadtreeObjectd>();
    object._object->_userData = entity.getID();
    object._object->_circle.set(pos, size._size * 0.5);
    _quadtree.insert(object._object);
}

void GameLayer::liftShop()
{
    auto &object = _shop.getComponent<QuadtreeObjectComponent>();
    _quadtree.remove(object._object);
    auto &vis = _shop.getComponent<VisualComponent>();
    vis._visible = false;    
}

void GameLayer::onLandShop(const iaTime &time)
{
    landShop();
}

void GameLayer::onShopLanded()
{
    auto &pos = _shop.getComponent<PositionComponent>();
    auto &object = _shop.getComponent<QuadtreeObjectComponent>();
    object._object->_circle._center = pos._position;
    _quadtree.insert(object._object);
}

void GameLayer::landShop()
{
    auto &vis = _shop.getComponent<VisualComponent>();
    vis._visible = true;
    auto &pos = _shop.getComponent<PositionComponent>();
    pos._position.set(iaRandom::getNextFloat() * PLAYFIELD_WIDTH, iaRandom::getNextFloat() * PLAYFIELD_HEIGHT);

    onShopLanded();
}

void GameLayer::createShop()
{
    _shop = _entityScene.createEntity("shop");
    _shop.addComponent<PositionComponent>(iaVector2d());
    auto size = _shop.addComponent<SizeComponent>(STANDARD_UNIT_SIZE * 4);
    _shop.addComponent<OrientationComponent>(iaVector2d(0.0, -1.0), false);
    _shop.addComponent<VelocityComponent>(getRandomDir(), 0.0, false);
    _shop.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile("shop.png"), true, false, iaTime::fromSeconds(iaRandom::getNextFloat()), false);
    _shop.addComponent<BuildingComponent>(BuildingType::Shop);

    auto &object = _shop.addComponent<QuadtreeObjectComponent>();
    object._object = std::make_shared<iQuadtreeObjectd>();
    object._object->_userData = _shop.getID();
    object._object->_circle.set(iaVector2d(), size._size * 0.5);
}

void GameLayer::createUnit(const iaVector2d &pos, uint32 party, iEntityID target, const EnemyClass &enemyClass)
{
    static uint32 counter = 0;
    iEntity entity = _entityScene.createEntity(enemyClass._name + iaString::toString(counter));
    counter++;

    entity.addComponent<PositionComponent>(pos);
    entity.addComponent<OrientationComponent>(iaVector2d(0.0, -1.0), false);
    entity.addComponent<VelocityComponent>(getRandomDir(), enemyClass._speed, false);
    entity.addComponent<ExperienceGainComponent>(enemyClass._xpDrop);

    auto size = entity.addComponent<SizeComponent>(enemyClass._size);
    entity.addComponent<PartyComponent>(party);
    entity.addComponent<DamageComponent>(enemyClass._damage);
    entity.addComponent<HealthComponent>(enemyClass._health);
    entity.addComponent<VisualComponent>(
        iTextureResourceFactory::getInstance().requestFile(enemyClass._texture),
        true,
        true,
        iaTime::fromSeconds(iaRandom::getNextFloat()),
        true);

    entity.addComponent<TargetComponent>(target); // I don't like this but it's quick

    auto &object = entity.addComponent<QuadtreeObjectComponent>();
    object._object = std::make_shared<iQuadtreeObjectd>();
    object._object->_userData = entity.getID();
    object._object->_circle.set(pos, size._size * 0.5);

    auto entityID = entity.getID();
    uint32 id = *reinterpret_cast<uint32 *>(reinterpret_cast<void *>(&entityID));
    _quadtree.insert(object._object);
}

void GameLayer::updateViewRectangleSystem()
{
    if (!_player.isValid())
    {
        return;
    }

    auto &viewportComp = _viewport.getComponent<ViewportComponent>();
    auto &targetOffset = viewportComp._targetOffset;
    const iaVector2d &playerPosition = _player.getComponent<PositionComponent>()._position;
    const iaVector2d lastPlayerPosition = viewportComp._viewport.getCenter() + targetOffset;
    const iaVector2d diff = playerPosition - lastPlayerPosition;

    const auto width = PLAYFIELD_VIEWPORT_MOVE_EDGE_WIDTH * 0.5;
    const auto height = PLAYFIELD_VIEWPORT_MOVE_EDGE_HEIGHT * 0.5;

    bool skipStep = false;

    if (std::abs(diff._x) > width ||
        std::abs(diff._y) > height)
    {
        skipStep = true;
    }

    if (!skipStep)
    {
        targetOffset += diff;
    }

    targetOffset._x = std::clamp(targetOffset._x, -width, width);
    targetOffset._y = std::clamp(targetOffset._y, -height, height);

    viewportComp._viewport.setCenter(playerPosition - targetOffset);
}

void GameLayer::onInit()
{
    iaRandom::setSeed(iaTime::getNow().getMicrosenconds());

    initExpLvlTable();
    loadSpecs("meta/supremacy.xml");

    _viewOrtho.setClearColor(0.3, 0.9, 0.5, 1.0);
    _viewOrtho.setName("view ortho");
    _viewOrtho.setClearColorActive(true);
    _viewOrtho.setClearDepthActive(false);
    _viewOrtho.setOrthogonal(0.0, static_cast<float32>(getWindow()->getClientWidth()), static_cast<float32>(getWindow()->getClientHeight()), 0.0);
    _viewOrtho.registerRenderDelegate(iDrawDelegate(this, &GameLayer::onRenderOrtho));
    getWindow()->addView(&_viewOrtho, getZIndex() + 1);

    _taskFlushTextures = iTaskManager::getInstance().addTask(new iTaskFlushTextures(getWindow()));

    _player = createPlayer();
    _viewport = createViewport(_player.getID());

    // game logic timer
    _updateTimerHandle = new iTimerHandle(iTimerTickDelegate(this, &GameLayer::onUpdate), iaTime::fromMilliseconds(10));
    _updateTimerHandle->start();

    _statsTimerHandle = new iTimerHandle(iTimerTickDelegate(this, &GameLayer::onUpdateStats), iaTime::fromMilliseconds(1000));
    _statsTimerHandle->start();

    _spawnUnitsTimerHandle = new iTimerHandle(iTimerTickDelegate(this, &GameLayer::onSpawnStuff), iaTime::fromMilliseconds(5000));
    _spawnUnitsTimerHandle->start();

    // run once right now so we have a few enemies right away
    _spawnUnitsTimerHandle->triggerNow();

    createShop();

    _spawnShopTimerHandle = new iTimerHandle(iTimerTickDelegate(this, &GameLayer::onLandShop), iaTime::fromMilliseconds(5000), true);
    _spawnShopTimerHandle->start();

    _backgroundTexture = iTextureResourceFactory::getInstance().loadFile("background.png");
    _shadow = iTextureResourceFactory::getInstance().requestFile("shadow.png");
    _shield = iTextureResourceFactory::getInstance().requestFile("shield.png");
    _rage = iTextureResourceFactory::getInstance().requestFile("rage.png");

    // init font for render profiler
    _font = iTextureFont::create("StandardFontOutlined.png");

    _coin = iTextureResourceFactory::getInstance().requestFile("coin.png");
    _damage = iTextureResourceFactory::getInstance().requestFile("fist.png");
    _attackSpeed = iTextureResourceFactory::getInstance().requestFile("bullets.png");
    _walkSpeed = iTextureResourceFactory::getInstance().requestFile("run.png");

    _levelUpDialog = new UpgradeDialog();
    _shopDialog = new ShopDialog();
}

void GameLayer::readEnemies(TiXmlElement *enemies)
{
    TiXmlElement *enemy = enemies->FirstChildElement("Enemy");

    do
    {
        iaString name(enemy->Attribute("name"));
        iaString description(enemy->Attribute("description"));
        iaString texture(enemy->Attribute("texture"));

        float64 damage = 0.0;
        enemy->Attribute("damage", &damage);
        float64 health = 0.0;
        enemy->Attribute("health", &health);
        float64 size = 1.0;
        enemy->Attribute("size", &size);
        float64 speed = 1.0;
        enemy->Attribute("speed", &speed);
        float64 xpDrop = 0.0;
        enemy->Attribute("xpDrop", &xpDrop);
        float64 coinDrop = 0.0;
        enemy->Attribute("coinDrop", &coinDrop);

        _enemies.push_back({name, description, texture, damage, health, size, speed, xpDrop, coinDrop});

    } while ((enemy = enemy->NextSiblingElement("Enemy")) != nullptr);
}

void GameLayer::readUpgrades(TiXmlElement *upgrades)
{
    TiXmlElement *upgrade = upgrades->FirstChildElement("Upgrade");

    do
    {
        iaString name(upgrade->Attribute("name"));
        iaString description(upgrade->Attribute("description"));
        iaString icon(upgrade->Attribute("icon"));

        float64 damageFactor = 0.0;
        upgrade->Attribute("damageFactor", &damageFactor);
        float64 attackIntervalFactor = 0.0;
        upgrade->Attribute("attackIntervalFactor", &attackIntervalFactor);
        float64 criticalHitChanceFactor = 0.0;
        upgrade->Attribute("criticalHitChanceFactor", &criticalHitChanceFactor);
        float64 criticalHitDamageFactor = 0.0;
        upgrade->Attribute("criticalHitDamageFactor", &criticalHitDamageFactor);
        float64 splashDamageRangeFactor = 0.0;
        upgrade->Attribute("splashDamageRangeFactor", &splashDamageRangeFactor);
        float64 walkSpeedFactor = 0.0;
        upgrade->Attribute("walkSpeedFactor", &walkSpeedFactor);
        float64 projectileSpeedFactor = 0.0;
        upgrade->Attribute("projectileSpeedFactor", &projectileSpeedFactor);

        _upgrades.push_back({name, description, icon, damageFactor, attackIntervalFactor, criticalHitChanceFactor,
                             criticalHitDamageFactor, splashDamageRangeFactor, walkSpeedFactor, projectileSpeedFactor});
    } while ((upgrade = upgrade->NextSiblingElement("Upgrade")) != nullptr);
}

void GameLayer::readShopItems(TiXmlElement *shopItems)
{
    TiXmlElement *shopItem = shopItems->FirstChildElement("ShopItem");

    do
    {
        iaString name(shopItem->Attribute("name"));
        iaString description(shopItem->Attribute("description"));
        iaString icon(shopItem->Attribute("icon"));
        int price = 0;
        shopItem->Attribute("price", &price);

        ShopItemType type = ShopItemType::None;

        TiXmlElement *weapon = shopItem->FirstChildElement("Weapon");
        if (weapon != nullptr)
        {
            iaString projectileTexture(weapon->Attribute("projectileTexture"));
            float64 projectileSize = 1.0;
            weapon->Attribute("projectileSize", &projectileSize);
            int projectileCount = 1;
            weapon->Attribute("projectileCount", &projectileCount);
            float64 accuracy = 0.0;
            weapon->Attribute("accuracy", &accuracy);
            float64 angularVelocity = 0.0;
            weapon->Attribute("angularVelocity", &angularVelocity);
            float64 projectileDamage = 0.0;
            weapon->Attribute("projectileDamage", &projectileDamage);
            float64 projectileSpeed = 0.0;
            weapon->Attribute("projectileSpeed", &projectileSpeed);
            float64 projectileRange = 0.0;
            weapon->Attribute("projectileRange", &projectileRange);
            float64 attackIntervall = 0.0;
            weapon->Attribute("attackIntervall", &attackIntervall);

            iaString temp(weapon->Attribute("needToStandStill"));
            bool needToStandStill = temp == "true" ? true : false;
            iaString temp2(weapon->Attribute("returnToSender"));
            bool returnToSender = temp2 == "true" ? true : false;

            type = ShopItemType::Weapon;

            WeaponComponent weaponComponent =
                {
                    projectileTexture,
                    projectileSize,
                    (uint32)projectileCount,
                    accuracy,
                    angularVelocity,
                    projectileDamage,
                    projectileSpeed,
                    projectileRange,
                    iaTime::fromMilliseconds(attackIntervall),
                    needToStandStill,
                    returnToSender,
                    iaVector2d(0.0, -STANDARD_UNIT_SIZE * 0.5),
                    iTimer::getInstance().getTime(),
                };

            con_endl(name << " " << projectileDamage / attackIntervall);

            _weapons[name] = weaponComponent;
        }

        _shopItems.push_back({name, description, icon, type, (uint32)price});

    } while ((shopItem = shopItem->NextSiblingElement("ShopItem")) != nullptr);
}

void GameLayer::loadSpecs(const iaString &filename)
{
    con_assert_sticky(!filename.isEmpty(), "empty filename");

    iaString path = iResourceManager::getInstance().getPath(filename);

    char temp[2048];
    path.getData(temp, 2048);

    TiXmlDocument document(temp);
    if (!document.LoadFile())
    {
        con_err("can't read \"" << filename << "\"");
        return;
    }

    TiXmlElement *root = document.FirstChildElement("Supremacy");
    if (root != nullptr)
    {
        TiXmlElement *upgrades = root->FirstChildElement("Upgrades");
        if (upgrades != nullptr)
        {
            readUpgrades(upgrades);
        }

        TiXmlElement *shopItems = root->FirstChildElement("ShopItems");
        if (shopItems != nullptr)
        {
            readShopItems(shopItems);
        }

        TiXmlElement *enemies = root->FirstChildElement("Enemies");
        if (enemies != nullptr)
        {
            readEnemies(enemies);
        }
    }
}

void GameLayer::initExpLvlTable()
{
    float64 xp = 100.0;
    for (int x = 1; x < 100; ++x)
    {
        _expLvl.push_back((uint32)xp);

        if (x < 5)
        {
            xp *= 2.0;
        }
        else if (x < 10)
        {
            xp *= 1.5;
        }
        else
        {
            xp *= 1.1341;
        }
    }

    _expLvl.push_back(1000000000);
}


void GameLayer::onSpawnStuff(const iaTime &time)
{
    if (!_player.isValid())
    {
        return;
    }

    PositionComponent &playerPosition = _player.getComponent<PositionComponent>();
    ExperienceComponent &playerXP = _player.getComponent<ExperienceComponent>();

    // scaling all available enemies across 100 player levels
    const int32 playerLevel = playerXP._level; //  * (_enemies.size() / 50.0);
    const int32 maxEnemyLevel = std::max(1, std::min(playerLevel, static_cast<int32>(_enemies.size())));
    const int32 minEnemyLevel = std::max(1, maxEnemyLevel - 4);

    if (!_stats.empty())
    {
        if (_stats.back()._enemyCount > 40)
        {
            return;
        }
    }

    uint32 enemiesToCreate = 3 + (playerLevel * 0.2);

    con_endl("enemiesToCreate " << enemiesToCreate);

    for (int i = 0; i < enemiesToCreate; ++i)
    {
        iaVector2d pos(iaRandom::getNextFloat() - 0.5, iaRandom::getNextFloat() - 0.5);
        pos.normalize();
        pos *= PLAYFIELD_VIEWPORT_WIDTH * 0.5 + iaRandom::getNextFloat() * 100.0;
        pos += playerPosition._position;

        if (pos._x < 0)
        {
            pos._x += PLAYFIELD_WIDTH;
        }

        if (pos._x > PLAYFIELD_WIDTH)
        {
            pos._x -= PLAYFIELD_WIDTH;
        }

        if (pos._y < 0)
        {
            pos._y += PLAYFIELD_HEIGHT;
        }

        if (pos._y > PLAYFIELD_HEIGHT)
        {
            pos._y -= PLAYFIELD_HEIGHT;
        }

        uint32 enemyLevel = iaRandom::getNextRangeExponentialDecrease(minEnemyLevel, maxEnemyLevel, 0.6);
        con_endl("create enemy level: " << enemyLevel);
        createUnit(pos, FOE, _player.getID(), _enemies[enemyLevel - 1]);
    }

    con_endl("minEnemyLevel " << minEnemyLevel);
    con_endl("maxEnemyLevel " << maxEnemyLevel);
}

void GameLayer::onUpdateStats(const iaTime &time)
{
    if (!_player.isValid())
    {
        return;
    }

    GameStats stats;

    auto view = _entityScene.getEntities<HealthComponent, PartyComponent>();
    for (auto entityID : view)
    {
        auto [health, party] = view.get<HealthComponent, PartyComponent>(entityID);

        if (party._partyID == FOE)
        {
            stats._enemyHealth += health._health;
            stats._enemyCount++;
        }
    }

    const auto &weapon = _player.getComponent<WeaponComponent>();

    const ModifierComponent &modifierComponent = _player.getComponent<ModifierComponent>();

    stats._playerDamage = (weapon._damage * modifierComponent._damageFactor) / (weapon._attackInterval.getSeconds() / modifierComponent._attackIntervalFactor);
    const auto &experience = _player.getComponent<ExperienceComponent>();
    stats._playerExperience = experience._experience;
    stats._playerLevel = experience._level;
    const auto &coins = _player.getComponent<CoinsComponent>();
    stats._playerCoins = coins._coins;

    _stats.push_back(stats);
}

void GameLayer::onUpdateQuadtreeSystem()
{
    // update quadtree data
    auto quadtreeView = _entityScene.getEntities<PositionComponent, QuadtreeObjectComponent>();
    for (auto entityID : quadtreeView)
    {
        auto [pos, object] = quadtreeView.get<PositionComponent, QuadtreeObjectComponent>(entityID);
        if (object._object == nullptr)
        {
            continue;
        }

        if (object._object->_parent.expired())
        {
            continue;
        }

        iaVector2d newPosition(pos._position._x, pos._position._y);
        _quadtree.update(object._object, newPosition);
    }
}

void GameLayer::onUpdateMovementControlSystem()
{
    auto movementControlView = _entityScene.getEntities<MovementControlComponent, VelocityComponent>();
    for (auto entity : movementControlView)
    {
        auto [movementControl, vel] = movementControlView.get<MovementControlComponent, VelocityComponent>(entity);

        vel._direction.set(0, 0);

        if (movementControl._left)
        {
            vel._direction._x -= 1.0;
        }
        if (movementControl._right)
        {
            vel._direction._x += 1.0;
        }
        if (movementControl._up)
        {
            vel._direction._y -= 1.0;
        }
        if (movementControl._down)
        {
            vel._direction._y += 1.0;
        }
    }
}

void GameLayer::doughnutQuery(const iaCircled &circle, std::vector<std::pair<iEntityID, iaVector2d>> &hits)
{
    iQuadtreeObjectsd objects;
    _quadtree.query(circle, objects);

    for (const auto &object : objects)
    {
        hits.emplace_back(std::pair<iEntityID, iaVector2d>(std::any_cast<iEntityID>(object->_userData), object->_circle._center - circle._center));
    }

    const bool right = circle._center._x - circle._radius < 0.0;
    const bool left = circle._center._x + circle._radius > _quadtree.getRootBox()._width;
    const bool bottom = circle._center._y - circle._radius < 0.0;
    const bool top = circle._center._y + circle._radius > _quadtree.getRootBox()._height;

    std::vector<iaCircled> additionalQueries;

    if (right || left)
    {
        iaCircled queryCircle = circle;
        if (right)
        {
            queryCircle._center._x += _quadtree.getRootBox()._width;
        }
        if (left)
        {
            queryCircle._center._x -= _quadtree.getRootBox()._width;
        }
        additionalQueries.push_back(queryCircle);
    }

    if (bottom || top)
    {
        iaCircled queryCircle = circle;
        if (bottom)
        {
            queryCircle._center._y += _quadtree.getRootBox()._height;
        }
        if (top)
        {
            queryCircle._center._y -= _quadtree.getRootBox()._height;
        }
        additionalQueries.push_back(queryCircle);
    }

    if (right || left && bottom || top)
    {
        iaCircled queryCircle = circle;
        if (right)
        {
            queryCircle._center._x += _quadtree.getRootBox()._width;
        }
        if (left)
        {
            queryCircle._center._x -= _quadtree.getRootBox()._width;
        }
        if (bottom)
        {
            queryCircle._center._y += _quadtree.getRootBox()._height;
        }
        if (top)
        {
            queryCircle._center._y -= _quadtree.getRootBox()._height;
        }
        additionalQueries.push_back(queryCircle);
    }

    for (const auto &queryCircle : additionalQueries)
    {
        _quadtree.query(queryCircle, objects);

        for (const auto &object : objects)
        {
            hits.emplace_back(std::pair<iEntityID, iaVector2d>(std::any_cast<iEntityID>(object->_userData), object->_circle._center - queryCircle._center));
        }
    }
}

bool GameLayer::intersectDoughnut(const iaVector2d &position, const iaRectangled &rectangle, iaVector2d &offset)
{
    if (iIntersection::intersects(position, rectangle))
    {
        return true;
    }

    const bool right = rectangle._x < 0.0;
    const bool left = rectangle.getRight() > _quadtree.getRootBox()._width;

    const bool bottom = rectangle._y < 0.0;
    const bool top = rectangle.getBottom() > _quadtree.getRootBox()._height;

    std::vector<iaRectangled> additionalQueries;

    if (right || left)
    {
        iaRectangled queryRectangle = rectangle;
        if (right)
        {
            queryRectangle._x += _quadtree.getRootBox()._width;
        }
        if (left)
        {
            queryRectangle._x -= _quadtree.getRootBox()._width;
        }
        additionalQueries.push_back(queryRectangle);
    }

    if (bottom || top)
    {
        iaRectangled queryRectangle = rectangle;
        if (bottom)
        {
            queryRectangle._y += _quadtree.getRootBox()._height;
        }
        if (top)
        {
            queryRectangle._y -= _quadtree.getRootBox()._height;
        }
        additionalQueries.push_back(queryRectangle);
    }

    if (right || left && bottom || top)
    {
        iaRectangled queryRectangle = rectangle;
        if (right)
        {
            queryRectangle._x += _quadtree.getRootBox()._width;
        }
        if (left)
        {
            queryRectangle._x -= _quadtree.getRootBox()._width;
        }
        if (bottom)
        {
            queryRectangle._y += _quadtree.getRootBox()._height;
        }
        if (top)
        {
            queryRectangle._y -= _quadtree.getRootBox()._height;
        }
        additionalQueries.push_back(queryRectangle);
    }

    for (const auto &queryRectangle : additionalQueries)
    {
        if (iIntersection::intersects(position, queryRectangle))
        {
            offset = rectangle.getTopLeft() - queryRectangle.getTopLeft();
            return true;
        }
    }

    return false;
}

bool GameLayer::intersectDoughnut(const iaVector2d &position, const iaCircled &circle, iaVector2d &offset)
{
    if (iIntersection::intersects(position, circle))
    {
        return true;
    }

    const bool right = circle._center._x - circle._radius < 0.0;
    const bool left = circle._center._x + circle._radius > _quadtree.getRootBox()._width;

    const bool bottom = circle._center._y - circle._radius < 0.0;
    const bool top = circle._center._y + circle._radius > _quadtree.getRootBox()._height;

    std::vector<iaCircled> additionalQueries;

    if (right || left)
    {
        iaCircled queryCircle = circle;
        if (right)
        {
            queryCircle._center._x += _quadtree.getRootBox()._width;
        }
        if (left)
        {
            queryCircle._center._x -= _quadtree.getRootBox()._width;
        }
        additionalQueries.push_back(queryCircle);
    }

    if (bottom || top)
    {
        iaCircled queryCircle = circle;
        if (bottom)
        {
            queryCircle._center._y += _quadtree.getRootBox()._height;
        }
        if (top)
        {
            queryCircle._center._y -= _quadtree.getRootBox()._height;
        }
        additionalQueries.push_back(queryCircle);
    }

    if (right || left && bottom || top)
    {
        iaCircled queryCircle = circle;
        if (right)
        {
            queryCircle._center._x += _quadtree.getRootBox()._width;
        }
        if (left)
        {
            queryCircle._center._x -= _quadtree.getRootBox()._width;
        }
        if (bottom)
        {
            queryCircle._center._y += _quadtree.getRootBox()._height;
        }
        if (top)
        {
            queryCircle._center._y -= _quadtree.getRootBox()._height;
        }
        additionalQueries.push_back(queryCircle);
    }

    for (const auto &queryCircle : additionalQueries)
    {
        if (iIntersection::intersects(position, queryCircle))
        {
            offset = circle._center - queryCircle._center;
            return true;
        }
    }

    return false;
}

void GameLayer::onUpdateFollowTargetSystem()
{
    // follow given target
    auto targetView = _entityScene.getEntities<PositionComponent, VelocityComponent, TargetComponent>();
    for (auto entity : targetView)
    {
        auto &target = targetView.get<TargetComponent>(entity);

        // skip if entity is not following targets
        if (!target._followTarget)
        {
            continue;
        }

        // skip if there is no valid target
        iEntity targetEntity(target._targetID, _entityScene);
        if (!targetEntity.isValid())
        {
            continue;
        }

        // skip if farget has no position
        if (!targetEntity.hasComponent<PositionComponent>())
        {
            continue;
        }

        auto [pos, vel] = targetView.get<PositionComponent, VelocityComponent>(entity);

        auto targetPosition = targetEntity.getComponent<PositionComponent>();
        const iaVector2d &targetPos = targetPosition._position;
        const iaVector2d &position = pos._position;
        iaVector2d offset;
        iaCircled circle(position, 1000.0);

        if (!intersectDoughnut(targetPos, circle, offset))
        {
            if (target._inRange)
            {
                vel._direction = getRandomDir();
                target._inRange = false;
            }
        }
        else
        {
            vel._direction = targetPos - position + offset;
            vel._direction.normalize();
            target._inRange = true;
        }
    }
}

void GameLayer::onUpdatePositionSystem()
{
    auto positionUpdateView = _entityScene.getEntities<PositionComponent, SizeComponent, VelocityComponent, PartyComponent, DamageComponent, HealthComponent>();
    for (auto entityID : positionUpdateView)
    {
        auto [pos, size, vel, party, damage, health] = positionUpdateView.get<PositionComponent, SizeComponent, VelocityComponent, PartyComponent, DamageComponent, HealthComponent>(entityID);

        iaVector2d &position = pos._position;
        const float64 radius = size._size * 0.5;
        float64 speed = vel._speed;
        iaVector2d diversion;
        iaVector2d direction = vel._direction;
        iEntity entity(entityID, _entityScene);

        ModifierComponent *modifierComponent = entity.tryGetComponent<ModifierComponent>();
        if (modifierComponent != nullptr)
        {
            speed *= modifierComponent->_walkSpeedFactor;
        }

        auto *range = entity.tryGetComponent<RangeComponent>();

        // don't calculate diversion if non blockable
        if (!vel._nonBlockable)
        {
            iaCircled circle(position, radius * 1.1);
            iQuadtreeObjectsd objects;
            _quadtree.query(circle, objects);

            uint32 totalHits = 0;

            for (const auto &object : objects)
            {
                const iEntityID otherEntityID = std::any_cast<iEntityID>(object->_userData);

                // skip self
                if (otherEntityID == entityID)
                {
                    continue;
                }

                // get other entity
                iEntity otherEntity(otherEntityID, _entityScene);

                // ignore other entity for diversion if non blockable
                auto *otherEntityVel = otherEntity.tryGetComponent<VelocityComponent>();
                if (otherEntityVel == nullptr || otherEntityVel->_nonBlockable)
                {
                    continue;
                }

                // calc diversion
                diversion += position - object->_circle._center;
                totalHits++;
            }

            if (totalHits)
            {
                diversion.normalize();
                diversion *= speed;
            }
        }

        direction *= speed;
        if (range != nullptr)
        {
            range->_distanceTraveled += diversion.length();
            range->_distanceTraveled += direction.length();
        }
        position += diversion;
        position += direction;

        // jump to other side
        if (position._x > PLAYFIELD_WIDTH)
        {
            position._x -= PLAYFIELD_WIDTH;
        }
        if (position._x < 0)
        {
            position._x += PLAYFIELD_WIDTH;
        }

        if (position._y > PLAYFIELD_HEIGHT)
        {
            position._y -= PLAYFIELD_HEIGHT;
        }
        if (position._y < 0)
        {
            position._y += PLAYFIELD_HEIGHT;
        }
    }
}

void GameLayer::onUpdateCollisionSystem()
{
    auto positionUpdateView = _entityScene.getEntities<PositionComponent, SizeComponent, PartyComponent, DamageComponent, HealthComponent>();
    for (auto entityID : positionUpdateView)
    {
        auto [pos, size, party, damage, health] = positionUpdateView.get<PositionComponent, SizeComponent, PartyComponent, DamageComponent, HealthComponent>(entityID);

        iaVector2d &position = pos._position;
        const float64 radius = size._size * 0.5;

        iaCircled circle(position, radius);
        iQuadtreeObjectsd objects;
        _quadtree.query(circle, objects);

        iEntity entity(entityID, _entityScene);

        for (const auto &object : objects)
        {
            const iEntityID otherEntityID = std::any_cast<iEntityID>(object->_userData);

            // skip self
            if (otherEntityID == entityID)
            {
                continue;
            }

            // get other entity
            iEntity otherEntity(otherEntityID, _entityScene);

            // check if we do damage to other entity
            auto *otherEntityParty = otherEntity.tryGetComponent<PartyComponent>();
            if (otherEntityParty != nullptr)
            {
                if (otherEntityParty->_partyID != party._partyID)
                {
                    auto *otherEntityHealth = otherEntity.tryGetComponent<HealthComponent>();
                    if (otherEntityHealth != nullptr)
                    {
                        otherEntityHealth->_health -= damage._damage;
                    }

                    if (otherEntityParty->_partyID != NEUTRAL &&
                        health._destroyOnImpact)
                    {
                        health._health = 0.0;
                    }
                }
            }
        }
    }
}

void GameLayer::fire(const iaVector2d &from, const iaVector2d &dir, uint32 party, const WeaponComponent &weapon, const ModifierComponent &modifier)
{
    // skip if out of range
    if (!iIntersection::intersects(from, _quadtree.getRootBox()))
    {
        return;
    }

    for (int i = 0; i < weapon._projectileCount; ++i)
    {
        auto bullet = _entityScene.createEntity(weapon._texture);
        bullet.addComponent<PositionComponent>(from + dir * weapon._size * 0.5);

        float32 angularVelocity = weapon._angularVelocity;

        if (angularVelocity != 0.0)
        {
            angularVelocity += (iaRandom::getNextFloat() - 0.5f) * 0.2;
        }

        bullet.addComponent<AngularVelocityComponent>(angularVelocity);
        bullet.addComponent<RangeComponent>(weapon._range);

        iaVector2d d = dir;
        d.rotateXY((iaRandom::getNextFloat() - 0.2) * weapon._accuracy * 0.2);
        float64 s = (weapon._speed * modifier._projectileSpeedFactor) + (weapon._accuracy * (iaRandom::getNextFloat() - 0.5));
        bullet.addComponent<VelocityComponent>(d, s, true);

        bullet.addComponent<OrientationComponent>(d, weapon._angularVelocity == 0.0);

        bullet.addComponent<PartyComponent>(party);
        bullet.addComponent<DamageComponent>(weapon._damage * modifier._damageFactor);
        bullet.addComponent<HealthComponent>(100.0, true);
        bullet.addComponent<SizeComponent>(weapon._size);
        bullet.addComponent<VisualComponent>(iTextureResourceFactory::getInstance().requestFile(weapon._texture), false, false, iaTime::fromSeconds(iaRandom::getNextFloat()), true);

        auto &object = bullet.addComponent<QuadtreeObjectComponent>();
        object._object = std::make_shared<iQuadtreeObjectd>(iaCircled(from, weapon._size * 0.5), bullet.getID());
        object._object->_userData = bullet.getID();
        object._object->_circle.set(from, weapon._size * 0.5);
        _quadtree.insert(object._object);
    }
}

void GameLayer::addExperience(iEntity &entity, float64 experience)
{
    auto &comp = entity.getComponent<ExperienceComponent>();

    comp._experience += experience;
    uint32 level = comp._level;
    comp._level = calcLevel(comp._experience);

    if (level < uint32(comp._level))
    {
        _levelUp = true;
    }
}

BuildingType GameLayer::onCheckForBuildingsNearBy(iEntity &entity)
{
    if (!entity.isValid())
    {
        return BuildingType::None;
    }

    auto &entitySize = entity.getComponent<SizeComponent>();
    auto &entityPosition = entity.getComponent<PositionComponent>();

    auto view = _entityScene.getEntities<BuildingComponent, PositionComponent, SizeComponent>();
    for (auto entityID : view)
    {
        auto [building, pos, size] = view.get<BuildingComponent, PositionComponent, SizeComponent>(entityID);

        const float64 distance = pos._position.distance(entityPosition._position);
        if (distance < entitySize._size + size._size)
        {
            return building._type;
        }
    }

    return BuildingType::None;
}

void GameLayer::onUpdatePickupSystem(iEntity &entity)
{
    if (!entity.isValid())
    {
        return;
    }

    // aquire target for player
    auto &position = entity.getComponent<PositionComponent>();
    auto &experience = entity.getComponent<ExperienceComponent>();
    auto &coins = entity.getComponent<CoinsComponent>();
    auto &health = entity.getComponent<HealthComponent>();

    iaCircled circle(position._position, 70.0);
    std::vector<std::pair<iEntityID, iaVector2d>> hits;
    doughnutQuery(circle, hits);

    for (const auto &hit : hits)
    {
        if (hit.first == entity.getID())
        {
            continue;
        }

        iEntity otherEntity(hit.first, _entityScene);

        auto *pickup = otherEntity.tryGetComponent<PickupComponent>();
        if (pickup == nullptr)
        {
            continue;
        }

        auto &expGain = otherEntity.getComponent<ExperienceGainComponent>();
        addExperience(entity, expGain._experience);

        auto &coinGain = otherEntity.getComponent<CoinGainComponent>();
        coins._coins += coinGain._coins;

        auto &damage = otherEntity.getComponent<DamageComponent>();
        health._health -= damage._damage;

        auto &heal = otherEntity.getComponent<HealComponent>();
        health._health += heal._heal;

        _deleteQueue.insert(hit.first);
    }
}

void GameLayer::aquireTargetFor(iEntity &entity)
{
    if (!entity.isValid())
    {
        return;
    }

    // aquire target for player
    auto &position = entity.getComponent<PositionComponent>();
    auto &party = entity.getComponent<PartyComponent>();
    auto &target = entity.getComponent<TargetComponent>();
    auto &weapon = entity.getComponent<WeaponComponent>();

    iaCircled circle(position._position, weapon._range * 1.1f);
    std::vector<std::pair<iEntityID, iaVector2d>> hits;
    doughnutQuery(circle, hits);

    float32 minDistance = std::numeric_limits<float32>::max();
    target._targetID = iInvalidEntityID;

    for (const auto &hit : hits)
    {
        if (hit.first == entity.getID())
        {
            continue;
        }

        iEntity entity(hit.first, _entityScene);

        auto *entParty = entity.tryGetComponent<PartyComponent>();
        if (entParty == nullptr ||
            entParty->_partyID == NEUTRAL ||
            entParty->_partyID == party._partyID)
        {
            continue;
        }

        const float32 dist = hit.second.length2();
        if (dist < minDistance)
        {
            minDistance = dist;
            target._targetID = hit.first;
        }
    }
}

void GameLayer::onUpdateWeaponSystem()
{
    iaTime now = iTimer::getInstance().getTime();

    auto view = _entityScene.getEntities<WeaponComponent, TargetComponent, PositionComponent, VelocityComponent, ModifierComponent>();
    for (auto entityID : view)
    {
        auto [weapon, target, position, velocity, modifier] = view.get<WeaponComponent, TargetComponent, PositionComponent, VelocityComponent, ModifierComponent>(entityID);

        // skip if there is no target
        if (target._targetID == iInvalidEntityID)
        {
            continue;
        }

        // check if unit needs to stand still
        if (weapon._standStillToFire && velocity._direction.length() * velocity._speed > 0.0)
        {
            continue;
        }

        // check if it is time to fire again
        if ((now - weapon._time) < (weapon._attackInterval / modifier._attackIntervalFactor))
        {
            continue;
        }
        weapon._time = now;

        // get target position
        iEntity targetEntity(target._targetID, _entityScene);
        auto &targetPosition = targetEntity.getComponent<PositionComponent>();

        const iaVector2d firePos = position._position + weapon._offset;
        iaVector2d direction = targetPosition._position - firePos;
        direction.normalize();

        fire(firePos, direction, FRIEND, weapon, modifier);
    }
}

void GameLayer::onUpdateRangeSystem()
{
    auto view = _entityScene.getEntities<PositionComponent, RangeComponent, HealthComponent>();
    for (auto entity : view)
    {
        auto [positiom, range, health] = view.get<PositionComponent, RangeComponent, HealthComponent>(entity);

        if (range._distanceTraveled > range._maxRange)
        {
            health._health = 0.0;
        }
    }
}

void GameLayer::onUpdateOrientationSystem()
{
    auto view = _entityScene.getEntities<AngularVelocityComponent, VelocityComponent, OrientationComponent>();
    for (auto entity : view)
    {
        auto [angularVel, vel, ori] = view.get<AngularVelocityComponent, VelocityComponent, OrientationComponent>(entity);

        if (ori.followVelocity)
        {
            ori._direction = vel._direction;
        }
        else
        {
            ori._direction.rotateXY(angularVel._velocity);
        }
    }
}

void GameLayer::onUpdateCleanUpTheDeadSystem()
{
    bool playerValid = _player.isValid();

    // pick up the dead
    auto healthView = _entityScene.getEntities<HealthComponent, PartyComponent, QuadtreeObjectComponent>();

    for (auto entityID : healthView)
    {
        auto [health, party, object] = healthView.get<HealthComponent, PartyComponent, QuadtreeObjectComponent>(entityID);

        if (health._health <= 0.0)
        {
            if (party._partyID == FOE)
            {
                iEntity entity(entityID, _entityScene);
                const auto *exp = entity.tryGetComponent<ExperienceGainComponent>();
                if (exp != nullptr)
                {
                    if (playerValid)
                    {
                        addExperience(_player, exp->_experience);
                    }
                }

                const auto *pos = entity.tryGetComponent<PositionComponent>();
                if (pos != nullptr)
                {
                    createObject(pos->_position, NEUTRAL, ObjectType::Coin);
                }
            }

            _deleteQueue.insert(entityID);
        }
    }

    // dispose the dead
    for (const auto &entityID : _deleteQueue)
    {
        iEntity entity(entityID, _entityScene);
        auto *qud = entity.tryGetComponent<QuadtreeObjectComponent>();
        if (qud != nullptr)
        {
            _quadtree.remove(qud->_object);
            qud->_object = nullptr;
        }

        _entityScene.destroyEntity(entityID);
    }

    _deleteQueue.clear();
}

void GameLayer::onUpdate(const iaTime &time)
{
    onUpdateQuadtreeSystem();
    onUpdateMovementControlSystem();
    onUpdateFollowTargetSystem();
    onUpdateOrientationSystem();
    onUpdatePositionSystem();
    onUpdateCollisionSystem();

    aquireTargetFor(_player);
    onUpdatePickupSystem(_player);
    const BuildingType buildingType = onCheckForBuildingsNearBy(_player);

    onUpdateWeaponSystem();
    onUpdateRangeSystem();
    onUpdateCleanUpTheDeadSystem();
    updateViewRectangleSystem();

    onOpenBuilding(buildingType);

    if (_levelUp)
    {
        onLevelUp();
        _levelUp = false;
    }
}

void GameLayer::onDeinit()
{
    // release resources
    _font = nullptr;

    // stop timer
    if (_updateTimerHandle != nullptr)
    {
        delete _updateTimerHandle;
        _updateTimerHandle = nullptr;
    }

    // clean up window
    getWindow()->removeView(&_viewOrtho);
    _viewOrtho.unregisterRenderDelegate(iDrawDelegate(this, &GameLayer::onRenderOrtho));
}

void GameLayer::onPreDraw()
{
}

void GameLayer::onEvent(iEvent &event)
{
    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(GameLayer::onKeyDown));
    event.dispatch<iEventKeyUp>(IGOR_BIND_EVENT_FUNCTION(GameLayer::onKeyUp));
}

void GameLayer::onRenderStats()
{
    if (_stats.empty())
    {
        return;
    }

    iaMatrixd matrix;
    matrix.translate(0, 0, -1);
    iRenderer::getInstance().setModelMatrix(matrix);

    std::vector<iaVector3f> playerDamage;
    std::vector<iaVector3f> playerExperience;
    std::vector<iaVector3f> playerLevel;
    std::vector<iaVector3f> enemyHealth;
    std::vector<iaVector3f> enemyCount;

    float32 x = 10.0f;
    float32 y = getWindow()->getClientHeight() - 100 - 100;

    for (const auto &data : _stats)
    {
        playerDamage.push_back(iaVector3f(x, y - data._playerDamage * 0.1, 0.0));
        playerExperience.push_back(iaVector3f(x, y - data._playerExperience * 0.1, 0.0));
        playerLevel.push_back(iaVector3f(x, y - (uint32)data._playerLevel * 20.0, 0.0));
        enemyHealth.push_back(iaVector3f(x, y - data._enemyHealth * 0.05, 0.0));
        enemyCount.push_back(iaVector3f(x, y - data._enemyCount * 1.0, 0.0));
        x += 1.0f;
    }

    iRenderer::getInstance().setLineWidth(1);
    iRenderer::getInstance().drawLineStrip(enemyHealth, iaColor4f(1, 0, 0, 1));
    iRenderer::getInstance().drawLineStrip(enemyCount, iaColor4f(1, 1, 0, 1));
    iRenderer::getInstance().drawLineStrip(playerDamage, iaColor4f(0, 1, 0, 1));
    // iRenderer::getInstance().drawLineStrip(playerExperience, iaColor4f(0, 0, 1, 1));
    // iRenderer::getInstance().drawLineStrip(playerLevel, iaColor4f(0, 0, 0, 1));

    iRenderer::getInstance().drawString(10, getWindow()->getClientHeight() - 40, iaString("enemies: ") + iaString::toString((int)_stats.back()._enemyCount));
}

float64 GameLayer::calcLevel(uint32 experience)
{
    uint32 lowerBounds = 0;
    uint32 upperBounds = _expLvl.front();

    float64 level = 1.0;

    if (experience >= _expLvl.front())
    {
        for (int i = 0; i < _expLvl.size() - 1; ++i)
        {
            level += 1.0;
            lowerBounds = _expLvl[i];
            upperBounds = _expLvl[i + 1];

            if (experience >= lowerBounds &&
                experience < upperBounds)
            {
                break;
            }
        }
    }

    level += float64(experience - lowerBounds) / float64(upperBounds - lowerBounds);

    return level;
}

void GameLayer::onRenderHUD()
{
    if (!_player.isValid())
    {
        return;
    }

    auto &healthComp = _player.getComponent<HealthComponent>();
    auto &playerExperience = _player.getComponent<ExperienceComponent>();
    auto &playerCoins = _player.getComponent<CoinsComponent>();
    auto &modifiers = _player.getComponent<ModifierComponent>();
    auto &weapon = _player.getComponent<WeaponComponent>();
    auto &playerVelocity = _player.getComponent<VelocityComponent>();

    iaMatrixd matrix;
    matrix.translate(0, 0, -1);
    iRenderer::getInstance().setModelMatrix(matrix);

    iRenderer::getInstance().setFont(_font);
    iRenderer::getInstance().setFontSize(20.0f);
    iRenderer::getInstance().drawString(10, 10, iaString::toString(healthComp._health, 0));

    uint32 level = playerExperience._level;
    float64 percentOfLevel = playerExperience._level - float64(level);

    iRenderer::getInstance().drawString(10, 40, iaString("lvl: ") + iaString::toString(level));

    iRenderer::getInstance().drawRectangle(10, 80, 310, 20, iaColor4f::white);
    iRenderer::getInstance().drawFilledRectangle(10, 80, (300 * percentOfLevel) + 10, 20, iaColor4f::red);

    uint32 nextLevel = 0;
    for (auto val : _expLvl)
    {
        if (playerExperience._experience < val)
        {
            nextLevel = val;
            break;
        }
    }
    iRenderer::getInstance().drawString(350, 80, iaString::toString((int)playerExperience._experience) + " -> " + iaString::toString(nextLevel));

    iRenderer::getInstance().drawTexturedRectangle(10, 120, 40, 40, _coin, iaColor4f::white, true);
    iRenderer::getInstance().drawString(60, 130, iaString::toString(playerCoins._coins, 0));

    iRenderer::getInstance().drawTexturedRectangle(10, 160, 40, 40, _damage, iaColor4f::white, true);
    iRenderer::getInstance().drawString(60, 170, iaString::toString(weapon._damage * modifiers._damageFactor, 0));

    iRenderer::getInstance().drawTexturedRectangle(10, 200, 40, 40, _attackSpeed, iaColor4f::white, true);
    iRenderer::getInstance().drawString(60, 210, iaString::toString(1.0 / (weapon._attackInterval.getSeconds() / modifiers._attackIntervalFactor), 1));

    iRenderer::getInstance().drawTexturedRectangle(10, 240, 40, 40, _walkSpeed, iaColor4f::white, true);
    iRenderer::getInstance().drawString(60, 250, iaString::toString(modifiers._walkSpeedFactor * playerVelocity._speed, 1));
}

void GameLayer::onLevelUp()
{
    pause();

    _levelUpDialog->open(iDialogCloseDelegate(this, &GameLayer::onCloseLevelUpDialog), _upgrades);
}

void GameLayer::onOpenBuilding(BuildingType buildingType)
{
    if (_currentBuilding == buildingType)
    {
        return;
    }

    _currentBuilding = buildingType;

    if (_currentBuilding == BuildingType::None)
    {
        return;
    }

    pause();

    switch (_currentBuilding)
    {
    case BuildingType::Shop:
        openShop();
        break;
    }
}

void GameLayer::openShop()
{
    _shopDialog->open(iDialogCloseDelegate(this, &GameLayer::onCloseShopDialog), 100, _shopItems);
}

void GameLayer::onCloseShopDialog(iDialogPtr dialog)
{
    liftShop();
    play();

    // start timer to next shop landing
    _spawnShopTimerHandle->start();

    if (!_shopDialog->bought() ||
        !_player.isValid())
    {
        return;
    }

    const ShopItem &shopItem = _shopDialog->getSelection();
    auto &coins = _player.getComponent<CoinsComponent>();
    coins._coins -= shopItem._price;

    switch (shopItem._type)
    {
    case ShopItemType::Weapon:
        auto &weapon = _player.getComponent<WeaponComponent>();
        weapon = _weapons[shopItem._name];
        weapon._time = iTimer::getInstance().getTime();
        break;
    }
}

void GameLayer::upgrade(iEntity entity, const UpgradeConfiguration &upgradeConfiguration)
{
    if (!entity.isValid())
    {
        return;
    }

    ModifierComponent &modifierComponent = entity.getComponent<ModifierComponent>();

    modifierComponent._damageFactor += upgradeConfiguration._damageFactor;
    modifierComponent._attackIntervalFactor += upgradeConfiguration._attackIntervalFactor;
    modifierComponent._criticalHitChanceFactor += upgradeConfiguration._criticalHitChanceFactor;
    modifierComponent._criticalHitDamageFactor += upgradeConfiguration._criticalHitDamageFactor;
    modifierComponent._splashDamageRangeFactor += upgradeConfiguration._splashDamageRangeFactor;
    modifierComponent._walkSpeedFactor += upgradeConfiguration._walkSpeedFactor;
    modifierComponent._projectileSpeedFactor += upgradeConfiguration._projectileSpeedFactor;
}

void GameLayer::onCloseLevelUpDialog(iDialogPtr dialog)
{
    upgrade(_player, _levelUpDialog->getSelection());
    play();
}

void GameLayer::onRenderOrtho()
{
    auto &viewportComp = _viewport.getComponent<ViewportComponent>();
    const iaRectangled &viewRectangle = viewportComp._viewport;
    iaRectangled intersetRectangle = viewRectangle;
    float64 scale = viewRectangle._width * 0.1;
    intersetRectangle.adjust(-scale, -scale, scale * 2.0, scale * 2.0);

    iaMatrixd matrix;
    matrix.translate(0, 0, -100);
    matrix.scale(static_cast<float64>(getWindow()->getClientWidth() / PLAYFIELD_VIEWPORT_WIDTH),
                 (static_cast<float64>(getWindow()->getClientHeight()) / PLAYFIELD_VIEWPORT_HEIGHT), 1.0);
    matrix.translate(-viewRectangle._x, -viewRectangle._y, 0);
    iRenderer::getInstance().setModelMatrix(matrix);

    iRenderer::getInstance().drawTexturedRectangle(-1000, -1000, 3000, 3000, _backgroundTexture, iaColor4f::white, false, iaVector2f(10.0, 15.0));

    // draw entities
    auto view = _entityScene.getEntities<PositionComponent, SizeComponent, VisualComponent, OrientationComponent>();

    for (auto entity : view)
    {
        auto [pos, size, visual, ori] = view.get<PositionComponent, SizeComponent, VisualComponent, OrientationComponent>(entity);

        if(!visual._visible)
        {
            continue;
        }

        iaVector2d position;

        if (!intersectDoughnut(pos._position, intersetRectangle, position))
        {
            continue;
        }

        position += pos._position;

        float64 width = size._size;
        float64 height = size._size;

        if (visual._castShadow)
        {
            iRenderer::getInstance().drawTexturedRectangle(position._x - width * 0.25, position._y + height * 0.5, width * 0.5, height * 0.25, _shadow, iaColor4f::black, true);
        }

        iaMatrixf matrix;
        matrix.translate(position._x, position._y, 0.0);

        iaVector2d o = ori._direction;
        o._y *= -1.0;
        const float32 angle = o.angle();
        matrix.rotate(angle, iaAxis::Z);

        if (visual._scaleAnimation)
        {
            iaTime time = iTimer::getInstance().getTime() + visual._timerOffset;
            float64 timing = std::fmod((time.getMilliseconds() * 2.0), 1000.0) / 1000.0 * M_PI * 2;
            float64 value = 1.0f + (sin(timing) * 0.1);
            matrix.scale(width * value, height * (1.0 / value), 1.0);
        }
        else
        {
            matrix.scale(width, height, 1.0);
        }

        iRenderer::getInstance().drawTexturedQuad(matrix, visual._texture, iaColor4f::white, true);
    }

    // onRenderQuadtree(_quadtree.getRoot());

    onRenderHUD();
    onRenderStats();
}

void GameLayer::onRenderQuadtree(std::shared_ptr<iQuadtreeNode<float64>> node)
{
    if (node == nullptr)
    {
        return;
    }

    iRenderer::getInstance().drawRectangle(node->_box._x, node->_box._y, node->_box._width, node->_box._height, iaColor4f::red);

    for (auto object : node->_objects)
    {
        iRenderer::getInstance().drawCircle(object->_circle._center._x, object->_circle._center._y, object->_circle._radius);
    }

    for (auto child : node->_children)
    {
        onRenderQuadtree(child);
    }
}

bool GameLayer::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::ESC:
        iApplication::getInstance().stop();
        return true;
    }

    if (!_player.isValid())
    {
        return false;
    }

    MovementControlComponent &movementControl = _player.getComponent<MovementControlComponent>();

    switch (event.getKey())
    {
    case iKeyCode::W:
        movementControl._up = true;
        return true;

    case iKeyCode::A:
        movementControl._left = true;
        return true;

    case iKeyCode::S:
        movementControl._down = true;
        return true;

    case iKeyCode::D:
        movementControl._right = true;
        return true;
    }

    return false;
}

void GameLayer::pause()
{
    _gamePause = true;

    iTimer::getInstance().stop();

    resetKeyboardInput();
}

void GameLayer::play()
{
    _gamePause = false;
    iTimer::getInstance().start();
}

void GameLayer::resetKeyboardInput()
{
    if (_player.isValid())
    {
        MovementControlComponent &movementControl = _player.getComponent<MovementControlComponent>();
        movementControl._up = false;
        movementControl._down = false;
        movementControl._left = false;
        movementControl._right = false;
    }
}

bool GameLayer::onKeyUp(iEventKeyUp &event)
{
    if (!_player.isValid())
    {
        return false;
    }

    MovementControlComponent &movementControl = _player.getComponent<MovementControlComponent>();

    switch (event.getKey())
    {
    case iKeyCode::W:
        movementControl._up = false;
        return true;

    case iKeyCode::A:
        movementControl._left = false;
        return true;

    case iKeyCode::S:
        movementControl._down = false;
        return true;

    case iKeyCode::D:
        movementControl._right = false;
        return true;

    case iKeyCode::Space:
        _gamePause = !_gamePause;

        if (_gamePause)
        {
            iTimer::getInstance().stop();
        }
        else
        {
            iTimer::getInstance().start();
        }

        return true;
    }

    return false;
}