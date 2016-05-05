#include "BattleScene.h"
#include "TerrainLayer.h"
#include "WaterLayer.h"
#include "LifeBarLayer.h"
#include "TerritoryLayer.h"

USING_NS_CC;

Scene* BattleScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = BattleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BattleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Vec2 centering = 
        Vec2(
            visibleSize.width / 2 + origin.x, 
            visibleSize.height / 2 + origin.y);

    this->setPosition(centering);
    this->scheduleUpdate();

    /////////////////////////////
    // 3. add your codes below...
/*
    auto sprite = Sprite::create("BattleScene.png");
    sprite->setPosition(Vec2::ZERO);
    this->addChild(sprite, 0);
*/

    auto solidBackgroundLayer = LayerColor::create(Color4B::WHITE, visibleSize.width, visibleSize.height);
    this->addChild(solidBackgroundLayer);
    solidBackgroundLayer->setPosition(-centering);

    board_.setup();

    //マウスイベント設定
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](Event* event) {
        auto mouse = (EventMouse*)event;
        float x = mouse->getCursorX() - centering.x;
        float y = mouse->getCursorY() - centering.y;
        log("%f %f", x, y);
        player_.tap(Vec2(x, y));
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(
        mouseListener, this);

    auto terrainLayer = TerrainLayer::create();
    auto terrain = board_.terrain();
    addChild(terrainLayer);
    
    terrainLayer->render(
        board_.pathview(), 
        cocos2d::Vec2(0, 0),
        board_.pathview().size());

    auto waterLayer = WaterLayer::create();
    waterLayer->water = &board_.water();
    addChild(waterLayer);

    lifeBarLayer_ = LifeBarLayer::create();
    lifeBarLayer_->setValue(1.0f);
    lifeBarLayer_->setPosition(cocos2d::Vec2(280, 0));
    addChild(lifeBarLayer_);

    auto alphaTerritoryLayer = TerritoryLayer::create();
    alphaTerritoryLayer->setTeam(
        board_.team(TeamTag::Alpha).get(),
        cocos2d::Color4F(0.1f, 0, 1.0f, 0.1f));
    addChild(alphaTerritoryLayer);
    auto betaTerritoryLayer = TerritoryLayer::create();
    betaTerritoryLayer->setTeam(
        board_.team(TeamTag::Beta).get(),
        cocos2d::Color4F(0.8f, 0, 1.0f, 0.1f));
    addChild(betaTerritoryLayer);

    return true;
}


void BattleScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void BattleScene::update(float delta) {
    // log("update");
    board_.update(delta);
    ai_.think(delta);
    lifeBarLayer_->setValue(board_.team(TeamTag::Alpha)->energy());
}
