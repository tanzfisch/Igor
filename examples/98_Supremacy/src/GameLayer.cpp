// Igor game engine
// (c) Copyright 2012-2025 by Martin A. Loga
// see copyright notice in corresponding header file

#include "GameLayer.h"

#include <math.h>

GameLayer::GameLayer(iWindowPtr window)
    : iLayer(window, L"GameLayer")
{
}

void GameLayer::onInit()
{
    iEntitySystemModule::getInstance().registerComponentType<RangeComponent>(RangeComponent::createInstance, "SupremacyComponentRange");
    iEntitySystemModule::getInstance().registerComponentType<AngularVelocityComponent>(AngularVelocityComponent::createInstance, "SupremacyComponentAngularVelocity");
    iEntitySystemModule::getInstance().registerComponentType<HealthComponent>(HealthComponent::createInstance, "SupremacyComponentHealth");
    iEntitySystemModule::getInstance().registerComponentType<PickupComponent>(PickupComponent::createInstance, "SupremacyComponentPickup");
    iEntitySystemModule::getInstance().registerComponentType<HealComponent>(HealComponent::createInstance, "SupremacyComponentHeal");
    iEntitySystemModule::getInstance().registerComponentType<DamageComponent>(DamageComponent::createInstance, "SupremacyComponentDamage");
    iEntitySystemModule::getInstance().registerComponentType<ExperienceComponent>(ExperienceComponent::createInstance, "SupremacyComponentExperience");
    iEntitySystemModule::getInstance().registerComponentType<CoinsComponent>(CoinsComponent::createInstance, "SupremacyComponentCoins");
    iEntitySystemModule::getInstance().registerComponentType<ExperienceGainComponent>(ExperienceGainComponent::createInstance, "SupremacyComponentExperienceGain");
    iEntitySystemModule::getInstance().registerComponentType<CoinGainComponent>(CoinGainComponent::createInstance, "SupremacyComponentCoinGain");
    iEntitySystemModule::getInstance().registerComponentType<TargetComponent>(TargetComponent::createInstance, "SupremacyComponentTarget");
    iEntitySystemModule::getInstance().registerComponentType<MovementControlComponent>(MovementControlComponent::createInstance, "SupremacyComponentMovementControl");
    iEntitySystemModule::getInstance().registerComponentType<ViewportComponent>(ViewportComponent::createInstance, "SupremacyComponentViewport");
    iEntitySystemModule::getInstance().registerComponentType<WeaponComponent>(WeaponComponent::createInstance, "SupremacyComponentWeapon");
    iEntitySystemModule::getInstance().registerComponentType<ModifierComponent>(ModifierComponent::createInstance, "SupremacyComponentModifier");
    iEntitySystemModule::getInstance().registerComponentType<BuildingComponent>(BuildingComponent::createInstance, "SupremacyComponentBuilding");

    _entityScene = iProject::getInstance().getProjectScene();

    iaRandom::setSeed(iaTime::getNow().getMicroseconds());

    initExpLvlTable();
    loadSpecs("project/supremacy.xml");

    _viewOrtho.setName("view ortho");
    _viewOrtho.getRenderEvent().add({this, &GameLayer::onRenderOrtho});
    _viewOrtho.setEntityScene(_entityScene);
    getWindow()->addView(&_viewOrtho, getZIndex() + 1);

    // init font for render profiler
    _font = iTextureFont::create(iResourceManager::getInstance().loadResource<iTexture>("igor_font_default_outline"));

    _coin = iResourceManager::getInstance().requestResource<iSprite>("example_sprite_coin");

    _damage = iResourceManager::getInstance().createResource<iSprite>();
    _damage->setTexture(iResourceManager::getInstance().requestResource<iTexture>("example_texture_supremacy_fist"));

    _attackSpeed = iResourceManager::getInstance().createResource<iSprite>();
    _attackSpeed->setTexture(iResourceManager::getInstance().requestResource<iTexture>("example_texture_supremacy_bullets"));

    _walkSpeed = iResourceManager::getInstance().createResource<iSprite>();
    _walkSpeed->setTexture(iResourceManager::getInstance().requestResource<iTexture>("example_texture_supremacy_run"));

    _shadow = iResourceManager::getInstance().createResource<iSprite>();
    _shadow->setTexture(iResourceManager::getInstance().requestResource<iTexture>("example_texture_supremacy_shadow"));

    _shield = iResourceManager::getInstance().createResource<iSprite>();
    _shield->setTexture(iResourceManager::getInstance().requestResource<iTexture>("example_texture_supremacy_shield"));

    _rage = iResourceManager::getInstance().createResource<iSprite>();
    _rage->setTexture(iResourceManager::getInstance().requestResource<iTexture>("example_texture_supremacy_rage"));

    _bounceAnimation = iResourceManager::getInstance().requestResource<iAnimation>("example_animation_bounce");
    _shopIdleAnimation = iResourceManager::getInstance().requestResource<iAnimation>("example_animation_shop_idle");
    _coinSpinAnimation = iResourceManager::getInstance().requestResource<iAnimation>("example_animation_coin");

    _player = createPlayer();
    _camera = createCamera();
    createBackground();

    // game logic timer
    _updateTimerHandle = new iTimerHandle(iTimerTickDelegate(this, &GameLayer::onUpdate), iaTime::fromMilliseconds(10));
    _updateTimerHandle->start();

    _spawnUnitsTimerHandle = new iTimerHandle(iTimerTickDelegate(this, &GameLayer::onSpawnStuff), iaTime::fromMilliseconds(5000));
    _spawnUnitsTimerHandle->start();

    // run once right now so we have a few enemies right away
    _spawnUnitsTimerHandle->triggerNow();

    createShop();

    _spawnShopTimerHandle = new iTimerHandle(iTimerTickDelegate(this, &GameLayer::onLandShop), iaTime::fromMilliseconds(5000), true);
    _spawnShopTimerHandle->start();

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

        _enemies.push_back({name,
                            description,
                            texture,
                            static_cast<float32>(damage),
                            static_cast<float32>(health),
                            static_cast<float32>(size),
                            static_cast<float32>(speed),
                            static_cast<float32>(xpDrop),
                            static_cast<float32>(coinDrop)});

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
        float64 projectileRangeFactor = 0.0;
        upgrade->Attribute("projectileRangeFactor", &projectileRangeFactor);

        _upgrades.push_back({name, description, icon, damageFactor, attackIntervalFactor, criticalHitChanceFactor,
                             criticalHitDamageFactor, splashDamageRangeFactor, walkSpeedFactor, projectileSpeedFactor, projectileRangeFactor});
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
            float64 accuracy = 0.0f;
            weapon->Attribute("accuracy", &accuracy);
            float64 angularVelocity = 0.0f;
            weapon->Attribute("angularVelocity", &angularVelocity);
            float64 projectileDamage = 0.0f;
            weapon->Attribute("projectileDamage", &projectileDamage);
            float64 projectileSpeed = 0.0f;
            weapon->Attribute("projectileSpeed", &projectileSpeed);
            float64 projectileRange = 0.0f;
            weapon->Attribute("projectileRange", &projectileRange);
            float64 attackIntervall = 0.0f;
            weapon->Attribute("attackIntervall", &attackIntervall);

            iaString temp(weapon->Attribute("needToStandStill"));
            bool needToStandStill = temp == "true" ? true : false;
            iaString temp2(weapon->Attribute("returnToSender"));
            bool returnToSender = temp2 == "true" ? true : false;

            type = ShopItemType::Weapon;

            WeaponConfiguration weaponConfiguration =
                {
                    projectileTexture,
                    static_cast<float32>(projectileSize),
                    (uint32)projectileCount,
                    static_cast<float32>(accuracy),
                    static_cast<float32>(angularVelocity),
                    static_cast<float32>(projectileDamage),
                    static_cast<float32>(projectileSpeed),
                    static_cast<float32>(projectileRange),
                    iaTime::fromMilliseconds(attackIntervall),
                    needToStandStill,
                    returnToSender,
                    iaVector2d(0.0, -STANDARD_UNIT_SIZE * 0.5)};

            // con_endl(name << " " << projectileDamage / attackIntervall);

            _weapons[name] = weaponConfiguration;
        }

        _shopItems.push_back({name, description, icon, type, (uint32)price});

    } while ((shopItem = shopItem->NextSiblingElement("ShopItem")) != nullptr);
}

