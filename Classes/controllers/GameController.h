#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../views/GameView.h"
#include "../configs/LevelConfig.h"
#include "../configs/LevelConfigLoader.h"
#include "../controllers/CardController.h"
#include "../managers/UndoManager.h"

/**
 * @brief 游戏主控制器
 * 继承自cocos2d::Scene，负责协调游戏模型(GameModel)、视图(GameView)和关卡逻辑
 * 使用场景：处理游戏流程控制、卡牌交互、撤销操作管理及关卡加载
 */
class GameController : public cocos2d::Scene {
private:
    GameModel* _model;          ///< 游戏数据模型
    GameView* _view;            ///< 游戏主视图
    std::vector<CardController*> _cardControllers; ///< 卡牌控制器集合
    UndoManager _undoManager;   ///< 撤销操作管理器
    int _currentUndoId = 0;     ///< 当前撤销操作ID计数器

public:
    /**
     * @brief 创建游戏控制器实例的静态方法
     * @return GameController指针，创建失败返回nullptr
     */
    static GameController* createInstance();

    /**
     * @brief 初始化方法（继承自cocos2d::Scene）
     * @return 初始化成功返回true，失败返回false
     */
    bool init();

    /**
     * @brief 处理卡牌点击事件
     * @param cardId 被点击卡牌的ID
     */
    void handleCardClick(int cardId);

    /**
     * @brief 处理撤销操作点击事件
     */
    void handleUndoClick();

    /**
     * @brief 判断卡牌是否被覆盖
     * @param card 目标卡牌模型
     * @return 已覆盖返回true，否则返回false
     */
    bool isCardCovered(CardModel* card);

    /**
     * @brief 判断视图是否被其他卡牌覆盖
     * @param target 目标视图
     * @param other 对比视图
     * @return 存在覆盖返回true，否则返回false
     */
    bool isCardViewCovering(CardView* target, CardView* other);

    /**
     * @brief 根据ID查找卡牌模型
     * @param cardId 卡牌ID
     * @return 找到返回CardModel指针，否则返回nullptr
     */
    CardModel* findCardById(int cardId);

    /**
     * @brief 将卡牌从托盘中移动到游戏区域
     * @param cardToMove 待移动的卡牌模型
     */
    void replaceTrayWithPlayFieldCard(CardModel* cardToMove);

    /**
     * @brief 记录撤销操作
     * @param cardToMove 操作涉及的卡牌模型
     * @param type 操作类型（枚举值）
     * @param undoId 撤销操作ID
     */
    void recordUndoAction(CardModel* cardToMove, UndoActionType type, int undoId);

    /**
     * @brief 根据ID查找卡牌视图
     * @param cardId 卡牌ID
     * @return 找到返回CardView指针，否则返回nullptr
     */
    CardView* findCardViewById(int cardId);

    /**
     * @brief 判断卡牌是否在储备区
     * @param clickedCard 点击的卡牌模型
     * @return 在储备区返回true，否则返回false
     */
    bool isCardInReserve(CardModel* clickedCard);

    /**
     * @brief 处理储备区卡牌点击事件
     * @param clickedCard 点击的卡牌模型
     * @return 处理成功返回true，否则返回false
     */
    bool handleReserveCardClick(CardModel* clickedCard);

    /**
     * @brief 判断卡牌是否在游戏区域
     * @param clickedCard 点击的卡牌模型
     * @return 在游戏区域返回true，否则返回false
     */
    bool isCardInPlayField(CardModel* clickedCard);

    /**
     * @brief 处理游戏区域卡牌点击事件
     * @param clickedCard 点击的卡牌模型
     * @return 处理成功返回true，否则返回false
     */
    bool handlePlayFieldCardClick(CardModel* clickedCard);

    /**
     * @brief 检查游戏区域卡牌的有效性
     * @param clickedCard 点击的卡牌模型
     * @return 有效返回true，否则返回false
     */
    bool checkPlayFieldCardValidity(CardModel* clickedCard);

    /**
     * @brief 加载指定关卡
     * @param levelId 关卡ID（如1表示第一关）
     * @return 加载成功返回true，否则返回false
     */
    bool loadLevel(int levelId);

    CREATE_FUNC(GameController);

    /**
     * @brief 根据模型数据生成卡牌视图及控制器
     */
    void generateCardsFromModel();
};

#endif // GAME_CONTROLLER_H