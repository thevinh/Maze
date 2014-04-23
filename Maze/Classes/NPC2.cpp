//
//  NPC2.cpp
//  Maze
//
//  Created by Nguyen The Vinh on 10/9/13.
//
//

#include "NPC2.h"
#include "cocos2d.h"

using namespace cocos2d;

NPC2::NPC2(){
    
}

NPC2::~NPC2(){
    
}

bool NPC2::canMoveRight(cocos2d::CCPoint pCharPosition, cocos2d::CCTMXTiledMap *tileMap, cocos2d::CCTMXLayer *walls){
    int pCharWall;
	int abcdef = 6;
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

bool NPC2::canMoveLeft(cocos2d::CCPoint pMovePosition, cocos2d::CCTMXTiledMap *tileMap, cocos2d::CCTMXLayer *walls){
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

bool NPC2::canMoveDown(cocos2d::CCPoint pCharPosition, cocos2d::CCTMXTiledMap *tileMap, cocos2d::CCTMXLayer *walls){
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

bool NPC2::canMoveUp(cocos2d::CCPoint pMovePosition, cocos2d::CCTMXTiledMap *tileMap, cocos2d::CCTMXLayer *walls){
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

bool NPC2::makeMove(cocos2d::CCPoint pPlayerPosition, cocos2d::CCTMXTiledMap *tileMap, cocos2d::CCTMXLayer *walls){
    if (this == NULL) {
        return false;
    }
    setIsMove(true);
    int numMove = 2;
    CCPoint playerTileCoord = tileCoordForPosition(pPlayerPosition, tileMap);
    CCPoint npcTileCoord = tileCoordForPosition(charPosition, tileMap);
    CCPoint tempNpcPostion = charPosition;
    CCPoint movePosition;
    CCPoint firstStep;
    CCPoint secondStep;
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    float SIZE_RATIO_X = screenSize.width / 960;
    float SIZE_RATIO_Y = screenSize.height / 640;
    while ( ((playerTileCoord.x != npcTileCoord.x) || (playerTileCoord.y != npcTileCoord.y)) && numMove > 0 ) {
        if (npcTileCoord.y - playerTileCoord.y > 0) {// if need move up
            movePosition = ccp(tempNpcPostion.x, tempNpcPostion.y + tileMap->getTileSize().height*SIZE_RATIO_Y);
            if (canMoveUp(movePosition, tileMap, walls)) {
                if (numMove == 2) {
                    firstStep = movePosition;
                } else {
                    secondStep = movePosition;
                }
                npcTileCoord = tileCoordForPosition(movePosition, tileMap);
                tempNpcPostion = movePosition;
                numMove--;
                continue;
            } else {
                if (npcTileCoord.x - playerTileCoord.x > 0) { // if need move left
                    movePosition = ccp(tempNpcPostion.x - tileMap->getTileSize().width*SIZE_RATIO_X, tempNpcPostion.y);
                    if (canMoveLeft(movePosition, tileMap, walls)) {
                        if (numMove == 2) {
                            firstStep = movePosition;
                        } else {
                            secondStep = movePosition;
                        }
                        npcTileCoord = tileCoordForPosition(movePosition, tileMap);
                        tempNpcPostion = movePosition;
                        numMove--;
                        continue;
                    } else break;

                } else {// if need move right
                    if (npcTileCoord.x - playerTileCoord.x < 0) {
                        movePosition = ccp(tempNpcPostion.x + tileMap->getTileSize().width*SIZE_RATIO_X, tempNpcPostion.y);
                        if (canMoveRight(tempNpcPostion, tileMap, walls)) {
                            if (numMove == 2) {
                                firstStep = movePosition;
                            } else {
                                secondStep = movePosition;
                            }
                            npcTileCoord = tileCoordForPosition(movePosition, tileMap);
                            tempNpcPostion = movePosition;
                            numMove--;
                            continue;
                        } else break;
                    } else break;

                }
            }
        } else { // end of need move up
            if (npcTileCoord.y - playerTileCoord.y < 0) { // if need move down
                movePosition = ccp(tempNpcPostion.x, tempNpcPostion.y - tileMap->getTileSize().height*SIZE_RATIO_Y);
                if (canMoveDown(tempNpcPostion, tileMap, walls)) {
                    if (numMove == 2) {
                        firstStep = movePosition;
                    } else {
                        secondStep = movePosition;
                    }
                    npcTileCoord = tileCoordForPosition(movePosition, tileMap);
                    tempNpcPostion = movePosition;
                    numMove--;
                    continue;
                } else { // if still need move down but cant move down, so we check move right and left
                    if (npcTileCoord.x - playerTileCoord.x > 0) { // if need move left
                        movePosition = ccp(tempNpcPostion.x - tileMap->getTileSize().width*SIZE_RATIO_X, tempNpcPostion.y);
                        if (canMoveLeft(movePosition, tileMap, walls)) {
                            if (numMove == 2) {
                                firstStep = movePosition;
                            } else {
                                secondStep = movePosition;
                            }
                            npcTileCoord = tileCoordForPosition(movePosition, tileMap);
                            tempNpcPostion = movePosition;
                            numMove--;
                            continue;
                        } else break;
                        
                    } else {// if need move right
                        if (npcTileCoord.x - playerTileCoord.x < 0) {
                            movePosition = ccp(tempNpcPostion.x + tileMap->getTileSize().width*SIZE_RATIO_X, tempNpcPostion.y);
                            if (canMoveRight(tempNpcPostion, tileMap, walls)) {
                                if (numMove == 2) {
                                    firstStep = movePosition;
                                } else {
                                    secondStep = movePosition;
                                }
                                npcTileCoord = tileCoordForPosition(movePosition, tileMap);
                                tempNpcPostion = movePosition;
                                numMove--;
                                continue;
                            } else break;
                        } else break;
                        
                    }

                }
            } else { // if dont need move up and dont need move down
                if (npcTileCoord.x - playerTileCoord.x > 0) { // if need move left
                    movePosition = ccp(tempNpcPostion.x - tileMap->getTileSize().width*SIZE_RATIO_X, tempNpcPostion.y);
                    if (canMoveLeft(movePosition, tileMap, walls)) {
                        if (numMove == 2) {
                            firstStep = movePosition;
                        } else {
                            secondStep = movePosition;
                        }
                        npcTileCoord = tileCoordForPosition(movePosition, tileMap);
                        tempNpcPostion = movePosition;
                        numMove--;
                        continue;
                    } else break;
                    
                } else {// if need move right
                    if (npcTileCoord.x - playerTileCoord.x < 0) {
                        movePosition = ccp(tempNpcPostion.x + tileMap->getTileSize().width*SIZE_RATIO_X, tempNpcPostion.y);
                        if (canMoveRight(tempNpcPostion, tileMap, walls)) {
                            if (numMove == 2) {
                                firstStep = movePosition;
                            } else {
                                secondStep = movePosition;
                            }
                            npcTileCoord = tileCoordForPosition(movePosition, tileMap);
                            tempNpcPostion = movePosition;
                            numMove--;
                            continue;
                        } else break;
                    } else break;
                    
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
                                               CCCallFunc::create(this, callfunc_selector(NPC2::setIsMoveFalse)),
                                               NULL));
            this->charPosition = firstStep;
            break;
        case 0:
            this->runAction(CCSequence::create(CCMoveTo::create(0.2f, firstStep),
                                               CCMoveTo::create(0.2f, secondStep),
                                               CCCallFunc::create(this, callfunc_selector(NPC2::setIsMoveFalse)),
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
