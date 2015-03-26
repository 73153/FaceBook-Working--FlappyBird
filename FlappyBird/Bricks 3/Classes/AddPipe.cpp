//
//  AddPipe.cpp
//  FlappyBird
//
//  Created by meng-x on 2014/03/16.
//
//

#include "AddPipe.h"
#include "config.h"


AddPipe::AddPipe(){
    setName("AddPipe");
    dura = 0;
}

void AddPipe::update(float delta){
    parent = (HelloWorld*)getOwner();
    if (!parent) {
        return;
    }
    
    dura += delta;
    if (dura >= kTimeToAddPipe) {
        dura = 0;
        parent->addPipe();
    }
}