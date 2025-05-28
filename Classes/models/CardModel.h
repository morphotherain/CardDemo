#pragma once
#ifndef CARD_MODEL_H
#define CARD_MODEL_H

#include "cocos2d.h"
#include "../configs/CardResConfig.h"

USING_NS_CC;

/**
 * @brief 扑克牌数据模型
 * 存储单张扑克牌的属性（花色、点数、位置、状态等）
 * 使用场景：在游戏逻辑中表示和操作扑克牌的状态
 */
class CardModel {
private:
    int _cardId;              ///< 卡牌唯一标识符
    CardSuit _suit;           ///< 卡牌花色（梅花、方块、红桃、黑桃）
    CardFace _face;           ///< 卡牌点数（A、2-10、J、Q、K）
    bool _isFlipped;          ///< 卡牌是否翻开（正面朝上）
    bool _isMatched;          ///< 卡牌是否已匹配成功
    Vec2 _position;           ///< 卡牌在游戏场景中的位置坐标
    bool _isActive;           ///< 卡牌是否可用（可交互）
    int _originalZOrder;      ///< 卡牌原始Z轴顺序（渲染层级）

public:
    /**
     * @brief 构造函数
     * @param s 卡牌花色
     * @param f 卡牌点数
     */
    CardModel(CardSuit s, CardFace f);

    // Getter 方法
    /**
     * @brief 获取卡牌唯一ID
     * @return 卡牌ID
     */
    int getId() const { return _cardId; }

    /**
     * @brief 获取卡牌花色
     * @return 卡牌花色枚举值
     */
    CardSuit getSuit() const { return _suit; }

    /**
     * @brief 获取卡牌点数
     * @return 卡牌点数枚举值
     */
    CardFace getFace() const { return _face; }

    /**
     * @brief 判断卡牌是否已翻开
     * @return 已翻开返回true，否则返回false
     */
    bool isCardFlipped() const { return _isFlipped; }

    /**
     * @brief 判断卡牌是否已匹配
     * @return 已匹配返回true，否则返回false
     */
    bool isCardMatched() const { return _isMatched; }

    /**
     * @brief 获取卡牌位置
     * @return 卡牌位置坐标
     */
    Vec2 getPosition() const { return _position; }

    /**
     * @brief 判断卡牌是否可用
     * @return 可用返回true，否则返回false
     */
    bool isCardActive() const { return _isActive; }

    /**
     * @brief 获取卡牌原始Z轴顺序
     * @return Z轴顺序值
     */
    int getOriginalZOrder() const { return _originalZOrder; }

    // Setter 方法
    /**
     * @brief 设置卡牌唯一ID
     * @param id 要设置的ID值
     */
    void setId(int id) { _cardId = id; }

    /**
     * @brief 翻转卡牌状态（翻开/关闭）
     */
    void flipCard() { _isFlipped = !_isFlipped; }

    /**
     * @brief 设置卡牌匹配状态
     * @param matched 匹配状态值
     */
    void setMatched(bool matched) { _isMatched = matched; }

    /**
     * @brief 设置卡牌位置
     * @param pos 新位置坐标
     */
    void setPosition(const Vec2& pos) { _position = pos; }

    /**
     * @brief 设置卡牌可用状态
     * @param active 可用状态值
     */
    void setActive(bool active) { _isActive = active; }

    /**
     * @brief 设置卡牌原始Z轴顺序
     * @param zOrder Z轴顺序值
     */
    void setOriginalZOrder(int zOrder) { _originalZOrder = zOrder; }

    /**
     * @brief 判断两张牌是否可以匹配
     * 匹配规则：两张牌点数相差为1（如A和2、10和J）
     * @param other 另一张卡牌模型
     * @return 可以匹配返回true，否则返回false
     */
    bool canMatchWith(const CardModel* other) const {
        if (!other || other->_isMatched) return false;
        return abs(int(_face) - int(other->_face)) == 1;
    }
};

#endif // CARD_MODEL_H