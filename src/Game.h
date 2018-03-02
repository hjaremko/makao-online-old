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
        bool pushToStack( int, int );
        void dealOut( int );
        void makeStack();
        void printInfo();
        void refillDrawingDeck();
        void executeSpecial( Card& );
        void throwCard( std::string );
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
        Deck drawingDeck;
        Card lastThrown;

        sf::Packet cardPacket;
        sf::Packet turnPacket;

        int turn = 0;
        int toTake = 0;
        int toSkip = 0;
        int lastThrownBy = -1;
        int whoRequested_ = -1;
        bool result = false;
        std::string request = "-";
        std::string gameStatus = "-";
        std::string newColor = "-";
};

#endif // GAME_H
