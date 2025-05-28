#ifndef CARD_RESOURCE_CONFIG_H
#define CARD_RESOURCE_CONFIG_H

#include <string>

/**
 * @brief �˿��ƻ�ɫö��
 * �������˿��Ƶ����ֻ�ɫ�����Ӧ����ֵ
 */
enum CardSuit {
    SUIT_CLUBS = 0,    // ÷��
    SUIT_DIAMONDS,     // ����
    SUIT_HEARTS,       // ����
    SUIT_SPADES        // ����
};

/**
 * @brief �˿��Ƶ���ö��
 * �������˿��Ƶ�13�ֵ��������Ӧ����ֵ(FACE_A=1 �� FACE_K=13)
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
 * @brief �˿�����Դ������
 * �ṩ�˿��������Դ·���ľ�̬���ɷ���
 * ����������ģ�Ͷ��󣬽����������������ɶ�Ӧ��Դ·��
 * ʹ�ó�������UI��Ⱦʱ��ȡ�˿��Ƹ�����ͼ����Դ
 */
class CardResConfig {
public:
    /**
     * @brief ��ȡ�˿������Ͻ�С����ͼƬ·��
     * @param face �������(FACE_A-FACE_K)
     * @param suit ���滨ɫ(SUIT_CLUBS-SUIT_SPADES)
     * @return С����ͼƬ�����·��
     * ���磺"res/number/small_red_A.png"
     */
    static std::string getSmallNumberPath(CardFace face, CardSuit suit);

    /**
     * @brief ��ȡ�˿����м������ͼƬ·��
     * @param face �������(FACE_A-FACE_K)
     * @param suit ���滨ɫ(SUIT_CLUBS-SUIT_SPADES)
     * @return ������ͼƬ�����·��
     * ���磺"res/number/big_black_10.png"
     */
    static std::string getBigNumberPath(CardFace face, CardSuit suit);

    /**
     * @brief ��ȡ�˿��ƻ�ɫͼ��ͼƬ·��
     * @param suit ���滨ɫ(SUIT_CLUBS-SUIT_SPADES)
     * @return ��ɫͼ��ͼƬ�����·��
     * ���磺"res/suits/heart.png"
     */
    static std::string getSuitPath(CardSuit suit);

    /**
     * @brief ��ȡ�˿��Ʊ���ͼƬ·��
     * @return �˿��Ʊ���ͼƬ�����·��
     * ���磺"res/card_back.png"
     */
    static std::string getBackGroundPath();
};

#endif // CARD_RESOURCE_CONFIG_H