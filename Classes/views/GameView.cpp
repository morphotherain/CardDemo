// GameView.cpp
#include "GameView.h"
#include "CardView.h"
#include "ui/CocosGUI.h"
#include "../utils/GameUtils.h"

USING_NS_CC;
using namespace cocos2d::ui;

// 定义区域尺寸常量
const Size MAIN_AREA_SIZE(1080, 1500);
const Size PILE_AREA_SIZE(1080, 580);

// 定义区域颜色
const Color4B MAIN_AREA_COLOR(139, 69, 19, 255);    // 棕色
const Color4B PILE_AREA_COLOR(128, 0, 128, 255);    // 紫色

// 撤销按钮
const float UNDO_LABEL_X = 900;
const float UNDO_LABEL_Y = 300;
const float UNDO_LABEL_SIZE = 36;
const Color4B UNDO_LABEL_COLOR(255, 255, 255, 255); // 白色

GameView* GameView::create(GameModel* model) {
    GameView* layer = new GameView();
    if (layer && layer->initWithModel(model)) {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool GameView::init() {
    return Layer::init();
}

bool GameView::initWithModel(GameModel* model) {
    if (!this->init()) return false;
    this->model = model;

    // 创建背景区域
    createGameAreas();

    createUndoLabel();

    // 添加触摸事件监听器
    setupTouchHandling();

    return true;
}

void GameView::createGameAreas() {
    // 获取屏幕尺寸和锚点位置
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建主区域节点
    mainAreaNode = Node::create();
    mainAreaNode->setPosition(0, PILE_AREA_SIZE.height);
    this->addChild(mainAreaNode, 0);

    // 创建主牌区背景
    auto mainAreaBg = DrawNode::create();
    Vec2 mainAreaVertices[4] = {
        Vec2(0, 0),
        Vec2(MAIN_AREA_SIZE.width, 0),
        Vec2(MAIN_AREA_SIZE.width, MAIN_AREA_SIZE.height),
        Vec2(0, MAIN_AREA_SIZE.height)
    };

    // 将Color4B转换为Color4F
    Color4F mainAreaColorF(
        MAIN_AREA_COLOR.r / 255.0f,
        MAIN_AREA_COLOR.g / 255.0f,
        MAIN_AREA_COLOR.b / 255.0f,
        MAIN_AREA_COLOR.a / 255.0f
    );

    mainAreaBg->drawPolygon(mainAreaVertices, 4, mainAreaColorF, 0, mainAreaColorF);
    mainAreaNode->addChild(mainAreaBg, -1); // 置于底层

    // 创建堆牌区节点
    pileAreaNode = Node::create();
    pileAreaNode->setPosition(0, 0);
    this->addChild(pileAreaNode, 0);

    auto pileAreaBg = DrawNode::create();
    Vec2 pileAreaVertices[4] = {
        Vec2(0, 0),
        Vec2(PILE_AREA_SIZE.width, 0),
        Vec2(PILE_AREA_SIZE.width, PILE_AREA_SIZE.height),
        Vec2(0, PILE_AREA_SIZE.height)
    };

    Color4F pileAreaColorF(
        PILE_AREA_COLOR.r / 255.0f,
        PILE_AREA_COLOR.g / 255.0f,
        PILE_AREA_COLOR.b / 255.0f,
        PILE_AREA_COLOR.a / 255.0f
    );

    pileAreaBg->drawPolygon(pileAreaVertices, 4, pileAreaColorF, 0, pileAreaColorF);
    pileAreaNode->addChild(pileAreaBg, -1); // 置于底层
}

void GameView::createUndoLabel() {
    // 创建撤销标签
    _undoLabel = Label::createWithSystemFont(GameUtils::GBKToUTF8("回退"), "simhei", UNDO_LABEL_SIZE);
    if (_undoLabel) {
        _undoLabel->setPosition(UNDO_LABEL_X, UNDO_LABEL_Y);
        _undoLabel->setTextColor(Color4B(UNDO_LABEL_COLOR));
        _undoLabel->enableOutline(Color4B::BLACK, 1);
        pileAreaNode->addChild(_undoLabel, 0); // 置于顶层
    }
}

void GameView::setupTouchHandling() {
    auto touchListener = EventListenerTouchOneByOne::create();

    // 设置吞噬触摸，防止事件穿透
    touchListener->setSwallowTouches(true);

    touchListener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 touchLocation = touch->getLocation();

        // 检查是否点击了撤销标签
        if (_undoLabel && _undoLabel->getBoundingBox().containsPoint(_undoLabel->getParent()->convertToNodeSpace(touchLocation))) {
            if (_onUndoClickCallback) {
                _onUndoClickCallback();
            }
            return true;
        }
        // 检查是否点击了手牌区的牌
        Vec2 pileLocalPos = pileAreaNode->convertToNodeSpace(touchLocation);
        for (auto& child : pileAreaNode->getChildren()) {
            if (child->getBoundingBox().containsPoint(pileLocalPos)) {
                CardView* cardView = dynamic_cast<CardView*>(child);
                if (cardView) {
                    CardModel* cardModel = cardView->getCardModel();
                    if (cardModel && cardModel->getId() > 0) {
                        if (_onCardClickCallback) {
                            _onCardClickCallback(cardModel->getId());
                        }
                        return true;
                    }
                }
            }
        }

        Vec2 mainLocalPos = mainAreaNode->convertToNodeSpace(touchLocation);
        for (auto& child : mainAreaNode->getChildren()) {
            if (child->getBoundingBox().containsPoint(mainLocalPos)) {
                CardView* cardView = dynamic_cast<CardView*>(child);
                if (cardView) {
                    CardModel* cardModel = cardView->getCardModel();
                    if (cardModel && cardModel->getId() > 0) {
                        if (_onCardClickCallback) {
                            _onCardClickCallback(cardModel->getId());
                        }
                        return true;
                    }
                }
            }
        }
        return false;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}



void GameView::setOnCardClickCallback(const std::function<void(int)>& callback) {
    _onCardClickCallback = callback;
}
void GameView::setOnUndoClickCallback(const std::function<void()>& callback) {
    _onUndoClickCallback = callback;
}