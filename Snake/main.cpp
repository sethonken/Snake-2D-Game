#include "GL/freeglut.h"
#include "GL/SOIL.h"
#include <string>
#include <string.h>
#include <time.h> 
#include <cmath>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

using namespace std;

#define LOADING 0
#define START_MENU 1
#define HELP 2
#define GAME 3
#define GAMEOVER 4
#define GAMEOVER_APPENDIX 5
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define HEAD 1
#define TAIL 5
#define CURVE 

struct Segment {
	int row;
	int column;
	int direction = DOWN;
};


//global variables
int width = 800;
int height = width * 1.06;
const int tilesAcross = 15;
const int totalTileNumber = tilesAcross * tilesAcross;
int gridImage[tilesAcross][tilesAcross];
int screen = GAME;
int direction = DOWN;
int secondDirection = DOWN;
int moveCount = 0;
int screenCount = 0;
int afkCount = 0;
int score = 0;
int highscore = 0;
double refresh_rate = 0.15;
clock_t start;
GLuint gridImages[11];
GLuint images[6];
GLuint digits[10];

bool gameOver = false;
bool foodAvailable = false;
bool exitFlip = false;

vector<Segment> snakeSegments(0);

//functions
void display();
void spawnFood();
void resetGame();


void setDigit(int score, int index) {
	if (score > 99999) {
		score = 99999;
	}

	char digit = '0';
	string strScore = to_string(score);

	//get digit
	switch (index) {
	case 0:
		if (score > 9999) {
			digit = strScore[strScore.length() - 5];
		}
		break;
	case 1:
		if (score > 999) {
			digit = strScore[strScore.length() - 4];
		}
		break;
	case 2:
		if (score > 99) {
			digit = strScore[strScore.length() - 3];
		}
		break;
	case 3:
		if (score > 9) {
			digit = strScore[strScore.length() - 2];
		}
		break;
	case 4:
		digit = strScore[strScore.length() - 1];
		break;
	}

	//set digit
	switch (digit) {
	case '0':
		glBindTexture(GL_TEXTURE_2D, digits[0]);
		break;
	case '1':
		glBindTexture(GL_TEXTURE_2D, digits[1]);
		break;
	case '2':
		glBindTexture(GL_TEXTURE_2D, digits[2]);
		break;
	case '3':
		glBindTexture(GL_TEXTURE_2D, digits[3]);
		break;
	case '4':
		glBindTexture(GL_TEXTURE_2D, digits[4]);
		break;
	case '5':
		glBindTexture(GL_TEXTURE_2D, digits[5]);
		break;
	case '6':
		glBindTexture(GL_TEXTURE_2D, digits[6]);
		break;
	case '7':
		glBindTexture(GL_TEXTURE_2D, digits[7]);
		break;
	case '8':
		glBindTexture(GL_TEXTURE_2D, digits[8]);
		break;
	case '9':
		glBindTexture(GL_TEXTURE_2D, digits[9]);
		break;
	}
}

