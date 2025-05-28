#pragma once
#include <vector>
#include "../models/UndoModel.h"

/**
 * @brief ��������������
 * �����¼�������ִ����Ϸ�еĳ�������
 * ʹ�ó������������ƶ���״̬����ȿɳ�����������ʷ��¼
 */
class UndoManager {
public:
    /**
     * @brief ��¼һ���ɳ�������
     * @param action Ҫ��¼�ĳ����������󣨰����������͡�������Ϣ�ȣ�
     */
    void recordAction(const UndoAction& action);

    /**
     * @brief �ж��Ƿ���ڿɳ����Ĳ���
     * @return ���ڿɳ�����������true�����򷵻�false
     */
    bool canUndo() const;

    /**
     * @brief ��ȡ��һ��Ψһ�ĳ�������ID
     * @return ����������ID����1��ʼ��
     */
    int getNextUndoId();

    /**
     * @brief �������һ�μ�¼�Ĳ���
     * @param model ��Ϸ����ģ�ͣ�����ִ�г����߼�
     */
    void undoLastAction(GameModel* model);

    /**
     * @brief ������г���������ʷ��¼
     */
    void clearHistory();

private:
    std::vector<UndoAction> _undoStack; ///< ��������ջ������ȳ���
};