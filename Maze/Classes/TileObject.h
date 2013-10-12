//
//  TileObject.h
//  Maze
//
//  Created by Nguyen The Vinh on 10/11/13.
//
//

#ifndef __Maze__TileObject__
#define __Maze__TileObject__

#include "cocos2d.h"
class TileObject : public cocos2d::CCObject {
public:
    TileObject(int pX, int pY, int pWall);
    TileObject();
    ~TileObject();
    void reset();
    int getXCoord();
    int getYCoord();
    int getWall();
private:
    int xCoord, yCoord, wall;
    CC_SYNTHESIZE(bool, isCheck, IsCheck);
    CC_SYNTHESIZE(TileObject*, preTile, PreTile);
    
};

#endif /* defined(__Maze__TileObject__) */
