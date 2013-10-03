//
//  Player.h
//  Maze
//
//  Created by Nguyen The Vinh on 9/30/13.
//
//

#ifndef __Maze__Player__
#define __Maze__Player__

#include "GameSprite.h"
#include "cocos2d.h"
using namespace cocos2d;

class Player : public GameSprite {
public:
    Player(void);
    ~Player(void);
    
    void makeMove(CCPoint touchLocation, CCTMXTiledMap* tileMap, CCTMXLayer *walls);
    
    void move();
    
    bool canMoveLeft(int pMoveWall);
    bool canMoveRight(int pCharWall);
    bool canMoveUp(int pMoveWall);
    bool canMoveDown(int pCharWall);
    
    void moveLeft(CCPoint pos);
    void moveRight(CCPoint pos);
    void moveUp(CCPoint pos);
    void moveDown(CCPoint pos);
    
private:
    
    
//    CCPoint charPosition;
//    CCPoint movePositon;
//    CC_SYNTHESIZE(int, moveId, MoveId);
};

#endif /* defined(__Maze__Player__) */
