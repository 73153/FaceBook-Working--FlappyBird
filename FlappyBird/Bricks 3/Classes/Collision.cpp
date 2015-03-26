//
//  Collision.cpp
//  FlappyBird
//
//  Created by meng-x on 2014/03/16.
//
//

#include "Collision.h"


Collision::Collision(Bird* bird, bool getScore){
    setName("Collision");
    this->bird = bird;
    this->getScore = getScore;
}


void Collision::update(float delta){
    parent = (CCNode*)getOwner();
    if (!parent) {
        return;
    }
    
    if (bird->status == isDie) {
        return;
    }
    //printf("getWorldPosition...%f\n", getWorldPosition().x);
    if (getScore) {
        if (getWorldPosition().x <= bird->getPosition().x) {

            getScore = false;
            bird->updateScore();
        }
    }
    
    if (isCollision()) {
        bird->hit();
    }
}

bool Collision::isCollision()
{
    CCSize spipe = parent->boundingBox().size;
    CCPoint ppipe = getWorldPosition();
    
    CCSize sbird = bird->boundingBox().size;
    CCPoint pbird = bird->getPosition();
    
    float d = 6; 
    
    //the collision area is just like the symbol "#"
    
    //first get the # of those two areas, pipe and bird.
    
    float maxx1 = ppipe.x + spipe.width/2 - d;
    float minx1 = ppipe.x - spipe.width/2 + d;
    float maxy1 = ppipe.y + spipe.height/2 - d;
    float miny1 = ppipe.y - spipe.height/2 + d;
    
    float maxx2 = pbird.x + sbird.width/2 - d;
    float minx2 = pbird.x - sbird.width/2 + d;
    float maxy2 = pbird.y + sbird.height/2 - d;
    float miny2 = pbird.y - sbird.height/2 + d;
    
    //so here is the situation that the bird has no possibility to collision with the pipe
    //just need to return the opposite of this.
    return !(
             maxx1 < minx2 ||
             maxx2 < minx1 ||
             maxy1 < miny2 ||
             maxy2 < miny1
             );
    
}

CCPoint Collision::getWorldPosition()
{
    return ((CCParallaxNode*)parent->getParent())->convertToWorldSpace(parent->getPosition());
}













