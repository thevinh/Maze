//
//  MediumScene.cpp
//  Maze
//
//  Created by Nguyen The Vinh on 10/7/13.
//
//

#include "MediumScene.h"
#include "cocos2d.h"
#include "Level1Scene.h"

using namespace std;
using namespace cocos2d;

MediumScene::~MediumScene(){
    
}

CCScene* MediumScene::scene(int lvl){
    CCScene *scene = CCScene::create();
    MediumScene *layer = MediumScene::create(lvl);
    scene->addChild(layer);
    return scene;
}

bool MediumScene::init(int lvl){
    if ( !CCLayer::init() )
    {
        return false;
    }
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    _lvl = lvl;
    sprintf(labelText, "Level %d Finished, Touch to go next level",lvl);
    labelTitle = CCLabelTTF::create(labelText, "Thonburi", 50);
    labelTitle->setPosition(ccp(screenSize.width/2, screenSize.height/2));
    this->addChild(labelTitle);
    this->setTouchEnabled(true);
    return true;
}

void MediumScene::registerWithTouchDispatcher(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

MediumScene* MediumScene::create(int lvl){
    MediumScene *pRet = new MediumScene();
    if (pRet && pRet->init(lvl)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool MediumScene::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event){
    return true;
}

void MediumScene::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event){
    CCScene *s = Level1Scene::scene(_lvl+1);
    CCDirector::sharedDirector()->setDepthTest(true);
    CCTransitionScene *transition = CCTransitionFade::create(2.0f, s);
    CCDirector::sharedDirector()->replaceScene(transition);
}
