#ifndef CARD_CONTROLLER_H
#define CARD_CONTROLLER_H

USING_NS_CC;

#include "cocos2d.h"
#include "../models/CardModel.h"
#include "../views/CardView.h"

/**
 * @brief �˿��ƿ�����
 * ����Э��CardModel��CardView֮�������ͬ���ͽ����߼�
 * ʹ�ó����������˿��Ƶ��ƶ���״̬�仯��ƥ���߼�
 */
class CardController {
private:
    CardModel* _model;  ///< �˿�������ģ��
    CardView* _view;    ///< �˿�����ͼ����

public:
    /**
     * @brief ���캯��
     * @param model ������CardModelʵ��
     * @param view ������CardViewʵ��
     */
    CardController(CardModel* model, CardView* view)
        : _model(model), _view(view) {}

    /**
     * @brief ��ȡ�˿���ģ��
     * @return CardModelָ��
     */
    CardModel* getModel() const { return _model; }

    /**
     * @brief ��ȡ�˿�����ͼ
     * @return CardViewָ��
     */
    CardView* getView() const { return _view; }

    /**
     * @brief ͬ������λ��
     * @param pos ��λ������
     */
    void setPosition(const cocos2d::Vec2& pos) {
        _model->setPosition(pos);
        _view->setPosition(pos);
    }

    /**
     * @brief ����ƥ��״̬
     * @param matched �Ƿ�ƥ��ɹ�
     */
    void setMatched(bool matched) {
        _model->setMatched(matched);
        // �������ͼƥ��Ч���߼�
    }

    /**
     * @brief ���ü���״̬
     * @param active �Ƿ񼤻�
     */
    void setActive(bool active) {
        _model->setActive(active);
        // �������ͼ����Ч���߼�
    }

    /**
     * @brief �ж��������Ƿ����ƥ��
     * @param other ��һ���ƵĿ�����
     * @return ����ƥ�䷵��true�����򷵻�false
     */
    bool canMatchWith(const CardController* other) const {
        return _model->canMatchWith(other->getModel());
    }

    /**
     * @brief ����ģ��״̬������ͼ
     */
    void updateView() {
    }
};

#endif // CARD_CONTROLLER_H