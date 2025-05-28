#include "GameModel.h"

CardModel::CardModel(CardSuit s, CardFace f)
    : _suit(s), _face(f), _isFlipped(false), _isMatched(false)
    , _position(Vec2::ZERO), _isActive(true)
{
}
