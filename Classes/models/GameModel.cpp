#include "GameModel.h"

GameModel::GameModel():
    _currentStackTop(nullptr)
    , _playFieldCards()
    , _stackCards()
    , _reserveCards() {
}

GameModel::~GameModel() {
    for (CardModel* card : _playFieldCards) delete card;
    for (CardModel* card : _stackCards) delete card;
    for (CardModel* card : _reserveCards) delete card;
}


bool GameModel::initWithLevelConfig(const LevelConfig& config) {
    // �����������
    clearAllCards();

    int cardId = 0;
    // ���س��ؿ��ƣ�Playfield��
    for (const auto& cardInfo : config.playfieldCards) {
        // �������Ʋ����û�������
        CardModel* card = new CardModel(
            static_cast<CardSuit>(cardInfo.cardSuit),  // ��ɫö��ת��
            static_cast<CardFace>(cardInfo.cardFace)    // ����ö��ת��
        );
        card->setId(cardId++);
        card->setPosition(cocos2d::Vec2(cardInfo.position.x, cardInfo.position.y));

        card->flipCard();
        _playFieldCards.emplace_back(card);
    }

    // �����ƶѿ��ƣ�Stack��
    for (const auto& cardInfo : config.stackCards) {
        CardModel* card = new CardModel(
            static_cast<CardSuit>(cardInfo.cardSuit),
            static_cast<CardFace>(cardInfo.cardFace)
        );
        card->setId(cardId++);
        card->setPosition(cocos2d::Vec2(cardInfo.position.x, cardInfo.position.y));

        card->setMatched(false);
        _reserveCards.emplace_back(card);
    }

    // ���ƶѶ����ƴ�_reserveCards�ƶ���_stackCards
    moveTopCardToStack();

    return true;
}

void GameModel::clearAllCards() {
    for (auto* card : _playFieldCards) delete card;
    for (auto* card : _stackCards) delete card;
    for (auto* card : _reserveCards) delete card;
    _playFieldCards.clear();
    _stackCards.clear();
    _reserveCards.clear();
    _currentStackTop = nullptr;
}

void GameModel::moveTopCardToStack()
{
    if (_reserveCards.empty()) {
        return; // û���ƿ��ƶ�
    }

    // ��ȡ�ƶѶ�����
    CardModel* topCard = _reserveCards.back();

    // ��_reserveCards�Ƴ�
    _reserveCards.pop_back();

    // ��ӵ�_stackCards
    _stackCards.emplace_back(topCard);

    // �����ƶѶ�
    if (!_reserveCards.empty()) {
        _currentStackTop = topCard;
    }
    else {
        _currentStackTop = nullptr; // û������
    }
}


// ��ָ�������滻�ƶѶ�����
void GameModel::replaceTopStackCard(CardModel* card) {
    // ��鿨���Ƿ���������
    auto it = std::find(_reserveCards.begin(), _reserveCards.end(), card);
    if (it != _reserveCards.end()) {
        // ���������Ƴ�
        _reserveCards.erase(it);
    }
    it = std::find(_playFieldCards.begin(), _playFieldCards.end(), card);
    if (it != _playFieldCards.end()) {
        // ���������Ƴ�
        _playFieldCards.erase(it);
    }

    // ��ӵ��ƶ���
    _stackCards.push_back(card);

    // �����ƶѶ�ָ��
    if (!_stackCards.empty()) {
        _currentStackTop = _stackCards.back();
    }
    else {
        _currentStackTop = nullptr;
    }
}

void GameModel::undoReplaceTopStackCard(CardModel* card, CardField field)
{
    auto it = std::find(_stackCards.begin(), _stackCards.end(), card);
    if (it != _stackCards.end()) {
        // ���������Ƴ�
        _stackCards.erase(it);
    }

    if (field == CardField::PLAY_FIELD) {
        _playFieldCards.push_back(card);
    }
    if (field == CardField::RESERVE) {
        _reserveCards.push_back(card);
    }

    // �����ƶѶ�ָ��
    if (!_stackCards.empty()) {
        _currentStackTop = _stackCards.back();
    }
    else {
        _currentStackTop = nullptr;
    }
}
