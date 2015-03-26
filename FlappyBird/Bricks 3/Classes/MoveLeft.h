//
//  MoveLeft.h
//  FlappyBird
//
//  Created by meng-x on 2014/03/16.
//
//

#ifndef __FlappyBird__MoveLeft__
#define __FlappyBird__MoveLeft__

#include <cocos2d.h>

using namespace cocos2d;

class MoveLeft : public CCComponent{
public:
    MoveLeft(float w);
private:
    float w;
    virtual void onEnter();
    virtual void update(float delta);

    void actionMove();
};

#endif /* defined(__FlappyBird__MoveLeft__) */
