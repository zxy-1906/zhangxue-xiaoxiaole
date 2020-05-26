#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include"mainMenu.h"

class Loading : public cocos2d::Scene
{
	std::thread *_loadingAudioThread;
	
public:


	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onExit();

	void loadingAction();
	void loadingAudio();

	void enterGame(float dt);

	CREATE_FUNC(Loading);
};

#endif
