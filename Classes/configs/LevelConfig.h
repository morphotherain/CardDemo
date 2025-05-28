#ifndef LEVEL_CONFIG_H
#define LEVEL_CONFIG_H

#include <vector>
#include "cocos2d.h"

/**
 * @brief 游戏关卡配置类
 * 负责存储和解析游戏关卡中扑克牌的布局信息
 * 使用场景：在关卡加载时从JSON配置生成游戏初始布局
 */
class LevelConfig {
public:
    /**
     * @brief 扑克牌布局信息结构体
     * 存储单张扑克牌的属性和位置信息
     */
    struct CardInfo {
        int cardFace;         ///< 牌面点数(1-13)
        int cardSuit;         ///< 牌面花色(0-3)
        cocos2d::Vec2 position; ///< 卡牌在游戏中的位置坐标
    };

    std::vector<CardInfo> playfieldCards; ///< 游戏区域扑克牌布局信息
    std::vector<CardInfo> stackCards;     ///< 牌堆区域扑克牌布局信息

    /**
     * @brief 从JSON字符串解析关卡配置
     * @param jsonStr JSON格式的关卡配置字符串
     * @return 解析成功返回true，失败返回false
     */
    bool parseFromJson(const std::string& jsonStr);
};

#endif // LEVEL_CONFIG_H