void display() {
	if (screen == GAMEOVER) {
		//reset refresh rate
		refresh_rate = 0.15;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		score -= 1;
		glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
		glBegin(GL_POLYGON);
		glVertex2f(-1.0f, -0.89f);
		glVertex2f(1.0f, -0.89f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(-1.0f, 1.0f);
		glEnd();
		glutSwapBuffers();
		Sleep(1500);
		screen = GAMEOVER_APPENDIX;
		screenCount = 0;
		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	glClear(GL_COLOR_BUFFER_BIT);

	switch (screen) {
		case LOADING:
			glColor3f(1.0f, 1.0f, 1.0f);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, images[0]);
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			break;
		case START_MENU:
			glColor3f(1.0f, 1.0f, 1.0f);
			glEnable(GL_TEXTURE_2D);
			if (screenCount < 5) {
				glBindTexture(GL_TEXTURE_2D, images[1]);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, images[2]);
			}
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			break;
		case HELP:
			break;
		case GAME:
			glColor3f(1.0f, 1.0f, 1.0f);
			//score image
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, images[3]);
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			//score
			for (int i = 0; i < 5; i++) {
				glEnable(GL_TEXTURE_2D);
				//get right digit
				setDigit(score, i);
				glBegin(GL_POLYGON);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(0.4375 + 0.1125 * i, -1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(0.4375 + 0.1125 * (i + 1), -1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(0.4375 + 0.1125 * (i + 1), -0.89f);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(0.4375 + 0.1125 * i, -0.89f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}

			//highscore
			for (int i = 0; i < 5; i++) {
				glEnable(GL_TEXTURE_2D);
				//get right digit
				setDigit(highscore, i);
				glBegin(GL_POLYGON);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.4625 + 0.1125 * i, -1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(-0.4625 + 0.1125 * (i + 1), -1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(-0.4625 + 0.1125 * (i + 1), -0.89f);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.4625 + 0.1125 * i, -0.89f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}

			//tiles
			for (int i = 0; i < tilesAcross; i++) {
				for (int j = 0; j < tilesAcross; j++) {
					glEnable(GL_TEXTURE_2D);
					int rotate = 0;

					//set proper image
					if (gridImage[i][j] == 0) { //empty tile
						glBindTexture(GL_TEXTURE_2D, gridImages[0]);
					}
					else if (gridImage[i][j] < 5) { //head
						glBindTexture(GL_TEXTURE_2D, gridImages[2]);
						if (gridImage[i][j] == 2) {
							rotate = 1;
						}
						else if (gridImage[i][j] == 3) {
							rotate = 2;
						}
						else if (gridImage[i][j] == 4) {
							rotate = 3;
						}
					}
					else if (gridImage[i][j] < 7) { //body straight
						glBindTexture(GL_TEXTURE_2D, gridImages[3]);
						if (gridImage[i][j] == 6) {
							rotate = 1;
						}
					}
					else if (gridImage[i][j] < 11) { //body curve
						glBindTexture(GL_TEXTURE_2D, gridImages[4]);
						if (gridImage[i][j] == 8) {
							rotate = 1;
						}
						else if (gridImage[i][j] == 9) {
							rotate = 2;
						}
						else if (gridImage[i][j] == 10) {
							rotate = 3;
						}
					}
					else if (gridImage[i][j] < 15) { //tail
						glBindTexture(GL_TEXTURE_2D, gridImages[5]);
						if (gridImage[i][j] == 12) {
							rotate = 1;
						}
						else if (gridImage[i][j] == 13) {
							rotate = 2;
						}
						else if (gridImage[i][j] == 14) {
							rotate = 3;
						}
					}
					else { //mouse
						glBindTexture(GL_TEXTURE_2D, gridImages[1]);
					}


					//draw image
					glBegin(GL_POLYGON);
					switch (rotate) {
					case 0:
						glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f + 2 * (j / (1.0 * tilesAcross)), 1.0f - 1.89 * ((i + 1) / (1.0 * tilesAcross)));
						glTexCoord2f(1.0f, 0.0f); glVertex2f(-1.0f + 2 * ((j + 1) / (1.0 * tilesAcross)), 1.0f - 1.89 * ((i + 1) / (1.0 * tilesAcross)));
						glTexCoord2f(1.0f, 1.0f); glVertex2f(-1.0f + 2 * ((j + 1) / (1.0 * tilesAcross)), 1.0f - 1.89 * (i / (1.0 * tilesAcross)));
						glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f + 2 * (j / (1.0 * tilesAcross)), 1.0f - 1.89 * (i / (1.0 * tilesAcross)));
						break;
					case 1:
						glTexCoord2f(1.0f, 0.0f); glVertex2f(-1.0f + 2 * (j / (1.0 * tilesAcross)), 1.0f - 1.89 * ((i + 1) / (1.0 * tilesAcross)));
						glTexCoord2f(1.0f, 1.0f); glVertex2f(-1.0f + 2 * ((j + 1) / (1.0 * tilesAcross)), 1.0f - 1.89 * ((i + 1) / (1.0 * tilesAcross)));
						glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f + 2 * ((j + 1) / (1.0 * tilesAcross)), 1.0f - 1.89 * (i / (1.0 * tilesAcross)));
						glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f + 2 * (j / (1.0 * tilesAcross)), 1.0f - 1.89 * (i / (1.0 * tilesAcross)));
						break;
					case 2:
						glTexCoord2f(1.0f, 1.0f); glVertex2f(-1.0f + 2 * (j / (1.0 * tilesAcross)), 1.0f - 1.89 * ((i + 1) / (1.0 * tilesAcross)));
						glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f + 2 * ((j + 1) / (1.0 * tilesAcross)), 1.0f - 1.89 * ((i + 1) / (1.0 * tilesAcross)));
						glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f + 2 * ((j + 1) / (1.0 * tilesAcross)), 1.0f - 1.89 * (i / (1.0 * tilesAcross)));
						glTexCoord2f(1.0f, 0.0f); glVertex2f(-1.0f + 2 * (j / (1.0 * tilesAcross)), 1.0f - 1.89 * (i / (1.0 * tilesAcross)));
						break;
					case 3:
						glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f + 2 * (j / (1.0 * tilesAcross)), 1.0f - 1.89 * ((i + 1) / (1.0 * tilesAcross)));
						glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f + 2 * ((j + 1) / (1.0 * tilesAcross)), 1.0f - 1.89 * ((i + 1) / (1.0 * tilesAcross)));
						glTexCoord2f(1.0f, 0.0f); glVertex2f(-1.0f + 2 * ((j + 1) / (1.0 * tilesAcross)), 1.0f - 1.89 * (i / (1.0 * tilesAcross)));
						glTexCoord2f(1.0f, 1.0f); glVertex2f(-1.0f + 2 * (j / (1.0 * tilesAcross)), 1.0f - 1.89 * (i / (1.0 * tilesAcross)));
						break;
					}
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
			}
			break;
		case GAMEOVER_APPENDIX:
			glColor3f(1.0f, 1.0f, 1.0f);
			glEnable(GL_TEXTURE_2D);
			if (score <= highscore || screenCount % 5 == 0) {
				glBindTexture(GL_TEXTURE_2D, images[4]);
			}
			else {
				glBindTexture(GL_TEXTURE_2D, images[5]);
			}
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glColor3f(1.0f, 1.0f, 1.0f);

			//score
			for (int i = 0; i < 5; i++) {
				glEnable(GL_TEXTURE_2D);
				//get right digit
				setDigit(score, i);
				glBegin(GL_POLYGON);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(0.4375 + 0.1125 * i, -1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(0.4375 + 0.1125 * (i + 1), -1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(0.4375 + 0.1125 * (i + 1), -0.89f);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(0.4375 + 0.1125 * i, -0.89f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}

			//highscore
			for (int i = 0; i < 5; i++) {
				glEnable(GL_TEXTURE_2D);
				//get right digit
				if (score > highscore) {
					setDigit(score, i);
				}
				else {
					setDigit(highscore, i);
				}
				glBegin(GL_POLYGON);
				glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.4625 + 0.1125 * i, -1.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(-0.4625 + 0.1125 * (i + 1), -1.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex2f(-0.4625 + 0.1125 * (i + 1), -0.89f);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.4625 + 0.1125 * i, -0.89f);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			break;
	}

	//SwapBuffers
	glutSwapBuffers();
}

void loadImages() {
	//set screens
	images[0] = SOIL_load_OGL_texture("images/loading screen.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	screen = LOADING;
	display();
	images[1] = SOIL_load_OGL_texture("images/main screen 1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	images[2] = SOIL_load_OGL_texture("images/main screen 2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	images[3] = SOIL_load_OGL_texture("images/score screen.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	images[4] = SOIL_load_OGL_texture("images/game over screen.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	images[5] = SOIL_load_OGL_texture("images/game over screen 2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);


	//load snake images
	gridImages[0] = SOIL_load_OGL_texture("images/empty square.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	gridImages[1] = SOIL_load_OGL_texture("images/mouse.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	gridImages[2] = SOIL_load_OGL_texture("images/head down.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	gridImages[3] = SOIL_load_OGL_texture("images/body straight vertical.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	gridImages[4] = SOIL_load_OGL_texture("images/body curve top left.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	gridImages[5] = SOIL_load_OGL_texture("images/tail down.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	//load digit images
	digits[0] = SOIL_load_OGL_texture("images/digit 0.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	digits[1] = SOIL_load_OGL_texture("images/digit 1.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	digits[2] = SOIL_load_OGL_texture("images/digit 2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	digits[3] = SOIL_load_OGL_texture("images/digit 3.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	digits[4] = SOIL_load_OGL_texture("images/digit 4.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	digits[5] = SOIL_load_OGL_texture("images/digit 5.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	digits[6] = SOIL_load_OGL_texture("images/digit 6.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	digits[7] = SOIL_load_OGL_texture("images/digit 7.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	digits[8] = SOIL_load_OGL_texture("images/digit 8.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	digits[9] = SOIL_load_OGL_texture("images/digit 9.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	resetGame();

	screen = LOADING;
}

void resetGame() {
	//clear image tile board
	for (int i = 0; i < tilesAcross; i++) {
		for (int j = 0; j < tilesAcross; j++) {
			gridImage[i][j] = 0;
		}
	}

	direction = DOWN;
	moveCount = 0;

	snakeSegments.clear();
	int halfway = tilesAcross / 2;
	snakeSegments.reserve(totalTileNumber);
	gridImage[0][halfway] = 11; //tail
	gridImage[1][halfway] = HEAD + DOWN; //head

	Segment tail, head;
	tail.row = 0;
	tail.column = halfway;
	head.row = 1;
	head.column = halfway;

	snakeSegments.push_back(head);
	snakeSegments.push_back(tail);

	spawnFood();

	screen = START_MENU;
	screenCount = 0;

	//read in old highscore
	freopen("highscore.txt", "r", stdin);
	std::string line = "";
	std::cin >> line;
	if (line.length() > 1) {
		highscore = atoi(line.c_str());
	}
	fclose(stdin);


	//update highscore
	if (score > highscore) {
		highscore = score;
		//save highscore
		freopen("highscore.txt", "w", stdout);
		printf("%d\n", highscore);
		fclose(stdout);
	}

	score = 0;
}

bool outOfBounds(int snakeSegmentIndex) {
	if (snakeSegments[snakeSegmentIndex].row < 0 || snakeSegments[snakeSegmentIndex].row >(tilesAcross - 1) ||
		snakeSegments[snakeSegmentIndex].column < 0 || snakeSegments[snakeSegmentIndex].column >(tilesAcross - 1)) {
		return true;
	}
	return false;
}


void flipHead(int snakeSegmentIndex) {
	if (snakeSegments[snakeSegmentIndex].row < 0) {
		snakeSegments[snakeSegmentIndex].row = tilesAcross - 1;
	}
	else if (snakeSegments[snakeSegmentIndex].row > (tilesAcross - 1)) {
		snakeSegments[snakeSegmentIndex].row = 0;
	}

	if (snakeSegments[snakeSegmentIndex].column < 0) {
		snakeSegments[snakeSegmentIndex].column = tilesAcross - 1;
	}
	else if (snakeSegments[snakeSegmentIndex].column > (tilesAcross - 1)) {
		snakeSegments[snakeSegmentIndex].column = 0;
	}
}

void checkCornerCase() {
	//check if head is in corner
	bool segmentInCorner = false;
	if (snakeSegments[0].row == 0 || snakeSegments[0].row == (tilesAcross - 1)) {
		if (snakeSegments[0].column == 0 || snakeSegments[0].column == (tilesAcross - 1)) {
			segmentInCorner = true;
		}
	}
	if (!segmentInCorner) {
		return;
	}

	//check if second segment is in corner
	segmentInCorner = false;
	if (snakeSegments[1].row == 0 || snakeSegments[1].row == (tilesAcross - 1)) {
		if (snakeSegments[1].column == 0 || snakeSegments[1].column == (tilesAcross - 1)) {
			segmentInCorner = true;
		}
	}
	if (!segmentInCorner) {
		return;
	}

	//check if third segment is opposite second segment
	if (abs(snakeSegments[2].row - snakeSegments[1].row) < 2) {
		if (abs(snakeSegments[2].column - snakeSegments[1].column) < 2) {
			return;
		}
	}

	//update corner image for second segment
	if (snakeSegments[1].row == 0) {
		if (snakeSegments[1].column == 0) { //top left
			gridImage[snakeSegments[1].row][snakeSegments[1].column] = 7;
		}
		else if (snakeSegments[1].column == (tilesAcross - 1)) { //top right
			gridImage[snakeSegments[1].row][snakeSegments[1].column] = 8;
		}
	}
	else if (snakeSegments[1].row == (tilesAcross - 1)) {
		if (snakeSegments[1].column == 0) { //bottom left
			gridImage[snakeSegments[1].row][snakeSegments[1].column] = 10;
		}
		else if (snakeSegments[1].column == (tilesAcross - 1)) { //bottom right
			gridImage[snakeSegments[1].row][snakeSegments[1].column] = 9;
		}
	}
}

void updateSnakePosition() {
	bool flipped = false;
	bool corner = false;

	if (!foodAvailable) {
		//new segment where head was
		Segment newSegment;
		newSegment.row = snakeSegments[0].row;
		newSegment.column = snakeSegments[0].column;
		snakeSegments.insert(snakeSegments.begin() + 1, newSegment);

		//move head
		Segment oldHead = snakeSegments[0];
		switch (direction) {
		case UP:
			snakeSegments[0].row = snakeSegments[0].row - 1;
			snakeSegments[0].column = snakeSegments[0].column;
			if (outOfBounds(0)) {
				flipped = true;
				flipHead(0);
			}
			if (gridImage[snakeSegments[0].row][snakeSegments[0].column] != 0) { //ran into itself
				if (gridImage[snakeSegments[0].row][snakeSegments[0].column] < 1 || gridImage[snakeSegments[0].row][snakeSegments[0].column] > 4) {
					gameOver = true;
					screen = GAMEOVER;
					snakeSegments[0] = oldHead;
					return;
				}
			}
			gridImage[snakeSegments[0].row][snakeSegments[0].column] = 3;
			break;
		case RIGHT:
			snakeSegments[0].row = snakeSegments[0].row;
			snakeSegments[0].column = snakeSegments[0].column + 1;
			if (outOfBounds(0)) {
				flipped = true;
				flipHead(0);
			}
			if (gridImage[snakeSegments[0].row][snakeSegments[0].column] != 0) { //ran into itself
				if (gridImage[snakeSegments[0].row][snakeSegments[0].column] < 1 || gridImage[snakeSegments[0].row][snakeSegments[0].column] > 4) {
					gameOver = true;
					screen = GAMEOVER;
					snakeSegments[0] = oldHead;
					return;
				}
			}
			gridImage[snakeSegments[0].row][snakeSegments[0].column] = 4;
			break;
		case DOWN:
			snakeSegments[0].row = snakeSegments[0].row + 1;
			snakeSegments[0].column = snakeSegments[0].column;
			if (outOfBounds(0)) {
				flipped = true;
				flipHead(0);
			}
			if (gridImage[snakeSegments[0].row][snakeSegments[0].column] != 0) { //ran into itself
				if (gridImage[snakeSegments[0].row][snakeSegments[0].column] < 1 || gridImage[snakeSegments[0].row][snakeSegments[0].column] > 4) {
					gameOver = true;
					screen = GAMEOVER;
					snakeSegments[0] = oldHead;
					return;
				}
			}
			gridImage[snakeSegments[0].row][snakeSegments[0].column] = 1;
			break;
		case LEFT:
			snakeSegments[0].row = snakeSegments[0].row;
			snakeSegments[0].column = snakeSegments[0].column - 1;
			if (outOfBounds(0)) {
				flipped = true;
				flipHead(0);
			}
			if (gridImage[snakeSegments[0].row][snakeSegments[0].column] != 0) { //ran into itself
				if (gridImage[snakeSegments[0].row][snakeSegments[0].column] < 1 || gridImage[snakeSegments[0].row][snakeSegments[0].column] > 4) {
					gameOver = true;
					screen = GAMEOVER;
					snakeSegments[0] = oldHead;
					return;
				}
			}
			gridImage[snakeSegments[0].row][snakeSegments[0].column] = 2;
			break;
		}

		//adjust image for new segment
		if (snakeSegments[2].row < snakeSegments[1].row) {
			if (snakeSegments[0].row != snakeSegments[1].row) { //vertical straight
				gridImage[snakeSegments[1].row][snakeSegments[1].column] = 5;
			}
			else if (snakeSegments[0].column > snakeSegments[1].column) { //curve TR
				gridImage[snakeSegments[1].row][snakeSegments[1].column] = 8;
				if (flipped) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 7;
				}
				else if (exitFlip) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 9;
				}
			}
			else { //curve TL
				gridImage[snakeSegments[1].row][snakeSegments[1].column] = 7;
				if (flipped) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 8;
				}
				else if (exitFlip) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 10;
				}
			}
		}
		else if (snakeSegments[2].row > snakeSegments[1].row) {
			if (snakeSegments[0].row != snakeSegments[1].row) { //vertical straight
				gridImage[snakeSegments[1].row][snakeSegments[1].column] = 5;
			}
			else if (snakeSegments[0].column > snakeSegments[1].column) { //curve BR
				gridImage[snakeSegments[1].row][snakeSegments[1].column] = 9;
				if (flipped) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 10;
				}
				else if (exitFlip) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 8;
				}
			}
			else { //curve BL
				gridImage[snakeSegments[1].row][snakeSegments[1].column] = 10;
				if (flipped) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 9;
				}
				else if (exitFlip) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 7;
				}
			}
		}
		else if (snakeSegments[2].column > snakeSegments[1].column) {
			if (snakeSegments[0].column != snakeSegments[1].column) { //horizonal straight
				gridImage[snakeSegments[1].row][snakeSegments[1].column] = 6;
			}
			else if (snakeSegments[0].row > snakeSegments[1].row) { //curve BR
				gridImage[snakeSegments[1].row][snakeSegments[1].column] = 9;
				if (flipped) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 8;
				}
				else if (exitFlip) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 10;
				}
			}
			else { //curve TR
				gridImage[snakeSegments[1].row][snakeSegments[1].column] = 8;
				if (flipped) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 9;
				}
				else if (exitFlip) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 7;
				}
			}
		}
		else {
			if (snakeSegments[0].column != snakeSegments[1].column) { //horizonal straight
				gridImage[snakeSegments[1].row][snakeSegments[1].column] = 6;
			}
			else if (snakeSegments[0].row > snakeSegments[1].row) { //curve BL
				gridImage[snakeSegments[1].row][snakeSegments[1].column] = 10;
				if (flipped) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 7;
				}
				else if (exitFlip) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 9;
				}
			}
			else { //curve TL
				gridImage[snakeSegments[1].row][snakeSegments[1].column] = 7;
				if (flipped) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 10;
				}
				else if (exitFlip) {
					gridImage[snakeSegments[1].row][snakeSegments[1].column] = 8;
				}
			}
		}

		checkCornerCase();
	}
	else {
		for (int i = snakeSegments.size() - 1; i >= 0; i--) {
			//update segment position
			if (i == snakeSegments.size() - 1) { //tail
				int oldRow, oldColumn;
				oldRow = snakeSegments[i].row;
				oldColumn = snakeSegments[i].column;
				snakeSegments[i].row = snakeSegments[i - 1].row;
				snakeSegments[i].column = snakeSegments[i - 1].column;

				//reset last tail position to clear tile
				gridImage[oldRow][oldColumn] = 0;
			}
			else if (i == 0) { //head
				Segment oldHead = snakeSegments[i];
				switch (direction) {
				case UP:
					snakeSegments[i].row = snakeSegments[i].row - 1;
					snakeSegments[i].column = snakeSegments[i].column;
					if (outOfBounds(i)) {
						flipped = true;
						flipHead(i);
					}
					if (gridImage[snakeSegments[i].row][snakeSegments[i].column] == 15) { //ate food
						foodAvailable = false;
					}
					else if (gridImage[snakeSegments[i].row][snakeSegments[i].column] != 0) { //ran into itself
						if (gridImage[snakeSegments[i].row][snakeSegments[i].column] < 1 || gridImage[snakeSegments[i].row][snakeSegments[i].column] > 4) {
							gameOver = true;
							screen = GAMEOVER;
							snakeSegments[i] = oldHead;
							return;
						}
					}
					gridImage[snakeSegments[i].row][snakeSegments[i].column] = 3;
					break;
				case RIGHT:
					snakeSegments[i].row = snakeSegments[i].row;
					snakeSegments[i].column = snakeSegments[i].column + 1;
					if (outOfBounds(i)) {
						flipped = true;
						flipHead(i);
					}
					if (gridImage[snakeSegments[i].row][snakeSegments[i].column] == 15) { //ate food
						foodAvailable = false;
					}
					else if (gridImage[snakeSegments[i].row][snakeSegments[i].column] != 0) { //ran into itself
						if (gridImage[snakeSegments[i].row][snakeSegments[i].column] < 1 || gridImage[snakeSegments[i].row][snakeSegments[i].column] > 4) {
							gameOver = true;
							screen = GAMEOVER;
							snakeSegments[i] = oldHead;
							return;
						}
					}
					gridImage[snakeSegments[i].row][snakeSegments[i].column] = 4;
					break;
				case DOWN:
					snakeSegments[i].row = snakeSegments[i].row + 1;
					snakeSegments[i].column = snakeSegments[i].column;
					if (outOfBounds(i)) {
						flipped = true;
						flipHead(i);
					}
					if (gridImage[snakeSegments[i].row][snakeSegments[i].column] == 15) { //ate food
						foodAvailable = false;
					}
					else if (gridImage[snakeSegments[i].row][snakeSegments[i].column] != 0) { //ran into itself
						if (gridImage[snakeSegments[i].row][snakeSegments[i].column] < 1 || gridImage[snakeSegments[i].row][snakeSegments[i].column] > 4) {
							gameOver = true;
							screen = GAMEOVER;
							snakeSegments[i] = oldHead;
							return;
						}
					}
					gridImage[snakeSegments[i].row][snakeSegments[i].column] = 1;
					break;
				case LEFT:
					snakeSegments[i].row = snakeSegments[i].row;
					snakeSegments[i].column = snakeSegments[i].column - 1;
					if (outOfBounds(i)) {
						flipped = true;
						flipHead(i);
					}
					if (gridImage[snakeSegments[i].row][snakeSegments[i].column] == 15) { //ate food
						foodAvailable = false;
					}
					else if (gridImage[snakeSegments[i].row][snakeSegments[i].column] != 0) { //ran into itself
						if (gridImage[snakeSegments[i].row][snakeSegments[i].column] < 1 || gridImage[snakeSegments[i].row][snakeSegments[i].column] > 4) {
							gameOver = true;
							screen = GAMEOVER;
							snakeSegments[i] = oldHead;
							return;
						}
					}
					gridImage[snakeSegments[i].row][snakeSegments[i].column] = 2;
					break;
				}

				//move adjust segment before head
				if (snakeSegments.size() > 2) {
					if (snakeSegments[i + 2].row < snakeSegments[i + 1].row) {
						if (snakeSegments[i].row != snakeSegments[i + 1].row) { //vertical straight
							gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 5;
						}
						else if (snakeSegments[i].column > snakeSegments[i + 1].column) { //curve TR
							gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 8;
							if (flipped) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 7;
							}
							else if (exitFlip) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 9;
							}
						}
						else { //curve TL
							gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 7;
							if (flipped) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 8;
							}
							else if (exitFlip) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 10;
							}
						}
					}
					else if (snakeSegments[i + 2].row > snakeSegments[i + 1].row) {
						if (snakeSegments[i].row != snakeSegments[i + 1].row) { //vertical straight
							gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 5;
						}
						else if (snakeSegments[i].column > snakeSegments[i + 1].column) { //curve BR
							gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 9;
							if (flipped) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 10;
							}
							else if (exitFlip) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 8;
							}
						}
						else { //curve BL
							gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 10;
							if (flipped) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 9;
							}
							else if (exitFlip) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 7;
							}
						}
					}
					else if (snakeSegments[i + 2].column > snakeSegments[i + 1].column) {
						if (snakeSegments[i].column != snakeSegments[i + 1].column) { //horizonal straight
							gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 6;
						}
						else if (snakeSegments[i].row > snakeSegments[i + 1].row) { //curve BR
							gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 9;
							if (flipped) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 8;
							}
							else if (exitFlip) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 10;
							}
						}
						else { //curve TR
							gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 8;
							if (flipped) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 9;
							}
							else if (exitFlip) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 7;
							}
						}
					}
					else {
						if (snakeSegments[i].column != snakeSegments[i + 1].column) { //horizonal straight
							gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 6;
						}
						else if (snakeSegments[i].row > snakeSegments[i + 1].row) { //curve BL
							gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 10;
							if (flipped) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 7;
							}
							else if (exitFlip) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 9;
							}
						}
						else { //curve TL
							gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 7;
							if (flipped) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 10;
							}
							else if (exitFlip) {
								gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 8;
							}
						}
					}

					checkCornerCase();
				}
				else { //just head and tail
					switch (direction) {
					case UP:
						gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 13;
						break;
					case RIGHT:
						gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 14;
						break;
					case DOWN:
						gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 11;
						break;
					case LEFT:
						gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 12;
						break;
					}
				}
			}
			else if (i == snakeSegments.size() - 2) { //segment after tail for tail direction
				snakeSegments[i].row = snakeSegments[i - 1].row;
				snakeSegments[i].column = snakeSegments[i - 1].column;

				//update tail direction
				if (snakeSegments[i].row > snakeSegments[i + 1].row) {
					gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 11;
				}
				else if (snakeSegments[i].row < snakeSegments[i + 1].row) {
					gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 13;
				}
				else if (snakeSegments[i].column < snakeSegments[i + 1].column) {
					gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 12;
				}
				else {
					gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 14;
				}

				//update tail for flipping sides
				if (snakeSegments[i + 1].row == 0 && snakeSegments[i].row == tilesAcross - 1) {
					gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 13;
				}
				else if (snakeSegments[i + 1].row == tilesAcross - 1 && snakeSegments[i].row == 0) {
					gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 11;
				}
				else if (snakeSegments[i + 1].column == 0 && snakeSegments[i].column == tilesAcross - 1) {
					gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 12;
				}
				else if (snakeSegments[i + 1].column == tilesAcross - 1 && snakeSegments[i].column == 0) {
					gridImage[snakeSegments[i + 1].row][snakeSegments[i + 1].column] = 14;
				}
			}
			else {
				snakeSegments[i].row = snakeSegments[i - 1].row;
				snakeSegments[i].column = snakeSegments[i - 1].column;
			}
		}
	}

	//update exitFlip for snake traversing to other side
	if (exitFlip) {
		exitFlip = false;
	}
	if (flipped) {
		exitFlip = true;
	}
}

