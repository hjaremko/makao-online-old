#ifndef TEXTUREDECK_H
#define TEXTUREDECK_H

#include <vector>

#include "TextureCard.h"

class TextureDeck
{
    public:
        TextureDeck();
        virtual ~TextureDeck();

        TextureCard popBack();
        int size();
        void pushBack( TextureCard );
        void shuffle();
        void fill();
        void clear();

    protected:
        std::vector<TextureCard> cards_;
};

#endif // TEXTUREDECK_H
