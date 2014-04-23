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
#include <queue>

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
	int abcdef = 6;
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
    walls->setVisible(false);
    
    // add player
    CCTMXObjectGroup *objectGroup = tileMap->objectGroupNamed("Objects");
    if (objectGroup == NULL) {
        CCLog("tile map has no objects object layer");
        return false;
    }
    
    
    // tile map position has multiplied with the ratio so when we calculate player and
    // npc1 position, we just plus with tile map position, we
    // dont need to multiplied wih the ratio anymore
    
    //init exit point
    CCDictionary *exitPointDic = objectGroup->objectNamed("ExitPoint");
    int xE = ((CCString)*exitPointDic->valueForKey("x")).intValue();
    int yE =  ((CCString)*exitPointDic->valueForKey("y")).intValue();
//    exitPointCoord = tileCoordForPosition(ccp(x2,y2));
    CCSprite *exitPointImg = CCSprite::create("lvl1/exitpoint.png");
    exitPointImg->setPosition(ccp(xE*SIZE_RATIO_X + tileMap->getPositionX(), yE*SIZE_RATIO_Y + tileMap->getPositionY()));
    exitPointImg->setScaleX(SIZE_RATIO_X);
    exitPointImg->setScaleY(SIZE_RATIO_Y);
    this->addChild(exitPointImg);
    exitPointCoord = tileCoordForPosition(ccp(xE*SIZE_RATIO_X + tileMap->getPositionX(), yE*SIZE_RATIO_Y + tileMap->getPositionY()));
    
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
    player->setIsMove(false);

    // init NPC1
    CCDictionary *npc1SpawnPoint = objectGroup->objectNamed("NPC1SpawnPoint");
    if (npc1SpawnPoint != NULL) {
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
        npc1->setIsMove(false);
    }

    // init npc 2
    CCDictionary *npc2SpawnPoint = objectGroup->objectNamed("NPC2SpawnPoint");
    if (npc2SpawnPoint != NULL) {
        int x2 = ((CCString)*npc2SpawnPoint->valueForKey("x")).intValue();
        int y2 = ((CCString)*npc2SpawnPoint->valueForKey("y")).intValue();
        
        npc2 = (NPC2*)GameSprite::gameSpriteWithFile("lvl1/redmummy.png");
        //    npc1->setPosition(ccp(x1, y1));
        npc2->setPosition(ccp(x2*SIZE_RATIO_X + tileMap->getPositionX(), y2*SIZE_RATIO_Y + tileMap->getPositionY()));
        npc2->setScaleX(SIZE_RATIO_X);
        npc2->setScaleY(SIZE_RATIO_Y);
        this->addChild(npc2);
        tileCoord = this->tileCoordForPosition(npc2->getPosition());
        tileGid = walls->tileGIDAt(tileCoord);
        if (tileGid) {
            CCDictionary *properties = tileMap->propertiesForGID(tileGid);
            if (properties) {
                CCString *wall = new CCString();
                *wall = *properties->valueForKey("Wall");
                CCString abc = *wall;
                CCLog("%d", abc.intValue());
                npc2->setCharWall(abc.intValue());
            }
        }
        npc2->setIsMove(false);
    }
    
    // init tile array
    int mazeRow = tileMap->getMapSize().height;
    int mazeCol = tileMap->getMapSize().width;
    if (true) {
        tileArray = new TileObject * [mazeRow];
        for (int i = 0; i < mazeRow; i++) {
            tileArray[i] = new TileObject [mazeCol];
            tileArray[i]->retain();
        }
        
        for (int i = 0; i < mazeRow; i++) {
            for (int j = 0; j < mazeCol; j++) {
                // wall
                tileGid = walls->tileGIDAt(ccp(i, j));
                if (tileGid) {
                    CCDictionary *properties = tileMap->propertiesForGID(tileGid);
                    if (properties) {
                        CCString *wall = new CCString();
                        *wall = *properties->valueForKey("Wall");
                        CCString abc = *wall;
                        tileArray[i][j] = TileObject(i, j, abc.intValue());
                        CCLog("Wall properties cua tilearray i: %d j: %d la x: %d , y: %d la %d",i,j, tileArray[i][j].getXCoord(), tileArray[i][j].getYCoord(), tileArray[i][j].getWall());
                    }
                }
            }
        }
    }
    mapSize = tileMap->getMapSize();
    // init reset button
    resetButton = CCSprite::create("lvl1/resetButton.png");
    resetButton->cocos2d::CCNode::setPosition(screenSize.width - resetButton->getContentSize().width/2, resetButton->getContentSize().height/2);
    this->addChild(resetButton);
    
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
    if (resetButton->boundingBox().containsPoint(touchLocation)) {
        CCLog("touch reset button");
        Level1Scene::resetMap();
    }
    else if (!player->getIsMove() && !isGameOver
        && ( (npc1 == NULL) || (npc1 != NULL && !npc1->getIsMove()) )
        && ( (npc2 == NULL) || (npc2 != NULL && !npc2->getIsMove()) )
        ) {
        player->makeMove(touchLocation, tileMap, walls);
        this->schedule(schedule_selector(Level1Scene::update) , 0.3f);
        
        // test next function
        CCPoint touchCoord = tileCoordForPosition(touchLocation);
        TileObject* u = &tileArray[(int)touchCoord.x][(int)touchCoord.y];
        CCArray* nextTileArray = next(u, tileArray, mapSize);
        CCObject *obj;
        CCARRAY_FOREACH(nextTileArray, obj){
            TileObject* u = dynamic_cast<TileObject*>(obj);
            CCLog("Wall properties trong next array cua x: %d , y: %d la %d", u->getXCoord(), u->getYCoord(), u->getWall());
        }
    }
    
    
}