void spawnFood() {
	//speed up snake
	refresh_rate -= 0.0005;

	int unoccupiedTiles = 0;

	//get amount of unoccupied tiles
	for (int i = 0; i < tilesAcross; i++) {
		for (int j = 0; j < tilesAcross; j++) {
			if (gridImage[i][j] == 0) {
				unoccupiedTiles++;
			}
		}
	}

	int foodLocation = rand() % unoccupiedTiles;

	//place food in random location
	unoccupiedTiles = 0;
	for (int i = 0; i < tilesAcross; i++) {
		for (int j = 0; j < tilesAcross; j++) {
			if (gridImage[i][j] == 0) {
				if (unoccupiedTiles == foodLocation) {
					gridImage[i][j] = 15; //mouse image
					foodAvailable = true;
					return;
				}
				else {
					unoccupiedTiles++;
				}
			}
		}
	}
}

void idleFunc() {
	start = clock();

	switch (screen) {
	case GAME:
		if (!foodAvailable) {
			score += 350;
			updateSnakePosition();
			spawnFood();
			if (moveCount > 1) {
				direction = secondDirection;
				updateSnakePosition();
			}
		}
		else {
			if (moveCount > 1) {
				updateSnakePosition();
				direction = secondDirection;
			}
			updateSnakePosition();
		}

		//add one point for movement
		afkCount++;
		if (afkCount < tilesAcross) {
			score += 1;
		}
		display();

		moveCount = 0;
		break;
	case START_MENU:
		screenCount++;
		if (screenCount > 7) {
			screenCount = 0;
		}
		display();
		break;
	case LOADING:
		screenCount++;
		if (screenCount > 1) {
			screenCount = 0;
			screen = START_MENU;
		}
		break;
	case GAMEOVER_APPENDIX:
		screenCount++;
		if (screenCount > 35) {
			resetGame();
			screen = START_MENU;
			screenCount = 0;
		}
		break;
	}

	display();

	//apply refresh rate
	while (((clock() - start) / (double)CLOCKS_PER_SEC) < refresh_rate) {
		;
	}
}

