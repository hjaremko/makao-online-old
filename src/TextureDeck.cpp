#include <vector>
#include <array>
#include <random>
#include <algorithm>

#include "TextureDeck.h"

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
    std::array<std::string, 13> cardTypes = { "A", "K", "Q", "J", "10", "9", "8", "7", "6", "5", "4", "3", "2" };
    std::array<std::string, 4> cardColors = { "spades", "hearts", "diamonds", "clubs" };

    for ( std::string type : cardTypes )
    {
        for ( std::string color : cardColors )
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
