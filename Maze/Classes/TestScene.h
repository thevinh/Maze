//
//  TestScene.h
//  Maze
//
//  Created by Nguyen The Vinh on 10/14/13.
//
//

#ifndef __Maze__TestScene__
#define __Maze__TestScene__

#include "cocos2d.h"
#include "TileObject.h"
#include "GameSprite.h"
#include "Player.h"
#include <queue>
#include <stack>

using namespace std;
using namespace cocos2d;

class TestScene : public cocos2d::CCLayer {
public:
    ~TestScene(void);
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(TestScene);
    
    CCPoint tileCoordForPosition(CCPoint position, CCTMXTiledMap *tileMap);
    CCArray* next(TileObject* u, TileObject** tileArray, CCSize mapSize);
    void breadthFirstSearch(CCPoint playerPos, CCPoint npcPos, TileObject** tileArray, CCSize mapSize);
    
    virtual void update(float dt);
    bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    
    void ccTouchEnded(CCTouch *touch, CCEvent *event);
    
    void registerWithTouchDispatcher();
    std::stack<TileObject*> *findPath(CCPoint playerPos);
    
    void resetTileArray();
    void resetMap();
    void findpathTouch();

    
private:
    CCTMXTiledMap *tileMap;
    CCTMXLayer *background;
    CCTMXLayer *walls;
    Player *player;
    GameSprite *npc3;
    bool isGameOver;
    
    CCPoint exitPointCoord;
    CCSprite* resetButton;
    TileObject** tileArray;
    CCSize mapSize;
    int mazeRow;
    int mazeCol;

    CCSprite* findButton;
};

#endif /* defined(__Maze__TestScene__) */
