/*
The MIT License (MIT)

Copyright (c) 2013 SuXinDe (Email: suxinde2009@126.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
//add weixin friend
//http://blog.umeng.com/index.php/2013/09/android-cocos2d-x%E6%B8%B8%E6%88%8F%E5%A6%82%E4%BD%95%E9%9B%86%E6%88%90%E5%8F%8B%E7%9B%9F%E7%A4%BE%E4%BC%9A%E5%8C%96%E7%BB%84%E4%BB%B6%E5%88%86%E4%BA%AB%E5%8A%9F%E8%83%BD/
//http://blog.csdn.net/cangely/article/details/19809139
#include "HelloWorldScene.h"
//找字体
//http://www.zhaozi.cn/html/fonts/china/mini/2008-06-26/21636.html
#include "SGPlayMusic.h"
// git排除目录
// .gitignore 里添加两行 
// /libs/* 
// !/libs/armeabi 
// /libs/armeabi/* 
// !libs/armeabi/libjnigraphics.so 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "MobClickCpp.h"
#endif
#include <stdlib.h> 
USING_NS_CC;
//clone Flappy Bird
//http://comandobueiro.com.br/flappy-bird-clone-com-appcelerator-e-lanica.html
//http://lanica.co/flappy-clone/
//图片去底
//http://www.2gei.com/bgremover
//android key good@bird
float gBardis ;

static CCString ScreenShoot()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();  
	//定义一个屏幕大小的渲染纹理  
	CCRenderTexture* pScreen = CCRenderTexture::create(size.width,size.height, kCCTexture2DPixelFormat_RGBA8888);  
	//获得当前的场景指针  
	CCScene* pCurScene = CCDirector::sharedDirector()->getRunningScene();  
	//渲染纹理开始捕捉  
	pScreen->begin();  
	//当前场景参与绘制  
	pCurScene->visit();  
	//结束捕捉  
	pScreen->end();  
	char szfile[1024]={0};
	//string   pathToSave = "/sdcard/download/";//

	string pathToSave = "ScreenShoot.png";
	//sprintf(szfile,"%s%s",pathToSave.c_str(),"share.png");
	//保存为png
	if(pScreen->saveToFile(pathToSave.c_str(), kCCImageFormatPNG))
	{
		//CCMessageBox("saveToFile ok!", "saveToFile ok!");
	}
	pathToSave = CCFileUtils::sharedFileUtils()->getWritablePath()+pathToSave;
	//保存为jpg
	//pScreen->saveToFile("XXXXXX.jpg", kCCImageFormatJPEG);  
	CC_SAFE_DELETE(pScreen); 

	return pathToSave.c_str();
}
	
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "android/log.h"
#include "platform/android/jni/JniHelper.h"
#define  JAVA_PACKAGE_NAME "com/qgame/myflappybird/FlappyBird"
extern "C"{


	JNIEXPORT void JNICALL Java_com_qgame_myflappybird_FlappyBird_nativeShareReturn(JNIEnv* env, jobject thiz,jint result)
	{
		CCString *strresult = CCString::createWithFormat("%d",(unsigned int)result);
		//CCMessageBox(strresult->getCString(),strresult->getCString());
		if(((unsigned int)result)==0)
		{
			//CCMessageBox(strresult->getCString(),strresult->getCString());
			MobClickCpp::event("SNS_SHARE_OK");
		}
	}

	void Feedback()
	{
		// CCMessageBox("function Share() in !", "function Share() in!");
		bool hasMethod;
		JniMethodInfo jni_methodInfo;
		hasMethod = JniHelper::getStaticMethodInfo(jni_methodInfo, JAVA_PACKAGE_NAME, "Feedback", "()V");
		if(hasMethod){
			//CCMessageBox("function Share() was found!", "function Share() was found!");
			// CCLog("function Share() was found");
			if(jni_methodInfo.methodID){
				jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID,  jni_methodInfo.methodID);
				//CCLog("function Share() was called");
				//CCMessageBox("function Share() was called!", "function Share() was called!");
			}
		}else{
			//CCLog("function Share() was not found");
			//CCMessageBox("function Share() was not found!", "function Share() was not found!");
		}
	}


	//不使用官方SDK,实现微信分享（发送到朋友，发送到朋友圈）
	//java与c++传参数
	//http://wulin9005.blog.163.com/blog/static/13239748820132675949658/
	//http://blog.sina.com.cn/s/blog_7018d38201013znr.html
	void ShareWeixin()
	{

		bool hasMethod;
		JniMethodInfo jni_methodInfo;
		hasMethod = JniHelper::getStaticMethodInfo(jni_methodInfo, JAVA_PACKAGE_NAME, "shareTxtToTimeLine", "(Ljava/lang/String;)V");
		if(hasMethod){
			//CCMessageBox("function Share() was found!", "function Share() was found!");
			// CCLog("function Share() was found");
			if(jni_methodInfo.methodID){
				jstring stringargs = jni_methodInfo.env->NewStringUTF(ScreenShoot().getCString());   //将string转为jstring
				jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID,  jni_methodInfo.methodID,stringargs);
				//CCLog("function Share() was called");
				//CCMessageBox("function Share() was called!", "function Share() was called!");
			}
		}else{
			//CCLog("function Share() was not found");
			//CCMessageBox("function Share() was not found!", "function Share() was not found!");
		}
	}

	void 	openUmengShareJni()
	{
		JniMethodInfo jni_methodInfo  ;
		//  获取UmengGameActivity的静态方法openShareBoard	
		bool isHave = JniHelper::getStaticMethodInfo(jni_methodInfo,JAVA_PACKAGE_NAME, "openShareBoard", "(Ljava/lang/String;)V");

		if ( isHave )
		{
			jstring stringargs = jni_methodInfo.env->NewStringUTF(ScreenShoot().getCString());   //将string转为jstring
			// 实际调用UmengGameActivity中打开umeng分享平台选择面板
			jni_methodInfo.env->CallStaticVoidMethod( jni_methodInfo .classID, jni_methodInfo.methodID,stringargs);
		}
	}
}
#endif


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
	mbirdstr.push_back("birdblue");
	mbirdstr.push_back("birdred");
	mbirdstr.push_back("birdyellow");
	mbirdclolor=rand()%100%3;
//	EFFECT_PLAY(true,MUSIC_SWOOSHING);
	//播放背景音乐
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MUSIC_JUMP, true);
	////////////////////////////////////////////////////
	// 下面添加创建自己的Sprite的代码
	////////////////////////////////////////////////////

	mScreenSize = CCDirector::sharedDirector()->getWinSize();
	mFPS=CCDirector::sharedDirector()->getAnimationInterval();


	mfac=CCDirector::sharedDirector()->getContentScaleFactor();
	mfax = 320.f/mScreenSize.width;
	//gBardis = 5.f*gUpVelocity/mfac;
	gBardis = mScreenSize.height*2.f/9.0f-5;
	//m_addbartime=mScreenSize.width*mFPS*mfax*1.f/2.f/MOVESPEED-10;
	//m_addbartime=3;
	initWorld();	
	for (int i = 0; i<GBACKGROUNDNUM; i++)
	{
		addBackGround(i);
	}
	int imaxgroundnum=1;
	for (int i = 0; i<imaxgroundnum; i++)
	{
		imaxgroundnum=addGround(i);
	}
	m_ilastground=imaxgroundnum-1;
	CCLOG("m_pLastGround->boundingBox() first x:%f,:%f  ", (m_pGroundVec[m_ilastground])->boundingBox().getMaxX(),
		(m_pGroundVec[m_ilastground])->boundingBox().size.width);
	addStart();
	addTop();
	//addRate();
	//addFlappyBird();

	addBird();
	addScore();
	addGameOver();

	m_pScore->setVisible(false);
	m_pGameOver->setVisible(false);
	this->goReady();
	addBarContainer();

	setTouchEnabled(true);
	myflag=0;
	m_istatus=GETREADY;
	m_bhitbar=false;
	//scheduleOnce(schedule_selector(HelloWorld::startGame), 1);
	this->scheduleUpdate();
	//创建动画
	initAction();
	myangle=0.f;
	return true;
}

void HelloWorld::goReady(){
	m_pStart->setVisible(false);
	m_pTop->setVisible(false);
	//m_pRate->setVisible(false);
	//m_pFlappyBird->setVisible(false);
	m_pScore->setVisible(false);
	m_pGameOver->setVisible(false);
	addHand();
	addLeftTap();
	addRightTap();
	addUp();
	addGBird();
	addReady();
	//计分板
	addNumberNode();
	float fupx=m_pLeftTap->boundingBox().getMinX();
	float fupy=m_pUp->boundingBox().getMaxY();
	mBird->setPosition(ccp(fupx,fupy));

}


void HelloWorld::startGame(float dt){

	m_pUp->setVisible(false);
	m_pHand->setVisible(false);
	m_pLeftTap->setVisible(false);
	m_pRightTap->setVisible(false);
	m_pGBird->setVisible(false);
	m_pReady->setVisible(false);

	m_istatus=RUNNING;
	myscore=-1;

	//scheduleUpdate();
	//schedule(schedule_selector(HelloWorld::addBar),gAddBarTime*1.0/mfac);
	//每半个屏就放一对柱子
	//schedule(schedule_selector(HelloWorld::addBar),this->m_addbartime);
	scheduleOnce(schedule_selector(HelloWorld::addBar),3);
	//goReady();
}

void HelloWorld::stopGame(){

	msnn = ShowNumberNode::CreateShowNumberNode("menu_num.png", 923, 22/mfac, 30/mfac  ); 
	float xpos=m_pScore->boundingBox().getMaxX()-m_pScore->boundingBox().getMinX()-45/mfac;
	float ypos=m_pScore->boundingBox().getMaxY()-m_pScore->boundingBox().getMinY()-45/mfac;
	if (testnum>=10)
	{
		xpos = 192/mfac;
	}
	msnn->setPosition(ccp(xpos,ypos));  
	m_pScore->addChild(msnn,5,0);  
	//mBird->setVisible(false);
	ShowNumberNode * snn = (ShowNumberNode *)this->getChildByTag(0);
	snn->setVisible(false);
	//schedule(schedule_selector(HelloWorld::ScoreSchedule),0.05);
	mBird->stopAllActions();
	unscheduleUpdate();
	unschedule(schedule_selector(HelloWorld::addBar));
	m_pGameOver->setVisible(false);
	//m_pGameOver->setPosition(ccp(gameoverX,gameoverY));
	//CCActionInterval*  actionFade= CCFadeIn::create(1);
	//m_pGameOver->setVisible(true);
	//m_pScore->setVisible(true);

	//CCActionInterval*  actionFadeBack= actionFade->reverse();
	//m_pGameOver->runAction(CCSequence::create(actionFade,actionFadeBack,NULL));
	//m_pGameOver->runAction(actionFade);

	//m_pGameOver->setVisible(true);
	//动画效果 http://www.cocos2dev.com/?p=72
	scheduleOnce(schedule_selector(HelloWorld::MoveGameOver), 1);	

	//scheduleOnce(schedule_selector(HelloWorld::MoveStart), 2.5);
	//scheduleOnce(schedule_selector(HelloWorld::MoveTop), 2.5);
	scheduleOnce(schedule_selector(HelloWorld::MoveScore), 1);
	//scheduleOnce(schedule_selector(HelloWorld::MoveScoreAdd), 2);

	m_istatus=GAMEOVER;

	//addScoreNum(this->testnum);
}

void HelloWorld::MoveGameOver(float dt)  
{ 
	CCPoint pointBg=ccp(gameoverX,gameoverY);
	m_pGameOver->setPosition(pointBg);
	m_pGameOver->setVisible(true);
	CCPoint pointL=pointBg;
	CCPoint pointR=pointBg;
	pointL.y-=3;
	pointR.y+=3;
	CCMoveTo* moveLeft=CCMoveTo::create(0.08, pointL);
	CCMoveTo* moveRight=CCMoveTo::create(0.08, pointR);
	CCFiniteTimeAction* action= CCSequence::create(moveLeft,moveRight,NULL);
	//永久重复运动CCRepeatForever和指定次数的重复动作CCRepeat
	//http://5.quanpao.com/?p=274
	CCActionInterval* actionShake=CCRepeat::create((CCActionInterval*)action,2);
	m_pGameOver->stopAllActions();
//	EFFECT_PLAY(true,MUSIC_SWOOSHING);
	m_pGameOver->runAction(actionShake);

	/*CCBSequence *seq = getSequence(nSeqId);
	CCAction *completeAction = CCSequence::createWithTwoActions(
	CCDelayTime::create(seq->getDuration() + fTweenDuration),
	CCCallFunc::create(this, callfunc_selector(CCBAnimationManager::sequenceCompleted)));
	mRootNode->runAction(completeAction);*/

}


