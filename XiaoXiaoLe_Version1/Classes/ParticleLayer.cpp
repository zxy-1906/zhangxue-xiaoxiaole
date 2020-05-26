#include"ParticleLayer.h"

USING_NS_CC;

Layer *ParticleLayer::createScene()
{
	return ParticleLayer::create();
}

bool ParticleLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}