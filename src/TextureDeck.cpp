#include <vector>
#include <array>
#include <random>
#include <algorithm>

#include "TextureDeck.h"
#include "constants.h"

TextureDeck::TextureDeck()
{
    //ctor
}

TextureDeck::~TextureDeck()
{
    //dtor
}

void TextureDeck::pushBack( TextureCard card )
{
    cards_.push_back( card );
}

TextureCard TextureDeck::popBack()
{   
    TextureCard card = cards_.back();
    cards_.pop_back();

    return card;
}

int TextureDeck::size()
{
    return cards_.size();
}


void TextureDeck::shuffle()
{
    std::random_device rng;
    std::mt19937 urng( rng() );
    std::shuffle( cards_.begin(), cards_.end(), urng );
}

void TextureDeck::fill()
{
    for ( std::string type : makao::cardTypes )
    {
        for ( std::string color : makao::cardColors )
        {
            TextureCard newCard( type, color );
            pushBack( newCard );
        }
    }

    shuffle();
}

void TextureDeck::clear()
{
    cards_.clear();
}
