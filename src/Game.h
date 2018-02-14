#ifndef GAME_H
#define GAME_H

#include <iostream>

#include <SFML/Network.hpp>

#include "Deck.h"
#include "PlayingStack.h"
#include "Player.h"

class Game
{
    public:
        Game();
        virtual ~Game();

        bool makeTurn();
        void dealOut( int );
        void makeStack();
        void sendCardInfo();
        void printInfo();
        void refillDrawingDeck();
        void executeSpecial( Card& );

        std::array<Player, 4> players;
        PlayingStack stack;

        Game& operator++()
        {
            std::cout << "Next turn" << std::endl;

            turn++;
            if ( turn == players.size() )
                turn = 0;

            return *this;
        }

    private:
        unsigned int turn = 0;
        Deck drawingDeck;
        Card lastThrown;

        sf::Packet cardPacket;
        sf::Packet turnPacket;

        int toTake = 0;
        int whoRequested_ = -1;
        std::string request = "-";
        std::string gameStatus = "-";
        std::string newColor = "-";
};

#endif // GAME_H
