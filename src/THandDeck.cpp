#include "THandDeck.h"

void THandDeck::show( sf::RenderWindow& window )
{   
    int i = 1;
    for ( TextureCard& card : cards_ )
    {
        card.assignTexture();
        card.sprite.move( sf::Vector2f( i*30, 100 ) );

        card.draw( window );
        i++;
    }
}