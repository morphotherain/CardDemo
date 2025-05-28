// LevelConfigLoader.cpp
#include "LevelConfigLoader.h"
#include "cocos2d.h"

bool LevelConfigLoader::loadLevelConfig(const std::string& filePath, LevelConfig& config) {
    // 尝试打开文件
    FILE* file = fopen(filePath.c_str(), "r");
    if (!file) {
        CCLOGERROR("Failed to open file: %s", filePath.c_str());
        return false;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 读取文件内容
    std::string jsonStr(size, '\0');
    fread(&jsonStr[0], 1, size, file);
    fclose(file);

    // 解析JSON
    return config.parseFromJson(jsonStr);
}

bool LevelConfigLoader::loadLevelConfigFromResource(const int levelId, LevelConfig& config) {

    std::string filePath = "levels/level" + std::to_string(levelId) + ".json";
    // 使用Cocos2d-x的文件加载器加载资源
    cocos2d::Data data = cocos2d::FileUtils::getInstance()->getDataFromFile(filePath);
    if (data.getSize() == 0) {
        CCLOGERROR("Failed to load resource: %s", filePath.c_str());
        return false;
    }

    // 转换为字符串并解析
    std::string jsonStr(reinterpret_cast<const char*>(data.getBytes()), data.getSize());
    return config.parseFromJson(jsonStr);
}