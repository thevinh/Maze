//
//  MediumScene.h
//  Maze
//
//  Created by Nguyen The Vinh on 10/7/13.
//
//

#ifndef __Maze__MediumScene__
#define __Maze__MediumScene__

#include "cocos2d.h"
#include "Level1Scene.h"

using namespace cocos2d;

class MediumScene : public cocos2d::CCLayer
{
public:
    ~MediumScene(void);
    
    virtual bool init(int lvl);
    static cocos2d::CCScene* scene(int lvl);
    static MediumScene* create(int lvl);
    
    void registerWithTouchDispatcher();
    
    bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    void ccTouchEnded(CCTouch *touch, CCEvent *event);
private:
    int _lvl;
    char labelText[128];
    CCLabelTTF *labelTitle;

};

#endif /* defined(__Maze__MediumScene__) */
