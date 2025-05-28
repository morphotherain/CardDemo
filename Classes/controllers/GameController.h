#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../views/GameView.h"
#include "../configs/LevelConfig.h"
#include "../configs/LevelConfigLoader.h"
#include "../controllers/CardController.h"
#include "../managers/UndoManager.h"

/**
 * @brief ��Ϸ��������
 * �̳���cocos2d::Scene������Э����Ϸģ��(GameModel)����ͼ(GameView)�͹ؿ��߼�
 * ʹ�ó�����������Ϸ���̿��ơ����ƽ������������������ؿ�����
 */
class GameController : public cocos2d::Scene {
private:
    GameModel* _model;          ///< ��Ϸ����ģ��
    GameView* _view;            ///< ��Ϸ����ͼ
    std::vector<CardController*> _cardControllers; ///< ���ƿ���������
    UndoManager _undoManager;   ///< ��������������
    int _currentUndoId = 0;     ///< ��ǰ��������ID������

public:
    /**
     * @brief ������Ϸ������ʵ���ľ�̬����
     * @return GameControllerָ�룬����ʧ�ܷ���nullptr
     */
    static GameController* createInstance();

    /**
     * @brief ��ʼ���������̳���cocos2d::Scene��
     * @return ��ʼ���ɹ�����true��ʧ�ܷ���false
     */
    bool init();

    /**
     * @brief �����Ƶ���¼�
     * @param cardId ��������Ƶ�ID
     */
    void handleCardClick(int cardId);

    /**
     * @brief ��������������¼�
     */
    void handleUndoClick();

    /**
     * @brief �жϿ����Ƿ񱻸���
     * @param card Ŀ�꿨��ģ��
     * @return �Ѹ��Ƿ���true�����򷵻�false
     */
    bool isCardCovered(CardModel* card);

    /**
     * @brief �ж���ͼ�Ƿ��������Ƹ���
     * @param target Ŀ����ͼ
     * @param other �Ա���ͼ
     * @return ���ڸ��Ƿ���true�����򷵻�false
     */
    bool isCardViewCovering(CardView* target, CardView* other);

    /**
     * @brief ����ID���ҿ���ģ��
     * @param cardId ����ID
     * @return �ҵ�����CardModelָ�룬���򷵻�nullptr
     */
    CardModel* findCardById(int cardId);

    /**
     * @brief �����ƴ��������ƶ�����Ϸ����
     * @param cardToMove ���ƶ��Ŀ���ģ��
     */
    void replaceTrayWithPlayFieldCard(CardModel* cardToMove);

    /**
     * @brief ��¼��������
     * @param cardToMove �����漰�Ŀ���ģ��
     * @param type �������ͣ�ö��ֵ��
     * @param undoId ��������ID
     */
    void recordUndoAction(CardModel* cardToMove, UndoActionType type, int undoId);

    /**
     * @brief ����ID���ҿ�����ͼ
     * @param cardId ����ID
     * @return �ҵ�����CardViewָ�룬���򷵻�nullptr
     */
    CardView* findCardViewById(int cardId);

    /**
     * @brief �жϿ����Ƿ��ڴ�����
     * @param clickedCard ����Ŀ���ģ��
     * @return �ڴ���������true�����򷵻�false
     */
    bool isCardInReserve(CardModel* clickedCard);

    /**
     * @brief �����������Ƶ���¼�
     * @param clickedCard ����Ŀ���ģ��
     * @return ����ɹ�����true�����򷵻�false
     */
    bool handleReserveCardClick(CardModel* clickedCard);

    /**
     * @brief �жϿ����Ƿ�����Ϸ����
     * @param clickedCard ����Ŀ���ģ��
     * @return ����Ϸ���򷵻�true�����򷵻�false
     */
    bool isCardInPlayField(CardModel* clickedCard);

    /**
     * @brief ������Ϸ�����Ƶ���¼�
     * @param clickedCard ����Ŀ���ģ��
     * @return ����ɹ�����true�����򷵻�false
     */
    bool handlePlayFieldCardClick(CardModel* clickedCard);

    /**
     * @brief �����Ϸ�����Ƶ���Ч��
     * @param clickedCard ����Ŀ���ģ��
     * @return ��Ч����true�����򷵻�false
     */
    bool checkPlayFieldCardValidity(CardModel* clickedCard);

    /**
     * @brief ����ָ���ؿ�
     * @param levelId �ؿ�ID����1��ʾ��һ�أ�
     * @return ���سɹ�����true�����򷵻�false
     */
    bool loadLevel(int levelId);

    CREATE_FUNC(GameController);

    /**
     * @brief ����ģ���������ɿ�����ͼ��������
     */
    void generateCardsFromModel();
};

#endif // GAME_CONTROLLER_H