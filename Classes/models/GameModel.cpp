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
    // 清空现有数据
    clearAllCards();

    int cardId = 0;
    // 加载场地卡牌（Playfield）
    for (const auto& cardInfo : config.playfieldCards) {
        // 创建卡牌并设置基础属性
        CardModel* card = new CardModel(
            static_cast<CardSuit>(cardInfo.cardSuit),  // 花色枚举转换
            static_cast<CardFace>(cardInfo.cardFace)    // 点数枚举转换
        );
        card->setId(cardId++);
        card->setPosition(cocos2d::Vec2(cardInfo.position.x, cardInfo.position.y));

        card->flipCard();
        _playFieldCards.emplace_back(card);
    }

    // 加载牌堆卡牌（Stack）
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

    // 将牌堆顶的牌从_reserveCards移动到_stackCards
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
        return; // 没有牌可移动
    }

    // 获取牌堆顶的牌
    CardModel* topCard = _reserveCards.back();

    // 从_reserveCards移除
    _reserveCards.pop_back();

    // 添加到_stackCards
    _stackCards.emplace_back(topCard);

    // 更新牌堆顶
    if (!_reserveCards.empty()) {
        _currentStackTop = topCard;
    }
    else {
        _currentStackTop = nullptr; // 没有牌了
    }
}


// 用指定的牌替换牌堆顶的牌
void GameModel::replaceTopStackCard(CardModel* card) {
    // 检查卡牌是否在手牌区
    auto it = std::find(_reserveCards.begin(), _reserveCards.end(), card);
    if (it != _reserveCards.end()) {
        // 从手牌区移除
        _reserveCards.erase(it);
    }
    it = std::find(_playFieldCards.begin(), _playFieldCards.end(), card);
    if (it != _playFieldCards.end()) {
        // 从手牌区移除
        _playFieldCards.erase(it);
    }

    // 添加到牌堆区
    _stackCards.push_back(card);

    // 更新牌堆顶指针
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
        // 从手牌区移除
        _stackCards.erase(it);
    }

    if (field == CardField::PLAY_FIELD) {
        _playFieldCards.push_back(card);
    }
    if (field == CardField::RESERVE) {
        _reserveCards.push_back(card);
    }

    // 更新牌堆顶指针
    if (!_stackCards.empty()) {
        _currentStackTop = _stackCards.back();
    }
    else {
        _currentStackTop = nullptr;
    }
}