void HelloWorld::MoveScoreAdd(float dt)  
{  

	addBestScore();
	//addSilver();
	if (testnum>=15)
	{
		addGold();
	}
	else if (testnum>=10)
	{
		addSilver();
	}
	if(m_bnew)
	{
		addNew();
	}
	if (testnum>=10)
	{
		m_emitter = CCParticleFlower::createWithTotalParticles(10);
		m_emitter->retain();
		m_pScore->addChild(m_emitter);
		m_emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage("stars.png") );
		m_emitter->setPosition(ccp(49/mfac,51/mfac));
	}
}

void HelloWorld::MoveScore(float dt)  
{  
	
	msnn->f_ShowNumber(0);
	float fmovetime=0.2;
	m_pScore->setPosition(ccp(scoreX,0));
	CCActionInterval* actionTo = CCMoveTo::create(fmovetime, ccp(scoreX, scoreY));
//	EFFECT_PLAY(true,MUSIC_SWOOSHING);
	m_pScore->runAction(actionTo);
	m_pScore->setVisible(true);
	MoveScoreAdd(0);
	schedule(schedule_selector(HelloWorld::ScoreSchedule),0.1);
}

void HelloWorld::MoveStart(float dt)  
{  
	float fmovetime=0.2;
	m_pStart->setPosition(ccp(startX,0));
	CCActionInterval* actionTo = CCMoveTo::create(fmovetime, ccp(startX, startY));
	m_pStart->runAction(actionTo);
	m_pStart->setVisible(true);
}


