#include "HelloWorldScene.h"
#include "config.h"
#include "SimpleAudioEngine.h"
#include "MoveLoop.h"
#include "Flying.h"
#include "MoveLeft.h"
#include "AddPipe.h"
#include "Collision.h"
#include "TouchGround.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
  
    /*
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    */
    
    
    s = CCDirector::sharedDirector()->getWinSize();
    
    
    //load resource
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flappy.plist");
    
    //preload sound
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(SoundTap);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(SoundHit);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(SoundPoint);
    
    resetGame();
    
    this->addComponent(new AddPipe());
    GameOver = true;
    return true;
}

void HelloWorld::resetGame()
{
    this->removeAllChildrenWithCleanup(true);
    
    GameOver = false;
    
    addBackground();
    addBird();
    addLand();
    addLayerMoveLeft();
    addLabelScore();
    
}

void HelloWorld::addBackground(){
    CCSprite* bg = CCSprite::createWithSpriteFrameName("background.png");
    bg->CCNode::setPosition(s.width/2, s.height/2);
    this->addChild(bg);
}

void HelloWorld::addLand(){
    land = CCSprite::createWithSpriteFrameName("land.png");
    
    land->setZOrder(2); //pipe is 1
    land->setAnchorPoint(ccp(0, 0));
    
    land->CCNode::setPosition(-10, -4);
    
    land->addComponent(new MoveLoop());
    land->addComponent(new TouchGround(bird));
    
    this->addChild(land);
}

void HelloWorld::addBird(){
    bird = new Bird();
    bird->setZOrder(1000); //to the very front
    bird->CCNode::setPosition(s.width/2, s.height/2);
    
    // add Flying Component!!  finally!
    bird->addComponent(new Flying());
    
    this->addChild(bird);
}

void HelloWorld::addLayerMoveLeft(){
    layerMoveLeft = CCParallaxNode::create();
    layerMoveLeft->addComponent(new MoveLeft(land->boundingBox().size.width));
    this->addChild(layerMoveLeft);
}

void HelloWorld::addPipe(){
    int r = rand();
    r = r%5;
    float dy = r * 15;
    
    r = rand();
    r = r%2;
    if (r == 0) {
        dy = -dy;
    }
    
    
    CCSprite* pipe1 = CCSprite::createWithSpriteFrameName("pipe1.png");
    CCPoint pos = ccp(s.width + pipe1->boundingBox().size.width/2,
                      s.height/2 + pipe1->boundingBox().size.height/2 + kHoleBetweenPipe/2 + kLandHeight/2 + dy);
    pipe1->setPosition(layerMoveLeft->convertToNodeSpace(pos));
    
    CCSprite* pipe2 = CCSprite::createWithSpriteFrameName("pipe2.png");
    pos.y = s.height/2 - pipe2->boundingBox().size.height/2 - kHoleBetweenPipe + kLandHeight/2 + dy;
    pipe2->setPosition(layerMoveLeft->convertToNodeSpace(pos));
    
    // set priority
    pipe1->setZOrder(land->getZOrder()-1);
    pipe2->setZOrder(land->getZOrder()-1);
    
    pipe1->addComponent(new Collision(bird, true));
    pipe2->addComponent(new Collision(bird, false));
    
    layerMoveLeft->CCNode::addChild(pipe1);
    layerMoveLeft->CCNode::addChild(pipe2);
}

void HelloWorld::addLabelScore(){
    CCString* strScore = CCString::createWithFormat("%d", bird->score);
    lScore = CCLabelTTF::create(strScore->getCString(), "", 30);
    lScore->setZOrder(getChildrenCount());
    lScore->CCNode::setPosition(s.width/2, s.height - 40);
    
    this->addChild(lScore);
}

void HelloWorld::updateUiScore(){
    CCString* strScore = CCString::createWithFormat("%d", bird->score);
    lScore->setString(strScore->getCString());
}


void HelloWorld::playSound(std::string name){
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(name.c_str());
}

/////------------------- event handlers ----------

void HelloWorld::onEnter(){
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::onEnter();
}

void HelloWorld::onExit(){
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    
    if (GameOver) {
        resetGame();
        bird->tap();
    }else
        bird->tap();
    
    return true;
}



/*
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
*/
