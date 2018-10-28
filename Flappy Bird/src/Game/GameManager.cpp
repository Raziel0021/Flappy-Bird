#include "GameManager.h"

#include <raylib.h>

#include "GamePlay.h"
#include "Screens/MainMenu.h"
#include "Screens/Credits.h"
#include "Screens/GameOver.h"

using namespace Game;
using namespace MainMenu;
namespace Game 
{
	const int SCREENWIDTH = 1080;
	const int SCREENHEIGHT = 720;
	const float HALF_SCREENHEIGHT = SCREENHEIGHT / 2;
	const float HALF_SCREENWIDTH = SCREENWIDTH / 2;
	bool close=false;
	namespace GameManager 
	{
		void Run() 
		{
			InitializeApp();
			GamePlay::InitGame();
			while (!WindowShouldClose()&&!close)
			{
				if (!GamePlay::gameover && !menu && !Credits::credits) {
					GamePlay::Play();	
				}
				Draw();
			}
		}
		void Draw() 
		{
			BeginDrawing();
			ClearBackground(BLACK);
			if (!GamePlay::gameover && !menu && !Credits::credits)
			{
				GamePlay::DrawGame();
			}
			else if (menu)
			{
				MainMenu::showMenu();
			}
			else if (Credits::credits)
			{
				Credits::showCredits();
			}
			else if (GamePlay::gameover)
			{
				EndOfGame::showGameOver();
			}
			EndDrawing();
		}

		void InitializeApp()
		{
			// Initialization
			InitWindow(SCREENWIDTH, SCREENHEIGHT, "FlappyBird");
			SetExitKey(0);
			
			GamePlay::Bird = LoadTexture("res/Assets/Bird.png");
			GamePlay::Background = LoadTexture("res/Assets/Colinas.png");
			GamePlay::Ground = LoadTexture("res/Assets/Arboles y suelo.png");
			GamePlay::Sky = LoadTexture("res/Assets/Cielo.png");
			//#define AUDIO

			#ifdef AUDIO	
				InitAudioDevice();
				GamePlay::jumpSound = LoadSound("res/Shoot.wav");
				GamePlay::collisionSound= LoadSound("res/Explosion.wav");
				GamePlay::music = LoadMusicStream("res/BackgroundMusic.ogg");
				PlayMusicStream(GamePlay::music);
			#endif // AUDIO
		}
		void UnloadGame()
		{
			// TODO: Unload all dynamic loaded data (textures, sounds, models...)
			UnloadTexture (GamePlay::Bird);
			UnloadTexture(GamePlay::Background);
			#ifdef AUDIO
				UnloadSound(GamePlay::jumpSound);
				UnloadSound(GamePlay::collisionSound);
				UnloadMusicStream(GamePlay::music);
				CloseAudioDevice();
			#endif // AUDIO
			close = true;
		}
	}
}