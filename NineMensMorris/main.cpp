#include <iostream>
#include <SFML\Graphics.hpp>
#include <time.h>
#include <thread>
#include <chrono>
#include <string>
#include <map>
#include <iterator>
#include <vector>
#include <cmath>

using namespace sf;

/*
Class Positions
every figure on the board is an object of the class
using vectors we add multiple figures on the board
*/
class Positions {
public:
	float posX;
	float posY;
	char c;
	static int numOfFiguresW;
	static int numOfFiguresB;
	Positions(char player) : posX(375), posY(375), c(player) {
		if (player == 'w')
			numOfFiguresW++;
		if (player == 'b')
			numOfFiguresB++;
	}
};

/*
function processes the 12 critical points (since the squares are not equal)
used to set the directions that have a special amount of pixels to reach the other point
*/
void checkPosition(float x, float y, int& square, std::string& criticalDirection) {
	//first square
	if (x == 375 && y == 35) {
		criticalDirection = "Down";
		square = 1;
	}
	else if (x == 35 && y == 375) {
		criticalDirection = "Right";
		square = 1;
	}
	else if (x == 375 && y == 715) {
		criticalDirection = "Up";
		square = 1;
	}
	else if (x == 715 && y == 375) {
		criticalDirection = "Left";
		square = 1;
	}
	//second square
	else if (x == 150 && y == 375) {
		criticalDirection = "LeftRight";
		square = 2;
	}
	else if (x == 375 && y == 150) {
		criticalDirection = "UpDown";
		square = 2;
	}
	else if (x == 375 && y == 600) {
		criticalDirection = "UpDown";
		square = 2;
	}
	else if (x == 600 && y == 375) {
		criticalDirection = "LeftRight";
		square = 2;
	}
	//third square
	else if (x == 375 && y == 265) {
		criticalDirection = "Up";
		square = 3;
	}
	else if (x == 265 && y == 375) {
		criticalDirection = "Left";
		square = 3;
	}
	else if (x == 375 && y == 485) {
		criticalDirection = "Down";
		square = 3;
	}
	else if (x == 485 && y == 375) {
		criticalDirection = "Right";
		square = 3;
	}
	else
		criticalDirection = "none";
}

/*
parameters are coordinates of the moving object
return value is the square it is in
*/
int determineSquare(float x, float y) {
	if (x == 35 || y == 35 || x == 715 || y == 715)
		return 1;
	if (x == 150 || y == 150 || x == 600 || y == 600)
		return 2;
	if (x == 265 || y == 265 || x == 485 || y == 485)
		return 3;
	return 0;
}

/*
returns true if youre trying to go back into the middle
processes the four point you can enter from
*/
bool requestingMiddle(bool up, bool down, bool left, bool right, std::vector<Positions>& first, int i) {
	if (first[i].posX == 375 && first[i].posY == 265 && down)
		return true;
	if (first[i].posX == 265 && first[i].posY == 375 && right)
		return true;
	if (first[i].posX == 375 && first[i].posY == 485 && up)
		return true;
	if (first[i].posX == 485 && first[i].posY == 375 && left)
		return true;
	return false;
}

