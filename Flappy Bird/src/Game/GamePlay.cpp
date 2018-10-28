#include "GamePlay.h"

#include <raylib.h>
#include <iostream>
#include <list>

#include "GameManager.h"
#include "Screens/MainMenu.h"

using namespace Game;
using namespace std;
namespace Game {
	namespace GamePlay {

		bool gameover;
		bool pause;
		static bool mute;

		Sound jumpSound;
		Sound collisionSound;
		Music music;
		
		Texture2D Bird;
		Texture2D Background;
		static float backgroundPosition = 0.0f;
		static Rectangle frameRec = { 0.0f, 0.0f, (float)Bird.width / 4, (float)Bird.height };
		static int currentFrame = 0;
		static int framesCounter = 0;
		static int framesSpeed = 8;
		
		Player player;
		static const int TOP_LIMIT=10;
		static float sectionWidth;
		list<int> listSection;
		static int numSection;
		static float levelPosition = 0.0f;
		static const float LEVEL_SPEED = 210.0f;
		static const float CURRENCY_BETWEEN_SECTIONS = 0.4f;
		static const float GRAVITY = 800.0f;
		static const float GRAVITY_DIVIDER = 60.0f;
		static const float FIXED_POSITION_X_DIVIDER = 6.0f;
		static const float INIT_PLAYER_POS_Y_DIVIDER = 2;
		static const float ZERO_ACCELERATION = 0.0f;
		static const float JUMP_DIVIDER = 3.5f;
		static const Vector2 INIT_SIZE = { SCREENWIDTH/14,SCREENHEIGHT/14 };
		static const float OBSTACLE_WIDTH= 80.0f;
		static const float OBSTACLE_HEIGHT_DIVIDER = 4;
		static const int OBSTACLE_RAND_DIVIDER_HEIGHT = 2;
		static const int SECTIONS_MARGIN_WIDTH = 270;
		static const int GATE_SPACE_HEIGHT = 200;
		static const int ZERO = 0;
		static const int POINTS_POS_Y = 10;
		static const int FONT_SIZE_POINTS = 60;
		static const int DIVIDER_MEASURE_TEXT = 2;
		static const Vector2 INIT_POS = { 0,0 };
		static const float SCALE_DIVIDER_WIDTH = 25.0f;
		static const float SCALE_DIVIDER_HEIGHT = 10.0f;
		static const int NUM_FRAMES = 4;
		void InitGame()
		{
			MainMenu::menu = true;
			mute = false;
			pause = false;
			gameover = false;

			player.position = { SCREENWIDTH / FIXED_POSITION_X_DIVIDER,SCREENHEIGHT / INIT_PLAYER_POS_Y_DIVIDER };
			player.velocity = ZERO_ACCELERATION;
			player.acceleration =ZERO_ACCELERATION;
			player.size = INIT_SIZE;
			player.points = ZERO;

		
			frameRec = { INIT_POS.x, INIT_POS.y, (float)Bird.width / NUM_FRAMES, (float)Bird.height };
			currentFrame = ZERO;
			framesCounter = ZERO;
			framesSpeed = 8;

			listSection = { 0,0,0,0 };
			sectionWidth = SCREENWIDTH / (listSection.size() -CURRENCY_BETWEEN_SECTIONS);
		}

