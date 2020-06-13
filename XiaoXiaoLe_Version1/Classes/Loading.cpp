
#include "Loading.h"
#include "SimpleAudioEngine.h"
#include"mainMenu.h"

USING_NS_CC;

Scene* Loading::createScene()
{
	return Loading::create();
}

bool Loading::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto bg = Sprite::create("loadingbg.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);

	loadingAction();

	_loadingAudioThread = new std::thread(&Loading::loadingAudio, this);

	this->schedule(schedule_selector(Loading::enterGame), 1, 0, 3.0f);

	return true;
}

void Loading::enterGame(float dt)
{
	auto mainMenuScene = MainMenu::createScene();
	Director::getInstance()->pushScene(mainMenuScene);
}

void Loading::loadingAction()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	SpriteFrameCache* loadFramCache = SpriteFrameCache::getInstance();//
	loadFramCache->addSpriteFramesWithFile("loadingCube.plist");

	SpriteFrame* loadSpriteFram = loadFramCache->getSpriteFrameByName("loading1.png");
	auto loadingSprite = Sprite::createWithSpriteFrame(loadSpriteFram);
	loadingSprite->setPosition(Vec2(visibleSize.width / 2, 5 * visibleSize.height / 13));
	this->addChild(loadingSprite, 1);

	Animation* loadAnimation = Animation::create();
	for (int i = 1; i < 8; i++)
	{
		__String *nameImage = __String::createWithFormat("loading%d.png", i);
		loadSpriteFram = loadFramCache->getSpriteFrameByName(nameImage->getCString());
		loadAnimation->addSpriteFrame(loadSpriteFram);
	}

	loadAnimation->setDelayPerUnit(0.20f);
	loadAnimation->setRestoreOriginalFrame(true);

	Animate *loadAction = Animate::create(loadAnimation);
	loadingSprite->runAction(RepeatForever::create(loadAction));
}

void Loading::loadingAudio()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("pingfantianshi.mp3");
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("see you again .mp3");
}

void Loading::onExit()
{
	Scene::onExit();

	_loadingAudioThread->join();
	CC_SAFE_DELETE(_loadingAudioThread);
}