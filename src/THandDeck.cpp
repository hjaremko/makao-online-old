#include "THandDeck.h"

void THandDeck::show( sf::RenderWindow& window )
{   
    int i = 0;
    int j = ( 720 - ( ( size() - 1 ) * 30 + 100 ) ) / 2;
    for ( TextureCard& card : cards_ )
    {
        card.assignTexture();
        card.sprite.move( sf::Vector2f( j + ( i * 30 ), 350 ) );

        card.draw( window );
        i++;
    }
}
