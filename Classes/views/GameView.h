#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "cocos2d.h"
#include "../models/GameModel.h"

/**
 * @brief ��Ϸ����ͼ��
 * �̳���cocos2d::Layer��������Ϸ����Ĵ���û���������
 * ʹ�ó�������Ⱦ��Ϸ�����ƶ����򡢳�����ť�Ƚ���Ԫ�أ����������¼�
 */
class GameView : public cocos2d::Layer {
private:
    GameModel* model;                     ///< ��������Ϸ����ģ��
    cocos2d::Node* mainAreaNode;          ///< ��Ϸ������ڵ㣨��ſɽ������ƣ�
    cocos2d::Node* pileAreaNode;          ///< �ƶ�����ڵ㣨����ƶѿ��ƣ�
    std::function<void(int)> _onCardClickCallback; ///< ���Ƶ���¼��ص�����������Ϊ����ID��
    std::function<void()> _onUndoClickCallback;     ///< ������ť����¼��ص�����
    cocos2d::Label* _undoLabel;           ///< ����������ʾ��ǩ

public:
    /**
     * @brief ������Ϸ��ͼʵ���Ĺ�������
     * @param model ������GameModel����ģ��
     * @return �����ɹ�����GameViewָ�룬ʧ�ܷ���nullptr
     */
    static GameView* create(GameModel* model);

    /**
     * @brief ��ʼ���������̳���cocos2d::Layer��
     * @return ��ʼ���ɹ�����true��ʧ�ܷ���false
     */
    virtual bool init() override;

    /**
     * @brief ʹ������ģ�ͳ�ʼ����ͼ
     * @param model ��Ϸ����ģ��
     * @return ��ʼ���ɹ�����true��ʧ�ܷ���false
     */
    bool initWithModel(GameModel* model);

    /**
     * @brief ������Ϸ����������������ƶ�����
     */
    void createGameAreas();

    /**
     * @brief ��������������ǩ��������ť
     */
    void createUndoLabel();

    /**
     * @brief ��ȡ��Ϸ������ڵ�
     * @return ������ڵ�ָ��
     */
    cocos2d::Node* getMainAreaNode() const { return mainAreaNode; }

    /**
     * @brief ��ȡ�ƶ�����ڵ�
     * @return �ƶ�����ڵ�ָ��
     */
    cocos2d::Node* getPileAreaNode() const { return pileAreaNode; }

    /**
     * @brief ���ÿ��Ƶ���¼��ص�����
     * @param callback �ص�����������Ϊ��������Ƶ�ID��
     */
    void setOnCardClickCallback(const std::function<void(int)>& callback);

    /**
     * @brief ���ó�����ť����¼��ص�����
     * @param callback �ص�����
     */
    void setOnUndoClickCallback(const std::function<void()>& callback);

    /**
     * @brief ��ʼ�������¼�����
     */
    void setupTouchHandling();
};

#endif // GAME_VIEW_H