#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>

#include <SFML/Network.hpp>

class TextureCard;

class Card
{
    public:
        Card();
        Card( std::string, std::string );
        virtual ~Card();

        friend sf::Packet& operator>>( sf::Packet&, Card& );
        friend sf::Packet& operator>>( sf::Packet&, TextureCard& );
        friend TextureCard;

        std::string getType() const;
        std::string getColor() const;
        std::string get() const;
        bool isSpecial() const;
        bool setColor( std::string );
        void clear();

        bool operator==( const Card& card )
        {   
            return ( type_ == card.getType() ) || ( color_ == card.getColor() );
        }

        bool operator<( const Card& card )
        {
            return type_ < card.getType();
        }

    private:
        std::string type_ = "-";
        std::string color_ = "-";
        bool isSpecial_;
};

#endif // CARD_H
