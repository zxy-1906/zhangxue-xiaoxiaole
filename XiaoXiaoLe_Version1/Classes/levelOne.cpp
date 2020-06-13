#include"levelOne.h"
#include<vector>
#include<stack>
#include<random>
#include<ctime>
#include"mainMenu.h"
#include<sstream>

USING_NS_CC;

#define SIZE 7
#define TYPECELL 5

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
	origin = Vec2(visibleSize.width / 2-175, visibleSize.height / 2 - 175);

	Score = 0;
	Step = 18;
	Task = 16;
	update();
	lockCell = -1;

	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis(1, TYPECELL);

	taskSprite = dis(random);

	countEveryCell.resize(TYPECELL);

	auto bg = Sprite::create("guanqiabj.png");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg, 2);

	auto baseImage = Sprite::create("fangkuaibj.png");
	baseImage->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(baseImage, 0);

	__String *taskSpriteName = __String::createWithFormat("cell%d.png", taskSprite);
	auto taskSpriteDisplay = Sprite::create(taskSpriteName->getCString());
	taskSpriteDisplay->setPosition(Vec2(visibleSize.width /2-25, visibleSize.height * 10 / 11 - 20));
	this->addChild(taskSpriteDisplay,3);

	touchTime = std::clock() / CLOCKS_PER_SEC;

	randomMap();

	board.resize(SIZE);
	for (int i = 0; i < SIZE; i++) board[i].resize(SIZE);
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (referenceBoard[i][j] != 0)
			{
				__String *imagName = __String::createWithFormat("cell%d.png", referenceBoard[i][j]);
				auto sprite = Sprite::create(imagName->getCString());
				sprite->setPosition(Vec2(i * 50, j * 50) + origin + Vec2(25, 25));
				board[i][j] = sprite;
				board[i][j]->setTag(referenceBoard[i][j]);
				this->addChild(board[i][j]);
			}
			else board[i][j] = nullptr;
		}
	}

	cocos2d::MenuItemFont::setFontName("SketchFlow Print");
	cocos2d::MenuItemFont::setFontSize(30);
	cocos2d::MenuItemFont * level1 = cocos2d::MenuItemFont::create("BACK", CC_CALLBACK_1(LevelOne::menuBackCallback, this));
	level1->setPosition(cocos2d::Vec2(0, -visibleSize.height/3));
	level1->setColor(Color3B(0, 0, 0));
	cocos2d::Menu *menu = cocos2d::Menu::create(level1, NULL);
	this->addChild(menu, 3);

	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	return true;
}

void LevelOne::randomMap()
{
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis(1, 5);
	referenceBoard.resize(SIZE);
	for (int i = 0; i < SIZE; i++) referenceBoard[i].resize(SIZE);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			referenceBoard[i][j] = dis(random);
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 2; j < SIZE; j++) {
			if (referenceBoard[i][j - 1] == referenceBoard[i][j - 2]) {
				referenceBoard[i][j] = dis(random);
				while (referenceBoard[i][j] == referenceBoard[i][j - 1]) {
					referenceBoard[i][j] = dis(random);
				}
			}
			else
				referenceBoard[i][j] = dis(random);
		}
	}
	for (int i = 2; i < SIZE; i++) {
		for (int j = 0; j < 2; j++) {
			if (referenceBoard[i-1][j] == referenceBoard[i-2][j]) {
				referenceBoard[i][j] = dis(random);
				while (referenceBoard[i][j] == referenceBoard[i-1][j]) {
					referenceBoard[i][j] = dis(random);
				}
			}
			else
				referenceBoard[i][j] = dis(random);
		}
	}
	for (int i = 2; i < SIZE; i++) {
		for (int j = 2; j < SIZE; j++) {
			if (referenceBoard[i][j - 1] == referenceBoard[i][j - 2] || referenceBoard[i - 1][j] == referenceBoard[i - 2][j]) {
				referenceBoard[i][j] = dis(random);
				while (referenceBoard[i][j] == referenceBoard[i][j - 1] || referenceBoard[i][j] == referenceBoard[i - 1][j]) {
					referenceBoard[i][j] = dis(random);
				}
			}
			else
				referenceBoard[i][j] = dis(random);
		}
	}

}

