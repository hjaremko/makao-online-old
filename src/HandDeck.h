#ifndef HANDDECK_H
#define HANDDECK_H

#include "Card.h"
#include "Deck.h"

#include <SFML/Graphics.hpp>

class Player;

class HandDeck : public Deck
{
    public:
        friend Player;

        void show();
        Card get( int );

};

#endif // HANDDECK_H