/*
function used to move around the board
@param min, max -> boundaries for each square so you cannot jump from squares or exit the play area
@param shift -> sets the amount of pixels for wanted movement
@param cD -> processed from the function /checkPosition/
@param up, down, left, right -> chosen direction
*/
void choosePosition(Event event, int i, Sprite s, bool right, bool left, bool up, bool down, std::vector<Positions>& first, int square, float& shift, std::vector<Sprite> sp, RenderWindow& window, std::string cD) {
	//set limits
	int max = 0;
	int min = 0;
	switch (square) {
	case 1: if (cD == "none") { max = 600; min = 150; }
			else { max = 600; min = 150; } break;
	case 2: if (cD == "none") { max = 500; min = 200; }
			else { max = 600; min = 150; } break;
	case 3: if (cD == "none") { max = 400; min = 300; }
			else { max = 600; min = 150; } break;
	default:std::cout << "Something went wrong" << std::endl;
	}
	if (event.type == Event::KeyPressed) {
		if (event.key.code == Keyboard::Right)
			right = true;
		if (event.key.code == Keyboard::Left)
			left = true;
		if (event.key.code == Keyboard::Up)
			up = true;
		if (event.key.code == Keyboard::Down)
			down = true;
	}
	if (right && first[i].posX <= max && !requestingMiddle(up, down, left, right, first, i)) {
		switch (square) {							//set the number of pixels you move
		case 1: shift = 340; break;
		case 2: shift = 225; break;
		case 3: shift = 110; break;
		default: std::cout << "Something went wrong";
		}
		if (cD.find("Right") != std::string::npos)
			shift = 115;
		for (float j = 0; j < shift; j += (shift / 10)) {
			first[i].posX += (shift / 10);
			sp[i].setPosition(first[i].posX, first[i].posY);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			window.draw(s);
			window.draw(sp[i]);
			window.display();
		}
	}
	if (left && first[i].posX >= min && !requestingMiddle(up, down, left, right, first, i)) {
		switch (square) {							//set the number of pixels you move
		case 1: shift = 340; break;
		case 2: shift = 225; break;
		case 3: shift = 110; break;
		default: std::cout << "Something went wrong";
		}
		if (cD.find("Left") != std::string::npos)
			shift = 115;
		for (float j = 0; j < shift; j += (shift / 10)) {
			first[i].posX -= (shift / 10);
			sp[i].setPosition(first[i].posX, first[i].posY);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			window.draw(s);
			window.draw(sp[i]);
			window.display();
		}
	}
	if (down && first[i].posY <= max && !requestingMiddle(up, down, left, right, first, i)) {
		switch (square) {							//set the number of pixels you move
		case 1: shift = 340; break;
		case 2: shift = 225; break;
		case 3: shift = 110; break;
		default: std::cout << "Something went wrong";
		}
		if (cD.find("Down") != std::string::npos)
			shift = 115;
		for (float j = 0; j < shift; j += (shift / 10)) {
			first[i].posY += (shift / 10);
			sp[i].setPosition(first[i].posX, first[i].posY);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			window.draw(s);
			window.draw(sp[i]);
			window.display();
		}
	}
	if (up && first[i].posY >= min && !requestingMiddle(up, down, left, right, first, i)) {
		switch (square) {							//set the number of pixels you move
		case 1: shift = 340; break;
		case 2: shift = 225; break;
		case 3: shift = 110; break;
		default: std::cout << "Something went wrong";
		}
		if (cD.find("Up") != std::string::npos)
			shift = 115;
		for (float j = 0; j < shift; j += (shift / 10)) {
			first[i].posY -= (shift / 10);
			sp[i].setPosition(first[i].posX, first[i].posY);
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			window.draw(s);
			window.draw(sp[i]);
			window.display();
		}
	}
	left = false;
	right = false;
	up = false;
	down = false;
	float x = first[i].posX;
	float y = first[i].posY;
	square = determineSquare(x, y);
}

/*
returns true if you can place your figure in the spot (if spot is not occupied)
*/
bool spotIsAvailable(std::vector<Positions> figures, float x, float y, int i) {
	if (x == 375 && y == 375)	//cannot commit position -> middle
		return false;
	if (i > 0)
		for (size_t j = 0; j < figures.size() - 1; j++)
			if (figures[j].posX == x && figures[j].posY == y)
				return false;
	return true;
}

/*
this function is called when you have a "mill"
determines wether you are selecting an opponent
return values ->	-2		if you selected your own figure
					-1		if you selected nothing
					int		the enemies figures' position in vector
*/
int ifEnemy(std::vector<Positions>& figures, float x, float y, char player) {
	for (size_t i = 0; i < figures.size() - 1; i++) {
		if (figures[i].c != player && figures[i].posX == x && figures[i].posY == y) {
			if (player == 'b')
				return i;
			else
				return i;
		}
		if (figures[i].c == player && figures[i].posX == x && figures[i].posY == y)
			return -2;
	}
	return -1;
}

/*
switches the turns
*/
void playerSwitch(char& p) {
	(p == 'b') ? p = 'w' : p = 'b';
}

