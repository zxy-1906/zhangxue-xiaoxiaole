#ifndef __SETMUSIC_LAYER_H__
#define __SETMUSIC_LAYER_H__

#include"cocos2d.h"
#include "cocos-ext.h"//进度条头文件

USING_NS_CC_EXT;

class SetMusic :public cocos2d::Layer
{

public:
	static cocos2d::Layer* createScene();

	virtual bool init();

	void menuMusicCallback_1(cocos2d::Ref* pSender);
	void menuMusicCallback_2(cocos2d::Ref* pSender);
	void sliderChange(Ref* psender, Control::EventType event);

	CREATE_FUNC(SetMusic);

};

#endif