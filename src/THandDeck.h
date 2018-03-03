#ifndef THANDDECK_H
#define THANDDECK_H

#include "TextureCard.h"
#include "TextureDeck.h"

#include <SFML/Graphics.hpp>

class THandDeck : public TextureDeck
{
    public:
        THandDeck();
        THandDeck( int );

        void show( sf::RenderWindow& );
        bool containsMouse( int, sf::Event& );

        int orientation = 0;
};

#endif // THANDDECK_H
