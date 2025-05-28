#pragma once
#include <vector>
#include "../models/UndoModel.h"

/**
 * @brief 撤销操作管理器
 * 负责记录、管理和执行游戏中的撤销操作
 * 使用场景：处理卡牌移动、状态变更等可撤销操作的历史记录
 */
class UndoManager {
public:
    /**
     * @brief 记录一个可撤销操作
     * @param action 要记录的撤销操作对象（包含操作类型、卡牌信息等）
     */
    void recordAction(const UndoAction& action);

    /**
     * @brief 判断是否存在可撤销的操作
     * @return 存在可撤销操作返回true，否则返回false
     */
    bool canUndo() const;

    /**
     * @brief 获取下一个唯一的撤销操作ID
     * @return 递增的整数ID（从1开始）
     */
    int getNextUndoId();

    /**
     * @brief 撤销最后一次记录的操作
     * @param model 游戏数据模型，用于执行撤销逻辑
     */
    void undoLastAction(GameModel* model);

    /**
     * @brief 清空所有撤销操作历史记录
     */
    void clearHistory();

private:
    std::vector<UndoAction> _undoStack; ///< 撤销操作栈（后进先出）
};