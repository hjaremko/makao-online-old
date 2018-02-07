#ifndef HANDDECK_H
#define HANDDECK_H

#include "Card.h"
#include "Deck.h"

class Player;

class HandDeck : public Deck
{
    public:
        void show();
        Card get( int );

        friend Player;
};

#endif // HANDDECK_H
