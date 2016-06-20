#include "SFML\Graphics.hpp"

class Button {
public:
	Button(std::string, sf::Vector2f location);
	Button();
	void checkClick(sf::Vector2i);
	void setState(bool);
	void setText(std::string);
	bool getVar();
	void draw(sf::RenderWindow & window);
	sf::Sprite getSprite();
	sf::Text  getText();
private:

	sf::Texture unClickedTexture;
	sf::Texture clickedTexture;
	sf::Sprite normal;
	sf::Sprite clicked;
	sf::Text text;
	bool current;
	sf::Font font;
};