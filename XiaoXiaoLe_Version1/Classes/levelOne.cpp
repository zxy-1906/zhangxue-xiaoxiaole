#include"levelOne.h"
#include<vector>
#include<stack>

USING_NS_CC;


Layer* LevelOne::createScene()
{
	return LevelOne::create();
}

bool LevelOne::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize(); //+ Vec2(-150, -150);
	origin = Vec2(visibleSize.width / 2 - 120, visibleSize.height / 2 - 150);

	board.resize(3);
	for (int i = 0; i < 3; i++)
	{
		board[i].resize(3);
		for (int j = 0; j < 3; j++)
		{
			int order = CCRANDOM_0_1() * 4;
			__String *imagName = __String::createWithFormat("cell%d.png", order + 1);
			auto sprite = Sprite::create(imagName->getCString());
			sprite->setPosition(Vec2(i * 100, j * 100) + origin + Vec2(50, 50));
			board[i][j] = sprite;
			board[i][j]->setTag(order + 1);
			this->addChild(board[i][j]);
		}
	}

	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	nowScene = Director::getInstance()->getRunningScene();

	return true;
}

bool LevelOne::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	moveTerm = true;
	firstTouch = touch->getLocation();
	isTap(touch);

	if (touchCoor.x != -1 && touchCoor.y != -1)
	{
		log("Tag is %d ", board[touchCoor.x][touchCoor.y]->getTag());
		auto light = Sprite::create("light.png");
		light->setPosition(Vec2(touchCoor.x * 100, touchCoor.y * 100) + origin + Vec2(50, 50));
		this->addChild(light, 1, 12);

		return true;
	}
	return false;
}

void  LevelOne::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{
	Vec2 touchIn = touch->getLocation();

	Node *temps = this->getChildByTag(12);
	if (temps != nullptr) this->removeChildByTag(12);

	float l = (touchIn.x - firstTouch.x)*(touchIn.x - firstTouch.x) + (touchIn.y - firstTouch.y)*(touchIn.y - firstTouch.y);

	if (!moveTerm || l<10) return;


	if (touchIn.x - touchIn.y >(firstTouch.x - firstTouch.y))
	{
		if (touchIn.x + touchIn.y < (firstTouch.x + firstTouch.y))
		{
			if (touchCoor.y > 0){
				movePoint = Vec2(touchCoor.x, touchCoor.y-1);
				firstMoveBy = Vec2(0, -100), secondMoveBy = Vec2(0, 100);
			}
		}
		else
		{
			if (touchCoor.x < 2){
				movePoint = Vec2(touchCoor.x + 1, touchCoor.y);
				firstMoveBy = Vec2(100, 0), secondMoveBy = Vec2(-100, 0);
			}
		}
	}
	else
	{
		if (touchIn.x + touchIn.y < (firstTouch.x + firstTouch.y))
		{
			if (touchCoor.x > 0){
				movePoint = Vec2(touchCoor.x - 1, touchCoor.y);
				firstMoveBy = Vec2(-100, 0), secondMoveBy = Vec2(100, 0);
			}
		}
		else
		{
			if (touchCoor.y < 2){
				movePoint = Vec2(touchCoor.x, touchCoor.y + 1);
				firstMoveBy = Vec2(0, 100), secondMoveBy = Vec2(0, -100);
			}
		}
	}

	board[touchCoor.x][touchCoor.y]->runAction(MoveBy::create(0.5, firstMoveBy));
	board[movePoint.x][movePoint.y]->runAction(MoveBy::create(0.5, secondMoveBy));
	Sprite *temp = board[touchCoor.x][touchCoor.y];
	board[touchCoor.x][touchCoor.y] = board[movePoint.x][movePoint.y];
	board[movePoint.x][movePoint.y] = temp;


	this->schedule(schedule_selector(LevelOne::isRightMove), 1, 0, 0.5f);//ÑÓÊ±µ÷ÓÃ

	moveTerm = false;
}

void LevelOne::isRightMove(float dt)
{
	std::stack<Vec2 *> willClear = findCanClear();
	if (willClear.empty())
	{ 
		board[touchCoor.x][touchCoor.y]->runAction(MoveBy::create(0.5, firstMoveBy));
		board[movePoint.x][movePoint.y]->runAction(MoveBy::create(0.5, secondMoveBy));
		Sprite *temp = board[touchCoor.x][touchCoor.y];
		board[touchCoor.x][touchCoor.y] = board[movePoint.x][movePoint.y];
		board[movePoint.x][movePoint.y] = temp;
	}
	else
		clear(willClear);
}

