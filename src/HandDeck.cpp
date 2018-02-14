#include "HandDeck.h"

void HandDeck::show()
{
    for ( Card i : cards_ )
    {
        std::cout << i.get() << " ";
    }
    std::cout << std::endl;
}

void HandDeck::sort()
{
    std::sort( cards_.begin(), cards_.end() );
}

Card HandDeck::get( int which )
{
    return cards_[ which ];
}