void HelloWorld::MoveTop(float dt)  
{  
	float fmovetime=0.2;
	m_pTop->setPosition(ccp(topX,0));
	CCActionInterval* actionTo = CCMoveTo::create(fmovetime, ccp(topX, topY));
	m_pTop->runAction(actionTo);
	m_pTop->setVisible(true);
}

void HelloWorld::addNew()  
{  
	CCSprite *m_new= CCSprite::create("new.png");     
	m_new->setPosition(ccp(145/mfac,50/mfac));  
	m_pScore->addChild(m_new,SPRITE_TAG_BIRD,0); 

} 

void HelloWorld::addBestScore()  
{  
	ShowNumberNode *snn = ShowNumberNode::CreateShowNumberNode("menu_num.png", 923, 22/mfac, 30/mfac  ); 
	snn->f_ShowNumber(m_bestscore); 
	if (m_bestscore>=10)
	{
		snn->setPosition(ccp(192/mfac,30/mfac));
	}
	else
		snn->setPosition(ccp(184/mfac,30/mfac));  
	m_pScore->addChild(snn,5,0);    
	//schedule(schedule_selector(HelloWorld::logic), 2.0f);  

}  

void HelloWorld::initWorld(){
	mWorld = new b2World(b2Vec2(0, gDownVelocity*1.0/mfac));
	mWorld->SetContactListener(this);
}
//http://blog.csdn.net/farsa/article/details/9430155
void HelloWorld::addNumberNode()  
{  
	testnum=0;
	ShowNumberNode *snn = ShowNumberNode::CreateShowNumberNode("menu_num.png", 923, 22/mfac, 30/mfac  ); 
	snn->f_ShowNumber(testnum);  
	float xpos=mScreenSize.width/2.0f;
	float ypos=m_pReady->boundingBox().getMaxY()+15.f/mfax;
	//float ypos=mScreenSize.height*5/7.0f;
	snn->setPosition(ccp(xpos,ypos));  
	this->addChild(snn,5,0);  
	//schedule(schedule_selector(HelloWorld::logic), 2.0f);  

}  


void HelloWorld::addGold() {
	CCSprite *m_gold= CCSprite::create("gold.png");     
	m_gold->setPosition(ccp(49/mfac,51/mfac));  
	m_pScore->addChild(m_gold,SPRITE_TAG_BIRD,0);  



}


void HelloWorld::addSilver() {
	CCSprite *m_silver= CCSprite::create("silver.png");     
	m_silver->setPosition(ccp(49/mfac,51/mfac));  

	CCActionInterval *blinkAction = CCBlink::create(10, 20); 
	//m_silver->runAction(blinkAction); 
	m_pScore->addChild(m_silver,5,0); 
}




void HelloWorld::ScoreSchedule(float dt)  
{  
	//ShowNumberNode * snn = ShowNumberNode::CreateShowNumberNode("menu_num_white.png", 923, 9/mfac, 18/mfac  );  

	//float xpos=m_pScore->boundingBox().getMaxX()-m_pScore->boundingBox().getMinX()-45;
	//float ypos=m_pScore->boundingBox().getMaxY()-m_pScore->boundingBox().getMinY()-45;

	if (++myscore<= testnum)
	{
		msnn->f_ShowNumber(myscore);
	}
	else
	{
		scheduleOnce(schedule_selector(HelloWorld::MoveStart), 0.5);
		scheduleOnce(schedule_selector(HelloWorld::MoveTop), 0.5);
		unschedule(schedule_selector(HelloWorld::ScoreSchedule));
		scheduleOnce(schedule_selector(HelloWorld::addShare), 1);

		//m_pStart->setVisible(true);
		//m_pTop->setVisible(true);
	}
	//snn->setPosition(ccp(xpos,ypos));  
	//m_pScore->addChild(snn,5,0);  
	//schedule(schedule_selector(HelloWorld::logic), 2.0f);  

}  
void HelloWorld::logic(float dt)
{
	ShowNumberNode * snn = (ShowNumberNode *)this->getChildByTag(0);
	msnn->f_ShowNumber(testnum );
	testnum = testnum +1;


}

