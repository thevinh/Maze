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
#include "MediumScene.h"

using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

CCScene* Level1Scene::scene(int lvl)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Level1Scene *layer = Level1Scene::create(lvl);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Level1Scene::init(int lvl)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    _lvl = lvl;
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    SIZE_RATIO = (screenSize.width + screenSize.height) / (640 + 960);
    SIZE_RATIO_X = screenSize.width /  960;
    SIZE_RATIO_Y = screenSize.height / 640;
    // init the picture for exit door
    CCSprite* exitSprite = CCSprite::create("lvl1/exit.png");
    exitSprite->setPosition(ccp(exitSprite->getContentSize().width/2,screenSize.height - exitSprite->getContentSize().height/2));
    this->addChild(exitSprite);
    
    // init the map
    tileMap = new CCTMXTiledMap();
    char tileMapPath[15] = {0};
    sprintf(tileMapPath, "lvl%d/map.tmx", lvl);
    tileMap->initWithTMXFile(tileMapPath);
    tileMap->setPosition(0* SIZE_RATIO_X, 0*SIZE_RATIO_Y);
    background = tileMap->layerNamed("Background");
    // scale
//    background->setPosition(0, 0);
    background->setScaleX(SIZE_RATIO_X);
    background->setScaleY(SIZE_RATIO_Y);
    this->addChild(tileMap);
    
    walls = tileMap->layerNamed("Walls");
    //scale
    walls->setScaleX(SIZE_RATIO_X);
    walls->setScaleY(SIZE_RATIO_Y);
    walls->setVisible(true);
    
    // add player
    CCTMXObjectGroup *objectGroup = tileMap->objectGroupNamed("Objects");
    if (objectGroup == NULL) {
        CCLog("tile map has no objects object layer");
        return false;
    }
    
    //init exit point
    // tile map position has multiplied with the ratio so when we calculate player and
    // npc1 position, we just plus with tile map position, we
    // dont need to multiplied wih the ratio anymore
    CCDictionary *exitPointDic = objectGroup->objectNamed("ExitPoint");
    int x2 = ((CCString)*exitPointDic->valueForKey("x")).intValue();
    int y2 =  ((CCString)*exitPointDic->valueForKey("y")).intValue();
//    exitPointCoord = tileCoordForPosition(ccp(x2,y2));
    exitPointCoord = tileCoordForPosition(ccp(x2*SIZE_RATIO_X + tileMap->getPositionX(), y2*SIZE_RATIO_Y + tileMap->getPositionY()));
    
    // init player: position and charWall at this position
    CCDictionary *playerSpawnPoint = objectGroup->objectNamed("PlayerSpawnPoint");
    int x = ((CCString)*playerSpawnPoint->valueForKey("x")).intValue();
    int y = ((CCString)*playerSpawnPoint->valueForKey("y")).intValue();
    
    player = (Player*)GameSprite::gameSpriteWithFile("lvl1/player.png");
//    player->setPosition(ccp(x, y));
    player->setPosition(ccp(x*SIZE_RATIO_X + tileMap->getPositionX(), y*SIZE_RATIO_Y + tileMap->getPositionY()));
    player->setScaleX(SIZE_RATIO_X);
    player->setScaleY(SIZE_RATIO_Y);
    this->addChild(player);
    
    CCPoint tileCoord = this->tileCoordForPosition(player->getPosition());
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
//    npc1->setPosition(ccp(x1, y1));
    npc1->setPosition(ccp(x1*SIZE_RATIO_X + tileMap->getPositionX(), y1*SIZE_RATIO_Y + tileMap->getPositionY()));
    npc1->setScaleX(SIZE_RATIO_X);
    npc1->setScaleY(SIZE_RATIO_Y);
    this->addChild(npc1);
    tileCoord = this->tileCoordForPosition(npc1->getPosition());
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
    int x = (position.x - tileMap->getPositionX())/ tileMap->getTileSize().width / SIZE_RATIO_X;
    int y = ((tileMap->getMapSize().height * tileMap->getTileSize().height * SIZE_RATIO_Y) - position.y + tileMap->getPositionY()) / tileMap->getTileSize().height / SIZE_RATIO_Y;
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
            // with the map that the exit door is in the boundary
            if (exitPointCoord.y == 0) {
                player->runAction(CCSequence::create(CCDelayTime::create(0.2f),
                                                     CCMoveBy::create(0.3f, ccp(0,tileMap->getTileSize().height)),
                                                     NULL));
            } else {
                if (exitPointCoord.y == tileMap->getMapSize().height - 1) {
                    player->runAction(CCSequence::create(CCDelayTime::create(0.2f),
                                                         CCMoveBy::create(0.3f, ccp(0, -tileMap->getTileSize().height)),
                                                         NULL));
                }
                else {
                    if (exitPointCoord.x == 0) {
                        player->runAction(CCSequence::create(CCDelayTime::create(0.2f),
                                                             CCMoveBy::create(0.3f, ccp(-tileMap->getTileSize().width,0)),
                                                             NULL));

                    } else {
                        player->runAction(CCSequence::create(CCDelayTime::create(0.2f),
                                                             CCMoveBy::create(0.3f, ccp(tileMap->getTileSize().width,0)),
                                                             NULL));
                    }
                }
            }
            
            this->runAction(CCSequence::create(CCDelayTime::create(0.3f + 0.2f),
                                               CCCallFunc::create(this, callfunc_selector(Level1Scene::changeScene)),
                                               NULL));
            
        }
        this->unschedule(schedule_selector(Level1Scene::update));
    }
}

void Level1Scene::changeScene(){
    CCLog("Win cmnr");
    CCScene *s = MediumScene::scene(_lvl);
    CCDirector::sharedDirector()->setDepthTest(true);
    
    CCTransitionScene *transition = CCTransitionFade::create(2.0f, s);
    
    CCDirector::sharedDirector()->replaceScene(transition);

}

Level1Scene::~Level1Scene(){
    
}

Level1Scene* Level1Scene::create(int lvl){
    Level1Scene *pRet = new Level1Scene();
    if (pRet && pRet->init(lvl)) {
        pRet->autorelease();
        return pRet;
    }
    else{
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}
