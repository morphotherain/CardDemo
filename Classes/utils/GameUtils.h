#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#include "cocos2d.h"

USING_NS_CC;

/**
 * @brief 游戏工具类
 * 提供游戏开发中常用的静态工具方法，如字符编码转换、数学计算等
 */
class GameUtils {
public:
    /**
     * @brief 将GBK编码字符串转换为UTF-8编码
     * @param strGBK 输入的GBK编码字符串
     * @return 转换后的UTF-8编码字符串
     */
    static std::string GBKToUTF8(const std::string& strGBK);
};

#endif // GAME_UTILS_H