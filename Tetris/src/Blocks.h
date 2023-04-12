#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>


enum class StateOfBlock{ BOARD,BLOCK };

class Blocks
{
private:
	sf::Sprite sprite ;
	bool isHooked;
public:
	StateOfBlock state;
	int x, y;

	Blocks();
	~Blocks()=default;
	sf::Sprite GetSprite();
	void SetSpriteTexture(sf::Texture& texture);
	void SetSpritePosition(float x, float y);
	void SetSpriteTransparent();
	bool IsBlockState();
	void SetActiveState();
	void SetUnactiveState();
	void MakeCoordinates(int x, int y);
	bool IsHooked();
	void ChangeIsHooked();
};

