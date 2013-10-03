//
//  NPC1.cpp
//  Maze
//
//  Created by Nguyen The Vinh on 10/2/13.
//
//

#include "NPC1.h"
#include "cocos2d.h"

using namespace cocos2d;

NPC1::NPC1(void){
    
}

NPC1::~NPC1(){
    
}

bool NPC1::canMoveRight(int pCharWall){
    if (pCharWall % 2 == 0) {
        return true;
    }
    return false;
}

bool NPC1::canMoveLeft(int pMoveWall){
    if (pMoveWall % 2 == 0) {
        return true;
    }
    return false;
}

bool NPC1::canMoveDown(int pCharWall){
    if (pCharWall < 2) {
        return true;
    }
    return false;
}

bool NPC1::canMoveUp(int pMoveWall){
    if (pMoveWall < 2) {
        return true;
    }
    return false;
    
}

void NPC1::makeMove(cocos2d::CCPoint playerPosition, cocos2d::CCTMXTiledMap *tileMap, CCTMXLayer *walls){
    setIsMove(true);
    int n = 1;
    while (n > 0) {
        
        CCPoint npcPos = this->getPosition();
        if (tileCoordForPosition(npcPos, tileMap).x - tileCoordForPosition(playerPosition, tileMap).x > 0) {

            // move left
            npcPos.x -= tileMap->getTileSize().width;
            this->setMoveId(2);
        }
        
        else if (tileCoordForPosition(npcPos, tileMap).x - tileCoordForPosition(playerPosition, tileMap).x < 0) {
            // move right
            npcPos.x += tileMap->getTileSize().width;
            this->setMoveId(1);
        }
        
        else if (tileCoordForPosition(npcPos, tileMap).x - tileCoordForPosition(playerPosition, tileMap).x == 0) {
            if (tileCoordForPosition(npcPos, tileMap).y - tileCoordForPosition(playerPosition, tileMap).y > 0) {
                // move up
                npcPos.y += tileMap->getTileSize().height;
                this->setMoveId(3);
            } else {
                // move down
                npcPos.y -= tileMap->getTileSize().height;
                this->setMoveId(4);
            }
        }
        
        if (npcPos.x <= (tileMap->getMapSize().width * tileMap->getTileSize().width) &&
            npcPos.y <= (tileMap->getMapSize().height * tileMap->getTileSize().height) &&
            npcPos.y >= 0 &&
            npcPos.x >= 0 )
        {
            int moveWall = -1;
            CCPoint tileCoord = this->tileCoordForPosition(npcPos,tileMap);
            int tileGid = walls->tileGIDAt(tileCoord);
            if (tileGid) {
                CCDictionary *properties = tileMap->propertiesForGID(tileGid);
                if (properties) {
                    CCString *wall = new CCString();
                    *wall = *properties->valueForKey("Wall");
                    CCString abc = *wall;
                    CCLog("%d", abc.intValue());
                    moveWall = abc.intValue();
                    this->setMoveWall(moveWall);
                }
            }
            
            this->movePositon = npcPos;
            this->runAction(CCSequence::create(CCCallFunc::create(this, callfunc_selector(NPC1::move)),
                                               CCDelayTime::create(0.2f),
                                               CCCallFunc::create(this, callfunc_selector(NPC1::setIsMoveFalse)),
                                               NULL));
        }
        else this->setMoveId(0);
        n--;
    }
}

void NPC1::move(){
    CCPoint pos = movePositon;
    switch (moveId) {
        case 1:
                    // move right
            if (canMoveRight(charWall)) {
                this->moveRight(pos);
                        //set charWall in new position
                this->setCharWall(getMoveWall());
                        
            }
                    
            break;
        case 2:
                    if (canMoveLeft(getMoveWall())) {
                        this->moveLeft(pos);
                        //set charWall in new position
                        this->setCharWall(getMoveWall());
                        
                    }
                    
                    break;
        case 3:
                    if (canMoveUp(getMoveWall())) {
                        this->moveUp(pos);
                        //set charWall in new position
                        this->setCharWall(getMoveWall());
                        
                    }
                    
                    break;
        case 4:
                    if (canMoveDown(charWall)) {
                        this->moveDown(pos);
                        //set charWall in new position
                        this->setCharWall(getMoveWall());
                        
                    }
            
            break;
        default:
            break;
    }
}

void NPC1::moveRight(cocos2d::CCPoint pos){
    //    CCLog("move to the right");
    this->runAction(CCMoveTo::create(0.2f, pos));
    this->charPosition = pos;
}

void NPC1::moveLeft(cocos2d::CCPoint pos){
    this->runAction(CCMoveTo::create(0.2f, pos));
    this->charPosition = pos;
}

void NPC1::moveUp(cocos2d::CCPoint pos){
    this->runAction(CCMoveTo::create(0.2f, pos));
    this->charPosition = pos;
}

void NPC1::moveDown(cocos2d::CCPoint pos){
    this->runAction(CCMoveTo::create(0.2f, pos));
    this->charPosition = pos;
}
