#ifndef CHOICEMENU_H
#define CHOICEMENU_H

#include "THandDeck.h"

#include <SFML/Graphics.hpp>

class ChoiceMenu : public THandDeck
{
    public:
        ChoiceMenu();
        ChoiceMenu( std::string );

        void show( sf::RenderWindow& );

    private:
        std::string type_;
};

#endif // CHOICEMENU_H
