#include "HelloWorldScene.h"
#pragma execution_character_set("utf-8")
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

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
    if ( !Layer::init() )
    {
        return false;
    }
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	running = false;
	index = 180;
	timecount = 0;
	//显示时间
	scheduleUpdate();
	time = Label::createWithTTF(Value(index).asString(), "fonts/arial.ttf", 36);
	time->setPosition(visibleSize.width*0.5, visibleSize.height*0.8);
	this->addChild(time, 3);
   
	//设置按钮
	auto up = MenuItemFont::create("W", CC_CALLBACK_1(HelloWorld::upCallback, this));
	auto down = MenuItemFont::create("S", CC_CALLBACK_1(HelloWorld::downCallback, this));
	auto left = MenuItemFont::create("A", CC_CALLBACK_1(HelloWorld::leftCallback, this));
	auto right = MenuItemFont::create("D", CC_CALLBACK_1(HelloWorld::rightCallback, this));
	auto x = MenuItemFont::create("X", CC_CALLBACK_1(HelloWorld::xCallback, this));
	auto y = MenuItemFont::create("Y", CC_CALLBACK_1(HelloWorld::yCallback, this));
	setItem(up, Vec2(70, 100));
	setItem(down, Vec2(70, 30));
	setItem(left, Vec2(40, 65));
	setItem(right, Vec2(100, 65));
	setItem(x, Vec2(visibleSize.width - 30, 90));
	setItem(y, Vec2(visibleSize.width - 40, 30));
	//创建menu
	auto menu = Menu::create(up, down, left, right, x, y, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 3);

	//创建一张贴图
	auto texture = Director::getInstance()->getTextureCache()->addImage("$lucia_2.png");
	//从贴图中以像素单位切割，创建关键帧
	auto frame0 = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 113, 113)));
	//使用第一帧创建精灵
	player = Sprite::createWithSpriteFrame(frame0);
	player->setPosition(Vec2(origin.x + visibleSize.width / 2,
							origin.y + visibleSize.height/2));
	addChild(player, 3);

	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(100);
	pT->setPosition(Vec2(origin.x+14*pT->getContentSize().width,origin.y + visibleSize.height - 2*pT->getContentSize().height));
	addChild(pT,1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0,0);
	//auto animation = Animation::createWithSpriteFrames(idle, 0.1f);
	//auto animate = Animate::create(animation);
	// 静态动画
	idle.reserve(1);
	idle.pushBack(frame0);

	// 攻击动画
	attack.reserve(17);
	for (int i = 0; i < 17; i++) {
		auto frame = SpriteFrame::createWithTexture(texture, CC_RECT_PIXELS_TO_POINTS(Rect(113*i,0,113,113)));
		attack.pushBack(frame);
	}
	//恢复原来的状态
	attack.pushBack(attack.at(0));

	// 可以仿照攻击动画
	// 死亡动画(帧数：22帧，高：90，宽：79）
	auto texture2 = Director::getInstance()->getTextureCache()->addImage("$lucia_dead.png");
    // Todo
	dead.reserve(22);
	for (int i = 0; i < 22; i++)
	{
		auto frame = SpriteFrame::createWithTexture(texture2, CC_RECT_PIXELS_TO_POINTS(Rect(79 * i, 0, 79, 90)));
		dead.pushBack(frame);
	}
	//恢复原来的状态
	dead.pushBack(dead.at(0));
	
	//player->runAction(RepeatForever::create(animate));
	// 运动动画(帧数：8帧，高：101，宽：68）
	auto texture3 = Director::getInstance()->getTextureCache()->addImage("$lucia_forward.png");
    // Todo
	run.reserve(8);
	for (int i = 0; i < 8; i++)
	{
		auto frame = SpriteFrame::createWithTexture(texture3, CC_RECT_PIXELS_TO_POINTS(Rect(68 * i, 0, 68, 101)));
		run.pushBack(frame);
	}
	run.pushBack(run.at(0));

	return true;
}

void HelloWorld::upCallback(cocos2d::Ref* pSender) {

	if (running)
	{
		return;
	}
	auto currentPos = player->getPosition();
	auto moveTo = MoveTo::create(0.5, Vec2(currentPos.x, currentPos.y + 40));

	auto animation = Animation::createWithSpriteFrames(run, 0.1f);
	auto animate = Animate::create(animation);

	auto spawn = Spawn::create(moveTo, animate, NULL);
	//使用lambda作为Action函数
	auto actionMoveDone = CallFuncN::create([&](Ref* sender) {
		running = false;
	});

	auto actionMoveBegin = CallFuncN::create([&](Ref* sender) {
		running = true;
	});

	auto seq = Sequence::create(actionMoveBegin, spawn, actionMoveDone, NULL);

	player->runAction(seq);
}

