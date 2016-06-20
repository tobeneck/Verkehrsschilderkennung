#include "Button.hpp"

Button::Button() {

}
Button::Button(std::string words, sf::Vector2f location) {
	clickedTexture.loadFromFile(std::string("../Data/Buttons/buttonKlicked.jpg"));//Buttons/buttonKlicked.jpg
	unClickedTexture.loadFromFile(std::string("../Data/Buttons/buttonUnKlicked.jpg"));
	normal.setPosition(sf::Vector2f(location.x , location.y ));
	clicked.setPosition(sf::Vector2f(location.x , location.y ));
	normal.setTexture(unClickedTexture);
	clicked.setTexture(clickedTexture);

	current = false;

	text = sf::Text();
	font.loadFromFile(std::string("../Data/Font/arial.ttf"));
	text.setFont(font);
	text.setString(words);
	text.setCharacterSize(14);
	text.setPosition(location.x, location.y + unClickedTexture.getSize().y / 2 - text.getLocalBounds().height /2);
	text.setPosition(location.x + unClickedTexture.getSize().x / 2 - text.getLocalBounds().width / 2, text.getPosition().y);
	text.setColor(sf::Color::Black);
	
}
void Button::checkClick(sf::Vector2i mousePos) {
	float a = normal.getTexture()->getSize().y;
	float b = normal.getTexture()->getSize().x;

	if (mousePos.x > normal.getPosition().x && mousePos.x<(normal.getPosition().x + b)) {
		if (mousePos.y > normal.getPosition().y && mousePos.y<(normal.getPosition().y + a)) {
			setState(!current);
		}
	}
}
void Button::setState(bool which) {
	current = which;
}
void Button::setText(std::string words) {
	text.setString(words);
}
bool Button::getVar() {
	return current;
}
sf::Sprite Button::getSprite() {
	if (current)
		return clicked;
	else
		return normal;
}

sf::Text  Button::getText() {
	return text;
}

void Button::draw(sf::RenderWindow &window) {
	normal.setTexture(unClickedTexture);
	clicked.setTexture(clickedTexture);
	text.setFont(font);

	if (current)
		window.draw(clicked);
	else
		window.draw(normal);
	window.draw(text);
}