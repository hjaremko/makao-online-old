#ifndef TEXTURECARD_H
#define TEXTURECARD_H

#include <iostream>
#include <string>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "Card.h"

class TextureCard : public Card
{
    public:
        friend sf::Packet& operator>>( sf::Packet&, TextureCard& );

        TextureCard();
        TextureCard( std::string, std::string );
        void draw( sf::RenderWindow& );
        void assignTexture();
        void center();

        bool operator==( TextureCard& card )
        {   
            return ( this->type_ == card.getType() ) || ( this->color_ == card.getColor() );
        }

        sf::Sprite sprite;
        
    private:
        sf::Texture texture;
};

#endif // TEXTURECARD_H
