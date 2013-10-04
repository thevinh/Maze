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
    
    bool makeMove(CCPoint pPlayerPosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls);
    
    bool canMoveLeft(CCPoint pMovePosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls);
    bool canMoveRight(CCPoint pCharPosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls);
    bool canMoveUp(CCPoint pMovePosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls);
    bool canMoveDown(CCPoint pCharPosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls);

//    CC_SYNTHESIZE(CCPoint, firstStep, FirstStep);
//    CC_SYNTHESIZE(CCPoint, secondStep, SecondStep);

protected:
    CC_SYNTHESIZE(int, firstWall, FirstWall);
    CC_SYNTHESIZE(int, secondWall, SecondWall);

};
#endif /* defined(__Maze__NPC1__) */