		void Play()
		{
			if (!pause) {
				//#define AUDIO
				#ifdef AUDIO
				if (!mute)
					UpdateMusicStream(music);
				#endif // AUDIO

				//Player Animation-Calc
				/*framesCounter++;
				if (framesCounter*GetFrameTime() >= (60 / framesSpeed))
				{
					framesCounter = 0;
					currentFrame++;

					if (currentFrame > 3) currentFrame = 0;

					frameRec.x = (float)currentFrame*(float)Bird.width / 4;
				}*/
				//Player Movement
				if (IsKeyPressed(KEY_SPACE)&& player.velocity >= GRAVITY/GRAVITY_DIVIDER )
				{
					player.acceleration = ZERO_ACCELERATION;
					player.velocity = -GRAVITY / JUMP_DIVIDER;
					player.points++;
				}
				else
				{
					player.acceleration += GRAVITY * GetFrameTime();
				}
				if (player.acceleration >= GRAVITY)
				{
					player.acceleration = GRAVITY;
				}
				
				player.velocity += player.acceleration * GetFrameTime();
				player.position.y += player.velocity*GetFrameTime();

				if ((player.position.y - player.size.y / TOP_LIMIT) <= ZERO)
				{
					player.position.y = player.size.y / TOP_LIMIT;
				}

				if (player.velocity > ZERO)
				{
					currentFrame = 0;
					frameRec.x = (float)currentFrame*(float)Bird.width / 4;
				}
				else
				{
					currentFrame = 3;
					frameRec.x = (float)currentFrame*(float)Bird.width / 4;
				}
				//Walls Movement
				levelPosition += LEVEL_SPEED*GetFrameTime();
				backgroundPosition += (LEVEL_SPEED / 3)*GetFrameTime();
				if (levelPosition >= sectionWidth) 
				{
					levelPosition -= sectionWidth;
					listSection.pop_front();
					int i = rand() % (SCREENHEIGHT - (SCREENHEIGHT /OBSTACLE_RAND_DIVIDER_HEIGHT));
					if (i <= SCREENHEIGHT / OBSTACLE_HEIGHT_DIVIDER)
					{
						i = ZERO;
					}
					listSection.push_back(i);
				}
				//Collision player- walls
				numSection = ZERO;
				for (auto var : listSection)
				{
					if (var != ZERO)
					{
						if (CheckCollisionRecs({(float) numSection*sectionWidth + SECTIONS_MARGIN_WIDTH - levelPosition,(float)SCREENHEIGHT - var,OBSTACLE_WIDTH,(float)SCREENHEIGHT }, {player.position.x, player.position.y,player.size.x, player.size.y }) ||
							(CheckCollisionRecs({(float)numSection*sectionWidth + SECTIONS_MARGIN_WIDTH - levelPosition,(float)ZERO, OBSTACLE_WIDTH, (float)SCREENHEIGHT - var - GATE_SPACE_HEIGHT }, { player.position.x, player.position.y,player.size.x, player.size.y })))
						{
							gameover = true;
						}
					}
					numSection++;
				}
			}
		}
		void DrawGame()
		{
			numSection = ZERO;
			for (auto var : listSection)
			{
					DrawTextureRec(Background, { backgroundPosition,0,(float)SCREENWIDTH,(float)SCREENHEIGHT }, INIT_POS, WHITE);
			}
			/*if (player.velocity>ZERO) 
			{
				DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, ORANGE);
			}
			else
			{
				DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, RED);
			}*/
			DrawTextureRec(Bird, frameRec, { player.position.x - (float)Bird.width / SCALE_DIVIDER_WIDTH,player.position.y - (float)Bird.height / SCALE_DIVIDER_HEIGHT }, WHITE);
			numSection = ZERO;
			for (auto var : listSection)
			{
				if (var != ZERO)
				{
					DrawRectangle(numSection*sectionWidth + SECTIONS_MARGIN_WIDTH - levelPosition,SCREENHEIGHT-var,OBSTACLE_WIDTH,SCREENHEIGHT,GREEN);
					DrawRectangle(numSection*sectionWidth + SECTIONS_MARGIN_WIDTH - levelPosition, ZERO, OBSTACLE_WIDTH, SCREENHEIGHT - var- GATE_SPACE_HEIGHT, GREEN);
				}
				numSection++;
			}
			DrawText(FormatText("%02i", player.points), HALF_SCREENWIDTH - (MeasureText(FormatText("%02i", player.points), FONT_SIZE_POINTS+8 ) / DIVIDER_MEASURE_TEXT -4), POINTS_POS_Y, FONT_SIZE_POINTS+4, BLACK);
			DrawText(FormatText("%02i", player.points), HALF_SCREENWIDTH- (MeasureText(FormatText("%02i", player.points), FONT_SIZE_POINTS) / DIVIDER_MEASURE_TEXT), POINTS_POS_Y, FONT_SIZE_POINTS, WHITE);
		}
	}
}