/*
checks wether you have a "mill"
does four checks	->horizontally current square
					->vertically current square
					->horizontally the connections between the squares
					->vertically the connections between the squares
*/
bool placedThreeTogether(std::vector<Positions>& figures, float x, float y, char player, int square, std::string cD) {
	int shift = 0;
	switch (square) {							//set the number of pixels you move
	case 1: shift = 340; break;
	case 2: shift = 225; break;
	case 3: shift = 110; break;
	default: std::cout << "Something went wrong";
	}
	int counter = 0;
	//horizontal
	if (shift != 115)
		for (size_t i = 0; i < figures.size() - 1; i++) {
			if (figures[i].posX + shift == x && figures[i].posY == y && figures[i].c == player)
				counter++;
			if (figures[i].posX + 2 * shift == x && figures[i].posY == y && figures[i].c == player)
				counter++;
			if (figures[i].posX - shift == x && figures[i].posY == y && figures[i].c == player)
				counter++;
			if (figures[i].posX - 2 * shift == x && figures[i].posY == y && figures[i].c == player)
				counter++;
		}
	//vertical
	if (counter != 2) {
		counter = 0;
		if (shift != 115)
			for (size_t i = 0; i < figures.size() - 1; i++) {
				if (figures[i].posY + shift == y && figures[i].posX == x && figures[i].c == player)
					counter++;
				if (figures[i].posY + 2 * shift == y && figures[i].posX == x && figures[i].c == player)
					counter++;
				if (figures[i].posY - shift == y && figures[i].posX == x && figures[i].c == player)
					counter++;
				if (figures[i].posY - 2 * shift == y && figures[i].posX == x && figures[i].c == player)
					counter++;
			}
	}
	shift = 115;
	//horizontal short
	if (counter != 2) {
		counter = 0;
		for (size_t i = 0; i < figures.size() - 1; i++) {
			if (figures[i].posX + shift == x && figures[i].posY == y && figures[i].c == player)
				counter++;
			if (figures[i].posX + 2 * shift == x && figures[i].posY == y && figures[i].c == player)
				counter++;
			if (figures[i].posX - shift == x && figures[i].posY == y && figures[i].c == player)
				counter++;
			if (figures[i].posX - 2 * shift == x && figures[i].posY == y && figures[i].c == player)
				counter++;
		}
	}
	//vertical short
	if (counter != 2) {
		counter = 0;
		for (size_t i = 0; i < figures.size() - 1; i++) {
			if (figures[i].posY + shift == y && figures[i].posX == x && figures[i].c == player)
				counter++;
			if (figures[i].posY + 2 * shift == y && figures[i].posX == x && figures[i].c == player)
				counter++;
			if (figures[i].posY - shift == y && figures[i].posX == x && figures[i].c == player)
				counter++;
			if (figures[i].posY - 2 * shift == y && figures[i].posX == x && figures[i].c == player)
				counter++;
		}
	}
	if (counter == 2)
		return true;
	return false;
}

/*
returns the index of the figure in a vector
@param x, y -> coordinates of the object
*/
int getFigureIndex(std::vector<Positions>& figures, float x, float y) {
	for (size_t i = 0; i < figures.size() - 1; i++)
		if (figures[i].posX == x && figures[i].posY == y)
			return i;
	return NULL;
}

/*
checks if moving your figure is legal
for loop -> checks if there is a player on the spor you want to occupy
if conditions -> checks if you have only moved to an adjacent point
*/
bool ifLegalMove(std::vector<Positions>& figures, int i, float prevX, float prevY) {
	for (size_t j = 0; j < figures.size(); j++)
		if (j != i)
			if (figures[i].posX == figures[j].posX && figures[i].posY == figures[j].posY)
				return false;
	if ((abs(figures[i].posX - prevX) == 115 && figures[i].posY - prevY == 0) || (abs(figures[i].posX - prevX) == 110 && figures[i].posY - prevY == 0) || (abs(figures[i].posX - prevX) == 340 && figures[i].posY - prevY == 0) || (abs(figures[i].posX - prevX) == 225 && figures[i].posY - prevY == 0))
		return true;
	if ((abs(figures[i].posY - prevY) == 115 && figures[i].posX - prevX == 0) || (abs(figures[i].posY - prevY) == 110 && figures[i].posX - prevX == 0) || (abs(figures[i].posY - prevY) == 340 && figures[i].posX - prevX == 0) || (abs(figures[i].posY - prevY) == 225 && figures[i].posX - prevX == 0))
		return true;
	return false;
}

/*
sets properties of wanted text
*/
void setDefaultFont(Text& t) {
	t.setCharacterSize(40);
	t.setFillColor(Color::Black);
	t.setStyle(sf::Text::Bold);
}

//static declarations
int Positions::numOfFiguresW = 0;
int Positions::numOfFiguresB = 0;