//number png http://i.pku.edu.cn/trac/bazinga/changeset/117/
void HelloWorld::initAction()
{
	string strplist=mbirdstr[mbirdclolor]+".plist";
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(strplist.c_str());
	CCArray *animFrames = CCArray::create();
	for (int i = 1; i < 4; i++)
	{
		CCString *name = CCString::createWithFormat("%s%d.png",mbirdstr[mbirdclolor].c_str(),i);
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name->getCString());
		if(i == 1)
		{
			CCSprite *pSprite = CCSprite::createWithSpriteFrame(frame);
			this->addChild(pSprite);
		}
		animFrames->addObject(frame);
	}    
	CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1);
	CCAnimate* pAnimate = CCAnimate::create(animation);
	CCActionInterval* seq = (CCActionInterval *)(CCSequence::create(pAnimate, NULL));    // 创建顺序执行action
	mBird->runAction(CCRepeatForever::create(seq));
	/*

	CCAnimation *pAnimation = CCAnimation::createWithSpriteFrames(pAniFrame, 0.05f);    // 0.05f是指经过50毫秒就要切换下一张png图
	CCAnimate* pAnimate = CCAnimate::create(pAnimation);
	CCActionInterval* seq = (CCActionInterval *)(CCSequence::create(pAnimate, NULL));    // 创建顺序执行action
	pSprite->runAction(CCRepeatForever::create(seq));*/
}



void HelloWorld::addStart() {
	m_pStart = CCSprite::create("start.png");     
	CCRect rcBounding = m_pStart->boundingBox();
	CCSize startSize = m_pStart->getContentSize();
	float fStartWidth=rcBounding.size.width/2.0f;
	float fStartHeight=rcBounding.size.height/2.0f;
	//m_pStart->setPosition(ccp(this->mScreenSize.width/2.0f/RATIO-(fStartWidth/2.0f), 
	//	groundSize.height/2.0f/RATIO+(fStartHeight/2.0f)));    // 设置在屏幕中间  
	startX=mScreenSize.width/2.0f-fStartWidth-10.f;
	startY=mgroundSize.height+startSize.height/2;
	//xpos-=rcBounding.size.width/2.;
	m_pStart->setPosition(ccp(startX,  startY));    // 设置在屏幕中间  
	this->addChild(m_pStart,SPRITE_TAG_CHAR);    // CHILD_ORDER_BACKGROUND精灵的层级，这里是 = 1  

}

void HelloWorld::addShare(float dt) {
	m_pShare = CCSprite::create("share.png");     
	CCRect rcBounding = m_pShare->boundingBox();
	CCSize startSize = m_pShare->getContentSize();
	float fWidth=rcBounding.size.width/2.0f;
	float fHeight=rcBounding.size.height/2.0f;
	//m_pStart->setPosition(ccp(this->mScreenSize.width/2.0f/RATIO-(fStartWidth/2.0f), 
	//	groundSize.height/2.0f/RATIO+(fStartHeight/2.0f)));    // 设置在屏幕中间  
	float shareX=this->mScreenSize.width/2.0f;
	float shareY=m_pGroundVec[0]->getContentSize().height*3.f/4;
	//xpos-=rcBounding.size.width/2.;
	CCPoint pointBg=ccp(shareX,  shareY);
	m_pShare->setPosition(pointBg);    // 设置在屏幕中间 
	CCPoint pointL=pointBg;
	CCPoint pointR=pointBg;
	pointL.y-=3;
	pointR.y+=3;
	CCMoveTo* moveLeft=CCMoveTo::create(0.2, pointL);
	CCMoveTo* moveRight=CCMoveTo::create(0.2, pointR);
	CCFiniteTimeAction* action= CCSequence::create(moveLeft,moveRight,NULL);
	CCActionInterval* actionShake=CCRepeatForever::create((CCActionInterval*)action);
	m_pShare->stopAllActions();
//	EFFECT_PLAY(true,MUSIC_SWOOSHING);
	m_pShare->runAction(actionShake);
	this->addChild(m_pShare,SPRITE_TAG_CHAR);    // CHILD_ORDER_BACKGROUND精灵的层级，这里是 = 1  
	CCMenuItemImage *pMenuItemTop = CCMenuItemImage::create("share.png", "share.png", this, menu_selector(HelloWorld::openUmengShare) );
	pMenuItemTop->setPosition(ccp( m_pShare->getContentSize().width/2,  m_pShare->getContentSize().height/2));
	CCMenu* pMenu1 =CCMenu::create(pMenuItemTop, NULL);

	pMenu1->setPosition( CCPointZero );
	m_pShare->addChild(pMenu1);
}


void HelloWorld::addFlappyBird() {
	m_pFlappyBird = CCSprite::create("flappybird.png");     
	CCRect rcBounding = m_pFlappyBird->boundingBox();
	float fFlappyBirdHeight=rcBounding.size.height/2.0f;
	float xpos=mScreenSize.width/2;
	float ypos=mScreenSize.height/2+fFlappyBirdHeight+30;

	m_pFlappyBird->setPosition(ccp(xpos,  ypos));    // 设置在屏幕中间  
	this->addChild(m_pFlappyBird, SPRITE_TAG_CHAR);  

}

void HelloWorld::addRate() {
	m_pRate = CCSprite::create("rate.png");     
	CCRect rcBounding = m_pRate->boundingBox();
	CCSize startSize = m_pRate->getContentSize();
	float fRateWidth=rcBounding.size.width/2.0f;
	float fRateHeight=rcBounding.size.height/2.0f;
	//m_pStart->setPosition(ccp(this->mScreenSize.width/2.0f/RATIO-(fStartWidth/2.0f), 
	//	groundSize.height/2.0f/RATIO+(fStartHeight/2.0f)));    // 设置在屏幕中间  
	float xpos=mScreenSize.width/2.0f;
	float ypos=m_pTop->boundingBox().getMaxY()+fRateHeight+10;
	//xpos-=rcBounding.size.width/2.;
	m_pRate->setPosition(ccp(xpos,  ypos));    // 设置在屏幕中间  
	this->addChild(m_pRate, SPRITE_TAG_CHAR);  


}

