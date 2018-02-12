#include "TextureCard.h"

const int width = 720;
const int height = 520;

TextureCard::TextureCard()
{
    //ctor
}

TextureCard::TextureCard( std::string type, std::string color )
{   
    type_ = type;
    color_ = color;

    switch ( type_[ 0 ] )
    {
        case 'A':
        case 'K':
        case 'Q':
        case 'J':
        case '4':
        case '3':
        case '2':
            isSpecial_ = true;
            break;
        default:
            isSpecial_ = false;
    }

    std::string textureSource;

    if ( type != "-" && color != "-" )
        textureSource = "img/" + this->get() + ".png";
    else
        textureSource = "img/back.png";

    this->texture.loadFromFile( textureSource.c_str() );
    texture.setSmooth( true );
    sprite.setTexture( texture );
    sprite.scale( sf::Vector2f( 0.2f, 0.2f ) );
} //ctor

void TextureCard::draw( sf::RenderWindow& window )
{
    window.draw( sprite );
}

void TextureCard::assignTexture()
{
    std::string textureSource;

    if ( type_ != "-" && color_ != "-" )
        textureSource = "img/" + this->get() + ".png";
    else
        textureSource = "img/back.png";

    texture.loadFromFile( textureSource.c_str() );
    texture.setSmooth( true );
    sprite.setTexture( texture );
    sprite.scale( sf::Vector2f( 0.2f, 0.2f ) );
}

void TextureCard::center()
{
    sprite.setPosition( sf::Vector2f( ( width  - 100 ) / 2.0f, ( height - 175 ) / 2.0f ) );
}
