//
//  Player.cpp
//  Maze
//
//  Created by Nguyen The Vinh on 9/30/13.
//
//

#include "Player.h"
#include "cocos2d.h"

using namespace cocos2d;

Player::Player(void){
    
}

Player::~Player(){
    
}

bool Player::canMoveRight(int pCharWall){
    if (pCharWall % 2 == 0) {
        return true;
    }
    return false;
}

bool Player::canMoveLeft(int pMoveWall){
    if (pMoveWall % 2 == 0) {
        return true;
    }
    return false;
}

bool Player::canMoveDown(int pCharWall){
    if (pCharWall < 2) {
        return true;
    }
    return false;
}

bool Player::canMoveUp(int pMoveWall){
    if (pMoveWall < 2) {
        return true;
    }
    return false;

}

void Player::makeMove(cocos2d::CCPoint touchLocation, cocos2d::CCTMXTiledMap *tileMap, CCTMXLayer *walls){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    float SIZE_RATIO_X = screenSize.width /  960;
    float SIZE_RATIO_Y = screenSize.height / 640;
    setIsMove(true);
    CCPoint playerPos = this->getPosition();
    CCPoint diff = ccpSub(tileCoordForPosition(touchLocation, tileMap), tileCoordForPosition(playerPos, tileMap));
    
    if (diff.x == 0 && diff.y == 0) {
        this->setIsMoveFalse();
    }
    else if (abs(diff.x) > abs(diff.y)) {
        if (diff.x > 0) {
            // move right
            playerPos.x += tileMap->getTileSize().width*SIZE_RATIO_X;
            this->setMoveId(1);
        } else {
            //move left
            playerPos.x -= tileMap->getTileSize().width*SIZE_RATIO_X;
            this->setMoveId(2);
        }
    } else {
        if (diff.y > 0) {
            // move down
            playerPos.y -= tileMap->getTileSize().height*SIZE_RATIO_Y;
            this->setMoveId(4);
            
        } else {
            // move up
            playerPos.y += tileMap->getTileSize().height*SIZE_RATIO_Y;
            this->setMoveId(3);
        }
    }
    
    if (playerPos.x - tileMap->getPositionX() <= (tileMap->getMapSize().width * tileMap->getTileSize().width * SIZE_RATIO_X) &&
        playerPos.y - tileMap->getPositionY() <= (tileMap->getMapSize().height * tileMap->getTileSize().height * SIZE_RATIO_Y) &&
        playerPos.y - tileMap->getPositionY() >= 0 &&
        playerPos.x - tileMap->getPositionX() >= 0 )
    {
        int moveWall = -1;
        CCPoint tileCoord = this->tileCoordForPosition(playerPos,tileMap);
        int tileGid = walls->tileGIDAt(tileCoord);
        if (tileGid) {
            moveWall = getWallProperty(tileGid, tileMap);
            this->setMoveWall(moveWall);
        }
        this->moveToPositon = playerPos;
        this->runAction(CCSequence::create(CCCallFunc::create(this, callfunc_selector(Player::move)),
                                           CCDelayTime::create(0.2f),
                                           CCCallFunc::create(this, callfunc_selector(Player::setIsMoveFalse)),
                                           NULL));
//            this->move(playerPos, moveWall);
    }
    else {
        this->setMoveId(0);
        this->setIsMove(false);
    }


}

void Player::move(){
    
    CCPoint pos = this->moveToPositon;
    
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

void Player::moveRight(cocos2d::CCPoint pos){
//    CCLog("move to the right");
    this->runAction(CCMoveTo::create(0.2f, pos));
    this->charPosition = pos;
}

void Player::moveLeft(cocos2d::CCPoint pos){
    this->runAction(CCMoveTo::create(0.2f, pos));
    this->charPosition = pos;
}

void Player::moveUp(cocos2d::CCPoint pos){
    this->runAction(CCMoveTo::create(0.2f, pos));
    this->charPosition = pos;
}

void Player::moveDown(cocos2d::CCPoint pos){
    this->runAction(CCMoveTo::create(0.2f, pos));
    this->charPosition = pos;
}