void LevelOne::update()
{
	auto visibleSize = Director::getInstance()->getVisibleSize(); //+ Vec2(-150, -150);
	auto scoreLabel = this->getChildByTag(SCORE_TAG);
	if (scoreLabel != nullptr) this->removeChild(scoreLabel);
	auto stepLabel = this->getChildByTag(STEP_TAG);
	if (stepLabel != nullptr) this->removeChild(stepLabel);
	auto taskLabel = this->getChildByTag(TASK_TAG);
	if (taskLabel != nullptr) this->removeChild(taskLabel);
	__String *score = __String::createWithFormat("%d", Score);
	scoreLabel = Label::createWithSystemFont(score->getCString(),"Fredoka One",30);
	scoreLabel->setPosition(Vec2(visibleSize.width/6-12,visibleSize.height* 10 / 11-20));
	scoreLabel->setColor(Color3B(205, 127, 50));
	scoreLabel->setTag(SCORE_TAG);
	__String *step = __String::createWithFormat("%d", Step);
	stepLabel = Label::createWithSystemFont(step->getCString(), "Fredoka One", 30);
	stepLabel->setPosition(Vec2(4*visibleSize.width / 5+15, visibleSize.height * 10 / 11-30));
	stepLabel->setColor(Color3B(205, 127, 50));
	stepLabel->setTag(STEP_TAG);
	__String *task = __String::createWithFormat("%d", Task);
	taskLabel = Label::createWithSystemFont(task->getCString(), "Fredoka One", 30);
	taskLabel->setPosition(Vec2(visibleSize.width*4 /7, visibleSize.height * 10 / 11-20));
	taskLabel->setColor(Color3B(205, 127, 50));
	taskLabel->setTag(TASK_TAG);
	this->addChild(scoreLabel,4);
	this->addChild(stepLabel, 4);
	this->addChild(taskLabel, 4);
}

void LevelOne::menuBackCallback(cocos2d::Ref* pSender)
{
	auto mainMenuScene = MainMenu::createScene();
	Director::getInstance()->pushScene(mainMenuScene);
}

