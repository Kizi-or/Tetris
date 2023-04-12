#include "Blocks.h"
#include <vector>
#include <time.h>
#include <list>
#include "ModeOfBlock.h"

//Filling return our board with full of blocks that are all transparent and unactive
std::vector<std::vector<Blocks*>> Filling(const int sizeX,const int sizeY)
{
	std::vector<std::vector<Blocks*>> board;
	for (int i = 0; i < sizeX; i++)
	{

		board.push_back(std::vector<Blocks*>());
		for (int j = 0; j < sizeY; j++)
		{
			board[i].push_back(new Blocks);
		}
	}
	return board;
}
//Return memory from heap 
void Reset(std::vector<std::vector<Blocks*>> board)
{
	for (int i = 0; i < board.size()-1; i++)
	{
		for (int j = 0; j < board[i].size()-1; j++)
		{
			delete board[i][j];
		}
		board[i].clear();
	}
}
//Config positions for all blocks 
void Config(std::vector<std::vector<Blocks*>>& board,const int sizeX,const int sizeY)
{
	for (int i = 0, l = 30; i < sizeX; i++, l += 30)
	{
		for (int j = 0, k = 30; j < sizeY; j++, k += 30)
		{
			board[i][j]->SetSpritePosition(l, k);
			board[i][j]->MakeCoordinates(i, j);
		}
	}
}
//IsFloor check for case when hooked blocks will reach the lowest level
void IsFloor(Blocks* hook[], std::vector<std::vector<Blocks*>>& board, const int sizeX, const int sizeY)
{
	for (int i = 0; i < 4; i++)
	{
		if (hook[i]->y == sizeY - 1)
		{
			for (int j = 0; j < 4; j++)
			{
				hook[j]->ChangeIsHooked();
				hook[j] = nullptr;
			}
			break;
		}
	}
}
//IsLeftWall check for left stop
bool IsLeftWall(Blocks* hook[], std::vector<std::vector<Blocks*>>& board)
{
	for (int i = 0; i < 4; i++)
	{
		if (hook[i]->x == 0)
		{
			return true;
		}
	}
	return false;
}
//IsRightWall check for right stop
bool IsRightWall(Blocks* hook[], std::vector<std::vector<Blocks*>>& board)
{
	for (int i = 0; i < 4; i++)
	{
		if (hook[i]->x == 9)
		{
			return true;
		}
	}
	return false;
}
//Stop hooked figure in case of touch active block from board
void IsStack(Blocks* hook[], std::vector<std::vector<Blocks*>>& board)
{
	for (int i = 0; i < 4; i++)
	{
		int previousX = hook[i]->x;
		int previousY = hook[i]->y;
		if (board[previousX][previousY + 1]->IsBlockState() && !board[previousX][previousY + 1]->IsHooked())
		{
			
			for (int j = 0; j < 4; j++)
			{
				hook[j]->ChangeIsHooked();
				hook[j] = nullptr;
			}
			break;
		}
	}
}
//Return true when first element of hook is set on nullptr
bool IsHookEmpty(Blocks* hook[])
{
	return hook[0] == nullptr ? true : false;
}
//Move hooked figure left
void SetLeft(Blocks* hook[], std::vector<std::vector<Blocks*>>& board,sf::Texture& texture)
{
	std::list<int> listOfAll;
	for (int i = 0; i < 4; i++)
	{
		listOfAll.push_back(hook[i]->x);
	}
	listOfAll.sort();
	listOfAll.unique();
	for (std::list<int>::iterator it = listOfAll.begin(); it != listOfAll.end(); it++)
	{
		for (int j = 0; j < 4; j++)
		{
			int previousX = hook[j]->x;
			int previousY = hook[j]->y;
			if (board[*it][previousY]->x == hook[j]->x)
			{
				hook[j]->SetSpriteTransparent();
				hook[j]->SetUnactiveState();
				hook[j]->ChangeIsHooked();
				hook[j] = board[previousX -1][previousY];
				hook[j]->SetSpriteTexture(texture);
				hook[j]->ChangeIsHooked();
			}
		}
	}
}
//Check for collision with active blocks from board on left side
bool IsStackLeft(Blocks* hook[], std::vector<std::vector<Blocks*>>& board)
{
	for (int i = 0; i < 4; i++)
	{
		int previousX = hook[i]->x;
		int previousY = hook[i]->y;
		if (board[previousX - 1][previousY]->IsBlockState() && !board[previousX - 1][previousY]->IsHooked())
		{
			return false;
		}
	}
	return true;
}
//Move hooked figure right
void SetRight(Blocks* hook[], std::vector<std::vector<Blocks*>>& board, sf::Texture& texture)
{
	std::list<int> listOfAll;
	for (int i = 0; i < 4; i++)
	{
		listOfAll.push_back(hook[i]->x);
	}
	listOfAll.sort();
	listOfAll.unique();
	for (std::list<int>::reverse_iterator it = listOfAll.rbegin(); it != listOfAll.rend(); it++)
	{
		for (int j = 0; j < 4; j++)
		{
			int previousX = hook[j]->x;
			int previousY = hook[j]->y;
			if (board[*it][previousY]->x == hook[j]->x)
			{
				hook[j]->SetSpriteTransparent();
				hook[j]->SetUnactiveState();
				hook[j]->ChangeIsHooked();
				hook[j] = board[previousX + 1][previousY];
				hook[j]->SetSpriteTexture(texture);
				hook[j]->ChangeIsHooked();
			}
		}
	}
}
//Check for collision with active blocks from board on right side
bool IsStackRight(Blocks* hook[], std::vector<std::vector<Blocks*>>& board)
{
	for (int i = 0; i < 4; i++)
	{
		int previousX = hook[i]->x;
		int previousY = hook[i]->y;
		if (board[previousX + 1][previousY]->IsBlockState() && !board[previousX + 1][previousY]->IsHooked())
		{
			return false;
		}
	}
	return true;
}
//Get 3x3 martix with center of hooked figure and check for posibility to rotate
bool RotationCondition(Blocks* hook[], std::vector<std::vector<Blocks*>>& board)
{
	Blocks temp[3][3];
	Blocks background[3][3];
	int centerX = hook[1]->x;
	int centerY = hook[1]->y;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (centerX - 1 + j < 0 || centerX - 1 + j >= 10 || centerY - 1 + j < 0 || centerY - 1 + j >= 17)
			{
				std::cout << "false" << "\n";
				return false;
			}
			temp[i][j] = *board[centerX-1+j][centerY-1+i];
			if (!temp[i][j].IsHooked())
			{
				temp[i][j].SetSpriteTransparent();
				temp[i][j].SetUnactiveState();
			}
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			background[i][j] = *board[centerX - 1 + j][centerY - 1 + i];
			if (background[i][j].IsHooked())
			{
				background[i][j].SetSpriteTransparent();
				background[i][j].SetUnactiveState();
			}
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = i; j < 3; j++)
		{
			Blocks temporaryBlock = temp[i][j];
			temp[i][j] = temp[j][i];
			temp[j][i] = temporaryBlock;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		int low = 0, high = 2;
		while (low < high)
		{
			Blocks temporaryBlock = temp[i][low];
			temp[i][low] = temp[i][high];
			temp[i][high] = temporaryBlock;
			low++;
			high--;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (temp[i][j].IsBlockState() && background[i][j].IsBlockState())
			{
				return false;
			}
		}
	}
	return true;
}
//Get 4x4 martix with center of hooked figure and check for posibility to rotate
bool IRotationCondition(Blocks* hook[], std::vector<std::vector<Blocks*>>& board,int dx)
{
	Blocks temp[4][4];
	Blocks background[4][4];
	int startX = 0;
	int startY = 0;
	switch (dx)
	{
	case 0:
		startX = hook[1]->x - 1;
		startY = hook[1]->y - 1;
		break;
	case 1:
		startX = hook[1]->x - 2;
		startY = hook[1]->y - 1;
		break;
	case 2:
		startX = hook[2]->x - 2;
		startY = hook[2]->y - 2;
		break;
	case 3:
		startX = hook[2]->x - 1;
		startY = hook[2]->y - 2;
		break;
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (startX + j < 0 || startX + j >= 10 || startY + j < 0 || startY + j >= 17)
			{
				std::cout << "false" << "\n";
				return false;
			}
			temp[i][j] = *board[startX + j][startY + i];
			if (!temp[i][j].IsHooked())
			{
				temp[i][j].SetSpriteTransparent();
				temp[i][j].SetUnactiveState();
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			background[i][j] = *board[startX + j][startY + i];
			if (background[i][j].IsHooked())
			{
				background[i][j].SetSpriteTransparent();
				background[i][j].SetUnactiveState();
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = i; j < 4; j++)
		{
			Blocks temporaryBlock = temp[i][j];
			temp[i][j] = temp[j][i];
			temp[j][i] = temporaryBlock;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		int low = 0, high = 3;
		while (low < high)
		{
			Blocks temporaryBlock = temp[i][low];
			temp[i][low] = temp[i][high];
			temp[i][high] = temporaryBlock;
			low++;
			high--;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (temp[i][j].IsBlockState() && background[i][j].IsBlockState())
			{
				return false;
			}
		}
	}
	return true;
}
//Get 3x3 martix and rotate 90 degrees
void Rotation(Blocks* hook[], std::vector<std::vector<Blocks*>>& board,sf::Texture& texture)
{
	Blocks temp[3][3];
	int centerX = hook[1]->x;
	int centerY = hook[1]->y;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp[i][j] = *board[centerX - 1 + j][centerY - 1 + i];
			if (!temp[i][j].IsHooked())
			{
				temp[i][j].SetSpriteTransparent();
				temp[i][j].SetUnactiveState();
			}
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = i; j < 3; j++)
		{
			Blocks temporaryBlock = temp[i][j];
			temp[i][j] = temp[j][i];
			temp[j][i] = temporaryBlock;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		int low = 0, high = 2;
		while (low < high)
		{
			Blocks temporaryBlock = temp[i][low];
			temp[i][low] = temp[i][high];
			temp[i][high] = temporaryBlock;
			low++;
			high--;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[centerX - 1 + j][centerY - 1 + i]->IsHooked())
			{
				board[centerX - 1 + j][centerY - 1 + i]->SetSpriteTransparent();
				board[centerX - 1 + j][centerY - 1 + i]->SetUnactiveState();
				board[centerX - 1 + j][centerY - 1 + i]->ChangeIsHooked();
			}
			if (temp[i][j].IsBlockState())
			{
				std::cout << "X"<<" ";
			}
			else {
				std::cout << "O"<<" ";
			}
		}
		std::cout << "\n";
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (temp[i][j].IsBlockState())
			{
				board[centerX - 1 + j][centerY - 1 + i]->SetSpriteTexture(texture);
				board[centerX - 1 + j][centerY - 1 + i]->ChangeIsHooked();
			}
		}
	}
	int count = 0;
	hook[1] = board[centerX][centerY];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (temp[i][j].IsBlockState()&&!(centerX - 1 + j == centerX && centerY == centerY - 1 + i))
			{
				hook[count]=board[centerX - 1 + j][centerY - 1 + i];
				count++;
				if (count == 1)
				{
					count++;
				}
			}
		}
	}
}
//Get 4x4 martix and rotate 90 degrees
//This function change dx and remember state of I figure to losing hooked blocks
void IRotation(Blocks* hook[], std::vector<std::vector<Blocks*>>& board, sf::Texture& texture, int& dx)
{
	Blocks temp[4][4];
	int startX = 0;
	int startY = 0;
	switch (dx)
	{
	case 0:
		startX = hook[1]->x - 1;
		startY = hook[1]->y - 1;
		break;
	case 1:
		startX = hook[1]->x - 2;
		startY = hook[1]->y - 1;
		break;
	case 2:
		startX = hook[1]->x - 1;
		startY = hook[1]->y - 2;
		break;
	case 3:
		startX = hook[1]->x - 1;
		startY = hook[1]->y - 1;
		break;
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp[i][j] = *board[startX + j][startY + i];
			if (!temp[i][j].IsHooked())
			{
				temp[i][j].SetSpriteTransparent();
				temp[i][j].SetUnactiveState();
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = i; j < 4; j++)
		{
			Blocks temporaryBlock = temp[i][j];
			temp[i][j] = temp[j][i];
			temp[j][i] = temporaryBlock;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		int low = 0, high = 3;
		while (low < high)
		{
			Blocks temporaryBlock = temp[i][low];
			temp[i][low] = temp[i][high];
			temp[i][high] = temporaryBlock;
			low++;
			high--;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[startX + j][startY + i]->IsHooked())
			{
				board[startX + j][startY + i]->SetSpriteTransparent();
				board[startX + j][startY + i]->SetUnactiveState();
				board[startX + j][startY + i]->ChangeIsHooked();
			}
			if (temp[i][j].IsBlockState())
			{
				std::cout << "X" << " ";
			}
			else {
				std::cout << "O" << " ";
			}
		}
		std::cout << "\n";
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (temp[i][j].IsBlockState())
			{
				board[startX + j][startY + i]->SetSpriteTexture(texture);
				board[startX + j][startY + i]->ChangeIsHooked();
			}
		}
	}
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (temp[i][j].IsBlockState())
			{
				hook[count] = board[startX + j][startY + i];
				count++;
			}
		}
	}
	dx++;
	if (dx == 4)
		dx = 0;
}
//Move figure to lower level
//This function change states of blocks that are hooked, next get one position lower blocks and make them active
void Move(Blocks* hook[], std::vector<std::vector<Blocks*>>& board,sf::Texture& texture)
{
	std::list<int> listOfAll;
	for (int i = 0; i < 4; i++)
	{
		listOfAll.push_back(hook[i]->y);
	}
	listOfAll.sort();
	listOfAll.unique();
	for (std::list<int>::reverse_iterator it = listOfAll.rbegin(); it != listOfAll.rend(); it++)
	{
		for (int j = 0; j < 4; j++)
		{
			int previousX = hook[j]->x;
			int previousY = hook[j]->y;
			if (board[previousX][*it]->y == hook[j]->y)
			{
				hook[j]->SetSpriteTransparent();
				hook[j]->SetUnactiveState();
				hook[j]->ChangeIsHooked();
				hook[j] = board[previousX][previousY + 1];
				hook[j]->SetSpriteTexture(texture);
				hook[j]->ChangeIsHooked();
			}
		}
	}
}
//Check for horizontal line of active blocks
void Filter(std::vector<std::vector<Blocks*>>& board,sf::Texture& texture)
{
	for (int j=0;j<18;j++) 
	{
		int limit = 0;
		for (int i = 9; i >= 0; i--)
		{
			if (board[i][j]->IsBlockState())
				limit++;
		}
		if (limit == 10)
		{
			for (int i = 9; i >= 0; i--)
			{
				board[i][j]->SetSpriteTransparent();
				board[i][j]->SetUnactiveState();
			}
			for (int k = j; k > 0; k--)
			{
				for (int i = 9; i >= 0; i--)
				{
					if (board[i][k - 1]->IsBlockState())
					{
						board[i][k - 1]->SetSpriteTransparent();
						board[i][k - 1]->SetUnactiveState();
						board[i][k]->SetSpriteTexture(texture);
						board[i][k]->SetActiveState();
					}
				}
			}
		}
	}
}
//Check for finish game when active blocks that are not hooked reach first horizontal line of board
void IsGameOver(std::vector<std::vector<Blocks*>>& board, bool& gameOver)
{
	for (int i = 0; i < 10; i++)
	{
		if (board[i][1]->IsBlockState())
		{
			gameOver = true;
		}
	}
}
int main() {
	sf::RenderWindow window(sf::VideoMode(360, 600), "Tetris");
	const int sizeX = 10;
	const int sizeY = 18;
	sf::Texture texture;
	sf::Texture background;
	sf::Sprite backgroundSprite;
	texture.loadFromFile("res/blocks.png");
	background.loadFromFile("res/background.jpg");
	backgroundSprite.setTextureRect(sf::IntRect(100, 160, 360, 600));
	backgroundSprite.setTexture(background);
	//Create board
	std::vector<std::vector<Blocks*>> board = Filling(sizeX,sizeY);
	//Configurate board
	Config(board, sizeX, sizeY);
	//Create hook for holding specific figure, all declared by nullptr that means there is no figure right now
	Blocks* hook[4] = { nullptr };
	ModeOfBlock mode = ModeOfBlock::O;
	srand(time(NULL));
	sf::Clock clock;
	//Direction of I figure
	int dx = 0;
	float timer = 0;
	bool gameOver = false;

	while (window.isOpen()) 
	{
		sf::Event event;
		float delay = 0.6;
		int random = rand() % 7;
		float times = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += times;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				Reset(board);
				window.close();
			}
			// Up key to change direction of figures
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !IsHookEmpty(hook))
			{
				//Specific case I figure
				if (mode == ModeOfBlock::I && IRotationCondition(hook,board,dx))
				{
					IRotation(hook, board, texture, dx);
				}
				//There is nothing to do for O figure
				else if(mode == ModeOfBlock::O)
				{
					continue;
				}
				//Rest of rotation cases
				else if(!(mode == ModeOfBlock::I) && RotationCondition(hook, board))
				{
					Rotation(hook, board, texture);
				}
			}
			// Left key to move left
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !IsLeftWall(hook,board) && IsStackLeft(hook, board) && !IsHookEmpty(hook))
			{
				SetLeft(hook,board,texture);
			}
			// Right key to move right
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !IsRightWall(hook, board) && IsStackRight(hook, board) && !IsHookEmpty(hook))
			{
				SetRight(hook, board, texture);
			}
			// Down key to move faster
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !IsHookEmpty(hook))
			{
				delay = 0.05;
			}
		}
		if (timer > delay)
		{
			timer = 0;
			//Case detect floor and clear hook that is holding our figure
			if (!IsHookEmpty(hook))
			{
				IsFloor(hook, board, sizeX, sizeY);
			}
			//Case detect collision and clear hook that is holding our figure
			if (!IsHookEmpty(hook))
			{
				IsStack(hook, board);
			}
			//Move
			if (!IsHookEmpty(hook))
			{
				Move(hook,board,texture);
			}
			//Hook gets new random figure when is empty
			//In this case we need to check if game is not over or any line is filled by blocks
			if (IsHookEmpty(hook))
			{
				Filter(board,texture);
				IsGameOver(board, gameOver);
				if (gameOver)
				{
					Reset(board);
					window.close();
				}
				switch (static_cast<ModeOfBlock>(random))
				{
				case ModeOfBlock::I:
					hook[0] = board[3][1];
					hook[1] = board[4][1];
					hook[2] = board[5][1];
					hook[3] = board[6][1];
					break;
				case ModeOfBlock::J:
					hook[0] = board[3][1];
					hook[1] = board[4][1];
					hook[2] = board[5][1];
					hook[3] = board[3][0];
					break;
				case ModeOfBlock::L:
					hook[0] = board[3][1];
					hook[1] = board[4][1];
					hook[2] = board[5][1];
					hook[3] = board[5][0];
					break;
				case ModeOfBlock::O:
					hook[0] = board[4][1];
					hook[1] = board[5][1];
					hook[2] = board[4][0];
					hook[3] = board[5][0];
					break;
				case ModeOfBlock::S:
					hook[0] = board[3][1];
					hook[1] = board[4][1];
					hook[2] = board[4][0];
					hook[3] = board[5][0];
					break;
				case ModeOfBlock::T:
					hook[0] = board[3][1];
					hook[1] = board[4][1];
					hook[2] = board[5][1];
					hook[3] = board[4][0];
					break;
				case ModeOfBlock::Z:
					hook[0] = board[5][1];
					hook[1] = board[4][1];
					hook[2] = board[3][0];
					hook[3] = board[4][0];
					break;
				}
				for (int i = 0; i < 4; i++)
				{
					hook[i]->SetSpriteTexture(texture);
					hook[i]->ChangeIsHooked();
				}
				dx = 0;
				mode = static_cast<ModeOfBlock>(random);
			}
		}
		window.clear(sf::Color::White);
		window.draw(backgroundSprite);
		for (int i = 0; i < sizeX; i++)
		{
			for (int j = 0; j < sizeY; j++)
			{
				window.draw(board[i][j]->GetSprite());
			}
		}
		window.display();
	}
	return 0;
}