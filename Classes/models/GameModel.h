#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include <vector>
#include "CardModel.h"
#include "../configs/LevelConfig.h"

/**
 * @brief ��Ϸ����ģ����
 * ������Ϸ�������˿��Ƶķ��飨��Ϸ�����ƶѡ�����������״̬
 * ʹ�ó������洢�ؿ���ʼ�����ݡ��������ƶ��߼����ṩ���ݲ�ѯ�ӿ�
 */
enum CardField {
    PLAY_FIELD = 0,  ///< ��Ϸ����
    STACK,           ///< �ƶ�����
    RESERVE          ///< ������
};

class GameModel {
private:
    std::vector<CardModel*> _playFieldCards; ///< ��Ϸ�������б�
    std::vector<CardModel*> _stackCards;     ///< �ƶ��������б�����ȳ���
    std::vector<CardModel*> _reserveCards;   ///< �����������б���ʼ���ֿ��ƣ�
    CardModel* _currentStackTop;             ///< ��ǰ�ƶѶ������ƣ�����ֵ��

public:
    /**
     * @brief ���캯��
     * ��ʼ���յ���Ϸģ��
     */
    GameModel();

    /**
     * @brief ��������
     * �������п�����Դ
     */
    ~GameModel();

    /**
     * @brief ʹ�ùؿ����ó�ʼ����Ϸģ��
     * @param config �ؿ��������ݣ������������Ʋ��֣�
     * @return ��ʼ���ɹ�����true��ʧ�ܷ���false
     */
    bool initWithLevelConfig(const LevelConfig& config);

    // Getter ����
    /**
     * @brief ��ȡ��Ϸ�������б�
     * @return ��Ϸ�������б�ֻ����
     */
    const std::vector<CardModel*>& getPlayFieldCards() const { return _playFieldCards; }

    /**
     * @brief ��ȡ�ƶ��������б�
     * @return �ƶ��������б�ֻ����
     */
    const std::vector<CardModel*>& getStackCards() const { return _stackCards; }

    /**
     * @brief ��ȡ�����������б�
     * @return �����������б�ֻ����
     */
    const std::vector<CardModel*>& getReserveCards() const { return _reserveCards; }

    /**
     * @brief ��ȡ��ǰ�ƶѶ�������
     * @return �ƶѶ�������ָ�룬�ƶ�Ϊ��ʱ����nullptr
     */
    CardModel* getCurrentStackTop() const { return _currentStackTop; }

    /**
     * @brief �����������Ŀ���
     */
    void clearAllCards();

    /**
     * @brief �����������������ƶ����ƶ�
     * �������ڻغ��ƿ����ƶ��߼���
     */
    void moveTopCardToStack();

    /**
     * @brief �滻�ƶѶ�������
     * @param card �����滻�Ŀ���ģ��
     */
    void replaceTopStackCard(CardModel* card);

    /**
     * @brief �����滻�ƶѶ������Ʋ���
     * @param card ���滻��ԭ����ģ��
     * @param field ԭ������������
     */
    void undoReplaceTopStackCard(CardModel* card, CardField field);
};

#endif // GAME_MODEL_H