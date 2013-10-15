//
//  TestScene.cpp
//  Maze
//
//  Created by Nguyen The Vinh on 10/14/13.
//
//

#include "TestScene.h"
#include "cocos2d.h"
#include "Player.h"
#include <queue>
#include <stack>

using namespace std;
using namespace cocos2d;

TestScene::~TestScene(){
    
}

CCPoint TestScene::tileCoordForPosition(cocos2d::CCPoint position, cocos2d::CCTMXTiledMap *tileMap){
    int x = (position.x - tileMap->getPositionX())/ tileMap->getTileSize().width;
    int y = ((tileMap->getMapSize().height * tileMap->getTileSize().height) - position.y + tileMap->getPositionY()) / tileMap->getTileSize().height;
    return ccp(x, y);
}

CCScene* TestScene::scene(){
    CCScene *scene = CCScene::create();
    TestScene *layer = TestScene::create();
    scene->addChild(layer);
    return scene;
}

bool TestScene::init(){
    if (!CCLayer::init()) {
        return false;
    }
    tileMap = new CCTMXTiledMap();
    tileMap->initWithTMXFile("lvl5/map2.tmx");
    background = tileMap->layerNamed("Background");
    this->addChild(tileMap);
    walls = tileMap->layerNamed("Walls");
    walls->setVisible(false);
    CCTMXObjectGroup *objectGroup = tileMap->objectGroupNamed("Objects");
    if (objectGroup == NULL) {
        CCLog("tile map has no objects layer");
        return false;
    }
    
    CCDictionary *exitPointDic = objectGroup->objectNamed("ExitPoint");
    int xE = ((CCString)*exitPointDic->valueForKey("x")).intValue();
    int yE =  ((CCString)*exitPointDic->valueForKey("y")).intValue();
    //    exitPointCoord = tileCoordForPosition(ccp(x2,y2));
    CCSprite *exitPointImg = CCSprite::create("lvl5/exitpoint.png");
    exitPointImg->setPosition(ccp(xE + tileMap->getPositionX(), yE + tileMap->getPositionY()));
    this->addChild(exitPointImg);
    exitPointCoord = tileCoordForPosition(ccp(xE + tileMap->getPositionX(), yE + tileMap->getPositionY()), tileMap);
    
    // init player
    CCDictionary *playerSpawnPoint = objectGroup->objectNamed("PlayerSpawnPoint");
    int x = ((CCString)*playerSpawnPoint->valueForKey("x")).intValue();
    int y = ((CCString)*playerSpawnPoint->valueForKey("y")).intValue();
    player = (Player*)GameSprite::gameSpriteWithFile("lvl5/player.png");
    //    player->setPosition(ccp(x, y));
    player->setPosition(ccp(x + tileMap->getPositionX(), y + tileMap->getPositionY()));
    this->addChild(player);
    
    CCPoint tileCoord = this->tileCoordForPosition(player->getPosition(), tileMap);
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
    
    CCDictionary *npc3SpawnPoint = objectGroup->objectNamed("NPC3SpawnPoint");
    if (npc3SpawnPoint != NULL) {
        int x3 = ((CCString)*npc3SpawnPoint->valueForKey("x")).intValue();
        int y3 = ((CCString)*npc3SpawnPoint->valueForKey("y")).intValue();
        
        npc3 = GameSprite::gameSpriteWithFile("lvl5/npc3.png");
        //    npc1->setPosition(ccp(x1, y1));
        npc3->setPosition(ccp(x3 + tileMap->getPositionX(), y3 + tileMap->getPositionY()));
        this->addChild(npc3);
        tileCoord = this->tileCoordForPosition(npc3->getPosition(), tileMap);
        tileGid = walls->tileGIDAt(tileCoord);
        if (tileGid) {
            CCDictionary *properties = tileMap->propertiesForGID(tileGid);
            if (properties) {
                CCString *wall = new CCString();
                *wall = *properties->valueForKey("Wall");
                CCString abc = *wall;
                CCLog("%d", abc.intValue());
                npc3->setCharWall(abc.intValue());
            }
        }
        npc3->setIsMove(false);
    }
    
    // init tile array
    mazeRow = tileMap->getMapSize().height;
    mazeCol = tileMap->getMapSize().width;
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
                    }
                }
            }
        }
    }
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    mapSize = tileMap->getMapSize();
    
    resetButton = CCSprite::create("lvl1/resetButton.png");
    resetButton->cocos2d::CCNode::setPosition(screenSize.width - resetButton->getContentSize().width/2, resetButton->getContentSize().height/2);
    this->addChild(resetButton);
    
    findButton = CCSprite::create("lvl5/findbutton.jpg");
    findButton->cocos2d::CCNode::setPosition(screenSize.width - findButton->getContentSize().width/2, screenSize.height - findButton->getContentSize().height/2);
    this->addChild(findButton);


    this->setTouchEnabled(true);
    return true;
}

void TestScene::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool TestScene::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event){
    return true;
}

void TestScene::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event){
    CCPoint touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    touchLocation = this->convertToNodeSpace(touchLocation);
    if (resetButton->boundingBox().containsPoint(touchLocation)) {
        CCLog("touch reset button");
        TestScene::resetMap();
    }
    else if(findButton->boundingBox().containsPoint(touchLocation)){
        findpathTouch();
        
    }else    if (!player->getIsMove() && !isGameOver) {
        player->makeMove(touchLocation, tileMap, walls);
    }

}

