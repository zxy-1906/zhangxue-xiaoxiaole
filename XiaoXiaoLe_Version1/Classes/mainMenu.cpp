#include "SimpleAudioEngine.h"
#include"mainMenu.h"
#include"levelOne.h"
#include<sstream>
#include"setMusic.h"
#include"levelTwo.h"
#include"levelThree.h"
#include"levelFour.h"




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

	auto bg = Sprite::create("mainMenuBg.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg,0);

	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(MainMenu::menuCloseCallback, this));
	closeItem->setPosition(Vec2(2*visibleSize.width/5,-2*visibleSize.height/5));

	Menu *mn = Menu::create( closeItem,NULL);
	this->addChild(mn, 1);

	if (UserDefault::isXMLFileExist()){
		int temp = UserDefault::getInstance()->getIntegerForKey("temp");
	}
	else
		UserDefault::getInstance()->setIntegerForKey("temp", 0);
	if (UserDefault::isXMLFileExist()){
		int score = UserDefault::getInstance()->getIntegerForKey("maxScore");
		setMaxScore(score);
		int coin = UserDefault::getInstance()->getIntegerForKey("coinNum");
		setCoinNum(coin);
	}
	else{
		UserDefault::getInstance()->setIntegerForKey("maxScore", 0);
		UserDefault::getInstance()->setIntegerForKey("coinNum", 0);
	}
	UserDefault::getInstance()->setBoolForKey("musicPlay", true);

	auto setMusicOff = MenuItemImage::create("setMusicOff.png", "setMusicOff.png");
	auto setMusicOn = MenuItemImage::create("setMusicOn.png", "setMusicOn.png");
	MenuItemToggle *setMusicItem = MenuItemToggle::createWithCallback(
		CC_CALLBACK_1(MainMenu::menuSetMusicCallback, this), setMusicOff, setMusicOn, NULL);
	setMusicItem->setPosition(Vec2(visibleSize.width * 4 / 7 + 80, visibleSize.height * 6 / 7 + 60));


	cocos2d::MenuItemFont::setFontName("SketchFlow Print");
	cocos2d::MenuItemFont::setFontSize(36);

	cocos2d::MenuItemFont *level[4];
	for (int i = 0; i < 4; i++) {
		__String *levelName = __String::createWithFormat("level %d", i+1);
		level[i] = cocos2d::MenuItemFont::create(levelName->getCString(), CC_CALLBACK_1(MainMenu::menuLevelCallback, this));
		level[i]->setPosition(cocos2d::Vec2(visibleSize.width / 2, visibleSize.height / 2 - i*95));
		level[i]->setColor(Color3B(0, 0, 0));
		level[i]->setTag(LEVEL_TAG + i);
	}

	cocos2d::Menu *menu = cocos2d::Menu::create(level[0],level[1],level[2],level[3], setMusicItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu,1);


	return true;
}


void MainMenu::onEnterTransitionDidFinish()
{
	Scene::onEnterTransitionDidFinish();
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("see you again .mp3", true);
	CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.3);
}

void MainMenu::menuCloseCallback(cocos2d::Ref* pSender)
{
	Director::getInstance()->end();
}

void MainMenu::menuLevelCallback(cocos2d::Ref* pSender)
{
	MenuItem *levelItem = (MenuItem *)pSender;
	Layer *level;
	switch (levelItem->getTag()) {
	case LEVEL_TAG: level = LevelOne::createScene(); break;
	case LEVEL_TAG + 1:level = LevelTwo::createScene(); break;
	case LEVEL_TAG + 2:level = LevelThree::createScene(); break;
	case LEVEL_TAG + 3:level = LevelFour::createScene(); break;
	}
	auto LevelScene = Scene::create();
	LevelScene->addChild(level);
	Director::getInstance()->replaceScene(LevelScene);
}

void MainMenu::setMaxScore(int score)
{
	auto scoreLabel = this->getChildByTag(MAXSCORE_TAG);
	if (scoreLabel != nullptr) this->removeChild(scoreLabel);
	auto vSize= Director::getInstance()->getVisibleSize();
	std::stringstream theScore;
	std::string str;
	theScore << score;
	theScore >> str;
	scoreLabel = Label::createWithSystemFont(str, "Fredoka One", 24);
	scoreLabel->setPosition(Vec2(vSize.width/4+30,4*vSize.height/5-15));
	scoreLabel->setColor(Color3B(255,215,0));
	scoreLabel->setTag(MAXSCORE_TAG);
	this->addChild(scoreLabel);
}

void MainMenu::setCoinNum(int coin)
{
	auto coinLabel = this->getChildByTag(COINNUM_TAG);
	if (coinLabel != nullptr) this->removeChild(coinLabel);
	auto vSize = Director::getInstance()->getVisibleSize();
	std::stringstream theCoin;
	std::string coinNum;
	theCoin << coin;
	theCoin >> coinNum;
	coinLabel = Label::createWithSystemFont(coinNum, "Fredoka One", 24);
	coinLabel->setPosition(Vec2(vSize.width / 4 + 30, 9 * vSize.height / 14 +10));
	coinLabel->setColor(Color3B(255, 215, 0));
	coinLabel->setTag(MAXSCORE_TAG);
	this->addChild(coinLabel);
}

void MainMenu::menuSetMusicCallback(Ref* pSender)
{
	auto soundToggle = (MenuItemToggle*)pSender;
	if (soundToggle->getSelectedIndex() == 1) {
		auto setMusic = SetMusic::createScene();
		setMusic->setTag(SETMUSIC_TAG);
		this->addChild(setMusic);
	}
	else{
		this->removeChildByTag(SETMUSIC_TAG);
	}
}

void MainMenu::sliderChange(Ref* psender, Control::EventType event)
{
	ControlSlider* slider = (ControlSlider*)psender;
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(slider->getValue());
	float volume = slider->getValue() / 1000;

	CocosDenshion::SimpleAudioEngine::sharedEngine()->getInstance()->setBackgroundMusicVolume(volume);
	//String* valueStr = String::createWithFormat("%f", slider->getValue());
	//log(valueStr->getCString());
	log("%f", CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume());

}