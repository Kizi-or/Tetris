#include "Blocks.h"

Blocks::Blocks()
{
	sprite.setTextureRect(sf::IntRect(30,0,30,30));
	state = StateOfBlock::BOARD;
	sprite.setColor(sf::Color::Transparent);
	isHooked = false;
	x = 0;
	y = 0;
}
sf::Sprite Blocks::GetSprite()
{
	sf::Sprite copy = this -> sprite;
	return copy;
}
void Blocks::SetSpriteTexture(sf::Texture& texture)
{
	this->sprite.setTexture(texture);
	this->sprite.setColor(sf::Color::White);
	this->state = StateOfBlock::BLOCK;
}
void Blocks::SetSpritePosition(float x,float y)
{
	this->sprite.setPosition(x,y);
}
void Blocks::SetSpriteTransparent()
{
	this->sprite.setColor(sf::Color::Transparent);
}
bool Blocks::IsBlockState() 
{
	if (this->state == StateOfBlock::BLOCK)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Blocks::SetActiveState()
{
	this->state = StateOfBlock::BLOCK;
}
void Blocks::SetUnactiveState()
{
	this->state = StateOfBlock::BOARD;
}
void Blocks::MakeCoordinates(int x, int y)
{
	this->x = x;
	this->y = y;
}
bool Blocks::IsHooked()
{
	return isHooked;
}
void Blocks::ChangeIsHooked()
{
	this->isHooked = !isHooked;
}