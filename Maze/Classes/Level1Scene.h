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
#include "NPC2.h"
#include "TileObject.h"

using namespace std;
using namespace cocos2d;

class Level1Scene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    ~Level1Scene(void);
    
    virtual bool init(int lvl);
    static cocos2d::CCScene* scene(int lvl);
    static Level1Scene* create(int lvl);
    
    bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    
    void ccTouchEnded(CCTouch *touch, CCEvent *event);
    
    void registerWithTouchDispatcher();
    
    CCPoint tileCoordForPosition(CCPoint position);
    
    virtual void update(float dt);
    
    void changeScene();
    void resetMap();
    
    // test about Shortest path problem
    // next return an array of all tiles that can come from tile u
    CCArray* next(TileObject* u, TileObject** tileArray);

    
private:
    CCTMXTiledMap *tileMap;
    CCTMXLayer *background;
    CCTMXLayer *walls;
    Player *player;
    NPC1 *npc1;
    NPC2 *npc2;
    CCPoint exitPointCoord;
    bool isGameOver;
    int _lvl;
    float SIZE_RATIO;
    float SIZE_RATIO_X;
    float SIZE_RATIO_Y;
    CCSprite* resetButton;
    TileObject** tileArray;

};

#endif /* defined(__Maze__Level1Scene__) */
