#ifndef CARD_VIEW_H
#define CARD_VIEW_H

#include "cocos2d.h"
#include "../models/CardModel.h"

/**
 * @brief �˿�����ͼ��
 * �̳���cocos2d::Sprite�������˿��Ƶ��Ӿ���Ⱦ
 * ʹ�ó���������Ϸ��������ʾ�˿��Ƶ����桢���漰����״̬ͼ��
 */
class CardView : public cocos2d::Sprite {
private:
    static const float kCardWidth;  ///< ���ƿ�ȣ��̶�ֵ����λ�����أ�
    static const float kCardHeight; ///< ���Ƹ߶ȣ��̶�ֵ����λ�����أ�

    CardModel* model;               ///< �������˿�������ģ��
    std::string cardBgPath;         ///< ���Ʊ���ͼƬ·��������������ͼ��

    /**
     * @brief ��Ⱦ���Ʊ���������������ͼ��
     */
    void renderCardBackground();

    /**
     * @brief ��Ⱦ���Ͻ�С���֣�������ʾ������
     */
    void renderTopLeftNumber();

    /**
     * @brief ��Ⱦ�м�����֣�������ʾ��Ҫ������
     */
    void renderCenterNumber();

    /**
     * @brief ��Ⱦ��ɫͼ�꣨÷��/����/����/���ң�
     */
    void renderSuitIcon();


public:
    /**
     * @brief ����������ͼʵ���Ĺ�������
     * @param model ������CardModel����ģ��
     * @param bgPath ���Ʊ���ͼƬ·�����米��ͼƬ·����
     * @return �����ɹ�����CardViewָ�룬ʧ�ܷ���nullptr
     */
    static CardView* create(CardModel* model, const std::string& bgPath);

    /**
     * @brief ��ȡ�������˿�������ģ��
     * @return CardModelָ��
     */
    CardModel* getCardModel() { return model; }

    /**
     * @brief ��ʼ���������̳���cocos2d::Sprite��
     * @return ��ʼ���ɹ�����true��ʧ�ܷ���false
     */
    virtual bool init() override;

};

#endif // CARD_VIEW_H