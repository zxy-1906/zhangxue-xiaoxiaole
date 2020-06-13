#include"setMusic.h"
#include "cocos-ext.h"//进度条头文件
#include "SimpleAudioEngine.h"
#include"mainMenu.h"

USING_NS_CC_EXT;

USING_NS_CC;


Layer* SetMusic::createScene()
{
	return SetMusic::create();
}

bool SetMusic::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto bg = Sprite::create("musicbg.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg,3);

	auto soundOff = MenuItemImage::create("musicStop.png", "musicStop.png");
	auto soundOn = MenuItemImage::create("musicPlay.png", "musicPlay.png");

	MenuItemToggle *itemMusic;
	bool musicPlay = UserDefault::getInstance()->getBoolForKey("musicPlay");
	if (musicPlay) {
		itemMusic = MenuItemToggle::createWithCallback(
			CC_CALLBACK_1(SetMusic::menuMusicCallback_1, this), soundOff, soundOn, NULL);
	}
	else {
		itemMusic = MenuItemToggle::createWithCallback(
			CC_CALLBACK_1(SetMusic::menuMusicCallback_2, this), soundOn, soundOff, NULL);
	}
	
	itemMusic->setPosition(Vec2(visibleSize.width*4 / 7+20, visibleSize.height*6 / 7+60));

	cocos2d::Menu *menu = cocos2d::Menu::create(itemMusic, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 5);

	ControlSlider* slider = ControlSlider::create("sliderbg.png", "sliderprogress.png", "sliderThumb.png");
	slider->setPosition(Vec2(visibleSize.width / 3, visibleSize.height*6 / 7+60));
	//设置滑动条的范围  
	slider->setMinimumValue(0);
	slider->setMaximumValue(1000);
	//设置滑动条当前值  
	slider->setValue(300);
	slider->addTargetWithActionForControlEvents(this, cccontrol_selector(SetMusic::sliderChange), Control::EventType::VALUE_CHANGED);//控制音量
	this->addChild(slider,5);


	return true;
}

void SetMusic::menuMusicCallback_1(Ref* pSender)
{
	auto soundToggle = (MenuItemToggle*)pSender;
	if (soundToggle->getSelectedIndex() == 1) {
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		UserDefault::getInstance()->setBoolForKey("musicPlay", false);
	}
	else {
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		UserDefault::getInstance()->setBoolForKey("musicPlay", true);
	}

}

void SetMusic::menuMusicCallback_2(Ref* pSender) {
	auto soundToggle = (MenuItemToggle*)pSender;
	if (soundToggle->getSelectedIndex() == 1) {
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		UserDefault::getInstance()->setBoolForKey("musicPlay", true);
	}
	else {
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		UserDefault::getInstance()->setBoolForKey("musicPlay", false);
	}
}

void SetMusic::sliderChange(Ref* psender, Control::EventType event)
{
	ControlSlider* slider = (ControlSlider*)psender;
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(slider->getValue());
	float volume = slider->getValue() / 1000;

	CocosDenshion::SimpleAudioEngine::sharedEngine()->getInstance()->setBackgroundMusicVolume(volume);
	//String* valueStr = String::createWithFormat("%f", slider->getValue());
	//log(valueStr->getCString());
	log("%f", CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume());

}