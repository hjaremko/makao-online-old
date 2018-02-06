#include "Player.h"
#include "PlayingStack.h"

Player::Player()
{
    //ctor
}

Player::~Player()
{
    //dtor
}

void Player::getFromDeck( Deck& deck, int amount )
{
    for ( int i = 0; i < amount; ++i )
        this->handDeck.pushBack( deck.popBack() );
}

bool Player::pushToStack( PlayingStack& stack, int which, Card& last, bool isFight, std::string request )
{
    if ( stack.pushBack( handDeck.get( which ), isFight, request ) )
    {
        last = stack.getTop();
        handDeck.cards_.erase( handDeck.cards_.begin() + which );
    }
    else
        return false;

    hasMakao_ = ( handDeck.size() == 1 ) ? true : false;

    return true;
}

bool Player::hasMakao()
{
    return hasMakao_;
}


bool Player::getRequest( std::string which, std::string& request )
{
    Card temp( which, "hearts" );

    if ( !temp.isSpecial() )
    {
        std::cout << "Requested: " << which << std::endl;
        request = which;

        return true;
    }
    else
    {
        std::cout << "Wrong request!" << std::endl;
        return false;
    }
}