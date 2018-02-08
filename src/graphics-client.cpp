#include <cmath>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window( sf::VideoMode( 320, 240 ), "Makao Online" );

    while( window.isOpen() )
    {
        sf::Event event;
        while( window.pollEvent( event ) )
        {
            if( event.type == sf::Event::Closed )
                 window.close();
           
        } //while
        window.clear();
    
       
        window.display();
    } 

    return 0;
}