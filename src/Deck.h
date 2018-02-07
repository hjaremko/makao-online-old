#ifndef DECK_H
#define DECK_H

#include <vector>

#include "Card.h"

class Deck
{
    public:
        Deck();
        virtual ~Deck();

        void pushBack( Card );
        Card popBack();
        int size();
        void shuffle();
        void fill();
        void clear();

    protected:
        std::vector<Card> cards_;
};

#endif // DECK_H
