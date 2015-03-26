//
//  MoveLoop.cpp
//  FlappyBird
//
//  Created by meng-x on 2014/03/16.
//
//

#include "MoveLoop.h"
#include "config.h"
#include "HelloWorldScene.h"

MoveLoop::MoveLoop(){
    setName("MoveLoop");
}

void MoveLoop::onEnter(){
    actionMove();
}

void MoveLoop::update(float delta){

    //stop the loop when gameover
    CCNode* parent = (CCNode*)getOwner();
    if (parent == NULL) return;
    
    if ( ((HelloWorld*)parent->getParent())->GameOver )
    {
        parent->stopAllActions();
    }
}


void MoveLoop::actionMove(){
    CCNode* parent = (CCNode*)getOwner();
    if (!parent) {
        return;
    }
    
    float w = parent->boundingBox().size.width/kLandParts;
    
    CCMoveBy* moveLeft = CCMoveBy::create(kMoveLeftDura, ccp(-w, 0));
    CCMoveBy* moveRight = CCMoveBy::create(0, ccp(w, 0));
    
    CCRepeatForever* repeat = CCRepeatForever::create(CCSequence::create(moveLeft, moveRight, NULL));
    
    parent->runAction(repeat);
}