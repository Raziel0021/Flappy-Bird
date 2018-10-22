#ifndef GAMEPLAY
#define GAMEPLAY

#include <raylib.h>

namespace Game {
	namespace GamePlay {

		struct Player
		{
			Vector2 position;
			Vector2 size;
			float velocity;
			float acceleration;
			int points;
		};

		//extern Texture2D rocket;
		//extern Texture2D asteroid;
		//extern Sound shootSound;
		//extern Sound explosionSound;
		//extern Music music;
		extern Player player;
		extern bool gameover;
		extern bool pause;
		//extern bool victory;

		void InitGame();
		void Play();
		void DrawGame();
	}
}

#endif // !GAMEPLAY