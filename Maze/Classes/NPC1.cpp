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

bool NPC1::canMoveRight(CCPoint pCharPosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls){
    int pCharWall;
    CCPoint tileCoord = this->tileCoordForPosition(pCharPosition,tileMap);
    int tileGid = walls->tileGIDAt(tileCoord);
    if (tileGid) {
        pCharWall = getWallProperty(tileGid, tileMap);
    }

    if (pCharWall % 2 == 0) {
        return true;
    }
    return false;
}

bool NPC1::canMoveLeft(CCPoint pMovePosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls){
    int pMoveWall;
    CCPoint tileCoord = this->tileCoordForPosition(pMovePosition,tileMap);
    int tileGid = walls->tileGIDAt(tileCoord);
    if (tileGid) {
        pMoveWall = getWallProperty(tileGid, tileMap);
    }
    if (pMoveWall % 2 == 0) {
        return true;
    }
    return false;
}

bool NPC1::canMoveDown(CCPoint pCharPosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls){
    
    int pCharWall;
    CCPoint tileCoord = this->tileCoordForPosition(pCharPosition,tileMap);
    int tileGid = walls->tileGIDAt(tileCoord);
    if (tileGid) {
        pCharWall = getWallProperty(tileGid, tileMap);
    }
    if (pCharWall < 2) {
        return true;
    }
    return false;
}

bool NPC1::canMoveUp(CCPoint pMovePosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls){
    int pMoveWall;
    CCPoint tileCoord = this->tileCoordForPosition(pMovePosition,tileMap);
    int tileGid = walls->tileGIDAt(tileCoord);
    if (tileGid) {
        pMoveWall = getWallProperty(tileGid, tileMap);
    }
    if (pMoveWall < 2) {
        return true;
    }
    return false;
    
}

