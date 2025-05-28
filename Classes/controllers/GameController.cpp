#include "GameController.h"
#include "../views/CardView.h"

USING_NS_CC;

GameController* GameController::createInstance() {
    CCLOG("Enter createInstance");
    GameController* m_GameController = static_cast<GameController*>(GameController::create());
    return  m_GameController;
}

bool GameController::init() {
    CCLOG("Enter createInstance");
    if (!Scene::init()) return false;

    _model = new GameModel();
    _view = GameView::create(_model);

    _view->setOnCardClickCallback([this](int cardId) {
        this->handleCardClick(cardId);
        });
    _view->setOnUndoClickCallback([this]() {
        this->handleUndoClick();
        });

    // 默认加载第一关
    if (!loadLevel(1)) {
        CCLOGERROR("Failed to load level 1");
        return false;
    }

    this->addChild(_view);
    return true;
}

void GameController::handleCardClick(int cardId) {
    CCLOG("Handling card click: %d", cardId);

    // 查找点击的卡牌
    CardModel* clickedCard = findCardById(cardId);
    if (!clickedCard) {
        CCLOGERROR("Card with id %d not found", cardId);
        return;
    }

    // 分发到不同区域处理
    if (isCardInReserve(clickedCard)) {
        handleReserveCardClick(clickedCard);
    }
    else if (isCardInPlayField(clickedCard)) {
        handlePlayFieldCardClick(clickedCard);
    }
    else {
        CCLOG("Card not in reserve or play field");
    }
}
bool GameController::isCardInReserve(CardModel* clickedCard) {
    for (CardModel* card : _model->getReserveCards()) {
        if (card->getId() == clickedCard->getId()) {
            return true;
        }
    }
    return false;
}

bool GameController::handleReserveCardClick(CardModel* clickedCard) {
    // 检查是否为手牌区顶部的牌
    if (_model->getReserveCards().back() != clickedCard) {
        CCLOG("Clicked card is not the top card in reserve pile");
        return false;
    }
    // 执行替换操作
    replaceTrayWithPlayFieldCard(clickedCard);
    return true;
}
bool GameController::isCardInPlayField(CardModel* clickedCard) {
    for (CardModel* card : _model->getPlayFieldCards()) {
        if (card->getId() == clickedCard->getId()) {
            return true;
        }
    }
    return false;
}

bool GameController::handlePlayFieldCardClick(CardModel* clickedCard) {
    // 检查卡牌有效性（遮挡 + 堆顶牌存在）
    if (!checkPlayFieldCardValidity(clickedCard)) {
        return false;
    }

    // 检查是否可以匹配
    CardModel* stackTopCard = _model->getCurrentStackTop();
    if (clickedCard->canMatchWith(stackTopCard)) {
        replaceTrayWithPlayFieldCard(clickedCard);
        return true;
    }
    else {
        CCLOG("Cards cannot match (point difference is not 1)");
        return false;
    }
}

bool GameController::checkPlayFieldCardValidity(CardModel* clickedCard) {
    // 获取牌堆顶的牌
    CardModel* stackTopCard = _model->getCurrentStackTop();
    if (!stackTopCard) {
        CCLOG("No card in stack to match");
        return false;
    }

    // 检查卡牌是否被遮挡
    if (isCardCovered(clickedCard)) {
        CCLOG("Clicked card is covered by other cards");
        return false;
    }

    return true;
}

bool GameController::isCardCovered(CardModel* card) {
    if (!card) return false;

    // 获取点击卡牌在视图层的对应对象
    CardView* clickedCardView = findCardViewById(card->getId());
    if (!clickedCardView) return false;

    // 获取所有场地牌的视图对象
    std::vector<CardView*> playFieldCardViews;
    for (CardModel* modelCard : _model->getPlayFieldCards()) {
        CardView* viewCard = findCardViewById(modelCard->getId());
        if (viewCard) {
            playFieldCardViews.push_back(viewCard);
        }
    }

    // 检查是否有其他卡牌遮挡了当前卡牌
    for (CardView* otherCardView : playFieldCardViews) {
        if (otherCardView == clickedCardView) continue; // 跳过自身

        // 判断otherCardView是否遮挡了clickedCardView
        if (isCardViewCovering(clickedCardView, otherCardView)) {
            return true;
        }
    }

    return false;
}

