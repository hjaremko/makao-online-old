#include "Card.h"

Card::Card()
{
    type_ = "-";
    color_ = "-";
    //ctor
}

Card::Card( std::string type, std::string color ) : type_( type ), color_( color )
{
    switch ( type_[ 0 ] )
    {
        case 'A':
        case 'K':
        case 'Q':
        case 'J':
        case '4':
        case '3':
        case '2':
            isSpecial_ = true;
            break;
        default:
            isSpecial_ = false;
    }
    //ctor
}

Card::~Card()
{
    //dtor
}

std::string Card::getType() const
{
    return type_;
}

std::string Card::getColor() const
{
    return color_;
}

std::string Card::get() const
{
    return type_ + color_;
}

bool Card::isSpecial() const
{
    return isSpecial_;
}

void Card::clear()
{
    type_ = "-";
    color_ = "-";
    isSpecial_ = false;
}

void Card::setColor( std::string newColor )
{
    color_ = newColor;
}