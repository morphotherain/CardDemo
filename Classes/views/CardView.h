#ifndef CARD_VIEW_H
#define CARD_VIEW_H

#include "cocos2d.h"
#include "../models/CardModel.h"

/**
 * @brief 扑克牌视图类
 * 继承自cocos2d::Sprite，负责扑克牌的视觉渲染
 * 使用场景：在游戏界面中显示扑克牌的正面、背面及各类状态图标
 */
class CardView : public cocos2d::Sprite {
private:
    static const float kCardWidth;  ///< 卡牌宽度（固定值，单位：像素）
    static const float kCardHeight; ///< 卡牌高度（固定值，单位：像素）

    CardModel* model;               ///< 关联的扑克牌数据模型
    std::string cardBgPath;         ///< 卡牌背景图片路径（背面或正面底图）

    /**
     * @brief 渲染卡牌背景（背面或正面底图）
     */
    void renderCardBackground();

    /**
     * @brief 渲染左上角小数字（用于显示点数）
     */
    void renderTopLeftNumber();

    /**
     * @brief 渲染中间大数字（用于显示主要点数）
     */
    void renderCenterNumber();

    /**
     * @brief 渲染花色图标（梅花/方块/红桃/黑桃）
     */
    void renderSuitIcon();


public:
    /**
     * @brief 创建卡牌视图实例的工厂方法
     * @param model 关联的CardModel数据模型
     * @param bgPath 卡牌背景图片路径（如背面图片路径）
     * @return 创建成功返回CardView指针，失败返回nullptr
     */
    static CardView* create(CardModel* model, const std::string& bgPath);

    /**
     * @brief 获取关联的扑克牌数据模型
     * @return CardModel指针
     */
    CardModel* getCardModel() { return model; }

    /**
     * @brief 初始化方法（继承自cocos2d::Sprite）
     * @return 初始化成功返回true，失败返回false
     */
    virtual bool init() override;

};

#endif // CARD_VIEW_H