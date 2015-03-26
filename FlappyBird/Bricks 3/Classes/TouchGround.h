//
//  TouchGround.h
//  FlappyBird
//
//  Created by meng-x on 2014/03/18.
//
//

#ifndef __FlappyBird__TouchGround__
#define __FlappyBird__TouchGround__

#include <cocos2d.h>
#include "Bird.h"

using namespace cocos2d;

class TouchGround : public CCComponent{
public:
    TouchGround(Bird* bird);
    
private:
    CCNode* parent;
    Bird* bird;
    
    virtual void onEnter();
    virtual void update(float delta);
    bool isCollision();
};

#endif /* defined(__FlappyBird__TouchGround__) */
