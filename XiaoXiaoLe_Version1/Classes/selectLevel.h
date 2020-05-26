
#ifndef __LEVELSELECT_SCENE_H__
#define __LEVELSELECT_SCENE_H__

#include "cocos2d.h"

class levelSelected : public cocos2d::Layer
{
	
public:
	static cocos2d::Layer* createScene();

	virtual bool init();

	void menuLevelOneCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(levelSelected);
};

#endif