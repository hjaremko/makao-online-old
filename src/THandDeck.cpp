#include "THandDeck.h"

THandDeck::THandDeck()
{

} //ctor

THandDeck::THandDeck( int orientation ) : orientation_( orientation )
{

} //ctor

void THandDeck::show( sf::RenderWindow& window )
{   
    int i = 0;
    int j = ( 720 - ( ( size() - 1 ) * 30 + 100 ) ) / 2;
    int k = ( 520 - ( ( size() - 1 ) * 30 + 175 ) ) / 2;
    
    for ( TextureCard& card : cards_ )
    {
        card.assignTexture();

        if ( orientation_ == 0 )
            card.sprite.move( sf::Vector2f( j + ( i * 30 ), 350 ) );
        else
            card.sprite.move( sf::Vector2f( 50, k + ( i * 30 ) ) );

        card.draw( window );
        i++;
    }
}