//  按钮的回调函数
void HelloWorld::openUmengShare(CCObject * pSender)
{
	if(!m_pTop->isVisible())
	{
		return;
	}
	int i=0;
	i++;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//CCMessageBox("openUmengShare!", "openUmengShare!");
	//return;
	//  调用jni方法
	//Share();
	//ShareWeixin();
	openUmengShareJni();
#endif
	//ScreenShoot();
}



void HelloWorld::addTop() {
	CCSize groundSize = m_pGroundVec[0]->getContentSize();
	m_pTop = CCSprite::create("top.png");     
	//CCMenuItemImage *m_pTop = CCMenuItemImage::create("top.png", "top.png", this, menu_selector(HelloWorld::openUmengShare) );
	m_pTop->setPosition(ccp(mScreenSize.width/4,groundSize.height + 10 + m_pTop->getContentSize().height/2));
	CCRect rcBounding = m_pTop->boundingBox();
	CCSize startSize = m_pTop->getContentSize();
	float fTopWidth=rcBounding.size.width/2.0f;
	float fTopHeight=rcBounding.size.height/2.0f;
	//m_pStart->setPosition(ccp(this->mScreenSize.width/2.0f/RATIO-(fStartWidth/2.0f), 
	//	groundSize.height/2.0f/RATIO+(fStartHeight/2.0f)));    // 设置在屏幕中间  
	topX=mScreenSize.width/2.0f+fTopWidth+10;
	topY=m_pGroundVec[0]->boundingBox().getMaxY()+startSize.height/2;
	//xpos-=rcBounding.size.width/2.;
	m_pTop->setPosition(ccp(topX,  topY));    // 设置在屏幕中间  
	this->addChild(m_pTop, SPRITE_TAG_CHAR);    // CHILD_ORDER_BACKGROUND精灵的层级，这里是 = 1  
	CCMenuItemImage *pMenuItemTop = CCMenuItemImage::create("top.png", "top.png", this, menu_selector(HelloWorld::FeedbackCallback) );
	pMenuItemTop->setPosition(ccp( m_pTop->getContentSize().width/2,  m_pTop->getContentSize().height/2));
	CCMenu* pMenu1 =CCMenu::create(pMenuItemTop, NULL);

	pMenu1->setPosition( CCPointZero );
	m_pTop->addChild(pMenu1);

}

void HelloWorld::addGameOver() {
	m_pGameOver = CCSprite::create("gameover.png");     
	CCRect rcBounding = m_pGameOver->boundingBox();
	float fGameOverHeight=rcBounding.size.height/2.0f;
	gameoverX=mScreenSize.width/2;
	gameoverY=mScreenSize.height*3/4+25.f/mfac;

	m_pGameOver->setPosition(ccp(gameoverX,  gameoverY));    // 设置在屏幕中间  
	this->addChild(m_pGameOver, SPRITE_TAG_OVER);  

}


void HelloWorld::addScore() {
	m_pScore= CCSprite::create("score.png");     
	CCRect rcBounding = m_pScore->boundingBox();
	float fScoreHeight=rcBounding.size.height/2.0f;
	scoreX=mScreenSize.width/2;
	scoreY=startY+fScoreHeight+50/mfac;

	m_pScore->setPosition(ccp(scoreX,  scoreY));    // 设置在屏幕中间  
	this->addChild(m_pScore, SPRITE_TAG_OVER);  

}





void HelloWorld::addHand() {
	m_pHand= CCSprite::create("hand.png");     
	CCRect rcBounding = m_pHand->boundingBox();
	float fHandHeight=rcBounding.size.height/2.0f;
	float xpos=mScreenSize.width/2;
	float ypos=mScreenSize.height/3+fHandHeight;

	m_pHand->setPosition(ccp(xpos,  ypos));    // 设置在屏幕中间  
	this->addChild(m_pHand, SPRITE_TAG_OVER);  

}
void HelloWorld::addReady() {
	m_pReady = CCSprite::create("ready.png"); 
	//m_pReady = CCSprite::create("sprites.png",CCRectMake(330,642,186,56));     
	CCRect rcBounding = m_pReady->boundingBox();
	float fHeight=rcBounding.size.height/2.0f;
	float xpos=mScreenSize.width/2;
	float ypos=mScreenSize.height*3/4+20.f/mfac;

	m_pReady->setPosition(ccp(xpos,  ypos));    // 设置在屏幕中间  
	this->addChild(m_pReady, SPRITE_TAG_OVER);  

}

void HelloWorld::addGBird() {
	m_pGBird = CCSprite::create("gbird.png");     
	CCRect rcBounding = m_pGBird->boundingBox();
	float fHeight=rcBounding.size.height/2.0f;
	float xpos=mScreenSize.width/2;
	float ypos=m_pUp->boundingBox().getMaxY()+fHeight+20.f/mfac;

	m_pGBird->setPosition(ccp(xpos,  ypos));    // 设置在屏幕中间  
	this->addChild(m_pGBird, SPRITE_TAG_OVER);  

}
void HelloWorld::addUp() {
	m_pUp= CCSprite::create("up.png");     
	CCRect rcBounding = m_pUp->boundingBox();
	float fHeight=rcBounding.size.height/2.0f;
	float xpos=mScreenSize.width/2;
	float ypos=m_pHand->boundingBox().getMaxY()+fHeight+30.f/mfac;

	m_pUp->setPosition(ccp(xpos,  ypos));    // 设置在屏幕中间  
	this->addChild(m_pUp, SPRITE_TAG_OVER);  

}

void HelloWorld::addLeftTap() {
	m_pLeftTap= CCSprite::create("lefttap.png");     
	CCRect rcBounding = m_pLeftTap->boundingBox();
	float fwidth=rcBounding.size.width/2.0f;
	float fHeight=rcBounding.size.height/2.0f;
	float xpos=mScreenSize.width/2.0f-fwidth-20.f/mfac;
	float ypos=m_pHand->boundingBox().getMaxY()+fHeight;

	m_pLeftTap->setPosition(ccp(xpos,  ypos));    // 设置在屏幕中间   
	this->addChild(m_pLeftTap, SPRITE_TAG_OVER);  

}

