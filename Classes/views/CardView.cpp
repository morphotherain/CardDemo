#include "CardView.h"
#include "../configs/CardResConfig.h"

using namespace cocos2d;

const float CardView::kCardWidth = 182.0f;
const float CardView::kCardHeight = 282.0f;

CardView* CardView::create(CardModel* model, const std::string& bgPath) {
    CardView* node = new CardView();
    if (node && node->init()) {
        node->model = model;
        node->cardBgPath = bgPath;
        node->renderCardBackground();
        node->renderTopLeftNumber();
        node->renderCenterNumber();
        node->renderSuitIcon();
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool CardView::init() {
    if (!Sprite::init()) return false;
    this->setContentSize(Size(kCardWidth, kCardHeight));  // ���Ƴߴ�
    return true;
}

// ��Ⱦ���Ʊ���
void CardView::renderCardBackground() {
    auto bgSprite = Sprite::create(cardBgPath);
    if (bgSprite) {
        bgSprite->setPosition(getContentSize() / 2);
        this->addChild(bgSprite);
    }
}

// ��Ⱦ���Ͻ�С����
void CardView::renderTopLeftNumber() {
    auto numberSprite = Sprite::create(CardResConfig::getSmallNumberPath(model->getFace(), model->getSuit()));

    if (numberSprite) {
        numberSprite->setPosition(40, getContentSize().height - 40);
        this->addChild(numberSprite);
    }
}

// ��Ⱦ�м������
void CardView::renderCenterNumber() {
    auto numberSprite = Sprite::create(CardResConfig::getBigNumberPath(model->getFace(), model->getSuit()));
    if (numberSprite) {
        Vec2 center = getContentSize() / 2;
        numberSprite->setPosition(center.x ,center.y - 30);
        this->addChild(numberSprite);
    }
}

// ��Ⱦ��ɫͼ��
void CardView::renderSuitIcon() {
    auto suitSprite = Sprite::create(CardResConfig::getSuitPath( model->getSuit()));
    if (suitSprite) {
        suitSprite->setPosition(getContentSize().width - 40, getContentSize().height - 40);
        this->addChild(suitSprite);
    }
}

