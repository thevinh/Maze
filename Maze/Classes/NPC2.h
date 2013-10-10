//
//  NPC2.h
//  Maze
//
//  Created by Nguyen The Vinh on 10/9/13.
//
//

#ifndef __Maze__NPC2__
#define __Maze__NPC2__

#include "GameSprite.h"
#include "cocos2d.h"

using namespace cocos2d;

class NPC2 : public GameSprite {
public:
    NPC2(void);
    ~NPC2(void);
    
    bool makeMove(CCPoint pPlayerPosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls);
    bool canMoveLeft(CCPoint pMovePosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls);
    bool canMoveRight(CCPoint pCharPosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls);
    bool canMoveUp(CCPoint pMovePosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls);
    bool canMoveDown(CCPoint pCharPosition, CCTMXTiledMap* tileMap, CCTMXLayer *walls);    
    
};

#endif /* defined(__Maze__NPC2__) */
