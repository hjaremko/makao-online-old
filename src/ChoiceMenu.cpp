#include "ChoiceMenu.h"

ChoiceMenu::ChoiceMenu()
{

} //ctor

ChoiceMenu::ChoiceMenu( std::string type ) : type_( type )
{
    if ( type_ == "jack" )
    {
        int tmp = 5;

        for ( int i = 0; i < 6; i++ )
        {
            TextureCard temp( std::to_string( tmp + i ), "-" );
            pushBack( temp );
        }
    }
    else if ( type_ == "ace" )
    {
        std::array<std::string, 4> cardColors = { "hearts", "spades", "diamonds", "clubs" };

        for ( std::string color : cardColors )
        {
            TextureCard temp( "-", color );
            pushBack( temp );
        }

    }
} //ctor

void ChoiceMenu::show( sf::RenderWindow& window )
{   
    int i = 0;
    int j = ( 720 - ( size() * 50 ) ) / 2;
    int k = ( 520 - ( size() * 50 ) ) / 2;
    
    for ( TextureCard& card : cards_ )
    {
        card.assignTexture();

        if ( orientation_ == 0 )
            card.sprite.move( sf::Vector2f( j + ( i * 50 ), 300 ) );
        else
            card.sprite.move( sf::Vector2f( 50, k + ( i * 50 ) ) );

        card.draw( window );
        i++;
    }
}