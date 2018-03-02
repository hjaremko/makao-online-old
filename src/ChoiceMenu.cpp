#include "ChoiceMenu.h"
#include "constants.h"

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

        TextureCard noChoice( "x", "-" );
        pushBack( noChoice );
    }
    else if ( type_ == "ace" )
    {
        for ( std::string color : makao::cardColors )
        {
            TextureCard temp( "-", color );
            pushBack( temp );
        }
    }
} //ctor

void ChoiceMenu::show( sf::RenderWindow& window )
{   
    int i = 0;
    int j = ( makao::width - ( size() * 50 ) ) / 2;
    int k = ( makao::height - ( size() * 50 ) ) / 2;
    
    for ( TextureCard& card : cards_ )
    {
        card.assignTexture();

        if ( orientation_ == 0 )
            card.sprite.move( sf::Vector2f( j + ( i * 50 ), makao::height - 220 ) ); //520 - 220
        else
            card.sprite.move( sf::Vector2f( 50, k + ( i * 50 ) ) );

        card.draw( window );
        i++;
    }
}