bool GameController::isCardViewCovering(CardView* target, CardView* other) {
    if (!target || !other) return false;

    // 获取两个卡牌的世界坐标矩形
    cocos2d::Rect targetRect = target->getBoundingBox();
    cocos2d::Rect otherRect = other->getBoundingBox();

    // 判断other是否部分或完全覆盖了target
    if (!targetRect.intersectsRect(otherRect)) {
        return false; // 不相交，不覆盖
    }

    // 获取共同的父节点
    auto parent = target->getParent();
    if (!parent || parent != other->getParent()) {
        return false; // 不在同一个父节点下，无法比较
    }

    // 获取两个卡牌在父节点中的索引
    ssize_t targetIndex = parent->getChildren().getIndex(target);
    ssize_t otherIndex = parent->getChildren().getIndex(other);

    CCLOG("target index: %zd, other index: %zd", targetIndex, otherIndex);

    // 如果other在父节点中的索引更大（后添加），则认为覆盖
    return otherIndex > targetIndex;
}

CardModel* GameController::findCardById(int cardId) {
    // 在手牌区查找
    for (CardModel* card : _model->getReserveCards()) {
        if (card->getId() == cardId) {
            return card;
        }
    }

    // 在牌堆区查找
    for (CardModel* card : _model->getStackCards()) {
        if (card->getId() == cardId) {
            return card;
        }
    }

    // 在场地查找
    for (CardModel* card : _model->getPlayFieldCards()) {
        if (card->getId() == cardId) {
            return card;
        }
    }

    return nullptr;
}

void GameController::replaceTrayWithPlayFieldCard(CardModel* cardToMove) {
    recordUndoAction(cardToMove, UndoActionType::MOVE_CARD, _currentUndoId);
    recordUndoAction(cardToMove, UndoActionType::CHANGE_VECTOR, _currentUndoId);

    // 找到对应的视图
    CardView* cardView = findCardViewById(cardToMove->getId());
    if (!cardView) {
        CCLOGERROR("View for card %d not found", cardToMove->getId());
        return;
    }

    // 记录原始ZOrder和父节点
    cardToMove->setOriginalZOrder(cardView->getLocalZOrder());
    Node* originalParent = cardView->getParent();

    // 获取目标卡牌（牌堆顶）的信息
    CardModel* currentStackTopCard = _model->getCurrentStackTop();
    if (!currentStackTopCard) {
        CCLOGERROR("No stack top card found");
        return;
    }
    
    // 获取牌堆区节点（目标父节点）x`
    Node* pileAreaNode = _view->getPileAreaNode();

    // 1. 如果卡牌当前不在pileAreaNode中，将其从原父节点移到pileAreaNode
    if (cardView->getParent() != pileAreaNode) {
        recordUndoAction(cardToMove, UndoActionType::CHANGE_PARENT, _currentUndoId);
        // 获取卡牌在全局坐标系中的位置
        Vec2 worldPos = cardView->getParent()->convertToWorldSpace(cardView->getPosition());
        
        // 从原父节点移除
        // 在移除前保留引用
        cardView->retain();
        cardView->removeFromParent();
        
        // 添加到pileAreaNode（使用全局坐标转换为pileAreaNode的局部坐标）
        pileAreaNode->addChild(cardView);
        cardView->setPosition(pileAreaNode->convertToNodeSpace(worldPos));
        // 在操作完成后释放引用
        cardView->release();
    }

    // 2. 获取目标位置（在pileAreaNode坐标系下）
    Vec2 targetPos = currentStackTopCard->getPosition();

    // 3. 更新卡牌模型的位置
    cardToMove->setPosition(targetPos);

    // 4. 获取目标ZOrder
    CardView* topStackCardView = findCardViewById(currentStackTopCard->getId());
    int targetZOrder = topStackCardView ? topStackCardView->getLocalZOrder() + 1 : 100;

    // 5. 执行动画
    auto moveAction = MoveTo::create(0.5f, targetPos);
    auto changeZOrderAction = CallFunc::create([cardView, targetZOrder]() {
        cardView->setLocalZOrder(targetZOrder);
    });
    auto sequence = Sequence::create(changeZOrderAction, moveAction, nullptr);
    cardView->runAction(sequence);

    // 更新模型数据
    _model->replaceTopStackCard(cardToMove);
    _currentUndoId++;
}

