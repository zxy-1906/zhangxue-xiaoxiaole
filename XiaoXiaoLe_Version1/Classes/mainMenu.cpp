#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include"mainMenu.h"
#include"selectLevel.h"
#include"levelOne.h"
#include"ParticleLayer.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
	return MainMenu::create();
}


bool MainMenu::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto frame = Sprite::create("frame.png");
	frame->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(frame);

	rotationMusic = Sprite::create("rotationMusic.png");
	rotationMusic->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(-190, 200));// 
	this->addChild(rotationMusic);
	FiniteTimeAction *ac = (FiniteTimeAction *)rotationMusic->runAction(RotateBy::create(1, 50));
	ActionInterval *seq = Sequence::create(ac, NULL);
	rotationMusic->runAction(RepeatForever::create(seq));

	auto soundOff = MenuItemImage::create("musicStop.png", "musicStop.png");
	auto soundOn = MenuItemImage::create("musicPlay.png", "musicPlay.png");
	auto itemMusic = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(MainMenu::menuMusicCallback, this),
		soundOff, soundOn, NULL);
	itemMusic->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(-420, -230));

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MainMenu::menuCloseCallback, this));
	closeItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(-420, -630));

	Menu *mn = Menu::create(itemMusic, closeItem,NULL);
	this->addChild(mn, 1);

	auto selectlevel = levelSelected::create();
	selectlevel->setTag(22);
	this->addChild(selectlevel);

	auto theParctileLayer = ParticleLayer::createScene();
	theParctileLayer->setTag(341);
	this->addChild(theParctileLayer);

	return true;
}

void MainMenu::onEnterTransitionDidFinish()
{
	Scene::onEnterTransitionDidFinish();
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("see you again .mp3", true);
}

void MainMenu::menuMusicCallback(Ref* pSender)
{
	auto soundToggle = (MenuItemToggle*)pSender;
	rotationMusic->stopAllActions();
	if (soundToggle->getSelectedIndex() == 1)
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	else 
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		FiniteTimeAction *ac = (FiniteTimeAction *)rotationMusic->runAction(RotateBy::create(1, 50));
		ActionInterval *seq = Sequence::create(ac, NULL);
		rotationMusic->runAction(RepeatForever::create(seq));
	}
	
}

void MainMenu::menuCloseCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->end();
}

void MainMenu::enterLevelOne(int level)
{
	this->removeChildByTag(12);
	auto l = LevelOne::createScene();
	this->addChild(l);

}