bool LevelOne::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	moveTerm = true;
	firstTouch = touch->getLocation();
	isTap(touch);

	std::clock_t nowtime = std::clock()/ CLOCKS_PER_SEC;
	int timeDifference = 2*(nowtime - touchTime);
	log("time is %d ", timeDifference);
	if (timeDifference > 1)
		touchTime = nowtime;
	else
		return false;

	if (touchCoor.x != -1 && touchCoor.y != -1)
	{
		log("Tag is %d ", board[touchCoor.x][touchCoor.y]->getTag());
		//log(" x is %d, y is %d", touchCoor.x, touchCoor.y);
		auto light = Sprite::create("light.png");
		light->setPosition(Vec2(touchCoor.x * 50, touchCoor.y * 50) + origin + Vec2(25, 25));
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

	//log(" touchIn x %f,touchIn y %f", touchIn.x, touchIn.y);

	if (touchIn.x - touchIn.y >(firstTouch.x - firstTouch.y))
	{
		if (touchIn.x + touchIn.y < (firstTouch.x + firstTouch.y))
		{
			if (touchCoor.y > 0 && board[touchCoor.x][touchCoor.y - 1] != nullptr) {
				movePoint.point(touchCoor.x, touchCoor.y - 1);
				log("movePoint");
				log("movePoint : x %d  y %d", movePoint.x, movePoint.y);
				firstMoveBy = Vec2(0, -50), secondMoveBy = Vec2(0, 50);
			}
			else
				return;
		}
		else
		{
			if (touchCoor.x < SIZE - 1 && board[touchCoor.x + 1][touchCoor.y] != nullptr) {
				movePoint.point(touchCoor.x + 1, touchCoor.y);
				firstMoveBy = Vec2(50, 0), secondMoveBy = Vec2(-50, 0);
			}
			else
				return;
		}
	}
	else
	{
		if (touchIn.x + touchIn.y < (firstTouch.x + firstTouch.y))
		{
			if (touchCoor.x > 0 && board[touchCoor.x - 1][touchCoor.y] != nullptr) {
				movePoint.point(touchCoor.x - 1, touchCoor.y);
				firstMoveBy = Vec2(-50, 0), secondMoveBy = Vec2(50, 0);
			}
			else
				return;
		}
		else
		{
			if (touchCoor.y < SIZE - 1 && board[touchCoor.x][touchCoor.y + 1] != nullptr) {
				movePoint.point(touchCoor.x, touchCoor.y + 1);
				firstMoveBy = Vec2(0, 50), secondMoveBy = Vec2(0, -50);
			}
			else
				return;
		}
	}
	for(int i=0;i<TYPECELL;i++)
			countEveryCell[i] = 0;

	board[touchCoor.x][touchCoor.y]->runAction(MoveBy::create(0.5, firstMoveBy));
	board[movePoint.x][movePoint.y]->runAction(MoveBy::create(0.5, secondMoveBy));
	Sprite *temp = board[touchCoor.x][touchCoor.y];
	board[touchCoor.x][touchCoor.y] = board[movePoint.x][movePoint.y];
	board[movePoint.x][movePoint.y] = temp;

	if (board[touchCoor.x][touchCoor.y]->getTag() == SPECIAL_TAG) {
		lockCell = board[movePoint.x][movePoint.y]->getTag();
		specialEffect(lockCell);
		struct thePoint p;
		p.point(touchCoor.x, touchCoor.y);
		willClear.push(p);
		this->schedule(schedule_selector(LevelOne::repeatFind), 1, 0, 0.5f);
	}
	else if (board[movePoint.x][movePoint.y]->getTag() == SPECIAL_TAG) {
		lockCell = board[touchCoor.x][touchCoor.y]->getTag();
		specialEffect(lockCell);
		struct thePoint p;
		p.point(movePoint.x, movePoint.y);
		willClear.push(p);
		this->schedule(schedule_selector(LevelOne::repeatFind), 1, 0, 0.5f);
	}
	else
		this->schedule(schedule_selector(LevelOne::LegalOperation), 1, 0, 0.5f);

	moveTerm = false;
}

void LevelOne::specialEffect(int num)
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (board[i][j]->getTag() == num) {
				struct thePoint p;
				p.point(i, j);
				willClear.push(p);
			}
		}
	}
}

void LevelOne::reset()
{
	board[touchCoor.x][touchCoor.y]->runAction(MoveBy::create(0.5, firstMoveBy));
	board[movePoint.x][movePoint.y]->runAction(MoveBy::create(0.5, secondMoveBy));
	Sprite *temp = board[touchCoor.x][touchCoor.y];
	board[touchCoor.x][touchCoor.y] = board[movePoint.x][movePoint.y];
	board[movePoint.x][movePoint.y] = temp;
}

void LevelOne::delayTime()
{
	std::clock_t t1 = clock() / CLOCKS_PER_SEC;
	std::clock_t t2 = clock() / CLOCKS_PER_SEC;
	int deltT = t2 - t1;
	while (deltT < 3) { t2 = clock() / CLOCKS_PER_SEC; deltT = t2 - t1; }
	t2 = clock() / CLOCKS_PER_SEC;
	deltT = t2 - t1;
	log("deltT is %d", deltT);
}

