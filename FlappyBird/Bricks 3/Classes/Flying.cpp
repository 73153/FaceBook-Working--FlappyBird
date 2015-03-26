//
//  Flying.cpp
//  FlappyBird
//
//  Created by meng-x on 2014/03/16.
//
//

#include "Flying.h"
#include "config.h"

Flying::Flying(){
    setName("Flying");
    curAction = NULL;
}

void Flying::update(float delta){
    parent = (Bird*)getOwner();
    if (!parent) {
        return;
    }else if (parent->status == isTap) {
        parent->status = isIdle;
        actionTap();
    }else if(parent->status == isHit) {
        parent->status = isDie;
        actionHit();
    }
}

void Flying::actionTap(){
    CCSize win = CCDirector::sharedDirector()->getWinSize();
    if (parent->getPosition().y > win.height-20) {
        return;
    }
    
    
    stopAction();
    
    CCPoint uppos = parent->getPosition();
    uppos.y += kJumpHeight;  //JUMP!!
    
    //if fall, fall to this position
    
    CCPoint downpos = ccp(uppos.x, uppos.y - win.height); //kLandHeight);
    
    float duraDown = getDuraDown(uppos, downpos); //time of falling down
    
    ///
    ///
    //CCSize win = CCDirector::sharedDirector()->getWinSize();
    //TODO : make the fall looks better
    ///
    ///

    
    //-- face up jump --
    
    CCMoveTo* moveUp = CCMoveTo::create(kJumpDura, uppos);
    CCRotateTo* faceUp = CCRotateTo::create(kJumpDura, -30);
    CCSpawn* fly = CCSpawn::create(faceUp, CCEaseSineOut::create(moveUp), NULL);
    
    //-- face down fall --
    
    CCMoveTo* fallDown = CCMoveTo::create(duraDown, downpos);
    CCRotateTo* faceDown = CCRotateTo::create(duraDown, 90);
    CCSpawn* fall = CCSpawn::create(faceDown, CCEaseSineIn::create(fallDown), NULL);
    
    //-- anim fly and fall --
    
    CCSequence* tap = CCSequence::create(fly, fall, NULL);
    curAction = tap;
    
    parent->runAction(tap);
}

float Flying::getDuraDown(cocos2d::CCPoint up, cocos2d::CCPoint down){
    float dy = up.y - down.y;
    
    CCSize win = CCDirector::sharedDirector()->getWinSize();
    dy = win.height/1.5;
    
    
    float dura;
    
    if (dy < kJumpHeight) {
        dura = kJumpDura/2;
    }else{
        dura = dy * (kJumpDura/2) / kJumpHeight;
    }
    return dura;
}

void Flying::actionHit(){
    CCPoint upPos = parent->getPosition();
    parent->stopAllActions();
    
    CCPoint downPos = ccp(upPos.x, kLandHeight);
    float duraDown = getDuraDown(upPos, downPos) * 1/2;
    
    CCMoveTo* movedown = CCMoveTo::create(duraDown, downPos);
    CCRotateTo* facedown = CCRotateTo::create(duraDown, 90);
    CCSpawn* fall = CCSpawn::create(facedown, CCEaseSineIn::create(movedown), NULL);
    
    curAction = fall;
    parent->runAction(fall);
}

void Flying::stopAction(){
    if (curAction) {
        parent->stopAction(curAction);
        curAction = NULL;
    }
}