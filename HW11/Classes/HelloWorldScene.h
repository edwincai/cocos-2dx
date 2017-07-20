#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	
	void upCallback(cocos2d::Ref* pSender);
	void downCallback(cocos2d::Ref* pSender);
	void leftCallback(cocos2d::Ref* pSender);
	void rightCallback(cocos2d::Ref* pSender);
	void xCallback(cocos2d::Ref* pSender);
	void yCallback(cocos2d::Ref* pSender);

	void setItem(MenuItemFont* item, Vec2 pos);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	void update(float dt) override;
private:
	cocos2d::Sprite* player;
	cocos2d::Vector<SpriteFrame*> attack;
	cocos2d::Vector<SpriteFrame*> dead;
	cocos2d::Vector<SpriteFrame*> run;
	cocos2d::Vector<SpriteFrame*> idle;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::Label* time;
	int dtime;
	cocos2d::ProgressTimer* pT;
	bool running;
	int index;
	int timecount;
};

#endif // __HELLOWORLD_SCENE_H__
