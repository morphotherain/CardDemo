#ifndef LEVEL_CONFIG_H
#define LEVEL_CONFIG_H

#include <vector>
#include "cocos2d.h"

/**
 * @brief ��Ϸ�ؿ�������
 * ����洢�ͽ�����Ϸ�ؿ����˿��ƵĲ�����Ϣ
 * ʹ�ó������ڹؿ�����ʱ��JSON����������Ϸ��ʼ����
 */
class LevelConfig {
public:
    /**
     * @brief �˿��Ʋ�����Ϣ�ṹ��
     * �洢�����˿��Ƶ����Ժ�λ����Ϣ
     */
    struct CardInfo {
        int cardFace;         ///< �������(1-13)
        int cardSuit;         ///< ���滨ɫ(0-3)
        cocos2d::Vec2 position; ///< ��������Ϸ�е�λ������
    };

    std::vector<CardInfo> playfieldCards; ///< ��Ϸ�����˿��Ʋ�����Ϣ
    std::vector<CardInfo> stackCards;     ///< �ƶ������˿��Ʋ�����Ϣ

    /**
     * @brief ��JSON�ַ��������ؿ�����
     * @param jsonStr JSON��ʽ�Ĺؿ������ַ���
     * @return �����ɹ�����true��ʧ�ܷ���false
     */
    bool parseFromJson(const std::string& jsonStr);
};

#endif // LEVEL_CONFIG_H