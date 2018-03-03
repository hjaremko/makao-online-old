#include "THandDeck.h"
#include "constants.h"

THandDeck::THandDeck()
{

} //ctor

THandDeck::THandDeck( int o ) : orientation( o )
{

} //ctor

void THandDeck::show( sf::RenderWindow& window )
{   
    int i = 0;
    int j = ( makao::width - ( ( size() - 1 ) * 30 + 100 ) ) / 2;
    int k = ( makao::height - ( ( size() - 1 ) * 30 + 175 ) ) / 2;
    
    for ( TextureCard& card : cards_ )
    {
        card.assignTexture();

        if ( orientation == 0 )
            card.sprite.move( sf::Vector2f( j + ( i * 30 ), makao::height - 170 ) );
        else if ( orientation == 1 )
            card.sprite.move( sf::Vector2f( 50, k + ( i * 30 ) ) );
        else if ( orientation == 2 )
            card.sprite.move( sf::Vector2f( j + ( i * 30 ), 10 ) );
        else if ( orientation == 3 )
            card.sprite.move( sf::Vector2f( makao::width - 150, k + ( i * 30 ) ) );
        else if ( orientation == -1 )
            card.sprite.move( sf::Vector2f( j + ( i * 30 ), ( makao::height - 175 ) / 2.0f ) );

        card.draw( window );
        i++;
    }
}

bool THandDeck::containsMouse( int i, sf::Event& turn )
{
    return cards_[ i ].sprite.getGlobalBounds().contains( turn.mouseButton.x, turn.mouseButton.y );
}
