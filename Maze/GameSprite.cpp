//
//  GameSprite.cpp
//  Maze
//
//  Created by Nguyen The Vinh on 9/30/13.
//
//

#include "GameSprite.h"
#include "cocos2d.h"

using namespace cocos2d;

GameSprite::GameSprite(void){
    
}

GameSprite::~GameSprite(){
    CCLog("release gameSprite");
}

GameSprite* GameSprite::gameSpriteWithFile(const char *pszFileName){
    
    GameSprite *sprite = new GameSprite();
    if (sprite && sprite->initWithFile(pszFileName)) {
        CCLog("Ham khoi tao: %p dc goi", sprite);
        sprite->autorelease();
        return sprite;
    }
    
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void GameSprite::setPosition(const cocos2d::CCPoint &pos){
    CCSprite::setPosition(pos);
    this->charPosition = pos;
}

CCPoint GameSprite::tileCoordForPosition(cocos2d::CCPoint position, cocos2d::CCTMXTiledMap *tileMap){
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    float SIZE_RATIO_X = screenSize.width /  960;
    float SIZE_RATIO_Y = screenSize.height / 640;

    int x = (position.x - tileMap->getPositionX())/ tileMap->getTileSize().width / SIZE_RATIO_X;
    int y = ((tileMap->getMapSize().height * tileMap->getTileSize().height) - position.y/SIZE_RATIO_Y + tileMap->getPositionY()/ SIZE_RATIO_Y) / tileMap->getTileSize().height;
    return ccp(x, y);
}

void GameSprite::setIsMoveFalse(){
    setIsMove(false);
}

int GameSprite::getWallProperty(int tileGid, cocos2d::CCTMXTiledMap *tileMap){
    CCDictionary *properties = tileMap->propertiesForGID(tileGid);
    if (properties) {
        CCString *wall = new CCString();
        *wall = *properties->valueForKey("Wall");
        CCString abc = *wall;
        CCLog("%d", abc.intValue());
        return abc.intValue();
    }
    return -1;
}
