//
//  MoveLoop.h
//  FlappyBird
//
//  Created by meng-x on 2014/03/16.
//
//

#ifndef __FlappyBird__MoveLoop__
#define __FlappyBird__MoveLoop__

#include <cocos2d.h>

using namespace cocos2d;

class MoveLoop : public CCComponent{
public:
    MoveLoop();
    
private:
    virtual void onEnter();
    virtual void update(float delta);

    void actionMove();
};

#endif /* defined(__FlappyBird__MoveLoop__) */
