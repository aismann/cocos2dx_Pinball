/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
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
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "EffectSprite.h"//纹理
#include "LightEffect.h"//光源
#include "PhysicsShapeCache.h"//自定义物理体积


USING_NS_CC;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::createWithPhysics();//创建物理场景
	scene->getPhysicsWorld()->setGravity(Vec2(0, -50));//设置重力=0
	// optional: set debug draw
	scene->getPhysicsWorld()->setDebugDrawMask(0xffff);//debug
	
	auto layer = HelloWorld::create();
	scene->addChild(layer);
	
	//防止刚体穿透
	scene->getPhysicsWorld()->setAutoStep(false);
	return scene;
}

// Print useful error message instead of segfaulting when files are not there.报错信息
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
	
	//导入物理文件
	PhysicsShapeCache *PhyFile = PhysicsShapeCache::getInstance();
	PhyFile->addShapesWithFile("re/ball&flipper&map.plist");
	
	//地图
	auto pinballmap = Sprite::create("re/map.png");
	PhyFile->setBodyOnSprite("map.png", pinballmap);
	pinballmap->setPosition(50,0);
	Vec2 Size = Director::getInstance()->getVisibleSize();//图尺寸
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//原点位置
	//pinballmap->setPosition(origin.x + Size.x/2 , origin.y + Size.y/2);//设置地图位置
//	pinballmap->setPosition(origin.x, origin.y);
	//pinballmap->setScale(1.43);
	this->addChild(pinballmap);
	
	//挡板
	auto L = EffectSprite::create("re/leftflipper.png");//挡板原图
	L->setPosition(origin.x + 245 , 50);
	L->setScale(2);
	auto R = EffectSprite::create("re/rightflipper.png");
	R->setPosition(Size.x - 295, 50);
	R->setScale(2);
//	//挡板光
//	auto pointLight = LightEffect::create();//光源
//	pointLight->retain();//保持光源
//	pointLight->setLightPos(Vec3(origin.x + Size.x/3 - 40, 70, 30));//光源位置
//	pointLight->setLightCutoffRadius(1000);//无影响半径
//	pointLight->setBrightness(2.0);//亮度
//	L->setEffect(pointLight, "L_n.png");
//	R->setEffect(pointLight, "R_n.png");
	//挡板物理
//	PhysicsShapeCache *LPh = PhysicsShapeCache::getInstance();
//	LPh->addShapesWithFile("L&R.plist");
	PhyFile->setBodyOnSprite("leftflipper.png", L);//绑定物理
//	PhysicsShapeCache *RPh = PhysicsShapeCache::getInstance();
	PhyFile->setBodyOnSprite("rightflipper.png", R);//绑定物理
	L->getPhysicsBody()->setMass(10000);
	L->setRotation(30);
	R->setRotation(-30);
	this->addChild(L);
	this->addChild(R);

	Vec2 ballLocation(origin.x + 900 , 100);
	//球
	auto pinballSprite = EffectSprite::create("re/ball.png");
	pinballSprite->setPosition(ballLocation);
//	pinballSprite->setScale(0.2);
	PhyFile->setBodyOnSprite("ball.png", pinballSprite);
//	auto pinballPH = PhysicsBody::createCircle(40);
//	pinballPH->getShape(0)->setRestitution(3);//设置弹性
	pinballSprite->getPhysicsBody()->setMass(0.5);//质量
//	pinballPH->setLinearDamping(0);//线性摩擦力
//	pinballPH->setRotationEnable(false);//球不可旋转
//	pinballSprite->setPhysicsBody(pinballPH);
	this->addChild(pinballSprite, 1);
	
	
	
//	//点光源
//	pointLight = LightEffect::create();//光源
//	pointLight->retain();//保持光源
//	Vec3 lightPos(100, 100, 100);
//	pointLight->setLightPos(lightPos);
//	pointLight->setLightCutoffRadius(1000);//无影响半径
//	pointLight->setBrightness(2.0);//亮度
////	pointLight->setLightColor(cocos2d::Color3B::WHITE);//颜色
//	pinballSprite->setEffect(pointLight, "Pinball_n.png");//纹理+光源
//	//设置锚点
//	L->setAnchorPoint(Vec2(0,0.5));
//	R->setAnchorPoint(Vec2(1,0.5));
	//监听键盘
	this->star = 0;
	auto dirListener = Director::getInstance()->getEventDispatcher();
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = [this,L,R,ballLocation,pinballSprite,origin,Size](EventKeyboard::KeyCode keyCode, Event* event){
		this->keyDown[(int)keyCode] = true;
		if (keyDown[(int)EventKeyboard::KeyCode::KEY_Z])
		{
//			PhysicsMaterial toPowerful(1, 3, 0);
			//L->getPhysicsBody()->getFirstShape()->setRestitution(100);
//			L->getPhysicsBody()->applyTorque(1000);
//			pinballSprite->getPhysicsBody()->getFirstShape()->setRestitution(100);
			//旋转动画(导致刚体穿透)
			auto actionToL=cocos2d::RotateTo::create(0.1,-30);//时间 度数
			auto actionL = cocos2d::Sequence::create(actionToL, NULL);
			L->runAction(actionL->clone());
			/*
			L->getPhysicsBody()->setAngularVelocity(100);
			目前解决方法是让挡板有一个初始速度，让它转一定的时间后停止
			L->getPhysicsBody()->setAngularVelocity(0);
			 */
		}
		if (keyDown[(int)EventKeyboard::KeyCode::KEY_M])
		{
			//旋转动画（导致刚体穿透）
			auto actionToR=cocos2d::RotateTo::create(0.1,30);//时间 度数
			auto actionR = cocos2d::Sequence::create(actionToR, NULL);
			R->runAction(actionR->clone());
		}
		if(!star && keyDown[(int)EventKeyboard::KeyCode::KEY_SPACE])//开始
		{
			this->star = 1;
			pinballSprite->getPhysicsBody()->applyImpulse(Vec2(0,1000));//冲量
		}
		if(star && keyDown[(int)EventKeyboard::KeyCode::KEY_R])//重新
		{
			pinballSprite->setPosition(ballLocation);
			pinballSprite->setRotation(0);
			star=0;
		}
		printf("pressdown %d \n", keyCode);
	};
	//按键释放
	keyboardListener->onKeyReleased = [this,L,R](EventKeyboard::KeyCode keyCode, Event* event){
		this->keyDown[(int)keyCode] = false;
		if (!keyDown[(int)EventKeyboard::KeyCode::KEY_Z])
		{
			//旋转动画（导致刚体穿透）
			auto actionToL=cocos2d::RotateTo::create(0.05,30);//时间 度数
			auto actionL = cocos2d::Sequence::create(actionToL, NULL);
			L->runAction(actionL->clone());
		}
		if (!keyDown[(int)EventKeyboard::KeyCode::KEY_M])
		{
			//旋转动画（导致刚体穿透）
			auto actionToR=cocos2d::RotateTo::create(0.05,-30);//时间 度数
			auto actionR = cocos2d::Sequence::create(actionToR, NULL);
			R->runAction(actionR->clone());
		}
		printf("release %d\n", keyCode);
	};
	dirListener->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	
	
	this->scheduleUpdate();//每帧更新
    return true;
}


//每帧更新
void HelloWorld::update(float delta)
{
	// use fixed time and calculate 3 times per frame makes physics simulate more precisely.
	for (int i = 0; i < 100; ++i)
	{
		getScene()->getPhysicsWorld()->step(1/1800.0f);
	}
	
}





void HelloWorld::menuCloseCallback(Ref* pSender)//关闭函数
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}


