#ifndef CARD_CONTROLLER_H
#define CARD_CONTROLLER_H

USING_NS_CC;

#include "cocos2d.h"
#include "../models/CardModel.h"
#include "../views/CardView.h"

/**
 * @brief 扑克牌控制器
 * 负责协调CardModel和CardView之间的数据同步和交互逻辑
 * 使用场景：处理扑克牌的移动、状态变化和匹配逻辑
 */
class CardController {
private:
    CardModel* _model;  ///< 扑克牌数据模型
    CardView* _view;    ///< 扑克牌视图表现

public:
    /**
     * @brief 构造函数
     * @param model 关联的CardModel实例
     * @param view 关联的CardView实例
     */
    CardController(CardModel* model, CardView* view)
        : _model(model), _view(view) {}

    /**
     * @brief 获取扑克牌模型
     * @return CardModel指针
     */
    CardModel* getModel() const { return _model; }

    /**
     * @brief 获取扑克牌视图
     * @return CardView指针
     */
    CardView* getView() const { return _view; }

    /**
     * @brief 同步设置位置
     * @param pos 新位置坐标
     */
    void setPosition(const cocos2d::Vec2& pos) {
        _model->setPosition(pos);
        _view->setPosition(pos);
    }

    /**
     * @brief 设置匹配状态
     * @param matched 是否匹配成功
     */
    void setMatched(bool matched) {
        _model->setMatched(matched);
        // 可添加视图匹配效果逻辑
    }

    /**
     * @brief 设置激活状态
     * @param active 是否激活
     */
    void setActive(bool active) {
        _model->setActive(active);
        // 可添加视图禁用效果逻辑
    }

    /**
     * @brief 判断两张牌是否可以匹配
     * @param other 另一张牌的控制器
     * @return 可以匹配返回true，否则返回false
     */
    bool canMatchWith(const CardController* other) const {
        return _model->canMatchWith(other->getModel());
    }

    /**
     * @brief 根据模型状态更新视图
     */
    void updateView() {
    }
};

#endif // CARD_CONTROLLER_H