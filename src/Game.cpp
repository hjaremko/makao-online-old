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

        ( i == turn && players[ i ].skip_ <= 1 ) ? turnPacket << true : turnPacket << false;

        if ( players[ i ].skip_ > 0 )
            players[ i ].skip_--;
        
        specialStatus << gameStatus << toTake << request << static_cast<int>( players.size() );

        for ( int j = 0; j < players.size(); j++ )
        {
            if ( j != i )
                specialStatus << players[ j ].handDeck.size();
        }

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

        players[ turn ].socket->receive( choice );

        try
        {
            choice >> choiceMsg;

            if ( choiceMsg != "r" )
            {
                if ( gameStatus == "jack" && request == "-" )
                {   
                    std::cout << "Waiting for request" << std::endl;

                    result = players[ turn ].getRequest( choiceMsg, request );
                    lastThrown.clear();
                }
                else if ( gameStatus == "ace" )
                {   
                    std::cout << "Waiting for new color" << std::endl;

                    result = players[ turn ].getRequest( choiceMsg, newColor );
                    stack.getTop().setColor( newColor );
                    lastThrown.clear();
                    gameStatus = "-";
                }
                else
                {
                    choosenCard = std::stoi( choiceMsg );
                    result = players[ turn ].pushToStack( stack, choosenCard, lastThrown, gameStatus, request, newColor );

                    if ( result && turn == whoRequested_ && gameStatus == "jack" ) 
                    {
                        std::cout << "End of request" << std::endl;

                        request = "-";
                        gameStatus = "-";
                        whoRequested_ = -1;
                    }
                }

                if ( players[ turn ].handDeck.size() == 0 )
                    isEnd = true;
            }
            else 
            {
                if ( gameStatus == "fight" )
                {
                    players[ turn ].getFromDeck( drawingDeck, toTake );
                    lastThrown.clear();
                }
                else if ( gameStatus != "skip" )
                    players[ turn ].getFromDeck( drawingDeck, 1 );

                if ( gameStatus == "fight" || gameStatus == "skip" || ( turn == whoRequested_ && gameStatus == "jack" ) )
                {
                    request = "-";
                    gameStatus = "-";
                    whoRequested_ = -1;
                }

                toTake = 0;
                result = true;
            }
        }
        catch ( const std::exception& e )
        {
            result = false;
        }

        isSuccess << result;

        if ( players[ turn ].socket->send( isSuccess ) != sf::Socket::Done )
            std::cout << "Error sending success info!" << std::endl;

        choice.clear();
        isSuccess.clear();
    }

    executeSpecial( lastThrown );
    printInfo();

    return isEnd;
}

void Game::printInfo()
{
    std::cout << "Topmost card: " << stack.getTop().get() << std::endl;
    std::cout << "Cards in drawing deck: " << drawingDeck.size() << std::endl;
    std::cout << "Cards in stack: " << stack.size() << std::endl;
    std::cout << "Game status: " << gameStatus << std::endl;
    std::cout << "Jack: " << request << std::endl;
    std::cout << "Ace: " << newColor << std::endl;

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

void Game::executeSpecial( Card& last )
{
    if ( last.isSpecial() )
    {
        std::string type = stack.getTop().getType();
        std::cout << "Special card thrown! " << type << std::endl;

        switch ( type[ 0 ] )
        {
            case 'A':
            {
                gameStatus = "ace";

                sendCardInfo();
                makeTurn();

                break;
            }
            case 'K':
                break;
            case 'J':
            {
                whoRequested_ = turn;
                gameStatus = "jack";

                sendCardInfo();
                makeTurn();

                break;
            }
            case '4':
            {
                unsigned int nextPlayer = turn + 1;
                if ( nextPlayer == players.size() )
                    nextPlayer = 0;


                players[ nextPlayer ].skip_++;
                gameStatus = "skip";

                last.clear();

                break;
            }
            case '3':
            {
                toTake += 3;
                gameStatus = "fight";

                break;
            }
            case '2':
            {
                toTake += 2;
                gameStatus = "fight";

                break;
            }
        }
    }
}
