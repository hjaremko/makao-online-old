#include <iostream>

#include "HandDeck.h"

void HandDeck::show()
{
    for ( Card i : cards_ )
    {
        std::cout << i.get() << " ";
    }
    std::cout << std::endl;
}

Card HandDeck::get( int which )
{
    return cards_[ which ];
}