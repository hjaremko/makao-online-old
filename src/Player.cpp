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

    handDeck.sort();
}

bool Player::hasMakao()
{
    return hasMakao_;
}


bool Player::getRequest( std::string which, std::string& request )
{
    Card temp( which, "hearts" );
    bool isValidRequest = false;

    switch ( which[ 0 ] )
    {
        case '1':
            isValidRequest = ( which[ 1 ] != '1' ) ? true : false;
            break;
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            isValidRequest = true;
            break;
        default:
            isValidRequest = false;
    }

    if ( isValidRequest )
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
