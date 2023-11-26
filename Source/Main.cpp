#include <array>
#include <chrono>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/SpriteManager.hpp"
#include "Headers/Animation.hpp"
#include "Headers/Stripe.hpp"
#include "Headers/Decoration.hpp"
#include "Headers/Player.hpp"
#include "Headers/Steven.hpp"
#include "Headers/Game.hpp"

//Whatever you do, DON'T SCROLL DOWN!!!!
//There's nothing to see!
//DO NOT SCROLL DOWN!!!!

int main()
{
	//Randomizing the seed.
	srand(std::chrono::system_clock::now().time_since_epoch().count());

	std::chrono::microseconds lag(0);

	std::chrono::steady_clock::time_point previous_time;

	//This should be 30 since I switched to 30 FPS.
	std::string fps_text = "FPS: 60";

	Game game;

	previous_time = std::chrono::steady_clock::now();

	game.draw();

	while (1 == game.is_open())
	{
		std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);

		lag += delta_time;

		previous_time += delta_time;

		//The game lagged a lot during debugging so I needed to see the FPS.
		fps_text = "FPS: " + std::to_string(1000000.f / delta_time.count());

		while (gbl::SCREEN::FRAME_DURATION <= lag)
		{
			lag -= gbl::SCREEN::FRAME_DURATION;

			game.handle_events();
			game.update();

			if (gbl::SCREEN::FRAME_DURATION > lag)
			{
				game.draw();
				game.set_title(fps_text);
			}
		}
	}
}

//That's it! Now don't scroll further.
































//Hey, STOP!
































//What are you doing? I said STOP SCROLLING!!!!
































//YOU WILL REGRET IT!!!!

































/*

@@@@@@    @@@@@    @@@@@   @  @  @  @
@     @  @     @  @     @  @  @  @  @
@     @  @     @  @     @  @  @  @  @
@@@@@@   @     @  @     @  @  @  @  @
@     @  @     @  @     @  @  @  @  @
@     @  @     @  @     @            
@@@@@@    @@@@@    @@@@@   @  @  @  @

       _______
     _-       -.
	|   |   |   |
	|           |
	|   \__/    |
	 \._______./
	 /        \
   /|          |\
  / |          | \
 |  !--------__|  |
   /           \
  |             |
 _|             |_

...scared ya, didn't I?

*/
