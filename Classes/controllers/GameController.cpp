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

    // Ĭ�ϼ��ص�һ��
    if (!loadLevel(1)) {
        CCLOGERROR("Failed to load level 1");
        return false;
    }

    this->addChild(_view);
    return true;
}

void GameController::handleCardClick(int cardId) {
    CCLOG("Handling card click: %d", cardId);

    // ���ҵ���Ŀ���
    CardModel* clickedCard = findCardById(cardId);
    if (!clickedCard) {
        CCLOGERROR("Card with id %d not found", cardId);
        return;
    }

    // �ַ�����ͬ������
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
    // ����Ƿ�Ϊ��������������
    if (_model->getReserveCards().back() != clickedCard) {
        CCLOG("Clicked card is not the top card in reserve pile");
        return false;
    }
    // ִ���滻����
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
    // ��鿨����Ч�ԣ��ڵ� + �Ѷ��ƴ��ڣ�
    if (!checkPlayFieldCardValidity(clickedCard)) {
        return false;
    }

    // ����Ƿ����ƥ��
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
    // ��ȡ�ƶѶ�����
    CardModel* stackTopCard = _model->getCurrentStackTop();
    if (!stackTopCard) {
        CCLOG("No card in stack to match");
        return false;
    }

    // ��鿨���Ƿ��ڵ�
    if (isCardCovered(clickedCard)) {
        CCLOG("Clicked card is covered by other cards");
        return false;
    }

    return true;
}

bool GameController::isCardCovered(CardModel* card) {
    if (!card) return false;

    // ��ȡ�����������ͼ��Ķ�Ӧ����
    CardView* clickedCardView = findCardViewById(card->getId());
    if (!clickedCardView) return false;

    // ��ȡ���г����Ƶ���ͼ����
    std::vector<CardView*> playFieldCardViews;
    for (CardModel* modelCard : _model->getPlayFieldCards()) {
        CardView* viewCard = findCardViewById(modelCard->getId());
        if (viewCard) {
            playFieldCardViews.push_back(viewCard);
        }
    }

    // ����Ƿ������������ڵ��˵�ǰ����
    for (CardView* otherCardView : playFieldCardViews) {
        if (otherCardView == clickedCardView) continue; // ��������

        // �ж�otherCardView�Ƿ��ڵ���clickedCardView
        if (isCardViewCovering(clickedCardView, otherCardView)) {
            return true;
        }
    }

    return false;
}

bool GameController::isCardViewCovering(CardView* target, CardView* other) {
    if (!target || !other) return false;

    // ��ȡ�������Ƶ������������
    cocos2d::Rect targetRect = target->getBoundingBox();
    cocos2d::Rect otherRect = other->getBoundingBox();

    // �ж�other�Ƿ񲿷ֻ���ȫ������target
    if (!targetRect.intersectsRect(otherRect)) {
        return false; // ���ཻ��������
    }

    // ��ȡ��ͬ�ĸ��ڵ�
    auto parent = target->getParent();
    if (!parent || parent != other->getParent()) {
        return false; // ����ͬһ�����ڵ��£��޷��Ƚ�
    }

    // ��ȡ���������ڸ��ڵ��е�����
    ssize_t targetIndex = parent->getChildren().getIndex(target);
    ssize_t otherIndex = parent->getChildren().getIndex(other);

    CCLOG("target index: %zd, other index: %zd", targetIndex, otherIndex);

    // ���other�ڸ��ڵ��е��������󣨺���ӣ�������Ϊ����
    return otherIndex > targetIndex;
}

CardModel* GameController::findCardById(int cardId) {
    // ������������
    for (CardModel* card : _model->getReserveCards()) {
        if (card->getId() == cardId) {
            return card;
        }
    }

    // ���ƶ�������
    for (CardModel* card : _model->getStackCards()) {
        if (card->getId() == cardId) {
            return card;
        }
    }

    // �ڳ��ز���
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

    // �ҵ���Ӧ����ͼ
    CardView* cardView = findCardViewById(cardToMove->getId());
    if (!cardView) {
        CCLOGERROR("View for card %d not found", cardToMove->getId());
        return;
    }

    // ��¼ԭʼZOrder�͸��ڵ�
    cardToMove->setOriginalZOrder(cardView->getLocalZOrder());
    Node* originalParent = cardView->getParent();

    // ��ȡĿ�꿨�ƣ��ƶѶ�������Ϣ
    CardModel* currentStackTopCard = _model->getCurrentStackTop();
    if (!currentStackTopCard) {
        CCLOGERROR("No stack top card found");
        return;
    }
    
    // ��ȡ�ƶ����ڵ㣨Ŀ�길�ڵ㣩x`
    Node* pileAreaNode = _view->getPileAreaNode();

    // 1. ������Ƶ�ǰ����pileAreaNode�У������ԭ���ڵ��Ƶ�pileAreaNode
    if (cardView->getParent() != pileAreaNode) {
        recordUndoAction(cardToMove, UndoActionType::CHANGE_PARENT, _currentUndoId);
        // ��ȡ������ȫ������ϵ�е�λ��
        Vec2 worldPos = cardView->getParent()->convertToWorldSpace(cardView->getPosition());
        
        // ��ԭ���ڵ��Ƴ�
        // ���Ƴ�ǰ��������
        cardView->retain();
        cardView->removeFromParent();
        
        // ��ӵ�pileAreaNode��ʹ��ȫ������ת��ΪpileAreaNode�ľֲ����꣩
        pileAreaNode->addChild(cardView);
        cardView->setPosition(pileAreaNode->convertToNodeSpace(worldPos));
        // �ڲ�����ɺ��ͷ�����
        cardView->release();
    }

    // 2. ��ȡĿ��λ�ã���pileAreaNode����ϵ�£�
    Vec2 targetPos = currentStackTopCard->getPosition();

    // 3. ���¿���ģ�͵�λ��
    cardToMove->setPosition(targetPos);

    // 4. ��ȡĿ��ZOrder
    CardView* topStackCardView = findCardViewById(currentStackTopCard->getId());
    int targetZOrder = topStackCardView ? topStackCardView->getLocalZOrder() + 1 : 100;

    // 5. ִ�ж���
    auto moveAction = MoveTo::create(0.5f, targetPos);
    auto changeZOrderAction = CallFunc::create([cardView, targetZOrder]() {
        cardView->setLocalZOrder(targetZOrder);
    });
    auto sequence = Sequence::create(changeZOrderAction, moveAction, nullptr);
    cardView->runAction(sequence);

    // ����ģ������
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



// ��¼��������
void GameController::recordUndoAction(CardModel* cardToMove, UndoActionType type, int undoId) {

    if (!cardToMove) return; // ȷ��������Ч

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

// ���ҿ�����ͼ
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

    // �����ؿ������ļ�·��

    LevelConfig levelConfig;
    if (!LevelConfigLoader::loadLevelConfigFromResource(levelId, levelConfig)) {
        CCLOGERROR("Failed to load level config %i", levelId);
        return false;
    }

    // ʹ�ùؿ����ó�ʼ����Ϸģ��
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

    float xOffset = 300; // ��ʼXƫ����
    float yOffset = 300; // ��ʼXƫ����
    const float CARD_SPACING = 130; // ���Ƽ��
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