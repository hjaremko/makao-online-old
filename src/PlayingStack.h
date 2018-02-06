#ifndef PLAYINGSTACK_H
#define PLAYINGSTACK_H

#include "Card.h"
#include "Deck.h"

class PlayingStack : public Deck
{
    public:
        Card getTop();
        bool pushBack( Card, bool, std::string );
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

        bool isInPower = false;
    private:
};

#endif // PLAYINGSTACK_H
