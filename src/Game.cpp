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
} //ctor

Game::~Game()
{
    std::cout << "Disconnecting players..." << std::endl;

    for ( unsigned int i = 0; i < players.size(); ++i )
        players[ i ].socket->disconnect();
} //dtor

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

sf::Socket::Status Game::sendCardInfo()
{
    sf::Packet specialStatus;

    std::cout << "Sending card info to players..." << std::endl;

    for ( unsigned int i = 0; i < players.size(); ++i )
    {
        cardPacket << stack.getTop();

        for ( int j = 0; j < players[ i ].handDeck.size(); ++j )
            cardPacket << players[ i ].handDeck.get( j );

        ( i == turn && players[ i ].toSkip_ == 0 ) ? turnPacket << true : turnPacket << false;
        
        specialStatus << gameStatus << toTake << players[ i ].toSkip_ << request << static_cast<int>( players.size() );

        for ( unsigned int j = 0; j < players.size(); ++j )
        {
            if ( j != i )
                specialStatus << players[ j ].handDeck.size();
        }

        status = players[ i ].socket->send( cardPacket );
        status = players[ i ].socket->send( turnPacket );
        status = players[ i ].socket->send( specialStatus );

        cardPacket.clear();
        turnPacket.clear();
        specialStatus.clear();
    }

    return status;
}


bool Game::pushToStack( int which, int by )
{
    if ( stack.pushBack( players[ turn ].handDeck.get( which ), gameStatus, request, lastThrown, by, turn ) )
    {
        lastThrown = stack.getTop();
        players[ turn ].handDeck.cards_.erase( players[ turn ].handDeck.cards_.begin() + which );
    }
    else
        return false;

    players[ turn ].hasMakao_ = ( players[ turn ].handDeck.size() == 1 ) ? true : false;

    return true;
}

void Game::throwCard( std::string choiceMsg )
{
    int choosenCard = std::stoi( choiceMsg );
    result = pushToStack( choosenCard, lastThrownBy );

    if ( result && turn == whoRequested_ && gameStatus == "jack" && turn != lastThrownBy )
    {
        std::cout << "End of request" << std::endl;

        request = "-";
        gameStatus = "-";
        whoRequested_ = -1;
    }

    if ( result )
        lastThrownBy = turn;
}

bool Game::makeTurn()
{
    bool isEnd = false;
    bool nextTurn = false;
    std::string choiceMsg;
    sf::Packet choice;
    sf::Packet isSuccess;
    result = false;

    if ( drawingDeck.size() < 26 )
        refillDrawingDeck();

    if ( players[ turn ].toSkip_ > 0 )
    {
        std::cout << "Skipping" << std::endl;

        nextTurn = true;
        players[ turn ].toSkip_--;
    }
    else
    {
        while ( !result && status == sf::Socket::Done )
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
                        std::cout << "Setting request" << std::endl;

                        result = players[ turn ].getRequest( choiceMsg, request );
                        lastThrown.clear();

                        if ( !result )
                        {
                            gameStatus = "-";
                            result = true;
                        }
                    }
                    else if ( gameStatus == "ace" )
                    {
                        std::cout << "Setting new color" << std::endl;

                        result = stack.getTop().setColor( choiceMsg );
                        lastThrown.clear();

                        if ( result )
                            gameStatus = "-";
                        else
                            throwCard( choiceMsg );
                    }
                    else
                    {
                        throwCard( choiceMsg );
                    }

                    if ( players[ turn ].handDeck.size() == 0 )
                        isEnd = true;
                }
                else
                {
                    if ( toTake > 0 && lastThrownBy != turn )
                    {
                        players[ turn ].getFromDeck( drawingDeck, toTake );
                        toTake = 0;
                        gameStatus = "-";
                        lastThrown.clear();
                    }
                    else if ( toSkip > 0 && lastThrownBy != turn )
                    {
                        toSkip--;
                        players[ turn ].toSkip_ = toSkip;
                        toSkip = 0;
                        gameStatus = "-";
                        lastThrown.clear();
                    }
                    else if ( gameStatus != "skip" && lastThrownBy != turn )
                        players[ turn ].getFromDeck( drawingDeck, 1 );

                    if ( turn == lastThrownBy && gameStatus == "jack" && request == "-" )
                    {
                        gameStatus = "-";
                        lastThrown.clear();
                    }
                    else if ( turn == whoRequested_ && gameStatus == "jack" && turn != lastThrownBy )
                    {
                        std::cout << "End of request" << std::endl;

                        request = "-";
                        gameStatus = "-";
                        whoRequested_ = -1;
                    }

                    result = true;
                    nextTurn = true;
                    lastThrownBy = -1;
                }
            }
            catch ( const std::exception& e )
            {
                result = false;
            }

            isSuccess << result;

            status = players[ turn ].socket->send( isSuccess );

            choice.clear();
            isSuccess.clear();
        }

        executeSpecial( lastThrown );
    }

    // printInfo();

    if ( nextTurn )
        ++( *this );

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
    std::cout << "To take: " << toTake << std::endl;
    std::cout << "To skip: " << toSkip << std::endl;

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
                break;
            }
            case 'K':
            {
                if ( last.getColor() == "hearts" )
                {
                    toTake += 5;
                    gameStatus = "fight";
                    last.clear();
                }
                else if ( last.getColor() == "spades" )
                {
                    toTake += 5;
                    gameStatus = "fight";
                    last.clear();

                    turn--;
                    if ( turn < 0 )
                        turn = players.size() - 1;

                    turn--;
                    if ( turn < 0 )
                        turn = players.size() - 1;
                }

                break;
            }
            case 'J':
            {
                whoRequested_ = turn;
                gameStatus = "jack";
                request = "-";

                break;
            }
            case '4':
            {
                toSkip++;
                gameStatus = "skip";
                last.clear();

                break;
            }
            case '3':
            {
                toTake += 3;
                gameStatus = "fight";
                last.clear();

                break;
            }
            case '2':
            {
                toTake += 2;
                gameStatus = "fight";
                last.clear();

                break;
            }
        }
    }
}
