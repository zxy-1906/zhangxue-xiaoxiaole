#include"selectLevel.h"
#include"MainMenu.h"
#include"Loading.h"
#include"levelOne.h"


cocos2d::Layer* levelSelected::createScene()
{
	return levelSelected::create();
}

bool levelSelected::init()
{
	if (!Layer::init())
		return false;

	auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

	auto label = cocos2d::Label::createWithSystemFont("Please select a level !", "SketchFlow Print", 30);
	label->setPosition(cocos2d::Vec2(3 * visibleSize.width / 5, 5 * visibleSize.height / 6));
	this->addChild(label);

	cocos2d::MenuItemFont::setFontName("SketchFlow Print");
	cocos2d::MenuItemFont::setFontSize(30);
	cocos2d::MenuItemFont * level1 = cocos2d::MenuItemFont::create("Level 1", CC_CALLBACK_1(levelSelected::menuLevelOneCallback,this));
	level1->setPosition(cocos2d::Vec2(visibleSize.width / 10, visibleSize.height / 8));
	cocos2d::Menu *menu = cocos2d::Menu::create(level1, NULL);
	this->addChild(menu);

}

void levelSelected::menuLevelOneCallback(cocos2d::Ref* pSender)
{
	auto newL = LevelOne::createScene();
	cocos2d::Scene * runingScene= cocos2d::Director::getInstance()->getRunningScene();
	runingScene->addChild(newL);
	runingScene->removeChildByTag(22);
}