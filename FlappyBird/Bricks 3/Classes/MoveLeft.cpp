//
//  MoveLeft.cpp
//  FlappyBird
//
//  Created by meng-x on 2014/03/16.
//
//

#include "MoveLeft.h"
#include "config.h"
#include "HelloWorldScene.h"


MoveLeft::MoveLeft(float w){
    setName("MoveLeft");
    this->w = w / kLandParts;
}

void MoveLeft::onEnter(){
    actionMove();
}

void MoveLeft::update(float delta){
    CCNode* parent = (CCNode*)getOwner();
    if (parent == NULL) return;
    
    if ( ((HelloWorld*)parent->getParent())->GameOver )
    {
        parent->stopAllActions();
    }
}

void MoveLeft::actionMove(){
    CCNode* parent = (CCNode*)getOwner();
    CCMoveBy* moveleft = CCMoveBy::create(kMoveLeftDura, ccp(-w, 0));
    parent->runAction(CCRepeatForever::create(moveleft));
}