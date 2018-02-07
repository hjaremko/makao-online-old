#include <iostream>

#include "PlayingStack.h"

Card PlayingStack::getTop()
{   
    return cards_.back();
}

bool PlayingStack::pushBack( Card card, std::string status, std::string request )
{   
    if ( status == "fight" )
    {
        if ( ( card.getType() == "2" || card.getType() == "3" ) && getTop() == card )
        {
            cards_.push_back( card );
            return true;
        }
        else
            return false;
    }
    else if ( status == "skip" )
    {
        if ( card.getType() == "4" )
        {
            cards_.push_back( card );
            return true;
        }
        else
            return false;
    }
    else if ( status == "jack" )
    {   
        std::cout << card.getType() << " -- " << request << std::endl;
        if ( card.getType() == request || card.getType() == "J" )
        {
            cards_.push_back( card );
            return true;
        }
        else
            return false;
    }
    else
    {
        if ( cards_.size() == 0 || getTop() == card || getTop().getType() == "Q" || card.getType() == "Q" )
        {
            cards_.push_back( card );
            return true;
        }
        else
        {
            std::cout << "Wrong card" << std::endl;
            return false;
        }
    }
}

void PlayingStack::pushForce( Card card )
{   
    cards_.push_back( card );
}