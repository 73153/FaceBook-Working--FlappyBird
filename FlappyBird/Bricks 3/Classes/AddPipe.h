//
//  AddPipe.h
//  FlappyBird
//
//  Created by meng-x on 2014/03/16.
//
//

#ifndef __FlappyBird__AddPipe__
#define __FlappyBird__AddPipe__

#include <cocos2d.h>
#include "HelloWorldScene.h"

using namespace cocos2d;

class AddPipe : public CCComponent{
public:
    AddPipe();
private:
    HelloWorld* parent;
    
    float dura; //between pipe
    virtual void update(float delta);
};

#endif /* defined(__FlappyBird__AddPipe__) */
