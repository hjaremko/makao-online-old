#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Network.hpp>

#include "Deck.h"
#include "HandDeck.h"
#include "PlayingStack.h"

class Game;

class Player
{
    public:
        Player();
        virtual ~Player();

        void getFromDeck( Deck&, int );
        bool pushToStack( PlayingStack&, int, Card&, bool, std::string );
        bool hasMakao();
        bool getRequest( std::string, std::string& );


        sf::IpAddress address;
        sf::TcpSocket* socket;

        friend Game;
    protected:
        HandDeck handDeck;
        bool hasMakao_;
        int skip_ = 0;
};

#endif // PLAYER_H