void keyboard(int key, int x, int y) {
	if (screen == LOADING) {
		screenCount = 0;
		return;
	}
	if (screen == START_MENU) {
		screen = GAME;
		screenCount = 0;
		return;
	}
	if (key == GLUT_KEY_UP) {
		if (direction != DOWN && direction != UP) {
			afkCount = 0;
			if (moveCount == 0) {
				direction = UP;
				start = clock();
				moveCount++;
			}
			else if (moveCount == 1) {
				secondDirection = UP;
				start = clock();
				moveCount++;
			}
		}
	}
	else if (key == GLUT_KEY_RIGHT) {
		if (direction != LEFT && direction != RIGHT) {
			afkCount = 0;
			if (moveCount == 0) {
				direction = RIGHT;
				start = clock();
				moveCount++;
			}
			else if (moveCount == 1) {
				secondDirection = RIGHT;
				start = clock();
				moveCount++;
			}
		}
	}
	else if (key == GLUT_KEY_DOWN) {
		if (direction != UP && direction != DOWN) {
			afkCount = 0;
			if (moveCount == 0) {
				direction = DOWN;
				start = clock();
				moveCount++;
			}
			else if (moveCount == 1) {
				secondDirection = DOWN;
				start = clock();
				moveCount++;
			}
		}
	}
	else if (key == GLUT_KEY_LEFT) {
		if (direction != RIGHT && direction != LEFT) {
			afkCount = 0;
			if (moveCount == 0) {
				direction = LEFT;
				start = clock();
				moveCount++;
			}
			else if (moveCount == 1) {
				secondDirection = LEFT;
				start = clock();
				moveCount++;
			}
		}
	}
}

