#ifndef TEXT_H
#define TEXT_H

#include <iostream>

class Text
{
    public:
        Text( std::string, int );
        Text( std::string, int, int );
        virtual ~Text();

        void draw( sf::RenderWindow& );
        void centerDraw( sf::RenderWindow& );
        void set( std::string );
        void center();
        void clear();
        sf::String get();

    private:
        int y_;
        int x_;
        // std::string string_;
        sf::Text text_;
        sf::Font font_;
};

#endif // TEXT_H
