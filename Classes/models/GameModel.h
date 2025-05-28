#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include <vector>
#include "CardModel.h"
#include "../configs/LevelConfig.h"

/**
 * @brief 游戏数据模型类
 * 管理游戏中所有扑克牌的分组（游戏区域、牌堆、储备区）及状态
 * 使用场景：存储关卡初始化数据、处理卡牌移动逻辑、提供数据查询接口
 */
enum CardField {
    PLAY_FIELD = 0,  ///< 游戏区域
    STACK,           ///< 牌堆区域
    RESERVE          ///< 储备区
};

class GameModel {
private:
    std::vector<CardModel*> _playFieldCards; ///< 游戏区域卡牌列表
    std::vector<CardModel*> _stackCards;     ///< 牌堆区域卡牌列表（后进先出）
    std::vector<CardModel*> _reserveCards;   ///< 储备区卡牌列表（初始布局卡牌）
    CardModel* _currentStackTop;             ///< 当前牌堆顶部卡牌（缓存值）

public:
    /**
     * @brief 构造函数
     * 初始化空的游戏模型
     */
    GameModel();

    /**
     * @brief 析构函数
     * 清理所有卡牌资源
     */
    ~GameModel();

    /**
     * @brief 使用关卡配置初始化游戏模型
     * @param config 关卡配置数据（包含各区域卡牌布局）
     * @return 初始化成功返回true，失败返回false
     */
    bool initWithLevelConfig(const LevelConfig& config);

    // Getter 方法
    /**
     * @brief 获取游戏区域卡牌列表
     * @return 游戏区域卡牌列表（只读）
     */
    const std::vector<CardModel*>& getPlayFieldCards() const { return _playFieldCards; }

    /**
     * @brief 获取牌堆区域卡牌列表
     * @return 牌堆区域卡牌列表（只读）
     */
    const std::vector<CardModel*>& getStackCards() const { return _stackCards; }

    /**
     * @brief 获取储备区卡牌列表
     * @return 储备区卡牌列表（只读）
     */
    const std::vector<CardModel*>& getReserveCards() const { return _reserveCards; }

    /**
     * @brief 获取当前牌堆顶部卡牌
     * @return 牌堆顶部卡牌指针，牌堆为空时返回nullptr
     */
    CardModel* getCurrentStackTop() const { return _currentStackTop; }

    /**
     * @brief 清空所有区域的卡牌
     */
    void clearAllCards();

    /**
     * @brief 将储备区顶部卡牌移动到牌堆
     * （适用于回合制卡牌移动逻辑）
     */
    void moveTopCardToStack();

    /**
     * @brief 替换牌堆顶部卡牌
     * @param card 用于替换的卡牌模型
     */
    void replaceTopStackCard(CardModel* card);

    /**
     * @brief 撤销替换牌堆顶部卡牌操作
     * @param card 被替换的原卡牌模型
     * @param field 原卡牌所属区域
     */
    void undoReplaceTopStackCard(CardModel* card, CardField field);
};

#endif // GAME_MODEL_H