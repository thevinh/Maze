//
//  Level1Scene.h
//  Maze
//
//  Created by Nguyen The Vinh on 9/30/13.
//
//

#ifndef __Maze__Level1Scene__
#define __Maze__Level1Scene__

#include "cocos2d.h"
#include "Player.h"
#include "GameSprite.h"
#include "NPC1.h"

using namespace cocos2d;

class Level1Scene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(Level1Scene);
    
    bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    
    void ccTouchEnded(CCTouch *touch, CCEvent *event);
    
    void registerWithTouchDispatcher();
    
    CCPoint tileCoordForPosition(CCPoint position);
    
    virtual void update(float dt);

    
private:
    CCTMXTiledMap *tileMap;
    CCTMXLayer *background;
    CCTMXLayer *walls;
    Player *player;
    NPC1 *npc1;
    CCPoint exitPointCoord;
    bool isGameOver;

};

#endif /* defined(__Maze__Level1Scene__) */
