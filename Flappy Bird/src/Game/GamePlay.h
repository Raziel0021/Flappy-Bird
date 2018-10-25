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

		extern Texture2D Bird;
		extern Texture2D Background;
		extern Sound jumpSound;
		extern Sound collisionSound;
		extern Music music;
		extern Player player;
		extern bool gameover;
		extern bool pause;

		void InitGame();
		void Play();
		void DrawGame();
	}
}

#endif // !GAMEPLAY