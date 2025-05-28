#include "CardResConfig.h"

// 获取小数字图片路径
std::string CardResConfig::getSmallNumberPath(CardFace face, CardSuit suit) {
    static const char* faceNames[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
    std::string color = (suit == SUIT_DIAMONDS || suit == SUIT_HEARTS) ?
        "small_red_" : "small_black_";
    return "res/number/" + color + faceNames[face] + ".png";
}

// 获取大数字图片路径
std::string CardResConfig::getBigNumberPath(CardFace face, CardSuit suit) {
    static const char* faceNames[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
    std::string color = (suit == SUIT_DIAMONDS || suit == SUIT_HEARTS) ?
        "big_red_" : "big_black_";
    return "res/number/" + color + faceNames[face] + ".png";
}

// 获取花色图片路径
std::string CardResConfig::getSuitPath(CardSuit suit) {
    static const char* suitNames[] = { "club", "diamond", "heart", "spade" };
    return "res/suits/" + std::string(suitNames[suit]) + ".png";
}

std::string CardResConfig::getBackGroundPath()
{
    return std::string("res/card_general.png");
}
