#include <iostream>
#include <string>

#include <SFML/Network.hpp>

#include "Card.h"
#include "HandDeck.h"

sf::Packet& operator<<( sf::Packet& packet, const Card& card )
{
    return packet << card.getType() << card.getColor();
}

sf::Packet& operator>>( sf::Packet& packet, Card& card )
{
    return packet >> card.type_ >> card.color_;
}

int main()
{
    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect( "127.0.0.1", 55001 );

    while ( status == sf::Socket::Done )
    {
        std::cout << "Wait for your turn..." << std::endl;

        sf::Packet cardInfo;
        sf::Packet turnInfo;
        sf::Packet status;

        Card topCard;
        HandDeck hand;
        int toTake = 0;
        bool isYourTurn = false;
        std::string gameStatus = "-";
        std::string request = "-";

        socket.receive( cardInfo );
        socket.receive( turnInfo );
        socket.receive( status );

        cardInfo >> topCard;
        turnInfo >> isYourTurn;
        status >> gameStatus >> toTake >> request;

        std::cout << "\033[2J\033[1;1H";

        std::cout << "Top card: " << topCard.get() << std::endl;

        while ( !cardInfo.endOfPacket() )
        {
            Card temp;
            cardInfo >> temp;
            hand.pushBack( temp );
        }

        hand.show();

        for ( int i = 0; i < hand.size(); ++i )
        {
            std::cout << "[" << i << "] ";
        }
        std::cout << std::endl;

        while ( isYourTurn )
        {
            std::cout << "Your turn!" << std::endl;

            if ( toTake > 0 ) 
            {
                std::cout << "To take: " << toTake << std::endl;
            }
            if ( request != "-" )
                std::cout << "Request: " << request << std::endl;

            std::string which = "-";
            bool isSuccess = false;
            sf::Packet choice;
            sf::Packet result;

            std::cin >> which;
            choice << which;

            socket.send( choice );
            socket.receive( result );

            result >> isSuccess;

            if( isSuccess )
                isYourTurn = false;
        }
    }
    
    std::cout << "Error connecting to the server" << std::endl;


    return 0;
}
