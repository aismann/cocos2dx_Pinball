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
	scene->getPhysicsWorld()->setGravity(Vec2(0, -100));//设置重力=0
	// optional: set debug draw
	scene->getPhysicsWorld()->setDebugDrawMask(0xffff);//debug
	
	auto layer = HelloWorld::create();
	scene->addChild(layer);
	
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
	
	//地图
	PhysicsShapeCache * shapeCache = PhysicsShapeCache::getInstance();
	shapeCache->addShapesWithFile("pinballmap.plist");
	
	auto pinballmap = Sprite::create("pinballmap.png");
	shapeCache->setBodyOnSprite("Image004.png", pinballmap);
	pinballmap->setPosition(0,0);
	Vec2 Size = Director::getInstance()->getVisibleSize();//图尺寸
	Vec2 origin = Director::getInstance()->getVisibleOrigin();//原点位置
	pinballmap->setPosition(origin.x + Size.x/2 , origin.y + Size.y/2);//设置地图位置
	pinballmap->setScale(1.43);
	
	this->addChild(pinballmap);
	
	
	
	//球
	auto pinBallSprite = EffectSprite::create("Pinball.png");//纹理原图
	pinBallSprite->setPosition(origin.x + Size.x - 20 , 100);
	pinBallSprite->setScale(0.2);
	auto pinballPH = PhysicsBody::createCircle(40);
	pinballPH->setMass(1);//质量
	pinballPH->setLinearDamping(0);//线性摩擦力
	pinBallSprite->setPhysicsBody(pinballPH);
	this->addChild(pinBallSprite, 1);
	//点光源
	auto pointLight = LightEffect::create();//光源
	pointLight->retain();//保持光源
	Vec3 lightPos(100, 100, 100);
	pointLight->setLightPos(lightPos);
	pointLight->setLightCutoffRadius(1000);//无影响半径
	pointLight->setBrightness(2.0);//亮度
//	pointLight->setLightColor(cocos2d::Color3B::WHITE);//颜色
	pinBallSprite->setEffect(pointLight, "Pinball_n.png");//纹理+光源
	

	
	
	//监听键盘
	auto dirListener = Director::getInstance()->getEventDispatcher();
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event){
		this->keyDown[(int)keyCode] = true;
		printf("pressdown %d \n", keyCode);
	};
	keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event){
		this->keyDown[(int)keyCode] = false;
		printf("release %d\n", keyCode);
	};
	dirListener->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	
	
	this->scheduleUpdate();//每帧更新
    return true;
}


//每帧更新
void HelloWorld::update(float delta)
{
	
	
}





void HelloWorld::menuCloseCallback(Ref* pSender)//关闭函数
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
