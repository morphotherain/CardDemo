#ifndef LEVEL_CONFIG_LOADER_H
#define LEVEL_CONFIG_LOADER_H

#include "LevelConfig.h"
#include <string>

/**
 * @brief �ؿ����ü�����
 * ������ļ�����Ϸ��Դ�м��عؿ���������
 * ʹ�ó���������Ϸ������ؿ��л�ʱ���ض�Ӧ���˿��Ʋ�������
 */
class LevelConfigLoader {
public:
    /**
     * @brief ���ļ����عؿ�����
     * @param filePath �ļ�·��
     * @param config �洢���ؽ����LevelConfig��������
     * @return ���سɹ�����true��ʧ�ܷ���false
     */
    static bool loadLevelConfig(const std::string& filePath, LevelConfig& config);

    /**
     * @brief ��Cocos2d-x��Դϵͳ���عؿ�����
     * @param levelId �ؿ�ID
     * @param config �洢���ؽ����LevelConfig��������
     * @return ���سɹ�����true��ʧ�ܷ���false
     */
    static bool loadLevelConfigFromResource(const int levelId, LevelConfig& config);
};

#endif // LEVEL_CONFIG_LOADER_H