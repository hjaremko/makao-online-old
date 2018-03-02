#include <iostream>

#include <SFML/Graphics.hpp>

#include "Text.h"
#include "constants.h"

Text::Text( std::string string, int y ) : y_( y )
{   
    font_.loadFromFile( "img/OperatorMono-Bold.otf" );
    text_.setFont( font_ );
    text_.setString( string );
    text_.setFillColor( sf::Color::White );
    text_.setOutlineThickness( 1 );

    center();
} //ctor

Text::Text( std::string string, int y, int x ) : y_( y ), x_( x )
{
    font_.loadFromFile( "img/OperatorMono-Bold.otf" );
    text_.setFont( font_ );
    text_.setString( string );
    text_.setFillColor( sf::Color::White );
    text_.setOutlineThickness( 1 );

    text_.setOrigin( 0, 0 );
    text_.setPosition( sf::Vector2f( x_, y_ ) );
} //ctor

Text::~Text()
{
    
} //dtor

void Text::draw( sf::RenderWindow& window )
{
    window.draw( text_ );
}

void Text::centerDraw( sf::RenderWindow& window )
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
    text_.setPosition( sf::Vector2f( makao::width / 2.0f, makao::height / 2.0f ) );
}

void Text::clear()
{
    text_.setString( "" );
}

sf::String Text::get()
{
    return text_.getString();
}