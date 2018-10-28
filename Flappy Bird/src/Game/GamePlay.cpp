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
		Texture2D Ground;
		Texture2D Sky;
		static float backgroundPosition = 0.0f;
		static float skyPosition = 0.0f;
		static float groundPosition = 0.0f;
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
		
		//Pause
		static Vector2 mousePoint;
		static const int FONT_SIZE_OPTIONS = 50;
		static const int FONT_SIZE_PAUSE_BUTTON = 35;
		static const float HORIZONTAL_MARGIN = 10;
		static const float VERTICAL_MARGIN = 30;
		static const float OPTIONS_LINE_DIVIDER = 3;
		static const float PAUSE_LINE_DIVIDER = 1.05;
		static const float PAUSE_PANEL_DIVIDER_Y = 3;
		static const float PAUSE_PANEL_DIVIDER_X = 3.34f;
		static const float PAUSE_PANEL_WIDTH= HALF_SCREENHEIGHT+50;
		static const float PAUSE_PANEL_HEIGHT = SCREENHEIGHT / 3;
		static const float MULTIPLIER_BUTTON_WIDTH = 1.15f;
		static const float SPACE_BETWEEN_LINES = 1.3f;

		static Rectangle pauseButton;
		static Rectangle menuPanel;
		static Rectangle resumeButton;
		static Rectangle menuButton;
		static Rectangle resetButton;
		static Rectangle muteButton;


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

			//Pause
			pauseButton = { (float)Game::SCREENWIDTH - (MeasureText("PAUSE", FONT_SIZE_PAUSE_BUTTON)* MULTIPLIER_BUTTON_WIDTH) - HORIZONTAL_MARGIN,
				(float)Game::SCREENHEIGHT / PAUSE_LINE_DIVIDER,
				(float)MeasureText("PAUSE", FONT_SIZE_PAUSE_BUTTON)* MULTIPLIER_BUTTON_WIDTH ,
				(float)FONT_SIZE_PAUSE_BUTTON };

			menuPanel = { (float)SCREENWIDTH / PAUSE_PANEL_DIVIDER_X  ,
				(float)Game::SCREENHEIGHT / PAUSE_PANEL_DIVIDER_Y,
				(float)PAUSE_PANEL_WIDTH ,
				(float)PAUSE_PANEL_HEIGHT };

			resumeButton = { (float)Game::HALF_SCREENWIDTH - (MeasureText("Resume", FONT_SIZE_OPTIONS) / DIVIDER_MEASURE_TEXT * MULTIPLIER_BUTTON_WIDTH) ,
				(float)Game::SCREENHEIGHT / PAUSE_PANEL_DIVIDER_Y + VERTICAL_MARGIN,
				(float)MeasureText("Resume", FONT_SIZE_OPTIONS)* MULTIPLIER_BUTTON_WIDTH ,
				(float)FONT_SIZE_PAUSE_BUTTON* MULTIPLIER_BUTTON_WIDTH };

			menuButton = { (float)Game::HALF_SCREENWIDTH - (MeasureText("Menu", FONT_SIZE_OPTIONS) / DIVIDER_MEASURE_TEXT * MULTIPLIER_BUTTON_WIDTH) ,
				(float)Game::SCREENHEIGHT / PAUSE_PANEL_DIVIDER_Y + FONT_SIZE_OPTIONS + VERTICAL_MARGIN,
				(float)MeasureText("Menu", FONT_SIZE_OPTIONS)* MULTIPLIER_BUTTON_WIDTH ,
				(float)FONT_SIZE_PAUSE_BUTTON* MULTIPLIER_BUTTON_WIDTH };

			resetButton = { (float)Game::HALF_SCREENWIDTH - (MeasureText("Restart", FONT_SIZE_OPTIONS) / DIVIDER_MEASURE_TEXT * MULTIPLIER_BUTTON_WIDTH) ,
				(float)Game::SCREENHEIGHT / PAUSE_PANEL_DIVIDER_Y + FONT_SIZE_OPTIONS * 2 + VERTICAL_MARGIN,
				(float)MeasureText("Restart", FONT_SIZE_OPTIONS)* MULTIPLIER_BUTTON_WIDTH ,
				(float)FONT_SIZE_PAUSE_BUTTON* MULTIPLIER_BUTTON_WIDTH };
			muteButton = { (float)Game::HALF_SCREENWIDTH - (MeasureText("Mute ON/OFF", FONT_SIZE_OPTIONS) / DIVIDER_MEASURE_TEXT * MULTIPLIER_BUTTON_WIDTH) ,
				(float)Game::SCREENHEIGHT / PAUSE_PANEL_DIVIDER_Y + FONT_SIZE_OPTIONS * 3 + VERTICAL_MARGIN,
				(float)MeasureText("Mute ON/OFF", FONT_SIZE_OPTIONS) + HORIZONTAL_MARGIN,
				(float)FONT_SIZE_PAUSE_BUTTON* MULTIPLIER_BUTTON_WIDTH };
		}

		void Play()
		{
			mousePoint = GetMousePosition();
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
				backgroundPosition += (LEVEL_SPEED / 6)*GetFrameTime();
				groundPosition += (LEVEL_SPEED / 3)*GetFrameTime();
				skyPosition +=(LEVEL_SPEED / 9)*GetFrameTime();
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
				if ((player.position.y - player.size.y ) >= SCREENHEIGHT) 
				{
					gameover = true;
				}

				//Pause-Button
				if (CheckCollisionPointRec(mousePoint, pauseButton))
				{
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
					{
						pause = !pause;
					}
				}
			}

			if (pause) {
				if (CheckCollisionPointRec(mousePoint, resumeButton))
				{
					if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
					{
						pause = !pause;
					}
				}
				if (CheckCollisionPointRec(mousePoint, menuButton))
				{
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
					{
						GamePlay::InitGame();
					}
				}
				if (CheckCollisionPointRec(mousePoint, resetButton))
				{
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
					{
						GamePlay::InitGame();
						MainMenu::menu = false;
					}
				}
				if (CheckCollisionPointRec(mousePoint, muteButton))
				{
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
					{
						mute = !mute;
					}
				}
			}
		}
		void DrawGame()
		{
			numSection = ZERO;
			for (auto var : listSection)
			{
					DrawTextureRec(Sky, { skyPosition,0,(float)SCREENWIDTH,(float)SCREENHEIGHT }, INIT_POS, WHITE);
					DrawTextureRec(Background, { backgroundPosition,0,(float)SCREENWIDTH,(float)SCREENHEIGHT }, INIT_POS, WHITE);
					DrawTextureRec(Ground, { groundPosition,0,(float)SCREENWIDTH,(float)SCREENHEIGHT }, INIT_POS, WHITE);
			}
		#ifdef _DEBUG
			if (player.velocity>ZERO) 
			{
				DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, ORANGE);
			}
			else
			{
				DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, RED);
			}
		#endif
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
		
			//Pause-Button
			if (!pause)
			{
				DrawRectangle(pauseButton.x, pauseButton.y, pauseButton.width, pauseButton.height, LIGHTGRAY);
				DrawText(FormatText("PAUSE"), Game::SCREENWIDTH - (MeasureText("PAUSE", FONT_SIZE_PAUSE_BUTTON)*MULTIPLIER_BUTTON_WIDTH - HORIZONTAL_MARGIN) - HORIZONTAL_MARGIN, Game::SCREENHEIGHT / PAUSE_LINE_DIVIDER, FONT_SIZE_PAUSE_BUTTON, DARKGRAY);
			}
			//Pause-Menu
			if (pause)
			{
				//Panel
				DrawRectangle(menuPanel.x, menuPanel.y, menuPanel.width, menuPanel.height, DARKGRAY);
				//Buttons
				DrawRectangle(resumeButton.x, resumeButton.y, resumeButton.width, resumeButton.height, LIGHTGRAY);
				DrawText(FormatText("Resume"), Game::HALF_SCREENWIDTH - (MeasureText("Resume", FONT_SIZE_OPTIONS) / DIVIDER_MEASURE_TEXT * MULTIPLIER_BUTTON_WIDTH) + HORIZONTAL_MARGIN, Game::SCREENHEIGHT / PAUSE_PANEL_DIVIDER_Y + VERTICAL_MARGIN, FONT_SIZE_OPTIONS, RAYWHITE);

				DrawRectangle(menuButton.x, menuButton.y, menuButton.width, menuButton.height, LIGHTGRAY);
				DrawText(FormatText("Menu"), Game::HALF_SCREENWIDTH - (MeasureText("Menu", FONT_SIZE_OPTIONS) / DIVIDER_MEASURE_TEXT * MULTIPLIER_BUTTON_WIDTH) + HORIZONTAL_MARGIN, Game::SCREENHEIGHT / PAUSE_PANEL_DIVIDER_Y + FONT_SIZE_OPTIONS + VERTICAL_MARGIN, FONT_SIZE_OPTIONS, RAYWHITE);

				DrawRectangle(resetButton.x, resetButton.y, resetButton.width, resetButton.height, LIGHTGRAY);
				DrawText(FormatText("Restart"), Game::HALF_SCREENWIDTH - (MeasureText("Restart", FONT_SIZE_OPTIONS) / DIVIDER_MEASURE_TEXT * MULTIPLIER_BUTTON_WIDTH) + HORIZONTAL_MARGIN, Game::SCREENHEIGHT / PAUSE_PANEL_DIVIDER_Y + FONT_SIZE_OPTIONS * 2 + VERTICAL_MARGIN, FONT_SIZE_OPTIONS, RAYWHITE);

				DrawRectangle(muteButton.x, muteButton.y, muteButton.width, muteButton.height, LIGHTGRAY);
				DrawText(FormatText("Mute ON/OFF"), Game::HALF_SCREENWIDTH - (MeasureText("Mute ON/OFF", FONT_SIZE_OPTIONS) / DIVIDER_MEASURE_TEXT * MULTIPLIER_BUTTON_WIDTH) + HORIZONTAL_MARGIN, Game::SCREENHEIGHT / PAUSE_PANEL_DIVIDER_Y + FONT_SIZE_OPTIONS * 3 + VERTICAL_MARGIN, FONT_SIZE_OPTIONS, RAYWHITE);
			}
		}
	}
}