void GameController::handleUndoClick()
{
    int undoId = 0;
    if (!_undoManager.canUndo())
        return;
    undoId = _undoManager.getNextUndoId();
    while(_undoManager.canUndo() && undoId == _undoManager.getNextUndoId())
        _undoManager.undoLastAction(_model);
}



// 记录撤销操作
void GameController::recordUndoAction(CardModel* cardToMove, UndoActionType type, int undoId) {

    if (!cardToMove) return; // 确保卡牌有效

    UndoAction action;
    action.type = type;
    action.id = undoId;
    action.card = cardToMove;
    action.cardView = findCardViewById(cardToMove->getId());
    action.originalParent = action.cardView->getParent();
    action.originalPosition = cardToMove->getPosition();
    action.id = cardToMove->getId();
    Node* pileAreaNode = _view->getPileAreaNode();
    Node* mainAreaNode = _view->getMainAreaNode();

    CardView* cardView = findCardViewById(cardToMove->getId());
    if (cardView->getParent() == pileAreaNode) {
        action.field = CardField::RESERVE;
    }
    if (cardView->getParent() == mainAreaNode) {
        action.field = CardField::PLAY_FIELD;
    }
    _undoManager.recordAction(action);
}

// 查找卡牌视图
CardView* GameController::findCardViewById(int cardId) {
    for (CardController* ctrl : _cardControllers) {
        if (ctrl->getModel()->getId() == cardId) {
            return ctrl->getView();
        }
    }
    return nullptr;
}

bool GameController::loadLevel(int levelId) {
    CCLOG("Loading level %d", levelId);

    // 构建关卡配置文件路径

    LevelConfig levelConfig;
    if (!LevelConfigLoader::loadLevelConfigFromResource(levelId, levelConfig)) {
        CCLOGERROR("Failed to load level config %i", levelId);
        return false;
    }

    // 使用关卡配置初始化游戏模型
    if (!_model->initWithLevelConfig(levelConfig)) {
        CCLOGERROR("Failed to initialize game model with level config");
        return false;
    }
    generateCardsFromModel();
    return true;
}

void GameController::generateCardsFromModel() {
    for (CardController* ctrl : _cardControllers) {
        delete ctrl;
    }
    _cardControllers.clear();
    for (CardModel* modelCard : _model->getPlayFieldCards()) {
        CardView* viewCard = CardView::create(modelCard, CardResConfig::getBackGroundPath());
        if (!viewCard) continue;

        CardController* ctrl = new CardController(modelCard, viewCard);
        _cardControllers.emplace_back(ctrl);
        ctrl->setPosition(modelCard->getPosition());
        _view->getMainAreaNode()->addChild(viewCard);
    }
    for (CardModel* modelCard : _model->getStackCards()) {
        cocos2d::Vec2 position = modelCard->getPosition();
        position.x += 700;
        position.y += 300;
        modelCard->setPosition(position);
        CardView* viewCard = CardView::create(modelCard, CardResConfig::getBackGroundPath());
        if (!viewCard) continue;
        CardController* ctrl = new CardController(modelCard, viewCard);
        _cardControllers.emplace_back(ctrl);
        ctrl->setPosition(modelCard->getPosition());
        _view->getPileAreaNode()->addChild(viewCard);
    }

    float xOffset = 300; // 起始X偏移量
    float yOffset = 300; // 起始X偏移量
    const float CARD_SPACING = 130; // 卡牌间距
    for (CardModel* modelCard : _model->getReserveCards()) {
        cocos2d::Vec2 position = modelCard->getPosition();
        position.x += xOffset;
        position.y += yOffset;
        modelCard->setPosition(position);
        CardView* viewCard = CardView::create(modelCard, CardResConfig::getBackGroundPath());
        if (!viewCard) continue;
        CardController* ctrl = new CardController(modelCard, viewCard);
        _cardControllers.emplace_back(ctrl);
        ctrl->setPosition(modelCard->getPosition());
        xOffset += CARD_SPACING;
        _view->getPileAreaNode()->addChild(viewCard);
    }

}