void HelloWorld::addRightTap() {
	m_pRightTap = CCSprite::create("righttap.png");     
	CCRect rcBounding = m_pRightTap->boundingBox();
	float fwidth=rcBounding.size.width/2.0f;
	float fHeight=rcBounding.size.height/2.0f;
	float xpos=mScreenSize.width/2.0f+fwidth+20.f/mfac;
	float ypos=m_pHand->boundingBox().getMaxY()+fHeight;

	m_pRightTap->setPosition(ccp(xpos,  ypos));    // 设置在屏幕中间  
	this->addChild(m_pRightTap, SPRITE_TAG_OVER);  

}
//喵汪大战 http://www.netfoucs.com/article/iamlazybone/59841.html
//不规则碰撞
//http://codingnow.cn/cocos2d-x/1424.html
void HelloWorld::addBird(){
	string strplist=mbirdstr[mbirdclolor]+".png";
	mBird = B2Sprite::create(strplist.c_str());

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	// 单位为 米
	//bodyDef.position = b2Vec2(mScreenSize.width*gBirdX/RATIO, mScreenSize.height*gBirdY/RATIO); // 10m以内模拟效果较高效。
	bodyDef.position = b2Vec2(mScreenSize.width*gBirdX/RATIO, mScreenSize.height*gBirdY/RATIO); // 10m以内模拟效果较高效。
	b2Body *birdBody = mWorld->CreateBody(&bodyDef);

	// 碰撞
	CCSize birdSize = mBird->getContentSize();
	int num = 8;
	//顶点数组在windows使用PointHelper制作。
	VPOINT birdVec[]= {	
		{-14.442324829101562, -2.0309642791748046},
		{-8.905843353271484, -8.238534545898437},
		{1.6638025283813476, -9.07739486694336},
		{11.226815795898437, -7.3996734619140625},
		{14.750030517578125, -2.534280776977539},
		{10.723499298095703, 5.518783187866211},
		{4.348157119750977, 10.384175872802734},
		{-8.905843353271484, 8.203137969970703}
	};
	b2Vec2* pVert= new b2Vec2[num] ;
	for(int i=0;i<num;i++)
	{
		pVert[i]=b2Vec2(birdVec[i].x / mfax / RATIO, birdVec[i].y / mfac / RATIO);
	}
	b2PolygonShape birdShape;
	//birdShape.SetAsBox(birdSize.width/2.0f/RATIO, birdSize.height/2.0f/RATIO); // 半宽，半高
	birdShape.Set(pVert, num);
	b2FixtureDef birdFixtureDef;
	birdFixtureDef.restitution = 0;
	birdFixtureDef.shape = &birdShape;
	birdFixtureDef.filter.maskBits = 0x0006;
	birdBody->CreateFixture(&birdFixtureDef);

	mBird->setPTMRatio(RATIO);
	mBird->setB2Body(birdBody);
	//mBird->setAnchorPoint(ccp(pVert[3].x/mfax/ RATIO,pVert[3].y/mfac / RATIO));
	addChild(mBird,4);
	delete []pVert;

}
//1 2 4 6
//0 1 2 3
int HelloWorld::addGround(int index) {
	B2Sprite* pGround = B2Sprite::create("ground.png");
	CCSize groundSize = pGround->getContentSize();
	mgroundSize = pGround->getContentSize();
	//m_pGround[index]->setTag(index);
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;

	bodyDef.position = b2Vec2((groundSize.width*(index*2)/2.0f)/RATIO,
		groundSize.height/2.0f/RATIO);

	b2Body *groundBody = mWorld->CreateBody(&bodyDef);

	b2PolygonShape groundShape;
	groundShape.SetAsBox(groundSize.width/2.0f/RATIO, groundSize.height/2.0f/RATIO);
	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundShape;
	groundFixtureDef.filter.categoryBits = 0x0002;
	groundBody->CreateFixture(&groundFixtureDef);

	pGround->setB2Body(groundBody);
	pGround->setPTMRatio(RATIO);
	addChild(pGround,3,SPRITE_TAG_GROUND);
	m_pGroundVec.push_back(pGround);
	int iwin=int(mScreenSize.width/(groundSize.width)+2);
	CCLOG("addGround win x:%f,groundSize.width:%f,mfac:%f,iwin:%d",mScreenSize.width,
		groundSize.width,mfac,iwin);
	return iwin;

}



void HelloWorld::addBackGround(int index) {
	string bkstr[]={"daybackground.png","nightbackground.png"};
	// 设置背景,交替变换,全部相加要大于分辨率  
	m_pBackGround[index] = CCSprite::create(bkstr[rand()%100%2].c_str());     
	CCSize size = CCDirector::sharedDirector()->getWinSize();  
	CCRect rcBounding = m_pBackGround[index]->boundingBox();  
	m_pBackGround[index]->setPosition(ccp(rcBounding.size.width*(index*2+1) / 2, size.height / 2));    // 设置在屏幕中间  
	this->addChild(m_pBackGround[index], 1);    // CHILD_ORDER_BACKGROUND精灵的层级，这里是 = 1  


}


//增加柱子容器
void HelloWorld::addBarContainer() {
	mBarContainer = CCSprite::create();
	addChild(mBarContainer,2);
}

