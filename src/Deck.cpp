#include <vector>
#include <array>
#include <random>
#include <algorithm>

#include "Deck.h"
#include "constants.h"

Deck::Deck()
{
    //ctor
}

Deck::~Deck()
{
    //dtor
}

void Deck::pushBack( Card card )
{
    cards_.push_back( card );
}

Card Deck::popBack()
{   
    Card card = cards_.back();
    cards_.pop_back();

    return card;
}

int Deck::size()
{
    return cards_.size();
}


void Deck::shuffle()
{
    std::random_device rng;
    std::mt19937 urng( rng() );
    std::shuffle( cards_.begin(), cards_.end(), urng );
}

void Deck::fill()
{
    for ( std::string type : makao::cardTypes )
    {
        for ( std::string color : makao::cardColors )
        {
            Card newCard( type, color );
            pushBack( newCard );
        }
    }

    shuffle();
}

void Deck::clear()
{
    cards_.clear();
}
