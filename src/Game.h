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
        void printInfo();
        void refillDrawingDeck();
        void executeSpecial( Card& );
        sf::Socket::Status sendCardInfo();

        std::array<Player, 2> players;
        sf::Socket::Status status;
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
        int turn = 0;
        int lastThrownBy = -1;
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