//容器中放柱子
void HelloWorld::addBar(float dt){
	float offset = -rand()%6/mfac;
	float speed = 0;
	// 下面的柱子
	B2Sprite *downBar = B2Sprite::create("down_bar.png");
	CCSize downBarSize = downBar->getContentSize();


	b2BodyDef downBarBodyDef;
	downBarBodyDef.type = b2_kinematicBody;
	//x轴屏幕右方外两像素的位置,y轴是柱子一半
	downBarBodyDef.position = b2Vec2(mScreenSize.width/RATIO+2,
		downBarSize.height/RATIO/2+offset);
	downBarBodyDef.linearVelocity = b2Vec2(speed, 0);
	b2Body *downBarBody = mWorld->CreateBody(&downBarBodyDef);
	b2PolygonShape downBarShape;
	downBarShape.SetAsBox(downBarSize.width/2/RATIO,
		downBarSize.height/2/RATIO);
	b2FixtureDef downBarFixtureDef;
	downBarFixtureDef.restitution = 0;
	downBarFixtureDef.shape = &downBarShape;
	downBarFixtureDef.filter.categoryBits = 0x0004;
	downBarBody->CreateFixture(&downBarFixtureDef);

	downBar->setB2Body(downBarBody);
	downBar->setPTMRatio(RATIO);
	mBarContainer-> addChild(downBar,1,SPRITE_TAG_BAR);


	m_pDownBarVec.push_back(downBar);

	//上面的柱子
	B2Sprite *upBar = B2Sprite::create("up_bar.png");
	CCSize upBarSize = upBar->getContentSize();

	b2BodyDef upBarBodyDef;
	upBarBodyDef.type = b2_kinematicBody;
	upBarBodyDef.position = b2Vec2(mScreenSize.width/RATIO+2,
		downBarSize.height/RATIO+offset+gBardis/RATIO+upBarSize.height/2/RATIO);
	upBarBodyDef.linearVelocity = b2Vec2(speed, 0);
	b2Body *upBarDody = mWorld->CreateBody(&upBarBodyDef);
	b2PolygonShape upBarShape;
	upBarShape.SetAsBox(upBarSize.width/2/RATIO, upBarSize.height/2/RATIO);
	b2FixtureDef upBarFixtureDef;
	upBarFixtureDef.restitution = 0;
	upBarFixtureDef.shape = &upBarShape;
	upBarFixtureDef.filter.categoryBits = 0x0004;
	upBarDody->CreateFixture(&upBarFixtureDef);
	upBar->setB2Body(upBarDody);
	upBar->setPTMRatio(RATIO);
	mBarContainer->addChild(upBar,1,SPRITE_TAG_BAR);


}

void HelloWorld::update(float dt){
	if(m_istatus!=GAMEOVER){
		int lastgroud=-1;
		for (int i=0; i<m_pGroundVec.size(); i++)
		{
			m_pGroundVec[i]->setPositionX(m_pGroundVec[i]->getPositionX() - MOVESPEED/mfax);
			CCRect rcGroundBounding = m_pGroundVec[i]->boundingBox(); 
			if (rcGroundBounding.getMaxX()<=0)
			{
				lastgroud=i;			
			}
		}
		//float groundsize = m_pGround[i]->getContentSize().width*1.0;
		//m_pGround[i]->setPositionX(groundsize/2. + (GGROUNDNUM-1)*groundsize);
		if(lastgroud>-1){
			float moveposx=m_pGroundVec[m_ilastground]->boundingBox().getMaxX();
			moveposx+=m_pGroundVec[lastgroud]->getContentSize().width/2.;
			m_pGroundVec[lastgroud]->setPositionX(moveposx);
			m_ilastground=lastgroud;
			//CCLOG("win x:%f,Ground x:%f,mfac:%f,index=%d ",this->mScreenSize.width,
			//	moveposx,mfac,m_ilastground);
		}

	}
	if(m_istatus==GETREADY || m_istatus==GOSTART ){
		return;
	}
	//最后一个柱子离屏幕右边大于固定距离时放出来
	if(m_pDownBarVec.size()>0 && (mScreenSize.width-m_pDownBarVec.back()->getPositionX())
	>(mScreenSize.width*0.4))
	{
		addBar(0);
	}
	//重力响应
	mWorld->Step(dt, 8, 3); // 8和3为官方推荐数据
	myangle+=1.0f;
	if(myangle>=25)
	{
		iFlyKeep++;
		myangle=25;
	}
	if (myangle<25)
	{
		iFlyKeep=0;
		mBird->setRotation(myangle);
	}
	CCLOG("bird iFlyKeep:%d,angle:%0.2f,myangle:%d",iFlyKeep,mBird->getRotation(),myangle);
	if(iFlyKeep>30)
	{
		//if (mBird->getRotation() < 10.f/mfac && mBird->getRotation() > -85.f)
		//{
		//	mBird->setRotation(mBird->getRotation()-10.f);
		//}

		if(mBird->getRotation()-10.f<-90.f){
			mBird->setRotation(-90.f);
		}
		else{
			mBird->setRotation(mBird->getRotation()-6.f);
		}
		//if(mBird->getRotation() > -91)
		//mBird->setRotation(mBird->getRotation()-2.5f);
	}


	//CCLOG("bird angle:%0.2f,myangle:%0.2f ",mBird->getRotation(),myangle);
	if(myflag==1)
	{
		mBird->setRotation(-90);
		return;
	}
	//禁止小鸟越过屏幕高度
	if(mBird->getPositionY()>mScreenSize.height)
	{
		mBird->setPositionY(mScreenSize.height);
	}
	/*for (int i=0; i<gbackgroundNum; i++)
	{
	m_pBackGround[i]->setPositionX(m_pBackGround[i]->getPositionX() - 1); 
	CCRect rcBounding = m_pBackGround[i]->boundingBox(); 
	if (rcBounding.getMaxX() <= 0)    // 如果完全消失在屏幕上，就移动精灵1到精灵3的后面  
	{  
	int backgroundsize = m_pBackGround[i]->getContentSize().width;
	m_pBackGround[i]->setPositionX(backgroundsize/2 + (ggroundNum-1)*backgroundsize);  
	}  

	}
	*/

	//CCLOG("rcBounding1 MinX:%d MinY:%d MaxX:%d MaxY:%d", rcBounding1.getMinX()
	//, rcBounding1.getMinY(), rcBounding1.getMaxX(), rcBounding1.getMaxY());

	//给小鸟计分
	if(m_pDownBarVec.size()>0 && m_pDownBarVec.front()->boundingBox().getMaxX() <= this->mBird->boundingBox().getMaxX())
	{
		ShowNumberNode * snn = (ShowNumberNode *)this->getChildByTag(0);
		snn->f_ShowNumber(++testnum);
//		EFFECT_PLAY(true,MUSIC_POINT);
		m_pDownBarVec.pop_front();
	}

	CCSprite *s;
	std::vector<b2Body *>toDestroy;
	map<CCSprite *,int>::iterator itbar;
	for(b2Body *b = mWorld->GetBodyList(); b != NULL; b = b->GetNext()){
		s = (CCSprite *)b->GetUserData();
		if(s != NULL && s->getTag()==SPRITE_TAG_BAR){

			//CCLOG("bird x:%f guan x:%f ", mBird->getPositionX(),s->getPositionX());

			s->setPositionX(s->getPositionX() - MOVESPEED/mfax);
			if(s->getPositionX() < -20/mfac){
				s->setVisible(false);
				toDestroy.push_back(b);
				//s->removeFromParent();					
				//mWorld->DestroyBody(b);
				//b = mWorld->GetBodyList();
			}
		}
	}


	std::vector<b2Body*>::iterator pos2;
	for (pos2 = toDestroy.begin();pos2!=toDestroy.end();++pos2)
	{
		b2Body* body = *pos2;
		if(body->GetUserData()!=NULL)
		{
			CCSprite* sprite = (CCSprite*)body->GetUserData();
			this->removeChild(sprite,true);
		}

		mWorld->DestroyBody(body);
		body = NULL;

	}

}

