#include <string>
#include <iostream>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "TextureCard.h"
#include "THandDeck.h"
#include "ChoiceMenu.h"

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
                    // std::cout << "Connected" << std::endl;
                    isDone = false;
                    serverIp.clear();
                }

                ChoiceMenu jackMenu( "jack" );
                ChoiceMenu aceMenu( "ace" );

                while( status != sf::Socket::Disconnected )
                {
                    window.clear( green );

                    std::cout << "Wait for your turn..." << std::endl;

                    sf::Packet cardInfo;
                    sf::Packet turnInfo;
                    sf::Packet statusInfo;

                    TextureCard topCard;
                    THandDeck hand( 0 );

                    int toTake = 0;
                    int amountPlayers = 0;
                    bool isYourTurn = false;
                    std::string gameStatus = "-";
                    std::string request = "-";

                    status = socket.receive( cardInfo );
                    status = socket.receive( turnInfo );
                    status = socket.receive( statusInfo );

                    cardInfo >> topCard;
                    turnInfo >> isYourTurn;
                    statusInfo >> gameStatus >> toTake >> request >> amountPlayers;

                    THandDeck otherPlayers[ amountPlayers - 1 ];

                    for ( int k = 0; k < amountPlayers - 1; ++k )
                    {
                        int tmpSize = 0;
                        statusInfo >> tmpSize;

                        for ( int l = 0; l < tmpSize; ++l )
                        {
                            TextureCard temp;
                            otherPlayers[ k ].orientation_ = k + 1;
                            otherPlayers[ k ].pushBack( temp );
                        }

                        otherPlayers[ k ].show( window );
                    }

                    std::cout << "\033[2J\033[1;1H";

                    while ( !cardInfo.endOfPacket() )
                    {
                        TextureCard temp;
                        cardInfo >> temp;
                        hand.pushBack( temp );
                    }

                    topCard.assignTexture();
                    topCard.center();
                    topCard.draw( window );
                    hand.show( window );

                    if ( toTake > 0 ) 
                        std::cout << "To take: " << toTake << std::endl;

                    if ( request != "-" )
                        std::cout << "Request: " << request << std::endl;

                    if ( gameStatus == "jack" && request == "-" && isYourTurn )
                        jackMenu.show( window );
                    else if ( gameStatus == "ace" && isYourTurn )
                        aceMenu.show( window );

                    window.display();

                    while ( isYourTurn )
                    {
                        sf::Event turn;
                        std::string which = "-";


                        if ( window.waitEvent( turn ) && turn.type == sf::Event::MouseButtonPressed )
                        {
                            std::cout << "Your turn!" << std::endl;

                            if ( turn.mouseButton.button == sf::Mouse::Left )
                            {
                                bool isSuccess = false;
                                sf::Packet choice;
                                sf::Packet result;

                                if ( gameStatus == "jack" && request == "-" )
                                {
                                    window.display();

                                    int tmp = 5;

                                    for ( int i = jackMenu.size(); i >= 0; --i )
                                    {
                                        if ( jackMenu.containsMouse( i, turn ) )
                                        {
                                            which = std::to_string( tmp + i );
                                            break;
                                        }
                                    }

                                    std::cout << which << std::endl;

                                    if ( which == "-" )
                                        continue;
                                }
                                else if ( gameStatus == "ace" )
                                {
                                    std::array<std::string, 4> cardColors = { "hearts", "spades", "diamonds", "clubs" };
                                    window.display();

                                    for ( int i = 0; i < 4; i++ )
                                    {
                                        if ( aceMenu.containsMouse( i , turn ) )
                                        {
                                            which = cardColors[ i ];
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    for ( int i = hand.size(); i >= 0; --i )
                                    {
                                        if ( hand.containsMouse( i, turn ) )
                                        {
                                            which = std::to_string( i );
                                            break;
                                        }
                                    }
                                }

                                choice << which;

                                status = socket.send( choice );
                                status = socket.receive( result );

                                result >> isSuccess;

                                if( isSuccess )
                                    isYourTurn = false;
                            }
                            else if ( turn.mouseButton.button == sf::Mouse::Right )
                            {
                                std::string which = "-";
                                bool isSuccess = false;
                                sf::Packet choice;
                                sf::Packet result;

                                which = "r";
                                choice << which;

                                status = socket.send( choice );
                                status = socket.receive( result );

                                result >> isSuccess;

                                if( isSuccess )
                                    isYourTurn = false;
                            }
                        }
                        
                        while ( window.pollEvent( turn ) );
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
