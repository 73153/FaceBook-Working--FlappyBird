//
//  Bird.h
//  FlappyBird
//
//  Created by meng-x on 2014/03/16.
//
//

#ifndef __FlappyBird__Bird__
#define __FlappyBird__Bird__

#include <cocos2d.h>

using namespace cocos2d;

typedef enum {
    isTap,
    isDie,
    isHit,
    isIdle
} BirdStatus;

class Bird : public CCSprite{
public:
    Bird();
    
    BirdStatus status;
    int score;
    
    void tap();
    void hit();
    void updateScore();
    
private:
    CCRepeatForever* flying();
};

#endif /* defined(__FlappyBird__Bird__) */
