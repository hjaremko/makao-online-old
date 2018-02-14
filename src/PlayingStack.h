#ifndef PLAYINGSTACK_H
#define PLAYINGSTACK_H

#include "Card.h"
#include "Deck.h"

class PlayingStack : public Deck
{
    public:
        Card& getTop();
        bool pushBack( Card, std::string, std::string, std::string, Card, int, int );
        void pushForce( Card );

        void operator>>( Deck& deck )
        {   
            for ( Card& card : cards_ )
            {
                deck.pushBack( card );
            }
            deck.popBack();

            clear();
        }
};

#endif // PLAYINGSTACK_H
