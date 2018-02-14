#include <iostream>

#include "PlayingStack.h"

Card& PlayingStack::getTop()
{   
    return cards_.back();
}

bool PlayingStack::pushBack( Card card, std::string status, std::string request, std::string color, Card last, int by, int turn )
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
        if ( turn != by )
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
        else
        {
            if ( getTop().getType() == card.getType() )
            {
                cards_.push_back( card );
                return true;
            }
            else
            {
                std::cout << "Wrong card :(" << std::endl;
                return false;
            }
        }
    }
}

void PlayingStack::pushForce( Card card )
{   
    cards_.push_back( card );
}
