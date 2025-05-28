#pragma once
#include "cocos2d.h"
#include "CardModel.h"
#include "GameModel.h"

USING_NS_CC;
class CardView;

/**
 * @brief 撤销操作类型枚举
 * 定义可撤销操作的类型，用于标识不同的状态变更
 */
enum class UndoActionType {
    MOVE_CARD,        ///< 卡牌移动操作
    CHANGE_PARENT,    ///< 父节点变更操作
    CHANGE_VECTOR     ///< 容器列表变更操作
};

/**
 * @brief 撤销操作结构体
 * 存储执行撤销操作所需的完整信息，包括操作类型、卡牌状态和历史数据
 */
struct UndoAction {
    UndoActionType type;        ///< 操作类型（枚举值）
    CardModel* card;            ///< 操作涉及的卡牌模型
    CardView* cardView;         ///< 操作涉及的卡牌视图（可选）
    Node* originalParent;       ///< 卡牌原始父节点（用于恢复层级关系）
    Vec2 originalPosition;      ///< 卡牌原始位置坐标（用于移动撤销）
    int id;                     ///< 操作唯一标识（关联撤销栈ID）
    CardField field;            ///< 卡牌原始所属区域（PLAY_FIELD/STACK/RESERVE）
};