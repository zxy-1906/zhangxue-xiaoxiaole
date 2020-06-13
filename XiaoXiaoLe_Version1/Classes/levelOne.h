#ifndef __LEVELONE_LAYER_H__
#define __LEVELONE_LAYER_H__

#include"cocos2d.h"
#include<vector>
#include<stack>
#include<ctime>

#define SCORE_TAG 31
#define STEP_TAG 32
#define TASK_TAG 33
#define SPECIAL_TAG 34

struct thePoint
{
	int x, y;
	void point(int x1, int y1) { x = x1, y = y1; }
};

class LevelOne :public cocos2d::Layer
{
protected:
	bool moveTerm;

	std::clock_t touchTime;

	struct thePoint  touchCoor, movePoint;

	cocos2d::Vec2 origin, firstTouch;
	std::vector<std::vector<cocos2d::Sprite *>>board;
	std::vector<std::vector<int>> referenceBoard;
	std::vector<int > countEveryCell;
	std::vector<std::vector<cocos2d::Sprite *>>iceBoard;
	std::stack<struct thePoint> willClear;

	int Score,Step,Task;
	int taskSprite, lockCell;

	cocos2d::Vec2 firstMoveBy, secondMoveBy;
public:
	static cocos2d::Layer* createScene();

	virtual bool init();
	void findCanClear();
	void clear();
	void replenish(float dt);
	void delayTime();
	void isTap(cocos2d::Touch *touch);
	void reset();
	void LegalOperation(float dt);
	void repeatFind(float dt);
	void menuBackCallback(cocos2d::Ref* pSender);
	void update();
	void success();
	void failed();
	void situation();
	void randomMap();
	void specialEffect(int num);

	virtual bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
	virtual void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event);
	virtual void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event);

	CREATE_FUNC(LevelOne);
private:

};

#endif