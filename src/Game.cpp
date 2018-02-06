#include "Game.h"

sf::Packet& operator<<( sf::Packet& packet, const Card& card )
{
    return packet << card.getType() << card.getColor();
}

sf::Packet& operator>>( sf::Packet& packet, Card& card )
{
    return packet >> card.type_ >> card.color_;
}

Game::Game()
{
    std::cout << "Initializing deck..." << std::endl;

    drawingDeck.fill();
    //ctor
}

Game::~Game()
{
    //dtor
}

void Game::dealOut( int amount )
{
    std::cout << "Dealing out cards..." << std::endl;

    for ( Player& i : players )
        i.getFromDeck( drawingDeck, 5 );
}

void Game::makeStack()
{
    do
    {
        stack.pushForce( drawingDeck.popBack() );
    } while ( stack.getTop().isSpecial() );
}

void Game::sendCardInfo()
{
    sf::Packet specialStatus;

    std::cout << "Sending card info to players..." << std::endl;

    for ( unsigned int i = 0; i < players.size(); ++i )
    {
        cardPacket << stack.getTop();

        for ( int j = 0; j < players[ i ].handDeck.size(); ++j )
            cardPacket << players[ i ].handDeck.get( j );

        ( i == turn ) ? turnPacket << true : turnPacket << false;
        
        specialStatus << isFight << toTake << request;

        if ( players[ i ].socket->send( cardPacket ) != sf::Socket::Done )
            std::cout << "Error sending card info!" << std::endl;

        if ( players[ i ].socket->send( turnPacket ) != sf::Socket::Done )
            std::cout << "Error sending turn info!" << std::endl;

        if ( players[ i ].socket->send( specialStatus ) != sf::Socket::Done )
            std::cout << "Error sending special info!" << std::endl;

        cardPacket.clear();
        turnPacket.clear();
        specialStatus.clear();
    }
}

bool Game::makeTurn()
{
    bool result = false;
    bool isEnd = false;
    int choosenCard = 0;
    std::string choiceMsg;
    sf::Packet choice;
    sf::Packet isSuccess;

    if ( drawingDeck.size() < 26 )
        refillDrawingDeck();
    
    while ( !result )
    {
        std::cout << "Waiting for player " << turn << std::endl;

        if ( players[ turn ].skip_ > 0 )
        {
            std::cout << "Skip!" << std::endl;
            players[ turn ].skip_--;

            if ( players[ turn ].skip_ == 0 )
                isFight = false;


            turn++;
            if ( turn == players.size() )
                turn = 0;

        }

        players[ turn ].socket->receive( choice );

        try
        {
            choice >> choiceMsg;

            if ( choiceMsg != "r" )
            {
                if ( lastThrown.getType() == "J" )
                {   
                    std::cout << "JACK!" << std::endl;
                    result = players[ turn ].getRequest( choiceMsg, request );
                    lastThrown.clear();
                }
                else
                {
                    choosenCard = std::stoi( choiceMsg );
                    result = players[ turn ].pushToStack( stack, choosenCard, lastThrown, isFight, request );

                    if ( result && turn == whoRequested_ )
                        request = '-';
                }

                if ( players[ turn ].handDeck.size() == 0 )
                    isEnd = true;
            }
            else 
            {
                if ( lastThrown.isSpecial() )
                {
                    players[ turn ].getFromDeck( drawingDeck, toTake );
                    lastThrown.clear();
                }
                else
                    players[ turn ].getFromDeck( drawingDeck, 1 );

                toTake = 0;
                isFight = false;
                result = true;
            }
        }
        catch ( const std::exception& e )
        {
            result = false;
        }

        isSuccess << result;

        if ( players[ turn ].socket->send( isSuccess ) != sf::Socket::Done )
            std::cout << "Error sending turn info!" << std::endl;

        choice.clear();
        isSuccess.clear();
    }

    executeSpecial( lastThrown );

    turn++;
    if ( turn == players.size() )
        turn = 0;

    printInfo();

    return isEnd;
}

void Game::printInfo()
{
    std::cout << "Topmost card: " << stack.getTop().get() << std::endl;
    std::cout << "Cards in drawing deck: " << drawingDeck.size() << std::endl;
    std::cout << "Cards in stack: " << stack.size() << std::endl;

    for ( Player& i : players )
    {
        std::cout << "Player has: ";
        i.handDeck.show();
    }
}

void Game::refillDrawingDeck()
{
    std::cout << "Refilling drawing deck..." << std::endl;

    Card tempTop = stack.getTop();
    stack >> drawingDeck;
    stack.pushForce( tempTop );

    drawingDeck.shuffle();
}

void Game::executeSpecial( Card last )
{
    if ( last.isSpecial() )
    {
        std::string type = stack.getTop().getType();
        std::cout << "Special card thrown! " << type << std::endl;

        switch ( type[ 0 ] )
        {
            case 'A':
                break;
            case 'K':
                break;
            case 'J':
                turn--;
                isRequest = true;
                whoRequested_ = turn;

                break;
            case '4':
                players[ turn ].skip_ += 1;
                isFight = true;

                break;
            case '3':
                toTake += 3;
                isFight = true;

                break;
            case '2':
                toTake += 2;
                isFight = true;

                break;
        }
    }
}