void LevelOne::findCanClear()
{
	log("find can clear");
	std::stack<struct thePoint> horizon;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (board[i][j] != nullptr)
			{
				struct thePoint spriteCoor;	spriteCoor.point(i, j);
				if (!horizon.empty())
				{
					struct thePoint topSprite = horizon.top();
					if (horizon.size() < 3 && board[topSprite.x][topSprite.y]->getTag() != board[i][j]->getTag())
					{
						while (!horizon.empty())
						{
							horizon.pop();
						}
					}
					else if (horizon.size() >= 3 && board[topSprite.x][topSprite.y]->getTag() != board[i][j]->getTag())
					{
						while (!horizon.empty())
						{
							struct thePoint tempSprite = horizon.top();
							willClear.push(tempSprite);
							horizon.pop();
						}
					}
				}
				horizon.push(spriteCoor);
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
				struct thePoint tempSprite = horizon.top();
				willClear.push(tempSprite);
				horizon.pop();
			}
		}
	}
	for (int j = 0; j < SIZE; j++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			if (board[i][j] != nullptr)
			{
				struct thePoint spriteCoor;	spriteCoor.point(i, j);
				if (!horizon.empty())
				{
					struct thePoint topSprite = horizon.top();
					if (horizon.size() < 3 && board[topSprite.x][topSprite.y]->getTag() != board[i][j]->getTag())
					{
						while (!horizon.empty())
						{
							horizon.pop();
						}
					}
					else if (horizon.size() >= 3 && board[topSprite.x][topSprite.y]->getTag() != board[i][j]->getTag())
					{
						while (!horizon.empty())
						{
							struct thePoint tempSprite = horizon.top();
							willClear.push(tempSprite);
							horizon.pop();
						}
					}
					
				}
				horizon.push(spriteCoor);
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
				struct thePoint tempSprite = horizon.top();
				willClear.push(tempSprite);
				horizon.pop();
			}
		}
	}
}

void LevelOne::clear()
{
	while (!willClear.empty())
	{
		struct thePoint topSprite = willClear.top();
		if (board[topSprite.x][topSprite.y] != nullptr)
		{
			if (board[topSprite.x][topSprite.y]->getTag() == taskSprite && Task>0) Task--;
			if (board[topSprite.x][topSprite.y]->getTag() != SPECIAL_TAG) countEveryCell[board[topSprite.x][topSprite.y]->getTag()-1]++;
			this->removeChild(board[topSprite.x][topSprite.y]);
			Score++;
			board[topSprite.x][topSprite.y] = nullptr;

		}
		willClear.pop();
	}
	update();
	this->schedule(schedule_selector(LevelOne::replenish), 1, 0, 0.5f);
}

void LevelOne::replenish(float dt)
{
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis(1, 5);
	for (int i = 0; i < SIZE; i++)
	{
		int count = 0;
		for (int j = 0; j < SIZE; j++) {
			if (board[i][j] == nullptr)
				count++;
			else if (count>0) {
				board[i][j - count] = board[i][j];
				board[i][j] = nullptr;
				board[i][j - count]->runAction(MoveBy::create(0.5, Vec2(0, -50 * count)));
				//log("%d , %d",i, j);
			}
		}
		for (int k = count; k > 0; k--) {
			int numRandom = dis(random);
			__String *imagName = __String::createWithFormat("cell%d.png", numRandom);
			auto sprite = Sprite::create(imagName->getCString());
			//sprite->setPosition(Vec2(i * 50, k * -50) + origin + Vec2(25, 25));
			board[i][SIZE - k] = sprite;
			board[i][SIZE - k]->setPosition(Vec2(i * 50, (SIZE + count - k) * 50) + origin + Vec2(25, 25));
			this->addChild(board[i][SIZE - k]);
			board[i][SIZE - k]->runAction(MoveBy::create(0.5, Vec2(0, -50 * count)));
			board[i][SIZE - k]->setTag(numRandom);
		}
	}
	this->schedule(schedule_selector(LevelOne::repeatFind), 1, 0, 0.5f);
}

void  LevelOne::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	Node *temps = this->getChildByTag(12);
	if (temps != nullptr) this->removeChildByTag(12);
}

