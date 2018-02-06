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
    Game game;

    sf::TcpSocket socket;
    sf::TcpListener listener;
    listener.listen( 55001 );
    std::cout << "Listening..." << std::endl;

    sf::SocketSelector selector;
    selector.add( listener );

    // if ( listener == sf::Socket::Done )
    {
        if ( selector.wait( sf::seconds( 10 ) ) )
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
            
            do
            {   
                game.sendCardInfo();
            } while ( !game.makeTurn() );

            //==========================================
        }
    }

    return 0;
}