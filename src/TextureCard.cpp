#include "TextureCard.h"
#include "constants.h"

TextureCard::TextureCard()
{
    sprite.scale( sf::Vector2f( 0.2f, 0.2f ) );
} //ctor

TextureCard::TextureCard( std::string textureSource )
{
    textureSource = "img/" + textureSource + ".png";

    this->texture.loadFromFile( textureSource.c_str() );
    texture.setSmooth( true );
    sprite.setTexture( texture );
} //ctor

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

    assignTexture();
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
    else if ( type_ != "-" && color_ == "-" ){
        textureSource = "img/" + this->getType() + ".png";
        sprite.scale( sf::Vector2f( 1.25f, 1.25f ) );
    }
    else if ( type_ == "-" && color_ != "-" ){
        textureSource = "img/" + this->getColor() + ".png";
        sprite.scale( sf::Vector2f( 1.25f, 1.25f ) );
    }
    else
        textureSource = "img/back.png";

    texture.loadFromFile( textureSource.c_str() );
    texture.setSmooth( true );
    sprite.setTexture( texture );
}

void TextureCard::center()
{
    sprite.setPosition( sf::Vector2f( ( makao::width  - 100 ) / 2.0f, ( makao::height - 175 ) / 2.0f ) );
}
