//
//  GameSprite.h
//  Maze
//
//  Created by Nguyen The Vinh on 9/30/13.
//
//

#ifndef __Maze__GameSprite__
#define __Maze__GameSprite__

#include "cocos2d.h";

using namespace cocos2d;

class GameSprite : public CCSprite {
  
public:
    GameSprite(void);
    ~GameSprite(void);
    
    static GameSprite* gameSpriteWithFile(const char* pszFileName);
    CCPoint tileCoordForPosition(CCPoint position, CCTMXTiledMap *tileMap);
    void setIsMoveFalse();
    int getWallProperty(int tileGid, CCTMXTiledMap *tileMap);
    
    virtual void setPosition(const CCPoint& pos);
    void makeMove(CCPoint pMovePosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls);
    void move(CCPoint pos, CCTMXTiledMap *tileMap);
    
    bool canMoveLeft(CCPoint pos, CCTMXTiledMap *tileMap);
    bool canMoveRight(CCPoint pos, CCTMXTiledMap *tileMap);
    bool canMoveUp(CCPoint pos, CCTMXTiledMap *tileMap);
    bool canMoveDown(CCPoint pos, CCTMXTiledMap *tileMap);
    
    void moveLeft(CCPoint pos, CCTMXTiledMap *tileMap);
    void moveRight(CCPoint pos, CCTMXTiledMap *tileMap);
    void moveUp(CCPoint pos, CCTMXTiledMap *tileMap);
    void moveDown(CCPoint pos, CCTMXTiledMap *tileMap);
    
    
    
protected:
    CC_SYNTHESIZE(CCPoint, charPosition, CharPosition)
    CCPoint moveToPositon;
    CC_SYNTHESIZE(int, moveId, MoveId);
    CC_SYNTHESIZE(int, charWall, CharWall);
    CC_SYNTHESIZE(int, moveWall, MoveWall);
    CC_SYNTHESIZE(bool, isMove, IsMove);
    
    
//    virtual void setPosition(const CCPoint& pos);
    
};

#endif /* defined(__Maze__GameSprite__) */
