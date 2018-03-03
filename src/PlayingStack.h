#ifndef PLAYINGSTACK_H
#define PLAYINGSTACK_H

#include "Card.h"
#include "Deck.h"

class PlayingStack : public Deck
{
    public:
        bool pushBack( Card, std::string, std::string, Card, int, int );
        void pushForce( Card );
        Card& getTop();
        sf::Packet* getLastThreeInPacket();

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
