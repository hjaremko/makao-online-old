#include <iostream>
#include "PlayingStack.h"

Card PlayingStack::getTop()
{   
    return cards_.back();
}

bool PlayingStack::pushBack( Card card, bool status, std::string request )
{   
    if ( cards_.size() == 0 || getTop() == card || getTop().getType() == "Q" || card.getType() == "Q" )
    {
        if ( status )
        {
            if ( getTop().getType() == "2" || getTop().getType() == "3" || getTop().getType() == "4" || getTop().getType() == request )
            {
                cards_.push_back( card );
                return true;
            }
            else
                return false;
        }
        else
        {
            cards_.push_back( card );
            return true;
        }
        return true;
    }
    else
    {
        std::cout << "Wrong card" << std::endl;
        return false;
    }
}

void PlayingStack::pushForce( Card card )
{   
    cards_.push_back( card );
}