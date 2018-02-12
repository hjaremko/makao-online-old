#ifndef THANDDECK_H
#define THANDDECK_H

#include "TextureCard.h"
#include "TextureDeck.h"

#include <SFML/Graphics.hpp>

class Player;

class THandDeck : public TextureDeck
{
    public:
        THandDeck();
        THandDeck( int );

        void show( sf::RenderWindow& );

        bool containsMouse( int i, sf::Event& turn )
        {
            return cards_[ i ].sprite.getGlobalBounds().contains( turn.mouseButton.x, turn.mouseButton.y );
        }

        int orientation_ = 0;
    private:
};

#endif // THANDDECK_H
