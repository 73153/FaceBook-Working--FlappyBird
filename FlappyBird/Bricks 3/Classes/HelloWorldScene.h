#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Bird.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    //void menuCloseCallback(CCObject* pSender);
    
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
    void addPipe();
    bool GameOver;
    
    void resetGame();
    
    void updateUiScore();
    void playSound(std::string name);
    
private:
    cocos2d::CCSprite* land;
    Bird* bird;
    CCParallaxNode* layerMoveLeft;
    CCLabelTTF* lScore;
    cocos2d::CCSize s;
    
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    
    void addBackground();
    void addLand();
    void addBird();
    void addLayerMoveLeft();
    
    void addLabelScore();
};

#endif // __HELLOWORLD_SCENE_H__