void GameLayer::loadSpecs(const iaString &filename)
{
    con_assert_sticky(!filename.isEmpty(), "empty filename");

    iaString path = iResourceManager::getInstance().resolvePath(filename);

    char temp[2048];
    path.getData(temp, 2048);

    TiXmlDocument document(temp);
    if (!document.LoadFile())
    {
        con_err("can't read \"" << filename << "\". " << document.ErrorDesc());
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
    float32 xp = 100.0;
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

iaVector3d GameLayer::getRandomDir()
{
    iaVector2f direction(0.0, 1.0);
    direction.rotateXY(iaRandom::getNextFloat() * M_PI * 2.0);
    direction.normalize();
    return iaVector3d(direction._x, direction._y, 0.0);
}

iEntityID GameLayer::createPlayer()
{
    // init player
    iEntityPtr entity = _entityScene->createEntity("player");

    iTransformComponent *transform = static_cast<iTransformComponent *>(entity->addComponent(new iTransformComponent(iaVector3d(PLAYFIELD_WIDTH * 0.5f, PLAYFIELD_HEIGHT * 0.5f, 0.0))));
    entity->addComponent(new iGlobalBoundaryComponent(iGlobalBoundaryType::Repeat));
    entity->addComponent(new iVelocityComponent(iaVector3d(0, 0, 0)));

    iSpritePtr wagiu = iResourceManager::getInstance().createResource<iSprite>();
    wagiu->setTexture(iResourceManager::getInstance().requestResource<iTexture>("example_texture_supremacy_wagiu_a5"));
    entity->addComponent(new iSpriteRenderComponent(wagiu, iaVector2d(STANDARD_UNIT_SIZE * 1.5, STANDARD_UNIT_SIZE * 1.5)));
    entity->addComponent(new iPartyComponent(FRIEND));
    entity->addComponent(new iCircleComponent(STANDARD_UNIT_SIZE * 1.5 * 0.5));
    entity->addComponent(new iQuadtreeComponent());

    iAnimationControllerPtr animationController(new iAnimationController());
    animationController->addClip(iClip::createClip({_bounceAnimation}, true, true));
    entity->addComponent(new iAnimationComponent(animationController));
    entity->addComponent(new HealthComponent(100.0f));
    entity->addComponent(new TargetComponent(iEntityID::getInvalid(), false, false));
    entity->addComponent(new DamageComponent(0.0f));

    entity->addBehaviour({this, &GameLayer::onPlayerMovementBehaviour});
    entity->addBehaviour({this, &GameLayer::onAquireTarget});
    entity->addBehaviour({this, &GameLayer::onUpdateCollision});
    entity->addBehaviour({this, &GameLayer::onUpdateWeapon});

    entity->addComponent(new ExperienceComponent());
    entity->addComponent(new CoinsComponent());
    entity->addComponent(new ModifierComponent({1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f}));
    entity->addComponent(new WeaponComponent(_weapons["Knife"]));

    // add shadow
    iEntityPtr shadow = _entityScene->createEntity("player shadow");
    shadow->addComponent(new iTransformComponent({iaVector3d(0.0, STANDARD_UNIT_SIZE * 0.8, 0.0)}));
    shadow->addComponent(new iSpriteRenderComponent({_shadow, iaVector2d(STANDARD_UNIT_SIZE * 0.7, STANDARD_UNIT_SIZE * 0.3), iaColor4f::black, -1}));
    shadow->setParent(entity->getID());

    return entity->getID();
}

void GameLayer::onPlayerMovementBehaviour(iEntityPtr entity, std::any &userData)
{
    auto velocityComponent = entity->getComponent<iVelocityComponent>();
    auto modifier = entity->getComponent<ModifierComponent>();

    iaVector3d velocity;

    if (iKeyboard::getInstance().keyPressed(iKeyCode::W))
    {
        velocity._y -= 1.0;
    }

    if (iKeyboard::getInstance().keyPressed(iKeyCode::A))
    {
        velocity._x -= 1.0;
    }

    if (iKeyboard::getInstance().keyPressed(iKeyCode::S))
    {
        velocity._y += 1.0;
    }

    if (iKeyboard::getInstance().keyPressed(iKeyCode::D))
    {
        velocity._x += 1.0;
    }

    velocity.normalize();
    velocity *= modifier->_config._walkSpeedFactor;

    velocityComponent->setVelocity(velocity);
}

void GameLayer::onAquireTarget(iEntityPtr entity, std::any &userData)
{
    const auto transformComponent = entity->getComponent<iTransformComponent>();
    const auto entityParty = entity->getComponent<iPartyComponent>();
    const auto weapon = entity->getComponent<WeaponComponent>();
    const auto modifier = entity->getComponent<ModifierComponent>();
    auto target = entity->getComponent<TargetComponent>();

    const auto &position = transformComponent->getPosition();

    iaCircled circle(iaVector2d(position._x, position._y), weapon->_config._range * modifier->_config._projectileRangeFactor);
    std::vector<std::pair<iEntityID, iaVector2d>> hits;
    doughnutQuery(circle, hits);

    float32 minDistance = std::numeric_limits<float32>::max();
    target->_targetID = iEntityID::getInvalid();

    for (const auto &hit : hits)
    {
        if (hit.first == entity->getID())
        {
            continue;
        }

        iEntityPtr entityHit = _entityScene->getEntity(hit.first);

        auto entParty = entityHit->getComponent<iPartyComponent>();
        if (entParty == nullptr ||
            entParty->_partyID == NEUTRAL ||
            entParty->_partyID == entityParty->_partyID)
        {
            continue;
        }

        const float32 dist = hit.second.length2();
        if (dist < minDistance)
        {
            minDistance = dist;
            target->_targetID = hit.first;
        }
    }
}

void GameLayer::onUpdateCollision(iEntityPtr entity, std::any &userData)
{
    BuildingType buildingType = BuildingType::None;

    const auto transform = entity->getComponent<iTransformComponent>();
    auto body = entity->getComponent<iQuadtreeComponent>();
    auto experience = entity->getComponent<ExperienceComponent>();
    auto coins = entity->getComponent<CoinsComponent>();
    auto health = entity->getComponent<HealthComponent>();

    iaCircled circle = body->_object->_circle;
    circle._radius = 70.0;
    std::vector<std::pair<iEntityID, iaVector2d>> hits;
    doughnutQuery(circle, hits);

    for (const auto &hit : hits)
    {
        if (hit.first == entity->getID())
        {
            continue;
        }

        iEntityPtr otherEntity = _entityScene->getEntity(hit.first);

        auto pickup = otherEntity->getComponent<PickupComponent>();
        if (pickup != nullptr)
        {
            if (hit.second.length2() > 70.0 * 70.0)
            {
                continue;
            }

            auto expGain = otherEntity->getComponent<ExperienceGainComponent>();
            addExperience(entity, expGain->_experience);

            auto coinGain = otherEntity->getComponent<CoinGainComponent>();
            coins->_coins += coinGain->_coins;

            auto damage = otherEntity->getComponent<DamageComponent>();
            health->_health -= damage->_damage;

            auto heal = otherEntity->getComponent<HealComponent>();
            health->_health += heal->_heal;

            _entityScene->destroyEntity(otherEntity);
        }

        auto *buildingComponent = otherEntity->getComponent<BuildingComponent>();
        if (buildingComponent != nullptr)
        {
            auto otherBody = otherEntity->getComponent<iQuadtreeComponent>();
            if (iIntersection::intersects(otherBody->_object->_circle, body->_object->_circle))
            {
                buildingType = buildingComponent->_type;
            }
        }
    }

    onOpenBuilding(buildingType);
}

void GameLayer::onUpdateWeapon(iEntityPtr entity, std::any &userData)
{
    iaTime now = iTimer::getInstance().getTime();

    auto weapon = entity->getComponent<WeaponComponent>();
    auto transformComponent = entity->getComponent<iTransformComponent>();
    auto velocity = entity->getComponent<iVelocityComponent>();
    auto modifier = entity->getComponent<ModifierComponent>();
    auto target = entity->getComponent<TargetComponent>();

    // skip if there is no target
    if (!target->_targetID.isValid())
    {
        return;
    }

    // check if unit needs to stand still
    if (weapon->_config._standStillToFire && velocity->getVelocity().length() > 0.0)
    {
        return;
    }

    // check if it is time to fire again
    if ((now - weapon->_time) < (weapon->_config._attackInterval / modifier->_config._attackIntervalFactor))
    {
        return;
    }

    weapon->_time = now;

    // get target position
    iEntityPtr targetEntity = _entityScene->getEntity(target->_targetID);
    auto targetTransformComponent = targetEntity->getComponent<iTransformComponent>();

    const auto &srcPosition = transformComponent->getPosition();
    const auto &dstPosition = targetTransformComponent->getPosition();

    const iaVector2d firePos = iaVector2d(srcPosition._x, srcPosition._y) + weapon->_config._offset;
    iaVector2d direction = iaVector2d(dstPosition._x, dstPosition._y) - firePos;
    direction.normalize();

    fire(firePos, direction, FRIEND, weapon, modifier);
}

void GameLayer::onCameraFollowPlayer(iEntityPtr entity, std::any &userData)
{
    iEntityPtr player = _entityScene->getEntity(_player);

    if (player == nullptr)
    {
        return;
    }

    auto playerTransformComponent = player->getComponent<iTransformComponent>();
    auto camTransformComponent = entity->getComponent<iTransformComponent>();
    iaVector2d &targetOffset = std::any_cast<iaVector2d &>(userData);

    const auto &playerPos = playerTransformComponent->getPosition();
    const auto &camPos = camTransformComponent->getPosition();

    const iaVector2d playerPosition(playerPos._x, playerPos._y);
    const iaVector2d lastPlayerPosition(camPos._x + targetOffset._x,
                                        camPos._y + targetOffset._y);

    const iaVector2d diff = playerPosition - lastPlayerPosition;

    bool skipStep = false;
    const auto width = PLAYFIELD_VIEWPORT_MOVE_EDGE_WIDTH * 0.5;
    const auto height = PLAYFIELD_VIEWPORT_MOVE_EDGE_HEIGHT * 0.5;

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

    camTransformComponent->setPosition(iaVector3d(playerPosition._x - targetOffset._x, playerPosition._y - targetOffset._y, 0.0));

    userData = targetOffset;
}

iEntityID GameLayer::createCamera()
{
    iEntityPtr player = _entityScene->getEntity(_player);

    const auto playerTransform = player->getComponent<iTransformComponent>();

    iEntityPtr entity = _entityScene->createEntity("camera");

    entity->addComponent(new iTransformComponent(playerTransform->getPosition()));
    entity->addBehaviour({this, &GameLayer::onCameraFollowPlayer}, iaVector2d());

    auto cameraComponent = entity->addComponent(new iCameraComponent());

    cameraComponent->setOrthogonal(-PLAYFIELD_VIEWPORT_WIDTH * 0.5, PLAYFIELD_VIEWPORT_WIDTH * 0.5, PLAYFIELD_VIEWPORT_HEIGHT * 0.5, -PLAYFIELD_VIEWPORT_HEIGHT * 0.5);
    cameraComponent->setClearColorActive(false);
    cameraComponent->setClearDepthActive(false);

    return entity->getID();
}

void GameLayer::createBackground()
{
    iEntityPtr entity = _entityScene->createEntity("background");

    entity->addComponent(new iTransformComponent(iaVector3d(PLAYFIELD_WIDTH * 0.5f, PLAYFIELD_HEIGHT * 0.5f, 0.0), iaVector3d(), iaVector3d(200.0, 150.0, 1.0)));

    iSpritePtr background = iResourceManager::getInstance().createResource<iSprite>();
    background->setTexture(iResourceManager::getInstance().requestResource<iTexture>("example_texture_supremacy_background"));
    entity->addComponent(new iSpriteRenderComponent(background, iaVector2d(10.0, 15.0), iaColor4f::white, -100, iSpriteRenderComponent::iRenderMode::Tiled));
}

void GameLayer::createCoin(const iaVector2f &pos, uint32 party, ObjectType objectType)
{
    iEntityPtr entity = _entityScene->createEntity("object");
    const auto &transform = entity->addComponent(new iTransformComponent(iaVector3d(pos._x, pos._y, 0.0), iaVector3d(), iaVector3d(COIN_SIZE, COIN_SIZE, 1.0)));
    entity->addComponent(new iSpriteRenderComponent(iResourceManager::getInstance().requestResource<iSprite>("example_sprite_coin"), iaVector2d(1.0, 1.0), iaColor4f::white, -10));
    entity->addComponent(new iPartyComponent(party));
    entity->addComponent(new iCircleComponent(COIN_SIZE * 0.5));
    entity->addComponent(new iQuadtreeComponent());

    entity->addComponent(new PickupComponent(true));
    entity->addComponent(new ExperienceGainComponent(0.0f));
    entity->addComponent(new CoinGainComponent(objectType == ObjectType::Coin ? 1.0f : 0.0f));
    entity->addComponent(new DamageComponent(0.0f));
    entity->addComponent(new HealComponent(0.0f));

    iAnimationControllerPtr animationController(new iAnimationController());
    animationController->addClip(iClip::createClip({_coinSpinAnimation}, true, true));
    entity->addComponent(new iAnimationComponent(animationController));
}

void GameLayer::onLiftShop()
{    
    iEntityPtr shop = _entityScene->getEntity(_shop);
    if (shop == nullptr)
    {
        return;
    }

    shop->setActive(false);
}

void GameLayer::onLandShop(const iaTime &time)
{
    iEntityPtr shop = _entityScene->getEntity(_shop);
    iEntityPtr player = _entityScene->getEntity(_player);
    if (shop == nullptr ||
        player == nullptr)
    {
        return;
    }    

    auto playerTransformComponent = player->getComponent<iTransformComponent>();
    auto shopTransformComponent = shop->getComponent<iTransformComponent>();

    const auto &playerWorldMatrix = playerTransformComponent->getWorldMatrix();

    iaVector2d pos(300.0, 0.0);
    pos.rotateXY(iaRandom::getNextFloat() * 2 * M_PI);
    pos._x += playerWorldMatrix._pos._x;
    pos._y += playerWorldMatrix._pos._y;
    pos._x = std::fmod(pos._x, PLAYFIELD_WIDTH);
    pos._y = std::fmod(pos._y, PLAYFIELD_HEIGHT);

    shopTransformComponent->setPosition(iaVector3d(pos._x, pos._y, 0.0));

    shop->setActive(true);
}

void GameLayer::createShop()
{
    iEntityPtr shop = _entityScene->createEntity("shop");
    shop->setActive(false); // TODO need to figure out why we can't deactivate after adding iSpriteRenderComponent

    _shop = shop->getID();
    shop->addComponent(new iTransformComponent(iaVector3d(300,300,0), iaVector3d(), iaVector3d(STANDARD_UNIT_SIZE * 4, STANDARD_UNIT_SIZE * 4, 1.0)));
    shop->addComponent(new iVelocityComponent());
    shop->addComponent(new iGlobalBoundaryComponent(iGlobalBoundaryType::Repeat));
    shop->addComponent(new iSpriteRenderComponent(iResourceManager::getInstance().requestResource<iSprite>("example_sprite_shop")));
    shop->addComponent(new iPartyComponent(FRIEND));
    shop->addComponent(new iCircleComponent(STANDARD_UNIT_SIZE * 4 * 0.5));
    shop->addComponent(new BuildingComponent(BuildingType::Shop));
    shop->addComponent(new ModifierComponent({1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f}));
    shop->addComponent(new WeaponComponent(_weapons["Minigun"]));
    shop->addComponent(new TargetComponent(iEntityID::getInvalid(), false, false));
    shop->addComponent(new iQuadtreeComponent());
    shop->addBehaviour({this, &GameLayer::onAquireTarget});
    shop->addBehaviour({this, &GameLayer::onUpdateWeapon});

    iAnimationControllerPtr animationController(new iAnimationController());
    animationController->addClip(iClip::createClip({_shopIdleAnimation}, true, true));
    shop->addComponent(new iAnimationComponent(animationController));

    // add shadow
    iEntityPtr shadow = _entityScene->createEntity("shop shadow");
    shadow->addComponent(new iTransformComponent(iaVector3d(0.0, 0.25, 0.0)));
    shadow->addComponent(new iSpriteRenderComponent(_shadow, iaVector2d(0.8, 0.8), iaColor4f::black, -1));
    shadow->setParent(shop->getID());
}

void GameLayer::onFollowTarget(iEntityPtr entity, std::any &userData)
{
    auto velocityComp = entity->getComponent<iVelocityComponent>();
    iaVector3d velocity(0.0, 0.0, 0.0);
    auto target = entity->getComponent<TargetComponent>();

    // stop if entity is not following targets or if there is no valid target
    iEntityPtr targetEntity = _entityScene->getEntity(target->_targetID);
    if (!target->_followTarget ||
        targetEntity == nullptr)
    {
        velocityComp->setVelocity(velocity);
        return;
    }

    // stop if target has no position
    auto targetTransformComponent = targetEntity->getComponent<iTransformComponent>();
    if (targetTransformComponent == nullptr)
    {
        velocityComp->setVelocity(velocity);
        return;
    }

    auto transformComponent = entity->getComponent<iTransformComponent>();

    const auto &dstPos = targetTransformComponent->getPosition();
    const auto &entityPos = transformComponent->getPosition();

    const iaVector2d targetPos(dstPos._x, dstPos._y);
    const iaVector2d position(entityPos._x, entityPos._y);
    iaVector2d offset;
    iaCircled targetCircle(position, 1000.0);

    const float64 speed = 0.5; // TODO

    if (!intersectDoughnut(targetPos, targetCircle, offset))
    {
        if (target->_inRange)
        {
            velocityComp->setVelocity(getRandomDir() * speed);
            target->_inRange = false;
        }
    }
    else
    {
        iaVector2d newVel = targetPos - position + offset;
        newVel.normalize();
        newVel *= speed;
        velocityComp->setVelocity(iaVector3d(newVel._x, newVel._y, 0.0));
        target->_inRange = true;
    }

    auto quadComp = entity->getComponent<iQuadtreeComponent>();
    iaCircled circle = quadComp->_object->_circle;
    circle._radius *= 1.1;
    iQuadtreed::Objects objects;
    _entityScene->getQuadtree().query(circle, objects);

    iaVector2d diversion(0.0, 0.0);
    for (const auto &object : objects)
    {
        iEntityID otherEntityID = std::any_cast<iEntityID>(object->_userData);

        // skip self
        if (otherEntityID == entity->getID())
        {
            continue;
        }

        // calc diversion
        diversion += circle._center - object->_circle._center;
    }

    if (diversion.length2() > 0)
    {
        diversion.normalize();
        diversion *= speed;
        auto velocity = velocityComp->getVelocity();
        velocity._x += diversion._x;
        velocity._y += diversion._y;
        velocityComp->setVelocity(velocity);
    }
}

void GameLayer::createUnit(const iaVector2f &pos, uint32 party, iEntityID target, const EnemyClass &enemyClass)
{
    iEntityPtr unit = _entityScene->createEntity("unit");
    unit->addComponent(new iTransformComponent(iaVector3d(pos._x, pos._y, 0.0), iaVector3d()));
    unit->addComponent(new iGlobalBoundaryComponent(iGlobalBoundaryType::Repeat));
    unit->addComponent(new iVelocityComponent(getRandomDir() * enemyClass._speed));

    iSpritePtr sprite = iResourceManager::getInstance().createResource<iSprite>();
    sprite->setTexture(iResourceManager::getInstance().requestResource<iTexture>(enemyClass._texture));
    unit->addComponent(new iSpriteRenderComponent(sprite, iaVector2d(enemyClass._size, enemyClass._size)));
    unit->addComponent(new iPartyComponent(party));
    unit->addComponent(new iCircleComponent(enemyClass._size * 0.5));
    unit->addComponent(new iQuadtreeComponent());

    iAnimationControllerPtr animationController(new iAnimationController());
    animationController->addClip(iClip::createClip({_bounceAnimation}, true, true));
    unit->addComponent(new iAnimationComponent(animationController));
    unit->addComponent(new HealthComponent(enemyClass._health));
    unit->addComponent(new TargetComponent(target));
    unit->addComponent(new ExperienceGainComponent(enemyClass._xpDrop));
    unit->addComponent(new DamageComponent(enemyClass._damage));
    unit->addBehaviour({this, &GameLayer::onCheckCollision});
    unit->addBehaviour({this, &GameLayer::onFollowTarget});
    // unit->setMotionInteractionType(iMotionInteractionType::Divert);

    // add shadow
    iEntityPtr shadow = _entityScene->createEntity("shadow");
    shadow->addComponent(new iTransformComponent(iaVector3d(0.0, enemyClass._size * 0.4, 0.0)));
    shadow->addComponent(new iSpriteRenderComponent(_shadow, iaVector2d(enemyClass._size * 0.25, enemyClass._size * 0.25), iaColor4f::black, -1));
    shadow->setParent(unit->getID());
}

void GameLayer::onSpawnStuff(const iaTime &time)
{
    iEntityPtr player = _entityScene->getEntity(_player);

    if (player == nullptr)
    {
        return;
    }

    auto playerTransformComponent = player->getComponent<iTransformComponent>();
    const auto &pPos = playerTransformComponent->getPosition();
    const iaVector2f playerPosition(pPos._x, pPos._y);

    auto playerXP = player->getComponent<ExperienceComponent>();

    // scaling all available enemies across 100 player levels
    const int32 playerLevel = playerXP->_level; //  * (_enemies.size() / 50.0);
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

    // con_endl("enemiesToCreate " << enemiesToCreate);

    for (int i = 0; i < enemiesToCreate; ++i)
    {
        iaVector2f pos(iaRandom::getNextFloat() - 0.5, iaRandom::getNextFloat() - 0.5);
        pos.normalize();
        pos *= PLAYFIELD_VIEWPORT_WIDTH * 0.5 + iaRandom::getNextFloat() * 100.0;
        pos += playerPosition;

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
        // con_endl("create enemy level: " << enemyLevel);
        createUnit(pos, FOE, _player, _enemies[enemyLevel - 1]);
    }

    // con_endl("minEnemyLevel " << minEnemyLevel);
    // con_endl("maxEnemyLevel " << maxEnemyLevel);
}

void GameLayer::doughnutQuery(const iaCircled &circle, std::vector<std::pair<iEntityID, iaVector2d>> &hits)
{
    iQuadtreed::Objects objects;
    _entityScene->getQuadtree().query(circle, objects);

    for (const auto &object : objects)
    {
        hits.emplace_back(std::pair<iEntityID, iaVector2d>(std::any_cast<iEntityID>(object->_userData), object->_circle._center - circle._center));
    }

    const bool right = circle._center._x - circle._radius < 0.0;
    const bool left = circle._center._x + circle._radius > _entityScene->getQuadtree().getArea()._width;
    const bool bottom = circle._center._y - circle._radius < 0.0;
    const bool top = circle._center._y + circle._radius > _entityScene->getQuadtree().getArea()._height;

    std::vector<iaCircled> additionalQueries;

    if (right || left)
    {
        iaCircled queryCircle = circle;
        if (right)
        {
            queryCircle._center._x += _entityScene->getQuadtree().getArea()._width;
        }
        if (left)
        {
            queryCircle._center._x -= _entityScene->getQuadtree().getArea()._width;
        }
        additionalQueries.push_back(queryCircle);
    }

    if (bottom || top)
    {
        iaCircled queryCircle = circle;
        if (bottom)
        {
            queryCircle._center._y += _entityScene->getQuadtree().getArea()._height;
        }
        if (top)
        {
            queryCircle._center._y -= _entityScene->getQuadtree().getArea()._height;
        }
        additionalQueries.push_back(queryCircle);
    }

    if (right || left && bottom || top)
    {
        iaCircled queryCircle = circle;
        if (right)
        {
            queryCircle._center._x += _entityScene->getQuadtree().getArea()._width;
        }
        if (left)
        {
            queryCircle._center._x -= _entityScene->getQuadtree().getArea()._width;
        }
        if (bottom)
        {
            queryCircle._center._y += _entityScene->getQuadtree().getArea()._height;
        }
        if (top)
        {
            queryCircle._center._y -= _entityScene->getQuadtree().getArea()._height;
        }
        additionalQueries.push_back(queryCircle);
    }

    for (const auto &queryCircle : additionalQueries)
    {
        _entityScene->getQuadtree().query(queryCircle, objects);

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
    const bool left = rectangle.getRight() > _entityScene->getQuadtree().getArea()._width;

    const bool bottom = rectangle._y < 0.0;
    const bool top = rectangle.getBottom() > _entityScene->getQuadtree().getArea()._height;

    std::vector<iaRectangled> additionalQueries;

    if (right || left)
    {
        iaRectangled queryRectangle = rectangle;
        if (right)
        {
            queryRectangle._x += _entityScene->getQuadtree().getArea()._width;
        }
        if (left)
        {
            queryRectangle._x -= _entityScene->getQuadtree().getArea()._width;
        }
        additionalQueries.push_back(queryRectangle);
    }

    if (bottom || top)
    {
        iaRectangled queryRectangle = rectangle;
        if (bottom)
        {
            queryRectangle._y += _entityScene->getQuadtree().getArea()._height;
        }
        if (top)
        {
            queryRectangle._y -= _entityScene->getQuadtree().getArea()._height;
        }
        additionalQueries.push_back(queryRectangle);
    }

    if (right || left && bottom || top)
    {
        iaRectangled queryRectangle = rectangle;
        if (right)
        {
            queryRectangle._x += _entityScene->getQuadtree().getArea()._width;
        }
        if (left)
        {
            queryRectangle._x -= _entityScene->getQuadtree().getArea()._width;
        }
        if (bottom)
        {
            queryRectangle._y += _entityScene->getQuadtree().getArea()._height;
        }
        if (top)
        {
            queryRectangle._y -= _entityScene->getQuadtree().getArea()._height;
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
    const bool left = circle._center._x + circle._radius > _entityScene->getQuadtree().getArea()._width;

    const bool bottom = circle._center._y - circle._radius < 0.0;
    const bool top = circle._center._y + circle._radius > _entityScene->getQuadtree().getArea()._height;

    std::vector<iaCircled> additionalQueries;

    if (right || left)
    {
        iaCircled queryCircle = circle;
        if (right)
        {
            queryCircle._center._x += _entityScene->getQuadtree().getArea()._width;
        }
        if (left)
        {
            queryCircle._center._x -= _entityScene->getQuadtree().getArea()._width;
        }
        additionalQueries.push_back(queryCircle);
    }

    if (bottom || top)
    {
        iaCircled queryCircle = circle;
        if (bottom)
        {
            queryCircle._center._y += _entityScene->getQuadtree().getArea()._height;
        }
        if (top)
        {
            queryCircle._center._y -= _entityScene->getQuadtree().getArea()._height;
        }
        additionalQueries.push_back(queryCircle);
    }

    if (right || left && bottom || top)
    {
        iaCircled queryCircle = circle;
        if (right)
        {
            queryCircle._center._x += _entityScene->getQuadtree().getArea()._width;
        }
        if (left)
        {
            queryCircle._center._x -= _entityScene->getQuadtree().getArea()._width;
        }
        if (bottom)
        {
            queryCircle._center._y += _entityScene->getQuadtree().getArea()._height;
        }
        if (top)
        {
            queryCircle._center._y -= _entityScene->getQuadtree().getArea()._height;
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

void GameLayer::onUpdateProjectileOrientation(iEntityPtr entity, std::any &userData)
{
    auto velocityComp = entity->getComponent<iVelocityComponent>();
    if(velocityComp->getAngularVelocity().zero())
    {
        return;
    }

    const auto &velocity = velocityComp->getVelocity();
    auto transform = entity->getComponent<iTransformComponent>();

    iaVector2d vel2D(velocity._x, velocity._y);
    transform->rotate(iaVector3d(0.0, 0.0, vel2D.angle() + (M_PI * 0.5)));
}

void GameLayer::onCheckCollision(iEntityPtr entity, std::any &userData)
{
    auto transform = entity->getComponent<iTransformComponent>();
    auto party = entity->getComponent<iPartyComponent>();
    auto damage = entity->getComponent<DamageComponent>();
    auto health = entity->getComponent<HealthComponent>();
    auto body = entity->getComponent<iQuadtreeComponent>();

    iQuadtreed::Objects objects;
    _entityScene->getQuadtree().query(body->_object->_circle, objects);

    iEntityPtr player = _entityScene->getEntity(_player);

    if (player == nullptr)
    {
        return;
    }

    for (const auto &object : objects)
    {
        const iEntityID otherEntityID = std::any_cast<iEntityID>(object->_userData);

        // skip self
        if (otherEntityID == entity->getID())
        {
            continue;
        }

        // get other entity
        iEntityPtr otherEntity = _entityScene->getEntity(otherEntityID);
        if (otherEntity == nullptr)
        {
            continue;
        }

        // check if we do damage to other entity
        auto otherEntityParty = otherEntity->getComponent<iPartyComponent>();
        if (otherEntityParty != nullptr)
        {
            if (otherEntityParty->_partyID != party->_partyID)
            {
                auto otherEntityHealth = otherEntity->getComponent<HealthComponent>();
                if (otherEntityHealth != nullptr)
                {
                    otherEntityHealth->_health -= damage->_damage;
                    if (otherEntityHealth->_health < 0.0)
                    {
                        _entityScene->destroyEntity(otherEntity);

                        if (otherEntityParty->_partyID == FOE)
                        {
                            const auto *exp = otherEntity->getComponent<ExperienceGainComponent>();
                            if (exp != nullptr)
                            {
                                addExperience(player, exp->_experience);
                            }

                            const auto &transformComponent = otherEntity->getComponent<iTransformComponent>();
                            const auto &transPos = transformComponent->getPosition();
                            createCoin(iaVector2f(transPos._x, transPos._y), NEUTRAL, ObjectType::Coin);
                        }
                    }
                }

                if (otherEntityParty->_partyID != NEUTRAL &&
                    health->_destroyOnImpact)
                {
                    health->_health = 0.0;
                    _entityScene->destroyEntity(entity);
                }
            }
        }
    }
}

void GameLayer::onUpdateRange(iEntityPtr entity, std::any &userData)
{
    auto transformComponent = entity->getComponent<iTransformComponent>();
    auto range = entity->getComponent<RangeComponent>();
    auto health = entity->getComponent<HealthComponent>();

    range->_rangeLeft -= range->_lastPosition.distance(transformComponent->getPosition());

    if (range->_rangeLeft < 0.0)
    {
        health->_health = 0.0;
        _entityScene->destroyEntity(entity);
    }
}

void GameLayer::fire(const iaVector2d &from, const iaVector2d &dir, uint32 party, const WeaponComponent *weapon, const ModifierComponent *modifier)
{
    // skip if out of range
    if (!iIntersection::intersects(from, _entityScene->getQuadtree().getArea()))
    {
        return;
    }

    for (int i = 0; i < weapon->_config._projectileCount; ++i)
    {
        auto bullet = _entityScene->createEntity(weapon->_config._texture);
        float32 angle = dir.angle() + (90 * IGOR_GRAD2RAD);
        iaVector2d firePosition(from + dir * weapon->_config._size * 0.5);
        iaVector3d bulletSpawnPosition(firePosition._x, firePosition._y, 0.0);
        bullet->addComponent(new iTransformComponent({bulletSpawnPosition, iaVector3d(0.0, 0.0, angle), iaVector3d(weapon->_config._size, weapon->_config._size, 1.0)}));

        float64 angularVelocity = weapon->_config._angularVelocity;

        if (angularVelocity != 0.0)
        {
            angularVelocity += (iaRandom::getNextFloat() - 0.5) * 0.2;
        }

        bullet->addComponent(new RangeComponent({weapon->_config._range * modifier->_config._projectileRangeFactor, bulletSpawnPosition}));

        iaVector2d dir2 = dir;
        dir2.rotateXY((iaRandom::getNextFloat() - 0.2) * weapon->_config._accuracy * 0.2);
        iaVector3d d(dir2._x, dir2._y, 0.0);
        float32 s = (weapon->_config._speed * modifier->_config._projectileSpeedFactor) + (weapon->_config._accuracy * (iaRandom::getNextFloat() - 0.5));
        d *= s;
        bullet->addComponent(new iVelocityComponent({d, iaVector3d(0.0, 0.0, angularVelocity)}));
        bullet->addComponent(new iPartyComponent({party}));

        iSpritePtr sprite = iResourceManager::getInstance().createResource<iSprite>();
        sprite->setTexture(iResourceManager::getInstance().requestResource<iTexture>(weapon->_config._texture));
        bullet->addComponent(new iSpriteRenderComponent(sprite));
        bullet->addComponent(new iCircleComponent(weapon->_config._size * 0.5));
        bullet->addComponent(new iQuadtreeComponent());
        bullet->addComponent(new DamageComponent(weapon->_config._damage * modifier->_config._damageFactor));
        bullet->addComponent(new HealthComponent(100.0f, true));
        bullet->addBehaviour({this, &GameLayer::onUpdateProjectileOrientation});
        bullet->addBehaviour({this, &GameLayer::onCheckCollision});
    }
}

void GameLayer::addExperience(iEntityPtr entity, float32 experience)
{
    auto comp = entity->getComponent<ExperienceComponent>();

    comp->_experience += experience;
    uint32 level = comp->_level;
    comp->_level = calcLevel(comp->_experience);

    if (level < uint32(comp->_level))
    {
        _levelUp = true;
    }
}

void GameLayer::onUpdate(const iaTime &time)
{
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
    _viewOrtho.getRenderEvent().remove({this, &GameLayer::onRenderOrtho});
}

void GameLayer::onUpdate()
{
}

void GameLayer::onEvent(iEvent &event)
{
    event.dispatch<iEventKeyDown>(IGOR_BIND_EVENT_FUNCTION(GameLayer::onKeyDown));
    event.dispatch<iEventKeyUp>(IGOR_BIND_EVENT_FUNCTION(GameLayer::onKeyUp));
}

float32 GameLayer::calcLevel(uint32 experience)
{
    uint32 lowerBounds = 0;
    uint32 upperBounds = _expLvl.front();

    float32 level = 0.0;

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

    level += float32(experience - lowerBounds) / float32(upperBounds - lowerBounds);

    return level;
}

void GameLayer::onRenderPlayerHUD()
{
    /*    if (!_player.isValid())
        {
            return;
        }

        auto &playerTransform = _player.getComponent<iTransformComponent>();
        auto &shopTransform = shop->getComponent<iTransformComponent>();
        if (shop->isActive())
        {
            // TODO respect the doughnut
            iaVector2f playerPosition(playerTransform->_position._x, playerTransform->_position._y);
            iaVector2f dir = iaVector2f(shopTransform._position._x, shopTransform._position._y) - playerPosition;
            dir.normalize();
            iaVector2f tangent(dir._y, -dir._x);

            std::vector<iaVector2f> points;
            points.push_back(playerPosition + dir * 100.0);
            points.push_back(playerPosition + dir * 95.0 + tangent * 5.0);
            points.push_back(playerPosition + dir * 95.0 - tangent * 5.0);

            iRenderer::getInstance().drawLineLoop(points, iaColor4f::green);
        }*/
}

void GameLayer::onRenderHUD()
{
    iEntityPtr player = _entityScene->getEntity(_player);

    if (player == nullptr)
    {
        return;
    }

    auto healthComp = player->getComponent<HealthComponent>();
    auto playerExperience = player->getComponent<ExperienceComponent>();
    auto playerCoins = player->getComponent<CoinsComponent>();
    auto modifiers = player->getComponent<ModifierComponent>();
    auto weapon = player->getComponent<WeaponComponent>();
    auto playerVelocity = player->getComponent<iVelocityComponent>();

    iRenderer::getInstance().setOrtho(0, 1920, 1080, 0, 0.001, 10.0);

    iaMatrixd matrix;
    iRenderer::getInstance().setViewMatrix(matrix);

    matrix.translate(0, 0, -1);
    iRenderer::getInstance().setModelMatrix(matrix);

    iRenderer::getInstance().setFont(_font);
    iRenderer::getInstance().setFontSize(20.0f);
    iRenderer::getInstance().drawString(10, 10, iaString::toString(healthComp->_health, 0));

    uint32 level = playerExperience->_level;
    float32 percentOfLevel = playerExperience->_level - float32(level);

    iRenderer::getInstance().drawString(10, 40, iaString("lvl: ") + iaString::toString(level));

    iRenderer::getInstance().drawRectangle(10.0f, 80.0f, 310.0f, 20.0f, iaColor4f::white);
    iRenderer::getInstance().drawFilledRectangle(10.0f, 80.0f, (300.0f * percentOfLevel) + 10.0f, 20.0f, iaColor4f::red);

    uint32 nextLevel = 0;
    for (auto val : _expLvl)
    {
        if (playerExperience->_experience < val)
        {
            nextLevel = val;
            break;
        }
    }
    iRenderer::getInstance().drawString(350, 80, iaString::toString((int)playerExperience->_experience) + " -> " + iaString::toString(nextLevel));

    /*iRenderer::getInstance().drawTexturedRectangle(10, 120, 40, 40, _coin, iaColor4f::white, true);
    iRenderer::getInstance().drawString(60, 130, iaString::toString(playerCoins._coins, 0));

    iRenderer::getInstance().drawTexturedRectangle(10, 160, 40, 40, _damage, iaColor4f::white, true);
    iRenderer::getInstance().drawString(60, 170, iaString::toString(weapon._damage * modifiers._damageFactor, 0));

    iRenderer::getInstance().drawTexturedRectangle(10, 200, 40, 40, _attackSpeed, iaColor4f::white, true);
    iRenderer::getInstance().drawString(60, 210, iaString::toString(1.0 / (weapon._attackInterval.getSeconds() / modifiers._attackIntervalFactor), 1));

    iRenderer::getInstance().drawTexturedRectangle(10, 240, 40, 40, _walkSpeed, iaColor4f::white, true);
    iRenderer::getInstance().drawString(60, 250, iaString::toString(modifiers._walkSpeedFactor, 1));*/
}

void GameLayer::onLevelUp()
{
    iApplication::getInstance().pause(true);

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

    iApplication::getInstance().pause(true);

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
    onLiftShop();
    iApplication::getInstance().pause(false);

    // start timer to next shop landing
    _spawnShopTimerHandle->start();

    if (!_shopDialog->bought() ||
        !_player.isValid())
    {
        return;
    }

    iEntityPtr player = _entityScene->getEntity(_player);

    const ShopItem &shopItem = _shopDialog->getSelection();
    auto coins = player->getComponent<CoinsComponent>();
    coins->_coins -= shopItem._price;

    switch (shopItem._type)
    {
    case ShopItemType::Weapon:
        auto weapon = player->getComponent<WeaponComponent>();
        weapon->_config = _weapons[shopItem._name];
        weapon->_time = iTimer::getInstance().getTime();
        break;
    }
}

void GameLayer::upgrade(iEntityPtr entity, const UpgradeConfiguration &upgradeConfiguration)
{
    if (entity == nullptr)
    {
        return;
    }

    auto modifierComponent = entity->getComponent<ModifierComponent>();

    modifierComponent->_config._damageFactor += upgradeConfiguration._damageFactor;
    modifierComponent->_config._attackIntervalFactor += upgradeConfiguration._attackIntervalFactor;
    modifierComponent->_config._criticalHitChanceFactor += upgradeConfiguration._criticalHitChanceFactor;
    modifierComponent->_config._criticalHitDamageFactor += upgradeConfiguration._criticalHitDamageFactor;
    modifierComponent->_config._splashDamageRangeFactor += upgradeConfiguration._splashDamageRangeFactor;
    modifierComponent->_config._walkSpeedFactor += upgradeConfiguration._walkSpeedFactor;
    modifierComponent->_config._projectileSpeedFactor += upgradeConfiguration._projectileSpeedFactor;
    modifierComponent->_config._projectileRangeFactor += upgradeConfiguration._projectileRangeFactor;
}

void GameLayer::onCloseLevelUpDialog(iDialogPtr dialog)
{
    upgrade(_entityScene->getEntity(_player), _levelUpDialog->getSelection());
    iApplication::getInstance().pause(false);
}

void GameLayer::onRenderOrtho()
{
    onRenderHUD();
}

bool GameLayer::onKeyDown(iEventKeyDown &event)
{
    switch (event.getKey())
    {
    case iKeyCode::ESC:
        iApplication::getInstance().exit();
        return true;
    }

    return false;
}

bool GameLayer::onKeyUp(iEventKeyUp &event)
{
    switch (event.getKey())
    {
    case iKeyCode::Space:
        iApplication::getInstance().pause(!iApplication::getInstance().isPaused());
        return true;
    }

    return false;
}