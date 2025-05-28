#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#include "cocos2d.h"

USING_NS_CC;

/**
 * @brief ��Ϸ������
 * �ṩ��Ϸ�����г��õľ�̬���߷��������ַ�����ת������ѧ�����
 */
class GameUtils {
public:
    /**
     * @brief ��GBK�����ַ���ת��ΪUTF-8����
     * @param strGBK �����GBK�����ַ���
     * @return ת�����UTF-8�����ַ���
     */
    static std::string GBKToUTF8(const std::string& strGBK);
};

#endif // GAME_UTILS_H