#ifndef __LEVELONE_LAYER_H__
#define __LEVELONE_LAYER_H__

#include"cocos2d.h"
#include<vector>
#include<stack>

class LevelOne :public cocos2d::Layer
{
	bool moveTerm;

	cocos2d::Vec2 origin, firstTouch, touchCoor;
	std::vector<std::vector<cocos2d::Sprite *>>board;

	cocos2d::Scene *nowScene;

	cocos2d::Vec2 movePoint, firstMoveBy, secondMoveBy;
public:
	static cocos2d::Layer* createScene();


	virtual bool init();
	std::stack<cocos2d::Vec2 *> findCanClear();
	void clear(std::stack<cocos2d::Vec2 *> willClear);

	virtual bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
	virtual void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event);
	virtual void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event);

	void isTap(cocos2d::Touch *touch);

	void isRightMove(float dt);


	CREATE_FUNC(LevelOne);
private:

};

#endif