int main()
{
	RenderWindow window(VideoMode(800, 800), "Nine mans morris!");

	Texture t;							//background
	t.loadFromFile("images/m.png");
	Sprite s(t);

	Texture blackF;						//black figures
	blackF.loadFromFile("images/black.png");
	Sprite blackFigure(blackF);

	Texture whiteF;						//white figures
	whiteF.loadFromFile("images/white.png");
	Sprite whiteFigure(whiteF);

	Texture grayF;						//gray figure
	grayF.loadFromFile("images/gray.png");
	Sprite grayFigure(grayF);

	Font font;							//setting up the text
	font.loadFromFile("fonts/arialn.ttf");
	Text instructions;
	Text restart;
	Text turn;
	Text action;
	Text winner;

	setDefaultFont(instructions);
	setDefaultFont(restart);
	setDefaultFont(turn);
	setDefaultFont(action);

	instructions.setFont(font);
	instructions.setString("Movement: arrow keys");
	instructions.setPosition(20, 745);

	restart.setFont(font);
	restart.setString("Restart: R");
	restart.setPosition(620, 745);

	turn.setFont(font);
	turn.setString("Player 1");
	turn.setPosition(75, 0);

	action.setFont(font);
	action.setPosition(400, 0);

	winner.setFont(font);
	winner.setCharacterSize(100);
	winner.setFillColor(Color::Black);
	winner.setPosition(150, 325);
	winner.setStyle(sf::Text::Bold);
	winner.setString("");

	int square = 3;		//start in the inner most square
	int i = 0;			//how many players on the field
	int majorI = 0;		//how many total figures played
	int temp;
	float prevX;
	float prevY;
	float shift = 0;	//number of pixels you shift, initialization

	std::string criticalDirection = "";
	std::vector<Positions> figures;
	std::vector<Sprite> pictures;

	bool right = false;
	bool left = false;
	bool up = false;
	bool down = false;

	bool pounding = false;			//taking away oponents figure
	bool secondStage = false;
	bool characterChosen = false;
	bool caught = false;
	bool placeNew = false;
	bool moveThePlayer = false;

	char player = 'b';					//black goes first
	figures.push_back(Positions('b'));
	pictures.push_back(blackFigure);

	while (window.isOpen())
	{
		if (player == 'b')
			turn.setString("Player 1");
		else
			turn.setString("Player 2");

		window.clear();
		window.draw(s);
		window.draw(action);
		window.draw(turn);
		window.draw(instructions);
		window.draw(restart);
		window.draw(winner);

		Event event;
		while (window.pollEvent(event))
		{
			window.draw(winner);
			if (Positions::numOfFiguresB < 3 && i>10)
				winner.setString("Player 2 wins!");
			if (Positions::numOfFiguresW < 3 && i>10)
				winner.setString("Player 1 wins!");

			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::R) {
					figures.clear();						//clear out the vectors
					pictures.clear();						//
					Positions::numOfFiguresW = 0;			//nobody has figures when restarting
					Positions::numOfFiguresB = 0;			//
					i = 0;
					majorI = 0;
					player = 'b';							//black goes first
					figures.push_back(Positions('b'));		//
					pictures.push_back(blackFigure);		//
					winner.setString("");					//reset in case the game ended
					secondStage = false;					//reset in case you entered second stage then restarted
					break;
				}

			if (!secondStage) {									//if still in first stage

				if (pounding)
					action.setString("Remove opponents figure");
				else
					action.setString("Place figure (Enter)");

				choosePosition(event, figures.size() - 1, s, right, left, up, down, figures, square, shift, pictures, window, criticalDirection);
				checkPosition(figures[figures.size() - 1].posX, figures[figures.size() - 1].posY, square, criticalDirection);

				//if enter is pressed and spot is available, place it
				if (event.key.code == 13 && i < 18 && spotIsAvailable(figures, figures[figures.size() - 1].posX, figures[figures.size() - 1].posY, i) && !pounding) {

					//but if you got 3 in a row
					if (placedThreeTogether(figures, figures[figures.size() - 1].posX, figures[figures.size() - 1].posY, player, square, criticalDirection)) {
						figures.push_back(Positions('g'));		//create a gray figure
						pictures.push_back(grayFigure);
						pounding = true;
					}

					//if youre not deleting a character
					if (!pounding) {
						i++;										//next turn
						playerSwitch(player);						//switch players
						if (i == 18) {								//if its time for phase 2
							secondStage = true;						//commence phase 2
							figures.push_back(Positions('g'));		//create gray figure
							pictures.push_back(grayFigure);
						}
						else if (player == 'b') {					//create appropriate figure
							figures.push_back(Positions('b'));
							pictures.push_back(blackFigure);
						}
						else {
							figures.push_back(Positions('w'));
							pictures.push_back(whiteFigure);
						}
					}
				}

				if (event.key.code == 13 && pounding) {				//if youre deleting a figure
					int del = ifEnemy(figures, figures[figures.size() - 1].posX, figures[figures.size() - 1].posY, player);		//find the index

					if (del != -1 && del != -2) {					//if its an enemy you selected
						figures.erase(figures.begin() + del);		//delete the enemy
						pictures.erase(pictures.begin() + del);		//
						if (player == 'w')							//remove the enemies figure count
							Positions::numOfFiguresB--;
						else
							Positions::numOfFiguresW--;
						i++;										//go for next turn
						playerSwitch(player);
						pounding = false;
						if (i == 18) {
							secondStage = true;
							figures.push_back(Positions('g'));
							pictures.push_back(grayFigure);
						}
						else if (player == 'b') {
							figures.push_back(Positions('b'));
							pictures.push_back(blackFigure);
						}
						else {
							figures.push_back(Positions('w'));
							pictures.push_back(whiteFigure);
						}
						figures.erase(figures.begin() + figures.size() - 1);			//delete the gray figure
						pictures.erase(pictures.begin() + figures.size() - 1);			//
						figures[figures.size() - 1].posX = 375;
						figures[figures.size() - 1].posY = 375;
						pictures[pictures.size() - 1].setPosition(figures[figures.size() - 1].posX, figures[figures.size() - 1].posY);
					}
				}
			}

			//if second phase
			else {
				if (!pounding) {					//if not taking away opponents figures
					if (!moveThePlayer) {
						action.setString("Select figure to move");
						choosePosition(event, figures.size() - 1, s, right, left, up, down, figures, square, shift, pictures, window, criticalDirection);
						checkPosition(figures[figures.size() - 1].posX, figures[figures.size() - 1].posY, square, criticalDirection);
					}
					if (moveThePlayer) {
						action.setString("Move the figure");
						choosePosition(event, temp, s, right, left, up, down, figures, square, shift, pictures, window, criticalDirection);
						checkPosition(figures[temp].posX, figures[temp].posY, square, criticalDirection);
					}
				}
				if (pounding) {						//if you placed 3 in a row
					//move around and choose who to remove
					action.setString("Remove opponents figure");
					choosePosition(event, figures.size() - 1, s, right, left, up, down, figures, square, shift, pictures, window, criticalDirection);
					checkPosition(figures[figures.size() - 1].posX, figures[figures.size() - 1].posY, square, criticalDirection);
					if (event.key.code == 13) {
						//if you chose a spot
						int del = ifEnemy(figures, figures[figures.size() - 1].posX, figures[figures.size() - 1].posY, player);
						if (del != -1 && del != -2) {		//if its an enemy you selected
							figures[figures.size() - 1].posX = 375;
							figures[figures.size() - 1].posY = 375;
							pictures[pictures.size() - 1].setPosition(figures[figures.size() - 1].posX, figures[figures.size() - 1].posY);
							figures.erase(figures.begin() + del);			//delete the figure at the selected spot
							pictures.erase(pictures.begin() + del);			//
							if (player == 'w')								//adjust the number of figures
								Positions::numOfFiguresB--;					//
							else
								Positions::numOfFiguresW--;
							pounding = false;
							moveThePlayer = false;
							playerSwitch(player);
						}
					}
				}

				//if you have chosen an enemy
				if (ifEnemy(figures, figures[figures.size() - 1].posX, figures[figures.size() - 1].posY, player) == -2 && event.key.code == 13 && !pounding)
					characterChosen = true;

				//if youve placed a new character without pounding
				if (placeNew && event.key.code == 13 && ifLegalMove(figures, temp, prevX, prevY)) {
					if (placedThreeTogether(figures, figures[temp].posX, figures[temp].posY, player, square, criticalDirection))
						pounding = true;
					figures.push_back(Positions('g'));
					pictures.push_back(grayFigure);
					placeNew = false;
					moveThePlayer = false;
					if (!pounding)
						playerSwitch(player);
				}
			}
			if (characterChosen) {
				temp = getFigureIndex(figures, figures[figures.size() - 1].posX, figures[figures.size() - 1].posY);
				caught = true;
				characterChosen = false;
			}
			if (caught) {
				moveThePlayer = true;
				prevX = figures[temp].posX;
				prevY = figures[temp].posY;
				figures.erase(figures.begin() + figures.size() - 1);
				pictures.erase(pictures.begin() + pictures.size() - 1);
				caught = false;
				placeNew = true;
			}
		}

		//show all figures on the field
		for (size_t k = 0; k <= pictures.size() - 1; k++) {
			pictures[k].setPosition(figures[k].posX, figures[k].posY);
			window.draw(pictures[k]);
		}
		window.display();
	}
	return 0;
}