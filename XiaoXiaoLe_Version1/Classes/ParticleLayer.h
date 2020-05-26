#ifndef __PARTICLE_LAYER_H__
#define __PARTICLE_LAYER_H__

#include"cocos2d.h"

class ParticleLayer :public cocos2d::Layer
{
public:
	static cocos2d::Layer* createScene();

	virtual bool init();

	CREATE_FUNC(ParticleLayer);

};

#endif
