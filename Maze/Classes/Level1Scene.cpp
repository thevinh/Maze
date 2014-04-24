//
//  Level1Scene.cpp
//  Maze
//
//  Created by Nguyen The Vinh on 9/30/13.
//
//

#include "Level1Scene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Player.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* Level1Scene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Level1Scene *layer = Level1Scene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Level1Scene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    // init the picture for exit door
    CCSprite* exitSprite = CCSprite::create("lvl1/exit.png");
    exitSprite->setPosition(ccp(exitSprite->getContentSize().width/2,screenSize.height - exitSprite->getContentSize().height/2));
    this->addChild(exitSprite);
    
    // init the map
    tileMap = new CCTMXTiledMap();
    tileMap->initWithTMXFile("lvl1/map.tmx");
    background = tileMap->layerNamed("Background");
    this->addChild(tileMap);
    
    walls = tileMap->layerNamed("Walls");
    walls->setVisible(false);
    
    // add player
    CCTMXObjectGroup *objectGroup = tileMap->objectGroupNamed("Objects");
    if (objectGroup == NULL) {
        CCLog("tile map has no objects object layer");
        return false;
    }
    
    //init exit point
    CCDictionary *exitPointDic = objectGroup->objectNamed("exitPoint");
    int x2 = ((CCString)*exitPointDic->valueForKey("x")).intValue();
    int y2 =  ((CCString)*exitPointDic->valueForKey("y")).intValue();
    exitPointCoord = tileCoordForPosition(ccp(x2,y2));
    
    // init player: position and charWall at this position
    CCDictionary *playerSpawnPoint = objectGroup->objectNamed("PlayerSpawnPoint");
    int x = ((CCString)*playerSpawnPoint->valueForKey("x")).intValue();
    int y = ((CCString)*playerSpawnPoint->valueForKey("y")).intValue();
    
    player = (Player*)GameSprite::gameSpriteWithFile("lvl1/player.png");
    player->setPosition(ccp(x, y));
    this->addChild(player);
    
    CCPoint tileCoord = this->tileCoordForPosition(ccp(x, y));
    int tileGid = walls->tileGIDAt(tileCoord);
    if (tileGid) {
        CCDictionary *properties = tileMap->propertiesForGID(tileGid);
        if (properties) {
            CCString *wall = new CCString();
            *wall = *properties->valueForKey("Wall");
            CCString abc = *wall;
            CCLog("%d", abc.intValue());
            player->setCharWall(abc.intValue());
        }
    }

    // init NPC
    CCDictionary *npc1SpawnPoint = objectGroup->objectNamed("NPC1SpawnPoint");
    int x1 = ((CCString)*npc1SpawnPoint->valueForKey("x")).intValue();
    int y1 = ((CCString)*npc1SpawnPoint->valueForKey("y")).intValue();
    
    npc1 = (NPC1*)GameSprite::gameSpriteWithFile("lvl1/mummy.png");
    npc1->setPosition(ccp(x1, y1));
    this->addChild(npc1);
    tileCoord = this->tileCoordForPosition(ccp(x1, y1));
    tileGid = walls->tileGIDAt(tileCoord);
    if (tileGid) {
        CCDictionary *properties = tileMap->propertiesForGID(tileGid);
        if (properties) {
            CCString *wall = new CCString();
            *wall = *properties->valueForKey("Wall");
            CCString abc = *wall;
            CCLog("%d", abc.intValue());
            npc1->setCharWall(abc.intValue());
        }
    }

    
    player->setIsMove(false);
    npc1->setIsMove(false);
    // init
    this->setTouchEnabled(true);
    return true;
}

void Level1Scene::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

CCPoint Level1Scene::tileCoordForPosition(cocos2d::CCPoint position){
    int x = position.x/ tileMap->getTileSize().width;
    int y = ((tileMap->getMapSize().height * tileMap->getTileSize().height) - position.y) / tileMap->getTileSize().height;
    return ccp(x, y);
}

bool Level1Scene::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event){
    return true;
}

void Level1Scene::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event){
    CCPoint touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    touchLocation = this->convertToNodeSpace(touchLocation);
    if (!player->getIsMove() && !npc1->getIsMove() && !isGameOver) {
        player->makeMove(touchLocation, tileMap, walls);
        this->schedule(schedule_selector(Level1Scene::update) , 0.3f);
    }
}

void Level1Scene::update(float dt){
    if (!player->getIsMove() && !npc1->getIsMove()) {
        isGameOver = npc1->makeMove(player->getPosition(), tileMap, walls);
        if (!isGameOver && tileCoordForPosition(player->getCharPosition()).x == exitPointCoord.x
                        && tileCoordForPosition(player->getCharPosition()).y == exitPointCoord.y) {
            isGameOver = true;
            player->runAction(CCSequence::create(CCDelayTime::create(0.2f),
                                                 CCMoveBy::create(0.3f, ccp(0,tileMap->getTileSize().height)),
                                                 NULL));
            CCLog("Win cmnr");
            
        }
        
        this->unschedule(schedule_selector(Level1Scene::update));
    }
}
