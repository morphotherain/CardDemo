#include "UndoManager.h"
#include "../models/GameModel.h"
#include "../views/CardView.h"
#include "../controllers/CardController.h"
#include "../views/GameView.h"
#include <algorithm>

void UndoManager::recordAction(const UndoAction& action) {
    _undoStack.push_back(action);
}

bool UndoManager::canUndo() const {
    return !_undoStack.empty();
}

int UndoManager::getNextUndoId()
{
    return _undoStack[_undoStack.size()-1].id;
}

void UndoManager::undoLastAction(GameModel* model) {
    if (!canUndo()) return;

    UndoAction action = _undoStack.back();
    _undoStack.pop_back();

    CardView* cardView = action.cardView;

    switch (action.type) {
    case UndoActionType::MOVE_CARD:
        // 撤销移动操作
        if (cardView) {
            if (cardView) {
                Vec2 targetPos = action.originalPosition;
                int targetZOrder = action.card->getOriginalZOrder();
                action.card->setPosition(targetPos);
                action.card->setMatched(false);
                cardView->stopAllActions();
                cardView->setLocalZOrder(action.card->getOriginalZOrder());

                // 执行动画
                auto moveAction = MoveTo::create(0.5f, targetPos);
                auto changeZOrderAction = CallFunc::create([cardView, targetZOrder]() {
                    cardView->setLocalZOrder(targetZOrder);
                    });
                auto sequence = Sequence::create(changeZOrderAction, moveAction, nullptr);
                cardView->runAction(sequence);
                //cardView->setPosition(targetPos);
            }
        }
        break;

    case UndoActionType::CHANGE_PARENT:
        // 撤销父节点变更
        if (action.card && action.originalParent) {
            if (action.cardView && action.cardView->getParent() != action.originalParent) {
                // 保留全局位置
                cocos2d::Vec2 worldPos = action.cardView->getParent()->convertToWorldSpace(action.cardView->getPosition());

                action.cardView->retain();
                action.cardView->removeFromParent();

                action.originalParent->addChild(action.cardView);
                action.cardView->setPosition(action.originalParent->convertToNodeSpace(worldPos));

                action.cardView->release();
            }
        }
        break;

    case UndoActionType::CHANGE_VECTOR:
        // 撤销卡牌在不同向量间的移动
        if (action.card) {
            model->undoReplaceTopStackCard(action.card, action.field);
        }
        break;
    }
}

void UndoManager::clearHistory() {
    _undoStack.clear();
}