std::stack<Vec2 *> LevelOne::findCanClear()
{
	std::stack<Vec2 *> horizon;
	std::stack<Vec2 *> willClear;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Vec2 *spriteCoor = new Vec2(i, j);
			if (horizon.empty()) horizon.push(spriteCoor);
			else
			{
				Vec2 *topSprite = horizon.top();
				if (horizon.size() < 3 && board[topSprite->x][topSprite->y]->getTag() != board[i][j]->getTag())
				{
					while (!horizon.empty())
					{
						horizon.pop();
					}
				}
				else if (horizon.size() >= 3 && board[topSprite->x][topSprite->y]->getTag() != board[i][j]->getTag())
				{
					while (!horizon.empty())
					{
						Vec2 *tempSprite = horizon.top();
						willClear.push(tempSprite);
						horizon.pop();
					}
				}
				else if (board[topSprite->x][topSprite->y]->getTag() == board[i][j]->getTag())
				{
					horizon.push(spriteCoor);
				}
			}
		}
		if (horizon.size() < 3)
		{
			while (!horizon.empty())
				horizon.pop();
		}
		else
		{
			while (!horizon.empty())
			{
				Vec2 *tempSprite = horizon.top();
				willClear.push(tempSprite);
				horizon.pop();
			}
		}
	}
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			Vec2 *spriteCoor = new Vec2(i, j);
			if (horizon.empty()) horizon.push(spriteCoor);
			else
			{
				Vec2 *topSprite = horizon.top();
				if (horizon.size() < 3 && board[topSprite->x][topSprite->y]->getTag() != board[i][j]->getTag())
				{
					while (!horizon.empty())
					{
						horizon.pop();
					}
				}
				else if (horizon.size() >= 3 && board[topSprite->x][topSprite->y]->getTag() != board[i][j]->getTag())
				{
					while (!horizon.empty())
					{
						Vec2 *tempSprite = horizon.top();
						willClear.push(tempSprite);
						horizon.pop();
					}
				}
				else if (board[topSprite->x][topSprite->y]->getTag() == board[i][j]->getTag())
				{
					horizon.push(spriteCoor);
				}
			}
		}
		if (horizon.size() < 3)
		{
			while (!horizon.empty())
				horizon.pop();
		}
		else
		{
			while (!horizon.empty())
			{
				Vec2 *tempSprite = horizon.top();
				willClear.push(tempSprite);
				horizon.pop();
			}
		}
	}
	
	return willClear;
}

void LevelOne::clear(std::stack<Vec2 *> willClear)
{
	auto *theParticleLayer = nowScene->getChildByTag(341);
	while (!willClear.empty())
	{
		Vec2 *topSprite = willClear.top();
		if (board[topSprite->x][topSprite->y] != nullptr)
		{
			this->removeChild(board[topSprite->x][topSprite->y]);

			ParticleSystem *ps = ParticleExplosion::create();
			ps->setPosition(Vec2(topSprite->x * 100, topSprite->y * 100) + origin + Vec2(50, 50));
			ps->setLife(0.005f);
			ps->setSpeed(0.5f);
			ps->setDuration(0.01f);
			ps->setTag(1111);
			theParticleLayer->addChild(ps);

			board[topSprite->x][topSprite->y] = nullptr;
		}
		willClear.pop();
	}
}

void  LevelOne::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	Node *temps = this->getChildByTag(12);
	if (temps != nullptr) this->removeChildByTag(12);
	auto *theParticleLayer = nowScene->getChildByTag(341);
	theParticleLayer->removeAllChildren();
}

void LevelOne::isTap(Touch *touch)
{
	int x = (int)(touch->getLocation().x - origin.x) / 100;
	int y = (int)(touch->getLocation().y - origin.y) / 100;
	//log("touch -> ( %f , %f ) , origin -> ( %f , %f )", touch->getLocation().x, touch->getLocation().y, origin.x, origin.y);
	//log(" ( %d , %d )", x, y);
	if (x >= 0 && x < 3 && y >= 0 && y < 3)
		touchCoor.x = x, touchCoor.y = y;
	else
		touchCoor.x = -1, touchCoor.y = -1;
}