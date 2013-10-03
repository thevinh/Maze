//
//  NPC1.h
//  Maze
//
//  Created by Nguyen The Vinh on 10/2/13.
//
//

#ifndef __Maze__NPC1__
#define __Maze__NPC1__

#include "GameSprite.h"
#include "cocos2d.h"
using namespace cocos2d;

class NPC1 : public GameSprite {
public:
    NPC1(void);
    ~NPC1(void);
    
    void makeMove(CCPoint playerPosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls);
    
    void move();
    
    bool canMoveLeft(int pMoveWall);
    bool canMoveRight(int pCharWall);
    bool canMoveUp(int pMoveWall);
    bool canMoveDown(int pCharWall);
    
    void moveLeft(CCPoint pos);
    void moveRight(CCPoint pos);
    void moveUp(CCPoint pos);
    void moveDown(CCPoint pos);

};
#endif /* defined(__Maze__NPC1__) */
