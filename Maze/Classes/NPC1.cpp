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

bool NPC1::makeMove(cocos2d::CCPoint pPlayerPosition, cocos2d::CCTMXTiledMap *tileMap, cocos2d::CCTMXLayer *walls){
    setIsMove(true);
    int numMove = 2;
    CCPoint playerTileCoord = tileCoordForPosition(pPlayerPosition, tileMap);
    CCPoint npcTileCoord = tileCoordForPosition(charPosition, tileMap);
    CCPoint tempNpcPosition = charPosition;
    CCPoint movePositon;
    CCPoint firstStep;
    CCPoint secondStep;
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    float SIZE_RATIO_X = screenSize.width /  960;
    float SIZE_RATIO_Y = screenSize.height / 640;
    
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
            break;
        case 0:
            this->runAction(CCSequence::create(CCMoveTo::create(0.2f, firstStep),
                                               CCMoveTo::create(0.2f, secondStep),
                                               CCCallFunc::create(this, callfunc_selector(NPC1::setIsMoveFalse)),
                                               NULL));
            this->charPosition = secondStep;

        default:
            break;
    }
    
    if (tileCoordForPosition(this->charPosition, tileMap).x == playerTileCoord.x &&
        tileCoordForPosition(this->charPosition, tileMap).y == playerTileCoord.y) {
        CCLog("thua cmnr");
        return true;
        
    }
    else return false;
}

