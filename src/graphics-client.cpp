#include <string>
#include <iostream>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "TextureCard.h"
#include "THandDeck.h"
#include "ChoiceMenu.h"
#include "Text.h"

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

bool getString( sf::Event& event, Text& str )
{
    if ( event.type == sf::Event::TextEntered )
    {
        if ( event.text.unicode == 8 ) //backspace
        {
            if ( str.get().getSize() > 0 )
            {
                sf::String tmp = str.get();
                tmp.erase( str.get().getSize() - 1 );
                str.set( tmp );
            }
        }
        else if ( event.text.unicode == 13 ) //enter
        {
            return true;
        }
        else if ( event.text.unicode < 128 )
        {
            std::string tmp = str.get();
            tmp += static_cast<char>( event.text.unicode );
            str.set( tmp );
        }
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

    bool isDone = false;

    Text ipInputInfo( "ENTER SERVER IP", 200 );
    Text serverIp( "", 160 );
    Text turnString( "YOUR TURN", 200 );
    Text takeString( "TO TAKE", 160 );
    Text requestString( "REQUEST", 160 );

    while ( window.isOpen() )
    {
        sf::Event event;
        while ( window.pollEvent( event ) )
        {
            if ( event.type == sf::Event::Closed )
                window.close();

            if ( !isDone )
            {
                isDone = getString( event, serverIp );
            }
            else
            {
                sf::Socket::Status status = socket.connect( serverIp.get().toAnsiString(), 55001 );

                if ( status != sf::Socket::Done )
                {
                    isDone = false;
                    serverIp.set( "" );
                    window.clear( green );
                    continue;
                }

                ChoiceMenu jackMenu( "jack" );
                ChoiceMenu aceMenu( "ace" );

                while( status != sf::Socket::Disconnected )
                {
                    window.clear( green );

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

                    takeString.set( std::to_string( toTake ) );
                    requestString.set( request );

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
                        takeString.draw( window );

                    if ( request != "-" )
                        requestString.draw( window );

                    if ( gameStatus == "jack" && request == "-" && isYourTurn )
                        jackMenu.show( window );
                    else if ( gameStatus == "ace" && isYourTurn )
                        aceMenu.show( window );

                    if ( isYourTurn )
                        turnString.draw( window );

                    window.display();

                    while ( isYourTurn )
                    {
                        sf::Event turn;
                        std::string which = "-";

                        if ( window.waitEvent( turn ) && turn.type == sf::Event::MouseButtonPressed )
                        {
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
            ipInputInfo.draw( window );
            serverIp.draw( window );
            window.display();
        }
    } 

    return 0;
}
