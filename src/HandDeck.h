#ifndef HANDDECK_H
#define HANDDECK_H

#include "Card.h"
#include "Deck.h"

#include <SFML/Graphics.hpp>

class Game;

class HandDeck : public Deck
{
    public:
        friend Game;

        void show();
        void sort();
        Card get( int );

};

#endif // HANDDECK_H
