
#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos-ext.h"//进度条头文件
#include "cocos2d.h"

#define MAXSCORE_TAG 70
#define COINNUM_TAG 71
#define SETMUSIC_TAG 72
#define LEVEL_TAG 77

USING_NS_CC_EXT;

class MainMenu : public cocos2d::Scene
{
public:


	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnterTransitionDidFinish();

	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuLevelCallback(cocos2d::Ref* pSender);

	void setMaxScore(int score);
	void setCoinNum(int coin);

	void menuSetMusicCallback(cocos2d::Ref* pSender);
	void sliderChange(Ref* psender, Control::EventType event);

	CREATE_FUNC(MainMenu);
};

#endif