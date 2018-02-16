#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>

#include <SFML/Network.hpp>

#include "Card.h"
#include "Deck.h"
#include "PlayingStack.h"
#include "HandDeck.h"
#include "Player.h"
#include "Game.h"

int main()
{
    srand( time( NULL ) );
    // ----- The server -----

    sf::TcpSocket socket;
    sf::TcpListener listener;
    
    if ( listener.listen( 55001 ) == sf::Socket::Done )
    {
        while ( true )
        {
            std::cout << "Listening..." << std::endl;

            Game game;
            sf::SocketSelector selector;
            selector.add( listener );

            if ( selector.wait( sf::seconds( 60 ) ) )
            {
                for ( unsigned int i = 0; i < game.players.size(); i++ )
                {
                    if ( selector.isReady( listener ) )
                    {
                        sf::TcpSocket* tmp = new sf::TcpSocket;

                        if ( listener.accept( *tmp )  == sf::Socket::Done )
                        {
                            game.players[ i ].socket = tmp;
                            game.players[ i ].address = tmp->getRemoteAddress();

                            selector.add( *tmp );

                            std::cout << "New client connected: " << tmp->getRemoteAddress() << std::endl;
                        }
                        else
                            delete tmp;
                    }
                }
                //==========================================
                std::cout << "Game started!" << std::endl;

                game.makeStack();
                game.dealOut( 5 );
                game.printInfo();
                game.sendCardInfo();

                while ( !game.makeTurn() && game.status == sf::Socket::Done )
                {
                    game.sendCardInfo();

                    if ( game.status != sf::Socket::Done )
                        std::cout << "Connection lost!" << std::endl;
                }

                std::cout << "Game over!" << std::endl;
                //==========================================
            }
        }
    }

    return 0;
}