void NPC1::makeMove(cocos2d::CCPoint pPlayerPosition, cocos2d::CCTMXTiledMap *tileMap, cocos2d::CCTMXLayer *walls){
    setIsMove(true);
    int numMove = 2;
    CCPoint playerTileCoord = tileCoordForPosition(pPlayerPosition, tileMap);
    CCPoint npcTileCoord = tileCoordForPosition(charPosition, tileMap);
    CCPoint tempNpcPosition = charPosition;
    CCPoint movePositon;
    CCPoint firstStep;
    CCPoint secondStep;
    
    
    while ( ((playerTileCoord.x != npcTileCoord.x) || (playerTileCoord.y != npcTileCoord.y)) && numMove > 0 ) {
        if (npcTileCoord.x - playerTileCoord.x > 0) { // if need move left
            movePositon = ccp(tempNpcPosition.x - tileMap->getTileSize().width, tempNpcPosition.y);
            if (canMoveLeft(movePositon, tileMap, walls)) { // if can move left
                if (numMove == 2) {
                    firstStep = movePositon;
                } else {
                    secondStep =  movePositon;
                }
                // update npc tilecoord and position for next loop
                npcTileCoord = tileCoordForPosition(movePositon, tileMap); 
                tempNpcPosition = movePositon;
                numMove--;
                continue;
            } else { // if cant move left (but need move left) so we check move up and down
                if (npcTileCoord.y - playerTileCoord.y > 0) {// if need move up
                    movePositon = ccp(tempNpcPosition.x, tempNpcPosition.y + tileMap->getTileSize().height);
                    if (canMoveUp(movePositon, tileMap, walls)) { // if can move up
                        if (numMove == 2) {
                            firstStep = movePositon;
                        } else {
                            secondStep =  movePositon;
                        }
                        npcTileCoord = tileCoordForPosition(movePositon, tileMap);
                        tempNpcPosition = movePositon;
                        numMove--;
                        continue;
                    }
                    else break;// stand still
                } else {// if dont need move up
                    if (npcTileCoord.y - playerTileCoord.y < 0) { // if need move down
                        movePositon = ccp(tempNpcPosition.x, tempNpcPosition.y - tileMap->getTileSize().height);
                        if (canMoveDown(tempNpcPosition, tileMap, walls)) {
                            if (numMove == 2) {
                                firstStep = movePositon;
                            } else {
                                secondStep =  movePositon;
                            }
                            npcTileCoord = tileCoordForPosition(movePositon, tileMap);
                            tempNpcPosition = movePositon;
                            numMove--;
                            continue;

                        } else break;
                    }
                    else break;
                }
            }
            
        } else { // end of need move left
            if (npcTileCoord.x - playerTileCoord.x < 0) {
                movePositon = ccp(tempNpcPosition.x + tileMap->getTileSize().width, tempNpcPosition.y);
                if (canMoveRight(tempNpcPosition, tileMap, walls)) { // if can move right
                    if (numMove == 2) {
                        firstStep = movePositon;
                    } else {
                        secondStep =  movePositon;
                    }
                    npcTileCoord = tileCoordForPosition(movePositon, tileMap);
                    tempNpcPosition = movePositon;
                    numMove--;
                    continue;

                } else { // end of can move right
                    if (npcTileCoord.y - playerTileCoord.y > 0) {// if need move up
                        movePositon = ccp(tempNpcPosition.x, tempNpcPosition.y + tileMap->getTileSize().height);
                        if (canMoveUp(movePositon, tileMap, walls)) { // if can move up
                            if (numMove == 2) {
                                firstStep = movePositon;
                            } else {
                                secondStep =  movePositon;
                            }
                            npcTileCoord = tileCoordForPosition(movePositon, tileMap);
                            tempNpcPosition = movePositon;
                            numMove--;
                            continue;
                        }
                        else break;// stand still
                    } else {// if dont need move up
                        if (npcTileCoord.y - playerTileCoord.y < 0) { // if need move down
                            movePositon = ccp(tempNpcPosition.x, tempNpcPosition.y - tileMap->getTileSize().height);
                            if (canMoveDown(tempNpcPosition, tileMap, walls)) {
                                if (numMove == 2) {
                                    firstStep = movePositon;
                                } else {
                                    secondStep =  movePositon;
                                }
                                npcTileCoord = tileCoordForPosition(movePositon, tileMap);
                                tempNpcPosition = movePositon;
                                numMove--;
                                continue;
                                
                            } else break;
                        }
                        else break;
                    }

                }
            } else {
                if (npcTileCoord.y - playerTileCoord.y > 0) {// if need move up
                    movePositon = ccp(tempNpcPosition.x, tempNpcPosition.y + tileMap->getTileSize().height);
                    if (canMoveUp(movePositon, tileMap, walls)) { // if can move up
                        if (numMove == 2) {
                            firstStep = movePositon;
                        } else {
                            secondStep =  movePositon;
                        }
                        npcTileCoord = tileCoordForPosition(movePositon, tileMap);
                        tempNpcPosition = movePositon;
                        numMove--;
                        continue;
                    }
                    else break;// stand still
                } else {// if dont need move up
                    if (npcTileCoord.y - playerTileCoord.y < 0) { // if need move down
                        movePositon = ccp(tempNpcPosition.x, tempNpcPosition.y - tileMap->getTileSize().height);
                        if (canMoveDown(tempNpcPosition, tileMap, walls)) {
                            if (numMove == 2) {
                                firstStep = movePositon;
                            } else {
                                secondStep =  movePositon;
                            }
                            npcTileCoord = tileCoordForPosition(movePositon, tileMap);
                            tempNpcPosition = movePositon;
                            numMove--;
                            continue;
                            
                        } else break;
                    }
                    else break;
                }

            }
        }
    }
    
    switch (numMove) {
        case 2:
            setIsMove(false);
            break;
        case 1:
            this->runAction(CCSequence::create(CCMoveTo::create(0.2f, firstStep),
                                               CCCallFunc::create(this, callfunc_selector(NPC1::setIsMoveFalse)),
                                               NULL));
            this->charPosition = firstStep;
        case 0:
            this->runAction(CCSequence::create(CCMoveTo::create(0.2f, firstStep),
                                               CCMoveTo::create(0.2f, secondStep),
                                               CCCallFunc::create(this, callfunc_selector(NPC1::setIsMoveFalse)),
                                               NULL));
            this->charPosition = secondStep;

            
        default:
            break;
    }
}