void TestScene::update(float dt){
    if (!player->getIsMove()) {
        resetTileArray();
        breadthFirstSearch(player->getPosition(), npc3->getPosition(), tileArray, mapSize);
        std::stack<TileObject*>* path = findPath(player->getPosition());
        int n = 1;
        while (!path->empty()) {
            TileObject* u = path->top();
            CCLog("toa do tile thu: %d trong stack la x: %d va y: %d", n, u->getXCoord(), u->getYCoord());
            n++;
            path->pop();
        }
        this->unschedule(schedule_selector(TestScene::update));
    }
}

CCArray* TestScene::next(TileObject *u, TileObject **tileArray, cocos2d::CCSize mapSize){
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

void TestScene::breadthFirstSearch(cocos2d::CCPoint playerPos, cocos2d::CCPoint npcPos, TileObject **tileArray, cocos2d::CCSize mapSize){
    CCPoint npcCoord = tileCoordForPosition(npcPos, tileMap);
    CCPoint playerCoord = tileCoordForPosition(playerPos, tileMap);
    std::queue<TileObject*>* listCheckedTiles = new std::queue<TileObject*>;
    TileObject* u = &tileArray[(int)npcCoord.x][(int)npcCoord.y];
    u->setPreTile(NULL);
    u->setIsCheck(true);
    CCArray* nextArrayTile = new CCArray;
    nextArrayTile->autorelease();
    listCheckedTiles->push(u);
    while (!listCheckedTiles->empty()) {
        TileObject* v = listCheckedTiles->front();
        listCheckedTiles->pop();
        nextArrayTile = next(v, tileArray, mapSize);
        CCObject* obj;
        CCARRAY_FOREACH(nextArrayTile, obj){
            TileObject* u = dynamic_cast<TileObject*>(obj);
            if (u->getIsCheck() == false) {
                u->setIsCheck(true);
                u->setPreTile(v);
                listCheckedTiles->push(u);
                if (u->getXCoord() == playerCoord.x && u->getYCoord() == playerCoord.y) {
                    return;
                }
            }
        }
        
    }
}

std::stack<TileObject*>* TestScene::findPath(cocos2d::CCPoint playerPos){
    CCPoint playerCoord = tileCoordForPosition(playerPos, tileMap);
    TileObject* s = &tileArray[(int)playerCoord.x][(int)playerCoord.y];
    std::stack<TileObject*> *path = new std::stack<TileObject*>();
    path->push(s);
    TileObject* v = s->getPreTile();
    while (v != NULL) {
        path->push(v);
        v = v->getPreTile();
    }
    return path;
}

void TestScene::resetTileArray(){
    for (int i = 0; i < mazeRow; i++) {
        for (int j = 0; j < mazeCol; j++) {
            tileArray[i][j].setPreTile(NULL);
            tileArray[i][j].setIsCheck(false);
        }
    }

}

void TestScene::resetMap(){
    isGameOver = false;
    CCTMXObjectGroup *objectGroup = tileMap->objectGroupNamed("Objects");
    
    // reset player position, charWall, move status
    player->stopAllActions();
    CCDictionary *playerSpawnPoint = objectGroup->objectNamed("PlayerSpawnPoint");
    int x = ((CCString)*playerSpawnPoint->valueForKey("x")).intValue();
    int y = ((CCString)*playerSpawnPoint->valueForKey("y")).intValue();
    player->setPosition(ccp(x + tileMap->getPositionX(), y + tileMap->getPositionY()));
    player->setIsMove(false);
    CCPoint tileCoord = this->tileCoordForPosition(player->getPosition(), tileMap);
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

    if (npc3 != NULL) {
        npc3->stopAllActions();
        CCDictionary *npc1SpawnPoint = objectGroup->objectNamed("NPC3SpawnPoint");
        int x1 = ((CCString)*npc1SpawnPoint->valueForKey("x")).intValue();
        int y1 = ((CCString)*npc1SpawnPoint->valueForKey("y")).intValue();
        npc3->setPosition(ccp(x1 + tileMap->getPositionX(), y1 + tileMap->getPositionY()));
        npc3->setIsMove(false);
        tileCoord = this->tileCoordForPosition(npc3->getPosition(), tileMap);
        tileGid = walls->tileGIDAt(tileCoord);
        if (tileGid) {
            CCDictionary *properties = tileMap->propertiesForGID(tileGid);
            if (properties) {
                CCString *wall = new CCString();
                *wall = *properties->valueForKey("Wall");
                CCString abc = *wall;
                CCLog("%d", abc.intValue());
                npc3->setCharWall(abc.intValue());
            }
        }
    }
}

void TestScene::findpathTouch(){
    if (!player->getIsMove()) {
        resetTileArray();
        breadthFirstSearch(player->getPosition(), npc3->getPosition(), tileArray, mapSize);
        std::stack<TileObject*>* path = findPath(player->getPosition());
        path->pop();
        TileObject* u = path->top();
        CCPoint npcCoord = tileCoordForPosition(npc3->getPosition(), tileMap);
        CCPoint diffCoord = ccpSub(npcCoord, ccp(u->getXCoord(),u->getYCoord()));
        npc3->runAction(CCMoveBy::create(0.2f, ccp(- diffCoord.x*tileMap->getTileSize().width, diffCoord.y*tileMap->getTileSize().height)));
//        int n = 1;
//        while (!path->empty()) {
//            TileObject* u = path->top();
//            CCLog("toa do tile thu: %d trong stack la x: %d va y: %d", n, u->getXCoord(), u->getYCoord());
//            n++;
//            path->pop();
//        }
    }

}
