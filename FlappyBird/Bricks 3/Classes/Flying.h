//
//  Flying.h
//  FlappyBird
//
//  Created by meng-x on 2014/03/16.
//
//

#ifndef __FlappyBird__Flying__
#define __FlappyBird__Flying__

#include <cocos2d.h>
#include "Bird.h"

using namespace cocos2d;

class Flying : public CCComponent{
public:
    Flying();
    
    void actionTap();
    void actionHit();
    void stopAction();
    
private:
    Bird* parent;
    
    CCAction* curAction;
    float getDuraDown(CCPoint up, CCPoint down);
    
    virtual void update(float delta);
};

#endif /* defined(__FlappyBird__Flying__) */
