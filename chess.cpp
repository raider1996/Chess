#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include "Connector.hpp"
#include "TextureManager.h"
using namespace sf;

//file manager
Texture& TextureManager::getTexture(const std::string& filepath)
{
	if (textureMap.find(filepath) == textureMap.end())
	{
		Texture temptexture;
		if (temptexture.loadFromFile(filepath))
		{
			//temtexture did not load
		}
		textureMap[filepath] = temptexture;
	}
	return textureMap[filepath];
}//load unload textures, return the referance texture

void TextureManager::unloadTexture(const std::string& filepath)
{
	auto unwanted = textureMap.find(filepath);
	if (unwanted != textureMap.end())
	{
		textureMap.erase(unwanted);
	}
}
//unload textures to free memory
void TextureManager::clearAll()
{
	textureMap.clear();
}

//used to set position oF pieces
int Size = 56;


Sprite P[32];//how many pieces

//where pieces go on the board
int Board[8][8] =
{ -1,-2,-3,-4,-5,-3,-2,-1,
 -6,-6,-6,-6,-6,-6,-6,-6,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  6, 6, 6, 6, 6, 6, 6, 6,
  1, 2, 3, 4, 5, 3, 2, 1 };

//to record moves
std::string toChessNote(Vector2f F)
{
	std::string Pieces = "";
	Pieces += (F.x / Size + 97);
	Pieces += (7 - F.y / Size + 49);
	return Pieces;
}

//allows a piece to be taken 
Vector2f toCoord(char A, char B)
{
	int X = int(A) - 97;
	int Y = 7 - int(B) + 49;
	return Vector2f(X * Size, Y * Size);
}

void move(std::string str)
{
	Vector2f OldPos = toCoord(str[0], str[1]);
	Vector2f NewPos = toCoord(str[2], str[3]);

	for (int i = 0; i < 32; i++)
		if (P[i].getPosition() == NewPos) P[i].setPosition(-100, -100);

	for (int i = 0; i < 32; i++)
		if (P[i].getPosition() == OldPos) P[i].setPosition(NewPos);
}

//used for undoing a move
std::string position = "";

//set a loop to put the pieces in the correct place
void loadPosition()
{
	int k = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			int n = Board[i][j];
			if (!n) continue;
			int x = abs(n) - 1;
			int y = n > 0 ? 1 : 0;
			P[k].setTextureRect(IntRect(Size * x, Size * y, Size, Size));
			P[k].setPosition(Size * j, Size * i);
			k++;
		}
	for (int i = 0; i < position.length(); i += 5)
		move(position.substr(i, 4));

	
}

int main()

{
	// Turn on memory leak checking
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// creats a window with the size. 
	RenderWindow window(VideoMode(453, 453), "The Chess!");

	ConnectToEngine(const_cast<char*>("stockfish.exe"));

	//get image oF pieces
	Texture Figures1, Board2, Cannon1;
	Figures1.loadFromFile("images/Figures.png");
	//get image oF board
	Board2.loadFromFile("images/board.png");
	//get image of cannon
	Cannon1.loadFromFile("images/cannon.png");

	//display the pieces
	Sprite Pieces(Figures1);
	//display the board
	Sprite SBoard(Board2);
	//display the cannon
	Sprite CannonA(Cannon1);

	//start of animation
	//Sprite sprite(Texture, IntRect(0,0,30,40));

	//set the textures in the places oF the pieces
	for (int i = 0; i < 32; i++) P[i].setTexture(Figures1);
	loadPosition();



	//drag and drop is the mouse clicking and moving the object
	bool isMove = false;
	//drag and drop what is the new location oF the moved object
	float Newx = 0, Newy = 0;
	//record old position oF pieces and new position
	Vector2f OldPos, NewPos;
	std::string str;
	//drag and drop on board
	int n = 0;
	
	////creating text
	//Text text;
	//Font font; 
	//if (!font.loadFromFile("arial.ttf"))
	//{
	//	std::cout << "failed";
	//}
	//// select the font
	//text.setFont(font);
	//// set the character size
	//text.setCharacterSize(24); // in pixels, not points!
	//// set the color
	//text.setFillColor(Color::Red);
	//// set the text style
	//text.setStyle(Text::Bold | Text::Underlined);
	////set position 
	//text.setPosition(330, 10);

	////FPS varables
	//static int frames = 0;
	//static double starttime = 0;
	//static double timepassed = 0; 
	//static bool first = false;
	//static float fps = 0.0f;
	
	
	//while loop For window running
	while (window.isOpen())
	{

		////display fps
		//frames++;
		//	if (timepassed - starttime > 1000 && frames > 10)
		//{
		//	fps = (double)frames / ((timepassed - starttime)/1000);
		//	starttime = timepassed;
		//	frames = 0;
		//}
		//text.setString("FPS:" + std::to_string((int)fps));

		//get mouse position
		Vector2i Pos = Mouse::getPosition(window);

		//used to close window
		Event Open;
		while (window.pollEvent(Open))
		{
			if (Open.type == Event::Closed)
				window.close();

			//undoing a move
			if (Open.type == Event::KeyPressed)
				if (Open.key.code == Keyboard::BackSpace)
				{
					position.erase(position.length() - 6, 5); loadPosition();
				}

			//used drag and drop//
			if (Open.type == Event::MouseButtonPressed)
				if (Open.key.code == Mouse::Left)
					for (int i = 0; i < 32; i++)
						if (P[i].getGlobalBounds().contains(Pos.x, Pos.y))
						{
							isMove = true; n = i;
							Newx = Pos.x - P[i].getPosition().x;
							Newy = Pos.y - P[i].getPosition().y;
							OldPos = P[i].getPosition();
						}
			if (Open.type == Event::MouseButtonReleased)
				if (Open.key.code == Mouse::Left)
				{
					isMove = false;
					//drag and drop into the board
					Vector2f F = P[n].getPosition() + Vector2f(Size / 2, Size / 2);
					Vector2f NewPos = Vector2f(Size * int(F.x / Size), Size * int(F.y / Size));
					//print the moves
					str = toChessNote(OldPos) + toChessNote(NewPos);
					move(str); position += str + " ";
					std::cout << str << std::endl;
					P[n].setPosition(NewPos);
				}
		}

		//computer move
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			str = getNextMove(position);

			OldPos = toCoord(str[0], str[1]);
			NewPos = toCoord(str[2], str[3]);

			for (int i = 0; i < 32; i++) if (P[i].getPosition() == OldPos)n = i;

			//animation
			for (int k = 0; k < 50; k++)
			{
				Vector2f F = NewPos - OldPos;
				P[n].move(F.x / 50, F.y / 50);
				window.draw(SBoard);
				for (int i = 0; i < 32; i++) window.draw(P[i]); window.draw(P[n]);
				window.display();
			}

			move(str); position += str + " ";
			P[n].setPosition(NewPos);
		}

		if (isMove) P[n].setPosition(Pos.x - Newx, Pos.y - Newy);

		//draw window
		window.clear();
		window.draw(SBoard);
		for (int i = 0; i < 32; i++) window.draw(P[i]);
		//2window.draw(text);
		window.display();
		//timepassed = clock();
		
	}

	return 0;
}
