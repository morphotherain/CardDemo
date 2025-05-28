#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "cocos2d.h"
#include "../models/GameModel.h"

/**
 * @brief 游戏主视图类
 * 继承自cocos2d::Layer，负责游戏界面的搭建和用户交互处理
 * 使用场景：渲染游戏区域、牌堆区域、撤销按钮等界面元素，并处理触摸事件
 */
class GameView : public cocos2d::Layer {
private:
    GameModel* model;                     ///< 关联的游戏数据模型
    cocos2d::Node* mainAreaNode;          ///< 游戏主区域节点（存放可交互卡牌）
    cocos2d::Node* pileAreaNode;          ///< 牌堆区域节点（存放牌堆卡牌）
    std::function<void(int)> _onCardClickCallback; ///< 卡牌点击事件回调函数（参数为卡牌ID）
    std::function<void()> _onUndoClickCallback;     ///< 撤销按钮点击事件回调函数
    cocos2d::Label* _undoLabel;           ///< 撤销操作显示标签

public:
    /**
     * @brief 创建游戏视图实例的工厂方法
     * @param model 关联的GameModel数据模型
     * @return 创建成功返回GameView指针，失败返回nullptr
     */
    static GameView* create(GameModel* model);

    /**
     * @brief 初始化方法（继承自cocos2d::Layer）
     * @return 初始化成功返回true，失败返回false
     */
    virtual bool init() override;

    /**
     * @brief 使用数据模型初始化视图
     * @param model 游戏数据模型
     * @return 初始化成功返回true，失败返回false
     */
    bool initWithModel(GameModel* model);

    /**
     * @brief 创建游戏界面区域（主区域和牌堆区域）
     */
    void createGameAreas();

    /**
     * @brief 创建撤销操作标签及交互按钮
     */
    void createUndoLabel();

    /**
     * @brief 获取游戏主区域节点
     * @return 主区域节点指针
     */
    cocos2d::Node* getMainAreaNode() const { return mainAreaNode; }

    /**
     * @brief 获取牌堆区域节点
     * @return 牌堆区域节点指针
     */
    cocos2d::Node* getPileAreaNode() const { return pileAreaNode; }

    /**
     * @brief 设置卡牌点击事件回调函数
     * @param callback 回调函数（参数为被点击卡牌的ID）
     */
    void setOnCardClickCallback(const std::function<void(int)>& callback);

    /**
     * @brief 设置撤销按钮点击事件回调函数
     * @param callback 回调函数
     */
    void setOnUndoClickCallback(const std::function<void()>& callback);

    /**
     * @brief 初始化触摸事件处理
     */
    void setupTouchHandling();
};

#endif // GAME_VIEW_H