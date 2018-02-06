#ifndef GAME_H
#define GAME_H

#include <SFML/Network.hpp>
#include <iostream>

#include "Deck.h"
#include "PlayingStack.h"
#include "Player.h"

class Game
{
    public:
        Game();
        virtual ~Game();

        void dealOut( int );
        void makeStack();
        void sendCardInfo();
        bool makeTurn();
        void printInfo();
        void refillDrawingDeck();
        void executeSpecial( Card );

        std::array<Player, 2> players;
        PlayingStack stack;

    private:
        unsigned int turn = 0;
        Deck drawingDeck;
        Card lastThrown;

        sf::Packet cardPacket;
        sf::Packet turnPacket;

        bool isFight = false;
        bool isRequest = false;
        int toTake = 0;
        int whoRequested_ = 0;
        std::string request = "-";
};

#endif // GAME_H
