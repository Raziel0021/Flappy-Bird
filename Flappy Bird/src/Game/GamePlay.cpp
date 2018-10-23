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
		Texture2D Bird;
		Music music;
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
		static const Vector2 INIT_SIZE = { SCREENWIDTH/12,SCREENHEIGHT/12 };
		static const float OBSTACLE_WIDTH= 80.0f;
		static const float OBSTACLE_HEIGHT_DIVIDER = 4;
		static const int OBSTACLE_RAND_DIVIDER_HEIGHT = 2;
		static const int SECTIONS_MARGIN_WIDTH = 270;
		static const int GATE_SPACE_HEIGHT = 200;
		static const int ZERO = 0;
		static const int POINTS_POS_Y = 10;
		static const int FONT_SIZE_POINTS = 60;
		static const int DIVIDER_MEASURE_TEXT = 2;
		void InitGame()
		{
			MainMenu::menu = true;
			mute = false;
			pause = false;
			gameover = false;

			player.position = { SCREENWIDTH / FIXED_POSITION_X_DIVIDER,SCREENHEIGHT / INIT_PLAYER_POS_Y_DIVIDER };
			player.velocity = 0.0f;
			player.acceleration = 0.0f;
			player.size = INIT_SIZE;
			player.points = ZERO;

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
				//Walls Movement
				levelPosition += LEVEL_SPEED*GetFrameTime();

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
			if (player.velocity>ZERO) 
			{
				DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, ORANGE);
			}
			else
			{
				DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, RED);
			}
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
			DrawText(FormatText("%02i", player.points), HALF_SCREENWIDTH- (MeasureText(FormatText("%02i", player.points), FONT_SIZE_POINTS) / DIVIDER_MEASURE_TEXT), POINTS_POS_Y, FONT_SIZE_POINTS, WHITE);
		}
	}
}