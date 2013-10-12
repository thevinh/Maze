//
//  TileObject.cpp
//  Maze
//
//  Created by Nguyen The Vinh on 10/11/13.
//
//

#include "TileObject.h"
#include "cocos2d.h"

using namespace cocos2d;

TileObject::TileObject(){
    
}

TileObject::~TileObject(){
    
}

TileObject::TileObject(int pX, int pY, int pWall){
    xCoord = pX;
    yCoord = pY;
    wall = pWall;
    preTile = NULL;
    isCheck = false;
    CCLog("Tile Object create: %d, %d,  %d", xCoord, yCoord, wall);
}

void TileObject::reset(){
    this->setIsCheck(false);
    this->setPreTile(NULL);
}

int TileObject::getXCoord(){
    return xCoord;
}

int TileObject::getYCoord(){
    return yCoord;
}

int TileObject::getWall(){
    return wall;
}