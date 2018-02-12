#include <iostream>

#include <SFML/Graphics.hpp>

#include "Text.h"

Text::Text( std::string string, int y ) : y_( y )
{   
    font_.loadFromFile( "img/OperatorMono-Bold.otf" );
    text_.setFont( font_ );
    text_.setString( string );
    text_.setFillColor( sf::Color::White );

    center();
} //ctor

Text::~Text()
{
    
} //dtor

void Text::draw( sf::RenderWindow& window )
{   
    center();
    window.draw( text_ );
}

void Text::set( std::string str )
{
    text_.setString( str );
}

void Text::center()
{
    sf::FloatRect textRect = text_.getLocalBounds();
    text_.setOrigin( textRect.left + textRect.width / 2.0f, y_ );
    text_.setPosition( sf::Vector2f( 720 / 2.0f, 520 / 2.0f ) );
}

sf::String Text::get()
{
    return text_.getString();
}