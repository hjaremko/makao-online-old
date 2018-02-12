all: server client g-client
server: src/server.cpp src/Card.cpp src/Deck.cpp src/PlayingStack.cpp src/HandDeck.cpp src/Player.cpp src/Game.cpp
	g++ -std=c++11 src/server.cpp src/Card.cpp src/Deck.cpp src/PlayingStack.cpp src/HandDeck.cpp src/Player.cpp src/Game.cpp -o server.out -Wall -O2 -lsfml-network -lsfml-window -lsfml-system
client: src/client.cpp src/Card.cpp src/Deck.cpp src/PlayingStack.cpp src/HandDeck.cpp src/Player.cpp
	g++ -std=c++11 src/client.cpp src/Card.cpp src/Deck.cpp src/PlayingStack.cpp src/HandDeck.cpp src/Player.cpp -o client.out -Wall -O2 -lsfml-network -lsfml-window -lsfml-system
g-client: src/graphics-client.cpp src/Card.cpp src/TextureCard.cpp src/TextureDeck.cpp src/Deck.cpp src/PlayingStack.cpp src/THandDeck.cpp src/HandDeck.cpp src/Player.cpp src/ChoiceMenu.cpp
	g++ -std=c++11 src/graphics-client.cpp src/Card.cpp src/TextureCard.cpp src/TextureDeck.cpp src/Deck.cpp src/PlayingStack.cpp src/THandDeck.cpp src/HandDeck.cpp src/Player.cpp src/ChoiceMenu.cpp -o graphics-client.out -Wall -O2 -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system
