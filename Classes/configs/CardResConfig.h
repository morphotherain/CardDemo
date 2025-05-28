#ifndef CARD_RESOURCE_CONFIG_H
#define CARD_RESOURCE_CONFIG_H

#include <string>

/**
 * @brief 扑克牌花色枚举
 * 定义了扑克牌的四种花色及其对应整数值
 */
enum CardSuit {
    SUIT_CLUBS = 0,    // 梅花
    SUIT_DIAMONDS,     // 方块
    SUIT_HEARTS,       // 红桃
    SUIT_SPADES        // 黑桃
};

/**
 * @brief 扑克牌点数枚举
 * 定义了扑克牌的13种点数及其对应整数值(FACE_A=1 到 FACE_K=13)
 */
enum CardFace {
    FACE_A = 1,        // A
    FACE_2, FACE_3, FACE_4, FACE_5,
    FACE_6, FACE_7, FACE_8, FACE_9,
    FACE_10,
    FACE_J = 11,       // J
    FACE_Q = 12,       // Q
    FACE_K = 13        // K
};

/**
 * @brief 扑克牌资源配置类
 * 提供扑克牌相关资源路径的静态生成方法
 * 不依赖具体模型对象，仅根据牌面属性生成对应资源路径
 * 使用场景：在UI渲染时获取扑克牌各部分图像资源
 */
class CardResConfig {
public:
    /**
     * @brief 获取扑克牌左上角小数字图片路径
     * @param face 牌面点数(FACE_A-FACE_K)
     * @param suit 牌面花色(SUIT_CLUBS-SUIT_SPADES)
     * @return 小数字图片的相对路径
     * 例如："res/number/small_red_A.png"
     */
    static std::string getSmallNumberPath(CardFace face, CardSuit suit);

    /**
     * @brief 获取扑克牌中间大数字图片路径
     * @param face 牌面点数(FACE_A-FACE_K)
     * @param suit 牌面花色(SUIT_CLUBS-SUIT_SPADES)
     * @return 大数字图片的相对路径
     * 例如："res/number/big_black_10.png"
     */
    static std::string getBigNumberPath(CardFace face, CardSuit suit);

    /**
     * @brief 获取扑克牌花色图标图片路径
     * @param suit 牌面花色(SUIT_CLUBS-SUIT_SPADES)
     * @return 花色图标图片的相对路径
     * 例如："res/suits/heart.png"
     */
    static std::string getSuitPath(CardSuit suit);

    /**
     * @brief 获取扑克牌背面图片路径
     * @return 扑克牌背面图片的相对路径
     * 例如："res/card_back.png"
     */
    static std::string getBackGroundPath();
};

#endif // CARD_RESOURCE_CONFIG_H