void keyboard2(unsigned char key, int x, int y) {
	if (key == 27) {
		exit(0);
		return;
	}
	if (screen == LOADING) {
		screenCount = 0;
		return;
	}
	if (screen == START_MENU) {
		screen = GAME;
		screenCount = 0;
		return;
	}
	if (toupper(key) == 'W' || toupper(key) == 'I' || key == '8') {
		if (direction != DOWN && direction != UP) {
			afkCount = 0;
			if (moveCount == 0) {
				direction = UP;
				start = clock();
				moveCount++;
			}
			else if (moveCount == 1) {
				secondDirection = UP;
				start = clock();
				moveCount++;
			}
		}
	}
	else if (toupper(key) == 'D' || toupper(key) == 'L' || key == '6') {
		if (direction != LEFT && direction != RIGHT) {
			afkCount = 0;
			if (moveCount == 0) {
				direction = RIGHT;
				start = clock();
				moveCount++;
			}
			else if (moveCount == 1) {
				secondDirection = RIGHT;
				start = clock();
				moveCount++;
			}
		}
	}
	else if (toupper(key) == 'S' || toupper(key) == 'K' || key == '5') {
		if (direction != UP && direction != DOWN) {
			afkCount = 0;
			if (moveCount == 0) {
				direction = DOWN;
				start = clock();
				moveCount++;
			}
			else if (moveCount == 1) {
				secondDirection = DOWN;
				start = clock();
				moveCount++;
			}
		}
	}
	else if (toupper(key) == 'A' || toupper(key) == 'J' || key == '4') {
		if (direction != RIGHT && direction != LEFT) {
			afkCount = 0;
			if (moveCount == 0) {
				direction = LEFT;
				start = clock();
				moveCount++;
			}
			else if (moveCount == 1) {
				secondDirection = LEFT;
				start = clock();
				moveCount++;
			}
		}
	}
}

void reshape(int w, int h) {
	width = w;
	height = h;

	//keep proper screen format
	if (height > width) {
		height = width;
	}
	else {
		width = height;
	}
	height = width * 1.06;
	glutReshapeWindow(width, height);

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void init(void) {
	srand(time(NULL));
	/* select clearing (background) color */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, width, height);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float s = width / 2.0;
	if (height < width) s = height / 2;
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	//apply viewing direction
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	glutInit(&__argc, __argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Snake");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idleFunc);

	//keyboard function
	glutKeyboardFunc(keyboard2);
	glutSpecialFunc(keyboard);

	loadImages();

	glutMainLoop();
	return 0;
}