//获取系统当前时间

long millisecondNow()
{

	struct cc_timeval now;

	CCTime::gettimeofdayCocos2d(&now, NULL);

	return (now.tv_sec * 1000 + now.tv_usec / 1000);

}

void HelloWorld::BeginContact(b2Contact *contact){
	B2Sprite *p = (B2Sprite *)contact->GetFixtureA()->GetBody()->GetUserData();
	if(p->getTag()==SPRITE_TAG_GROUND)
	{
		CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.f);
		//mBird->setRotation(-90.f);

		//CCMessageBox("Game Over!", "Game Over!");
		//mBird->setVisible(false);
		this->runAction(CCShake::create(1,5));

		//std::string fullPath;
		//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)  
		//fullPath = CCFileUtils::sharedFileUtils()->getWritablePath(); 
		//fullPath += "score.abc";

		//unsigned long size;
		//unsigned char *str = CCFileUtils::sharedFileUtils()->getFileData(fullPath,"wt+",&size);
		m_bnew=false;
		string filedata;
		filedata = TDInvFileUtils::getFileByName("score.abc");
		int filescore = 0;
		if (filedata != "")
		{
			filescore = atoi(filedata.c_str());
			if (filescore < testnum)
			{
				m_bnew=true;
				m_bestscore = testnum;
				char str[10];
				char   temp[256];   
				sprintf(str,"%d",testnum); 
				TDInvFileUtils::saveFile(str,"score.abc");
			}
			else
			{
				m_bestscore = filescore;
			}

		}
		else
		{
			m_bestscore = testnum;
			char str[10];
			sprintf(str,"%d",testnum); 
			TDInvFileUtils::saveFile(str,"score.abc");
		}



		//#else   

		//#endif  
		//Sleep(1000);
		if(!m_bhitbar)
		{
//			EFFECT_PLAY(true,MUSIC_HIT);
		}
		else
		{
//			EFFECT_PLAY(true,MUSIC_DIE);
		}
		stopGame();


	}
	if(p == mBird || p == mBird){
		CCScheduler* pScheduler = CCDirector::sharedDirector()->getScheduler();
		pScheduler->setTimeScale(0.f);//实现减速效果
		//scheduleOnce(schedule_selector(HelloWorld::ResumeSpeed), 1);
		long now=millisecondNow();

		while(false){
			if((millisecondNow()-now)>1000)
			{
				break;
			}
		};
		CCDirector::sharedDirector()->getScheduler()->setTimeScale(1.f);
		//CCDirector::sharedDirector()->getScheduler()->setTimeScale(0.2f);
		//mBird->getB2Body()->SetLinearVelocity(b2Vec2(0, -5));
		myflag=1;
		b2Filter myfilter;
		myfilter.maskBits=0x0002;
		mBird->getB2Body()->GetFixtureList()->SetFilterData(myfilter);
		m_istatus=GAMEOVER;
		m_bhitbar=true;
//		EFFECT_PLAY(true,MUSIC_HIT);
		//stopGame();
		// CCMessageBox("Game Over!", "Game Over!");
	}
}


void HelloWorld::ResumeSpeed(float dt)
{

}
void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
	CCLOG("win x:%f,Ground x:%f,mfac:%f,index=%d ",this->mScreenSize.width,
		m_pGroundVec[m_ilastground]->boundingBox().getMaxX(),mfac,m_ilastground);
	static int flag;       
	CCTouch *pTouch = (CCTouch*)pTouches->anyObject();  
	CCPoint click2 = pTouch->getLocation();  
	CCLog("ccTouchesBegan,%d,%f,%f",++flag,click2.x,click2.y); 
	if(this->m_istatus!=RUNNING){
		if(m_istatus==GETREADY)
		{
			this->startGame(0);
		}
		CCPoint touchPoint = pTouch->getLocationInView();
		touchPoint = CCDirector::sharedDirector()->convertToGL(touchPoint);
		CCRect rect = m_pStart->boundingBox();
		//在判断你点击的这个点touchPoint 是否在图片按钮矩形里面rect ;
		if(rect.containsPoint(touchPoint))
		{


			if(m_istatus==GAMEOVER && m_pStart->isVisible())
			{
				//this->goReady();
				//m_istatus=GETREADY;
				//schedule(schedule_selector(HelloWorld::ScoreSchedule),0.05);

				CCScene *pScene = HelloWorld::scene();
				CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1,pScene));

				return;
			}
		}
		//为true就进来，说明点中在图片按钮矩形里面
		//CCScene* scen = HelloWorld::scene();
		//CCDirector::sharedDirector()->replaceScene(scen);

	}
	if(m_istatus!=RUNNING)
	{
		return;
	}
//	EFFECT_PLAY(true,MUSIC_WING);
	//MUSIC_PLAY(MUSIC_JUMP);
	//播放子弹射出去的声音
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(MUSIC_JUMP);
	mBird->getB2Body()->SetLinearVelocity(b2Vec2(0.0f, gUpVelocity/mfac));
	mBird->setRotation(25);
	myangle=25;
	iFlyKeep=0;

}


	//  按钮的回调函数
void HelloWorld::FeedbackCallback(CCObject * pSender)
{

	//ScreenShoot();
}
void HelloWorld::menuCloseCallback(CCObject* pSender)
{

	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

