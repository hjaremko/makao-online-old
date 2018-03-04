#include <string>
#include <iostream>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "TextureCard.h"
#include "THandDeck.h"
#include "ChoiceMenu.h"
#include "Text.h"

#include "constants.h"

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
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window( sf::VideoMode( makao::width, makao::height, 32 ), "Makao Online", sf::Style::Default, settings );
    sf::Color green( 40, 75, 35 );

    bool isDone = false;

    Text ipInputInfo( "ENTER SERVER IP", 200 );
    Text serverIp( "", 160 );
    Text turnString( "YOUR TURN", 200 );
    Text takeString( "TO TAKE", 160 );
    Text skipString( "TO SKIP", makao::height / 2 - 40, 250 );
    Text requestString( "REQUEST", 160 );
    Text connectionInfo( "", 130 );

    while ( window.isOpen() )
    {
        sf::TcpSocket socket;
        sf::Event event;

        while ( window.pollEvent( event ) )
        {
            if ( event.type == sf::Event::Closed )
                window.close();

            if ( !isDone )
            {
                connectionInfo.clear();
                isDone = getString( event, serverIp );
            }
            else
            {
                sf::Socket::Status status = socket.connect( serverIp.get().toAnsiString(), 55001 );

                if ( status != sf::Socket::Done )
                {
                    connectionInfo.set( "ERROR" );
                }
                else
                {
                    connectionInfo.set( "CONNECTED" );
                    window.clear( green );
                    connectionInfo.centerDraw( window );
                    window.display();


                    int toTake = 0;
                    int toSkip = 0;
                    int amountPlayers = 0;
                    bool isYourTurn = false;
                    std::string gameStatus = "-";
                    std::string request = "-";


                    while( status != sf::Socket::Disconnected )
                    {
                        window.clear( green );

                        ChoiceMenu jackMenu( "jack" );
                        ChoiceMenu aceMenu( "ace" );

                        sf::Packet cardInfo;
                        sf::Packet turnInfo;
                        sf::Packet statusInfo;

                        THandDeck topCards( -1 );
                        THandDeck hand( 0 );

                        status = socket.receive( cardInfo );
                        status = socket.receive( turnInfo );
                        status = socket.receive( statusInfo );

                        TextureCard temp;
                        for ( int k = 0; k < 3; ++k )
                        {
                            cardInfo >> temp;
                            if ( temp.getType() != "-" )
                                topCards.pushBack( temp );
                        }

                        turnInfo >> isYourTurn;
                        statusInfo >> gameStatus >> toTake >> toSkip >> request >> amountPlayers;

                        takeString.set( std::to_string( toTake ) );
                        skipString.set( std::to_string( toSkip ) );
                        requestString.set( request );

                        THandDeck otherPlayers[ amountPlayers - 1 ];

                        for ( int k = 0; k < amountPlayers - 1; ++k )
                        {
                            int tmpSize = 0;
                            statusInfo >> tmpSize;

                            for ( int l = 0; l < tmpSize; ++l )
                            {
                                TextureCard temp;
                                otherPlayers[ k ].orientation = k + 1;
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

                        topCards.show( window );
                        hand.show( window );

                        if ( toTake > 0 ) 
                            takeString.centerDraw( window );

                        if ( toSkip > 0 )
                            skipString.draw( window );

                        if ( request != "-" )
                            requestString.centerDraw( window );

                        if ( gameStatus == "jack" && request == "-" && isYourTurn )
                            jackMenu.show( window );
                        else if ( gameStatus == "ace" && isYourTurn )
                            aceMenu.show( window );

                        if ( isYourTurn )
                            turnString.centerDraw( window );

                        window.display();

                        while ( isYourTurn )
                        {
                            std::string which = "-";

                            if ( window.waitEvent( event ) && event.type == sf::Event::MouseButtonPressed )
                            {
                                if ( event.mouseButton.button == sf::Mouse::Left )
                                {
                                    bool isSuccess = false;
                                    sf::Packet choice;
                                    sf::Packet result;

                                    if ( gameStatus == "jack" && request == "-" )
                                    {
                                        int tmp = 5;

                                        for ( int i = jackMenu.size(); i >= 0; --i )
                                        {
                                            if ( jackMenu.containsMouse( i, event ) )
                                            {
                                                which = std::to_string( tmp + i );
                                                break;
                                            }
                                        }

                                        if ( which == "-" )
                                            continue;
                                    }
                                    else if ( gameStatus == "ace" )
                                    {
                                        for ( int i = 0; i < 4; i++ )
                                        {
                                            if ( aceMenu.containsMouse( i , event ) )
                                            {
                                                which = makao::cardColors[ i ];
                                                break;
                                            }

                                            for ( int i = hand.size(); i >= 0; --i )
                                            {
                                                if ( hand.containsMouse( i, event ) )
                                                {
                                                    which = std::to_string( i );
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    else
                                    {
                                        for ( int i = hand.size(); i >= 0; --i )
                                        {
                                            if ( hand.containsMouse( i, event ) )
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
                                else if ( event.mouseButton.button == sf::Mouse::Right )
                                {
                                    bool isSuccess = false;
                                    std::string which = "-";
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

                            while ( window.pollEvent( event ) );
                        }
                    }
                }

                serverIp.clear();
                isDone = false;
            }

            window.clear( green );
            ipInputInfo.centerDraw( window );
            serverIp.centerDraw( window );
            connectionInfo.centerDraw( window );
            window.display();
        }
    } 

    return 0;
}