void Level1Scene::update(float dt){
    if (!player->getIsMove()
        && ( (npc1 == NULL) || (npc1 != NULL && !npc1->getIsMove()) )
        && ( (npc2 == NULL) || (npc2 != NULL && !npc2->getIsMove()) )
        ) {
//        
//        // test tile array
//        int mazeRow = tileMap->getMapSize().height;
//        int mazeCol = tileMap->getMapSize().width;
//        for (int i = 0; i < mazeRow; i++ ) {
//            for (int j = 0; j < mazeCol; j++) {
////                int wall = tileArray[i][j]->getWall();
////                CCLog("Wall properties cua x: %d , y: %d la %d", j,i, wall);
//            }
//        }
        
        isGameOver = (npc1->makeMove(player->getPosition(), tileMap, walls) | npc2->makeMove(player->getPosition(), tileMap, walls));
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

void Level1Scene::resetMap(){
    isGameOver = false;
    CCTMXObjectGroup *objectGroup = tileMap->objectGroupNamed("Objects");
    
    // reset player position, charWall, move status
    player->stopAllActions();
    CCDictionary *playerSpawnPoint = objectGroup->objectNamed("PlayerSpawnPoint");
    int x = ((CCString)*playerSpawnPoint->valueForKey("x")).intValue();
    int y = ((CCString)*playerSpawnPoint->valueForKey("y")).intValue();
    player->setPosition(ccp(x*SIZE_RATIO_X + tileMap->getPositionX(), y*SIZE_RATIO_Y + tileMap->getPositionY()));
    player->setIsMove(false);
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
    
    if (npc1 != NULL) {
        npc1->stopAllActions();
        CCDictionary *npc1SpawnPoint = objectGroup->objectNamed("NPC1SpawnPoint");
        int x1 = ((CCString)*npc1SpawnPoint->valueForKey("x")).intValue();
        int y1 = ((CCString)*npc1SpawnPoint->valueForKey("y")).intValue();
        npc1->setPosition(ccp(x1*SIZE_RATIO_X + tileMap->getPositionX(), y1*SIZE_RATIO_Y + tileMap->getPositionY()));
        npc1->setIsMove(false);
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
    }
    
    if (npc2 != NULL) {
        npc2->stopAllActions();
        CCDictionary *npc2SpawnPoint = objectGroup->objectNamed("NPC2SpawnPoint");
        int x2 = ((CCString)*npc2SpawnPoint->valueForKey("x")).intValue();
        int y2 = ((CCString)*npc2SpawnPoint->valueForKey("y")).intValue();
        npc2->setPosition(ccp(x2*SIZE_RATIO_X + tileMap->getPositionX(), y2*SIZE_RATIO_Y + tileMap->getPositionY()));
        npc2->setIsMove(false);
        tileCoord = this->tileCoordForPosition(npc2->getPosition());
        tileGid = walls->tileGIDAt(tileCoord);
        if (tileGid) {
            CCDictionary *properties = tileMap->propertiesForGID(tileGid);
            if (properties) {
                CCString *wall = new CCString();
                *wall = *properties->valueForKey("Wall");
                CCString abc = *wall;
                CCLog("%d", abc.intValue());
                npc2->setCharWall(abc.intValue());
            }
        }
    }
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

CCArray* Level1Scene::next(TileObject* u, TileObject **tileArray, CCSize mapSize){
    CCArray* nextArray = new CCArray;
    nextArray->autorelease();
    int x = u->getXCoord();
    int y = u->getYCoord();
    // left
    int x1, y1;
    x1 = x - 1;
    y1 = y;
    if ( (x1 >= 0 && x1 < mapSize.width) &&
         (y1 >= 0 && y1 < mapSize.height) &&
         (tileArray[x1][y1].getWall() % 2) == 0) {
        TileObject* tile = &tileArray[x1][y1];
        nextArray->addObject(tile);
    }
    
    //up
    x1 = x;
    y1 = y -1;
    if ( (x1 >= 0 && x1 < mapSize.width) &&
        (y1 >= 0 && y1 < mapSize.height) &&
        tileArray[x1][y1].getWall() < 2 ) {
        TileObject* tile = &tileArray[x1][y1];
        nextArray->addObject(tile);
    }
    
    //right
    x1 = x + 1;
    y1 = y;
    if ( (x1 >= 0 && x1 < mapSize.width) &&
        (y1 >= 0 && y1 < mapSize.height) &&
        (u->getWall() % 2) == 0 ) {
        TileObject* tile = &tileArray[x1][y1];
        nextArray->addObject(tile);
    }
    
    //down
    x1 = x;
    y1 = y + 1;
    if ( (x1 >= 0 && x1 < mapSize.width) &&
        (y1 >= 0 && y1 < mapSize.height) &&
         (u->getWall() < 2) ) {
        TileObject* tile = &tileArray[x1][y1];
        nextArray->addObject(tile);
    }

    
    return nextArray;
}

std::queue<TileObject*>* Level1Scene::breadthFirstSearch(cocos2d::CCPoint playerPos, cocos2d::CCPoint npcPos, TileObject **tileArray, cocos2d::CCSize mapSize){
    CCPoint npcCoord = tileCoordForPosition(npcPos);
    CCPoint playerCoord = tileCoordForPosition(playerPos);
    std::queue<TileObject*>* listCheckedTiles = new std::queue<TileObject*>;
    TileObject* u = &tileArray[(int)npcCoord.x][(int)npcCoord.y];
    u->setPreTile(NULL);
    u->setIsCheck(true);
    listCheckedTiles->push(u);
    while (!listCheckedTiles->empty()) {
        TileObject* v = listCheckedTiles->front();
        CCArray* nextArrayTile = next(v, tileArray, mapSize);
        CCObject* obj;
        CCARRAY_FOREACH(nextArrayTile, obj){
            TileObject* u = dynamic_cast<TileObject*>(obj);
            if (u->getIsCheck() == false) {
                u->setIsCheck(true);
                u->setPreTile(v);
                listCheckedTiles->push(u);
                if (u->getXCoord() == playerCoord.x && u->getYCoord() == playerCoord.y) {
                    return listCheckedTiles;
                }
            }
        }
        
    }
    return NULL;
}