void LevelOne::isTap(Touch *touch)
{
	int x = (int)(touch->getLocation().x - origin.x) / 50;
	int y = (int)(touch->getLocation().y - origin.y) / 50;
	//log("touch -> ( %f , %f ) , origin -> ( %f , %f )", touch->getLocation().x, touch->getLocation().y, origin.x, origin.y);
	//log(" ( %d , %d )", x, y);
	if (x >= 0 && x < SIZE && y >= 0 && y < SIZE && board[x][y] != nullptr)
	{
		touchCoor.x = x, touchCoor.y = y; 
		log(" ( %d , %d )", touchCoor.x, touchCoor.y);
	}
	else
		touchCoor.x = -1, touchCoor.y = -1;
}

void LevelOne::LegalOperation(float dt)
{
	findCanClear();
	if (willClear.empty())
	{
		reset();
	}
	else
	{
		Step--;
		clear();

	}
}

void LevelOne::repeatFind(float dt)
{
	findCanClear();
	if (!willClear.empty())
		clear();
	else
		situation();
}

void LevelOne::situation()
{
	std::default_random_engine random(time(NULL));
	std::uniform_int_distribution<int> dis(0, SIZE - 1);
	auto visibleSize = Director::getInstance()->getVisibleSize(); //+ Vec2(-150, -150);
	if (Task == 0)
	{
		if (Step > 0)
		{
			auto lightLevel = Sprite::create("levelLighting.png");
			lightLevel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
			this->addChild(lightLevel,5);
			for (; Step > 0; Step--) {
				int x = dis(random), y = dis(random);
				struct thePoint p;
				p.point(x, y);
				willClear.push(p);
			}
			Step = 0;
			this->schedule(schedule_selector(LevelOne::repeatFind), 1, 0, 0.5f);
		}
		else
			success();
	}
	else if (Step == 0)
		failed();
	else{
		for(int i=0;i<TYPECELL;i++)
			if (countEveryCell[i] > 3 && lockCell == -1) {
				int x = dis(random), y = dis(random);
				this->removeChild(board[x][y]);
				auto specialSprite = Sprite::create("specialCell.png");
				specialSprite->setTag(SPECIAL_TAG);
				board[x][y] = specialSprite;
				board[x][y]->setPosition(Vec2(x * 50, y* 50) + origin + Vec2(25, 25));
				this->addChild(board[x][y]);
				break;
			}
	}
	lockCell = -1;
}

void LevelOne::success()
{
	auto visibleSize = Director::getInstance()->getVisibleSize(); //+ Vec2(-150, -150);
	auto successMessage = Sprite::create("success.png");
	successMessage->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(successMessage,5);
	cocos2d::MenuItemFont::setFontName("SketchFlow Print");
	cocos2d::MenuItemFont::setFontSize(36);
	cocos2d::MenuItemFont * okButton = cocos2d::MenuItemFont::create("OK", CC_CALLBACK_1(LevelOne::menuBackCallback, this));
	okButton->setPosition(cocos2d::Vec2(0, -49));
	okButton->setColor(Color3B(0, 0, 0));
	cocos2d::Menu *menu = cocos2d::Menu::create(okButton, NULL);
	this->addChild(menu, 6);

	int maxScore = UserDefault::getInstance()->getIntegerForKey("maxScore");
	if(Score > maxScore)
		UserDefault::getInstance()->setIntegerForKey("maxScore", Score);
	int coin = UserDefault::getInstance()->getIntegerForKey("coinNum");
	UserDefault::getInstance()->setIntegerForKey("coinNum", coin+100);

}

void LevelOne::failed() {
	auto visibleSize = Director::getInstance()->getVisibleSize(); //+ Vec2(-150, -150);
	auto failMessage = Sprite::create("failed.png");
	failMessage->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(failMessage, 5);
	cocos2d::MenuItemFont::setFontName("SketchFlow Print");
	cocos2d::MenuItemFont::setFontSize(36);
	cocos2d::MenuItemFont * okButton = cocos2d::MenuItemFont::create("OK", CC_CALLBACK_1(LevelOne::menuBackCallback, this));
	okButton->setPosition(cocos2d::Vec2(0, -165));
	okButton->setColor(Color3B(0, 0, 0));
	cocos2d::Menu *menu = cocos2d::Menu::create(okButton, NULL);
	this->addChild(menu, 6);
}