//void NPC1::makeMove(cocos2d::CCPoint playerPosition, cocos2d::CCTMXTiledMap *tileMap, CCTMXLayer *walls){
//    setIsMove(true);
//    int n = 1;
//    while (n > 0) {
//        
//        CCPoint npcPos = this->getPosition();
//        if (tileCoordForPosition(npcPos, tileMap).x - tileCoordForPosition(playerPosition, tileMap).x > 0) {
//
//            // move left
//            npcPos.x -= tileMap->getTileSize().width;
//            this->setMoveId(2);
//        }
//        
//        else if (tileCoordForPosition(npcPos, tileMap).x - tileCoordForPosition(playerPosition, tileMap).x < 0) {
//            // move right
//            npcPos.x += tileMap->getTileSize().width;
//            this->setMoveId(1);
//        }
//        
//        else if (tileCoordForPosition(npcPos, tileMap).x - tileCoordForPosition(playerPosition, tileMap).x == 0) {
//            if (tileCoordForPosition(npcPos, tileMap).y - tileCoordForPosition(playerPosition, tileMap).y > 0) {
//                // move up
//                npcPos.y += tileMap->getTileSize().height;
//                this->setMoveId(3);
//            } else {
//                // move down
//                npcPos.y -= tileMap->getTileSize().height;
//                this->setMoveId(4);
//            }
//        }
//        
//        if (npcPos.x <= (tileMap->getMapSize().width * tileMap->getTileSize().width) &&
//            npcPos.y <= (tileMap->getMapSize().height * tileMap->getTileSize().height) &&
//            npcPos.y >= 0 &&
//            npcPos.x >= 0 )
//        {
//            int moveWall = -1;
//            CCPoint tileCoord = this->tileCoordForPosition(npcPos,tileMap);
//            int tileGid = walls->tileGIDAt(tileCoord);
//            if (tileGid) {
//                CCDictionary *properties = tileMap->propertiesForGID(tileGid);
//                if (properties) {
//                    CCString *wall = new CCString();
//                    *wall = *properties->valueForKey("Wall");
//                    CCString abc = *wall;
//                    CCLog("%d", abc.intValue());
//                    moveWall = abc.intValue();
//                    this->setMoveWall(moveWall);
//                }
//            }
//            
//            this->movePositon = npcPos;
//            this->runAction(CCSequence::create(CCCallFunc::create(this, callfunc_selector(NPC1::move)),
//                                               CCDelayTime::create(0.2f),
//                                               CCCallFunc::create(this, callfunc_selector(NPC1::setIsMoveFalse)),
//                                               NULL));
//        }
//        else this->setMoveId(0);
//        n--;
//    }
//}

void NPC1::move(){
//    CCPoint pos = movePositon;
//    switch (moveId) {
//        case 1:
//                    // move right
//            if (canMoveRight(charWall)) {
//                this->moveRight(pos);
//                        //set charWall in new position
//                this->setCharWall(getMoveWall());
//                        
//            }
//                    
//            break;
//        case 2:
//                    if (canMoveLeft(getMoveWall())) {
//                        this->moveLeft(pos);
//                        //set charWall in new position
//                        this->setCharWall(getMoveWall());
//                        
//                    }
//                    
//                    break;
//        case 3:
//                    if (canMoveUp(getMoveWall())) {
//                        this->moveUp(pos);
//                        //set charWall in new position
//                        this->setCharWall(getMoveWall());
//                        
//                    }
//                    
//                    break;
//        case 4:
//                    if (canMoveDown(charWall)) {
//                        this->moveDown(pos);
//                        //set charWall in new position
//                        this->setCharWall(getMoveWall());
//                        
//                    }
//            
//            break;
//        default:
//            break;
//    }
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
