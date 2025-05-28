// LevelConfigLoader.cpp
#include "LevelConfigLoader.h"
#include "cocos2d.h"

bool LevelConfigLoader::loadLevelConfig(const std::string& filePath, LevelConfig& config) {
    // ���Դ��ļ�
    FILE* file = fopen(filePath.c_str(), "r");
    if (!file) {
        CCLOGERROR("Failed to open file: %s", filePath.c_str());
        return false;
    }

    // ��ȡ�ļ���С
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // ��ȡ�ļ�����
    std::string jsonStr(size, '\0');
    fread(&jsonStr[0], 1, size, file);
    fclose(file);

    // ����JSON
    return config.parseFromJson(jsonStr);
}

bool LevelConfigLoader::loadLevelConfigFromResource(const int levelId, LevelConfig& config) {

    std::string filePath = "levels/level" + std::to_string(levelId) + ".json";
    // ʹ��Cocos2d-x���ļ�������������Դ
    cocos2d::Data data = cocos2d::FileUtils::getInstance()->getDataFromFile(filePath);
    if (data.getSize() == 0) {
        CCLOGERROR("Failed to load resource: %s", filePath.c_str());
        return false;
    }

    // ת��Ϊ�ַ���������
    std::string jsonStr(reinterpret_cast<const char*>(data.getBytes()), data.getSize());
    return config.parseFromJson(jsonStr);
}