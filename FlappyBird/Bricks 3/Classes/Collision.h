//
//  Collision.h
//  FlappyBird
//
//  Created by meng-x on 2014/03/16.
//
//

#ifndef __FlappyBird__Collision__
#define __FlappyBird__Collision__

#include <cocos2d.h>
#include "Bird.h"

using namespace cocos2d;

class Collision : public CCComponent{
public:
    Collision(Bird* bird, bool getScore);
    
private:
    CCNode* parent;
    Bird* bird;
    bool getScore;
    
    virtual void update(float delta);
    
    bool isCollision();
    CCPoint getWorldPosition();
};

#endif /* defined(__FlappyBird__Collision__) */