void HelloWorld::downCallback(cocos2d::Ref* pSender) {
	if (running)
	{
		return;
	}
	auto currentPos = player->getPosition();
	auto moveTo = MoveTo::create(0.5, Vec2(currentPos.x, currentPos.y - 40));

	auto animation = Animation::createWithSpriteFrames(run, 0.1f);
	auto animate = Animate::create(animation);

	auto spawn = Spawn::create(moveTo, animate, NULL);

	auto actionMoveDone = CallFuncN::create([&](Ref* sender) {
		running = false;
	});

	auto actionMoveBegin = CallFuncN::create([&](Ref* sender) {
		running = true;
	});

	auto seq = Sequence::create(actionMoveBegin, spawn, actionMoveDone, NULL);

	player->runAction(seq);
}
void HelloWorld::leftCallback(cocos2d::Ref* pSender){
	if (running)
	{
		return;
	}
	auto currentPos = player->getPosition();
	auto moveTo = MoveTo::create(0.5, Vec2(currentPos.x - 40, currentPos.y));

	auto animation = Animation::createWithSpriteFrames(run, 0.1f);
	auto animate = Animate::create(animation);

	auto spawn = Spawn::create(moveTo, animate, NULL);

	auto actionMoveDone = CallFuncN::create([&](Ref* sender) {
		running = false;
	});

	auto actionMoveBegin = CallFuncN::create([&](Ref* sender) {
		running = true;
	});

	auto seq = Sequence::create(actionMoveBegin, spawn, actionMoveDone, NULL);

	player->runAction(seq);
}
void HelloWorld::rightCallback(cocos2d::Ref* pSender) {
	if (running)
	{
		return;
	}
	auto currentPos = player->getPosition();
	auto moveTo = MoveTo::create(0.5, Vec2(currentPos.x + 40, currentPos.y));

	auto animation = Animation::createWithSpriteFrames(run, 0.1f);
	auto animate = Animate::create(animation);

	auto spawn = Spawn::create(moveTo, animate, NULL);

	auto actionMoveDone = CallFuncN::create([&](Ref* sender) {
		running = false;
	});

	auto actionMoveBegin = CallFuncN::create([&](Ref* sender) {
		running = true;
	});

	auto seq = Sequence::create(actionMoveBegin, spawn, actionMoveDone, NULL);

	player->runAction(seq);
}
void HelloWorld::xCallback(cocos2d::Ref* pSender) {
	auto animation = Animation::createWithSpriteFrames(dead, 0.1f);
	auto animate = Animate::create(animation);

	auto actionMoveDone = CallFuncN::create([&](Ref* sender) {
		running = false;
	});

	auto actionMoveBegin = CallFuncN::create([&](Ref* sender) {
		running = true;
	});

	auto seq = Sequence::create(actionMoveBegin,animate, actionMoveDone, NULL);
	player->runAction(seq);
	//血条变化，调用progressTo
	auto change = ProgressTo::create(1, pT->getPercentage() - 20);
	pT->runAction(change);
}
void HelloWorld::yCallback(cocos2d::Ref* pSender) {
	auto animation = Animation::createWithSpriteFrames(attack, 0.1f);
	auto animate = Animate::create(animation);

	auto actionMoveDone = CallFuncN::create([&](Ref* sender) {
		running = false;
	});

	auto actionMoveBegin = CallFuncN::create([&](Ref* sender) {
		running = true;
	});

	auto seq = Sequence::create(actionMoveBegin, animate, actionMoveDone, NULL);
	player->runAction(seq);
	//血条变化，调用progressTo
	auto change = ProgressTo::create(1, pT->getPercentage() + 20);
	pT->runAction(change);
}

void HelloWorld::setItem(MenuItemFont * item, Vec2 pos)
{
	item->setFontNameObj("arial");
	item->setFontSizeObj(36);
	item->setPosition(pos);
}

void HelloWorld::update(float dt)
{
	timecount++;
	if (timecount == 60) {
		if (index < 0)
			index = 0;
		else {
			index--;
			time->setString(Value(index).asString());
			timecount = 0;
		}
	}
}
