#pragma once
#include "cocos2d.h"
#include "CardModel.h"
#include "GameModel.h"

USING_NS_CC;
class CardView;

/**
 * @brief ������������ö��
 * ����ɳ������������ͣ����ڱ�ʶ��ͬ��״̬���
 */
enum class UndoActionType {
    MOVE_CARD,        ///< �����ƶ�����
    CHANGE_PARENT,    ///< ���ڵ�������
    CHANGE_VECTOR     ///< �����б�������
};

/**
 * @brief ���������ṹ��
 * �洢ִ�г������������������Ϣ�������������͡�����״̬����ʷ����
 */
struct UndoAction {
    UndoActionType type;        ///< �������ͣ�ö��ֵ��
    CardModel* card;            ///< �����漰�Ŀ���ģ��
    CardView* cardView;         ///< �����漰�Ŀ�����ͼ����ѡ��
    Node* originalParent;       ///< ����ԭʼ���ڵ㣨���ڻָ��㼶��ϵ��
    Vec2 originalPosition;      ///< ����ԭʼλ�����꣨�����ƶ�������
    int id;                     ///< ����Ψһ��ʶ����������ջID��
    CardField field;            ///< ����ԭʼ��������PLAY_FIELD/STACK/RESERVE��
};