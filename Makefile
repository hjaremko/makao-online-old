all: server client text-client
server: src/server.cpp src/Card.cpp src/Deck.cpp src/PlayingStack.cpp src/HandDeck.cpp src/Player.cpp src/Game.cpp
	g++ -std=c++11 src/server.cpp src/Card.cpp src/Deck.cpp src/PlayingStack.cpp src/HandDeck.cpp src/Player.cpp src/Game.cpp -o server.out -Wall -O2 -lsfml-network -lsfml-window -lsfml-system
client: src/client.cpp src/Card.cpp src/TextureCard.cpp src/TextureDeck.cpp src/Deck.cpp src/THandDeck.cpp src/HandDeck.cpp src/Player.cpp src/ChoiceMenu.cpp src/Text.cpp
	g++ -std=c++11 src/client.cpp src/Card.cpp src/TextureCard.cpp src/TextureDeck.cpp src/Deck.cpp src/THandDeck.cpp src/HandDeck.cpp src/Player.cpp src/ChoiceMenu.cpp src/Text.cpp -o client.out -Wall -O2 -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system
text-client: src/text-client.cpp src/Card.cpp src/Deck.cpp src/PlayingStack.cpp src/HandDeck.cpp src/Player.cpp
	g++ -std=c++11 src/text-client.cpp src/Card.cpp src/Deck.cpp src/PlayingStack.cpp src/HandDeck.cpp src/Player.cpp -o text-client.out -Wall -O2 -lsfml-network -lsfml-window -lsfml-system
