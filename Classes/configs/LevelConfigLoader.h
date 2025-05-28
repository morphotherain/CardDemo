#ifndef LEVEL_CONFIG_LOADER_H
#define LEVEL_CONFIG_LOADER_H

#include "LevelConfig.h"
#include <string>

/**
 * @brief 关卡配置加载器
 * 负责从文件或游戏资源中加载关卡配置数据
 * 使用场景：在游戏启动或关卡切换时加载对应的扑克牌布局配置
 */
class LevelConfigLoader {
public:
    /**
     * @brief 从文件加载关卡配置
     * @param filePath 文件路径
     * @param config 存储加载结果的LevelConfig对象引用
     * @return 加载成功返回true，失败返回false
     */
    static bool loadLevelConfig(const std::string& filePath, LevelConfig& config);

    /**
     * @brief 从Cocos2d-x资源系统加载关卡配置
     * @param levelId 关卡ID
     * @param config 存储加载结果的LevelConfig对象引用
     * @return 加载成功返回true，失败返回false
     */
    static bool loadLevelConfigFromResource(const int levelId, LevelConfig& config);
};

#endif // LEVEL_CONFIG_LOADER_H