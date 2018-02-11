#include <cmath>
#include <string>
#include <iostream>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "TextureCard.h"
#include "THandDeck.h"

const int width = 720;
const int height = 520;

sf::Packet& operator<<( sf::Packet& packet, const TextureCard& card )
{
    return packet << card.getType() << card.getColor();
}

sf::Packet& operator>>( sf::Packet& packet, TextureCard& card )
{
    return packet >> card.type_ >> card.color_;
}

bool getString( sf::Event& event, std::string& str, sf::Text& input )
{
    if ( event.type == sf::Event::TextEntered )
    {
        if ( event.text.unicode == 8 ) //backspace
        {
            std::cout << "Backspace!" << std::endl;
            str.pop_back();
        }
        else if ( event.text.unicode < 128 )
        {
            str += static_cast<char>( event.text.unicode );
            std::cout << "ASCII character typed: " << static_cast<int>( event.text.unicode ) << std::endl;
        }
        if ( event.text.unicode == 13 ) //enter
        {
            std::cout << "Enter!" << std::endl;
            return true;
        }

        input.setString( str );
    }
    return false;
}

int main()
{   
    sf::TcpSocket socket;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window( sf::VideoMode( width, height ), "Makao Online", sf::Style::Default, settings );
    sf::Color green( 40, 75, 35 );

    std::string serverIp;
    sf::Text ipInputInfo;
    sf::Font font;
    font.loadFromFile( "img/OperatorMono-Bold.otf" );

    bool isDone = false;
    sf::Text input;
    input.setFont( font );
    input.setFillColor( sf::Color::Red );


    ipInputInfo.setFont( font );
    ipInputInfo.setString( "Enter server ip address" );
    ipInputInfo.setFillColor( sf::Color::Red );

    while ( window.isOpen() )
    {
        sf::Event event;
        while ( window.pollEvent( event ) )
        {
            if ( event.type == sf::Event::Closed )
                window.close();

            if ( !isDone )
                isDone = getString( event, serverIp, input );
            else
            {
                std::cout << serverIp << std::endl;
                sf::Socket::Status status = socket.connect( serverIp, 55001 );

                if ( status != sf::Socket::Done )
                {
                    std::cout << "Connected" << std::endl;
                    isDone = false;
                    serverIp.clear();
                }
                else
                {
                    while ( status == sf::Socket::Done )
                    {
                        window.clear( green );

                        std::cout << "Wait for your turn..." << std::endl;

                        sf::Packet cardInfo;
                        sf::Packet turnInfo;
                        sf::Packet status;

                        TextureCard topCard;
                        THandDeck hand;

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

                        topCard.assignTexture();

                        std::cout << "\033[2J\033[1;1H";

                        std::cout << "Top card: " << topCard.get() << std::endl;

                        topCard.center();
                        topCard.draw( window );

                        while ( !cardInfo.endOfPacket() )
                        {
                            TextureCard temp;
                            cardInfo >> temp;
                            hand.pushBack( temp );
                        }

                        hand.show( window );
                        window.display();

                        while ( isYourTurn )
                        {
                            sf::Event turn;

                            if ( window.waitEvent( turn ) )
                            {
                                if ( turn.type == sf::Event::MouseButtonPressed )
                                {
                                    if ( turn.mouseButton.button == sf::Mouse::Left )
                                    {
                                        std::cout << "Your turn!" << std::endl;

                                        for ( int i = hand.size(); i >= 0; --i )
                                        {
                                            if ( hand.containsMouse( i, turn ) )
                                            {
                                                if ( toTake > 0 ) 
                                                    std::cout << "To take: " << toTake << std::endl;

                                                if ( request != "-" )
                                                    std::cout << "Request: " << request << std::endl;

                                                std::string which = "-";
                                                bool isSuccess = false;
                                                sf::Packet choice;
                                                sf::Packet result;

                                                which = std::to_string( i );
                                                choice << which;

                                                socket.send( choice );
                                                socket.receive( result );

                                                result >> isSuccess;

                                                if( isSuccess )
                                                    isYourTurn = false;

                                                break;
                                            }
                                        }
                                    }
                                    else if ( turn.mouseButton.button == sf::Mouse::Right )
                                    {
                                        std::string which = "-";
                                        bool isSuccess = false;
                                        sf::Packet choice;
                                        sf::Packet result;

                                        which = "r";
                                        choice << which;

                                        socket.send( choice );
                                        socket.receive( result );

                                        result >> isSuccess;

                                        if( isSuccess )
                                            isYourTurn = false;

                                        break;
                                    }
                                }

                            }
                            // window.display();
                            while ( window.pollEvent( turn ) );
                        }
                    }
                }

            }

            window.clear( green );
            window.draw( input );
            window.display();
        }
       
    } 

    return 0;
}
