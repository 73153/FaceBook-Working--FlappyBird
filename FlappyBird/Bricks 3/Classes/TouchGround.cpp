//
//  TouchGround.cpp
//  FlappyBird
//
//  Created by meng-x on 2014/03/18.
//
//

#include "TouchGround.h"
#include "HelloWorldScene.h"


TouchGround::TouchGround(Bird* bird){
    setName("TouchGround");
    this->bird = bird;
}

void TouchGround::onEnter(){
    
}

void TouchGround::update(float delta){
    parent = (CCNode*)getOwner();
    if (!parent) {
        return;
    }
    
    if ( ((HelloWorld*)parent->getParent())->GameOver )
    {
        parent->stopAllActions();
    }
    
    if (bird->status == isDie) {
        return;
    }
    if (isCollision()) {
        bird->hit();
        bird->stopAllActions();
    }
}

bool TouchGround::isCollision(){
    CCSize sland = parent->boundingBox().size;
    CCPoint pland = parent->getPosition();
    
    CCSize sbird = bird->boundingBox().size;
    CCPoint pbird = bird->getPosition();
    
    float d = 6;

    
    float maxx1 = pland.x + sland.width - d;
    float minx1 = pland.x + d;
    float maxy1 = pland.y + sland.height - d;
    float miny1 = pland.y + d;
    
    float maxx2 = pbird.x + sbird.width/2 - d;
    float minx2 = pbird.x - sbird.width/2 + d;
    float maxy2 = pbird.y + sbird.height/2 - d;
    float miny2 = pbird.y - sbird.height/2 + d;
    
    return !(
             maxx1 < minx2 ||
             maxx2 < minx1 ||
             maxy1 < miny2 ||
             maxy2 < miny1
             );
}







