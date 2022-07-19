#include <SFML/Graphics.hpp>
#include <time.h>

#define SNAKE_MAX_SIZE 52 // determina o fim de jogo
#define SNAKE_INITIAL_DIRECTION "right"
#define SNAKE_VELOCITY 0.1 // quanto maior, mais rápido

using namespace sf;

int score = 2;
String direction = SNAKE_INITIAL_DIRECTION;


struct Snake {
	int posX, posY;
} snake[SNAKE_MAX_SIZE];

struct Fruit {
	int posX, posY;
} fruit;

void spawn_fruit() {
	fruit.posX = rand() % 58;
	fruit.posY = rand() % 32;
}

void snake_update() {
	for (int i = score; i > 0; i--) {
		snake[i].posX = snake[i - 1].posX;
		snake[i].posY = snake[i - 1].posY;
	}

	if (direction == "left") {
		snake[0].posX -= 1;
	}
	if (direction == "right") {
		snake[0].posX += 1;
	}
	if (direction == "up") {
		snake[0].posY -= 1;
	}
	if (direction == "down") {
		snake[0].posY += 1;
	}

	if (snake[0].posX > 59) {
		snake[0].posX = 0;
	}
	if (snake[0].posY > 33) {
		snake[0].posY = 0;
	}
	if (snake[0].posX < 0) {
		snake[0].posX = 59;
	}
	if (snake[0].posY < 0) {
		snake[0].posY = 33;
	}

	for (int i = 1; i < score; i++) {
		if (snake[0].posX == snake[i].posX && snake[0].posY == snake[i].posY) {
			score = 2;
		}
	}

	if (snake[0].posX == fruit.posX && snake[0].posY == fruit.posY) {
		score++;
		spawn_fruit();
	}
}

int main() {
	RenderWindow window(VideoMode(800, 800), "Snake Game - SFML", Style::Fullscreen);

	Font roboto;
	Texture t_snake, t_snake_head, t_fruit;

	Clock clock;
	float timer = 0;

	if(!t_snake.loadFromFile("Assets/Images/snake.png"))
		return EXIT_FAILURE;

	if (!t_snake_head.loadFromFile("Assets/Images/snake_head.png"))
		return EXIT_FAILURE;

	if(!t_fruit.loadFromFile("Assets/Images/fruit.png"))
		return EXIT_FAILURE;

	if(!roboto.loadFromFile("Assets/Fonts/Roboto-Regular.ttf"))
		return EXIT_FAILURE;

	Sprite s_snake(t_snake);
	Sprite s_snake_head(t_snake_head);
	Sprite s_fruit(t_fruit);

	Text t_score("Score: ", roboto, 50);
	Text t_exit("Pressione Q para sair", roboto, 70);
	Text t_win("Você venceu!", roboto, 150);

	spawn_fruit();

	while(window.isOpen()) {
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		while(window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				window.close();
			}
		}

		if(Keyboard::isKeyPressed(Keyboard::Q)) {
			window.close();
		}

		if(Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) {
			direction = "left";
		}

		if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) {
			direction = "right";
		}

		if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) {
			direction = "up";
		}

		if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
			direction = "down";
		}

		if (timer > SNAKE_VELOCITY) {
			timer = 0;
			snake_update();
		}

		window.clear(Color(146, 255, 124, 255));
		if (score != SNAKE_MAX_SIZE) {
			for (int i = 0; i < score; i++) {
				if (i > 0) {
					s_snake.setPosition(snake[i].posX * 32, snake[i].posY * 32);
					window.draw(s_snake);
				}
				else {
					if (direction == "left") {
						s_snake_head.setOrigin(0, 32);
						s_snake_head.setRotation(90);
					}
					if (direction == "right") {
						s_snake_head.setOrigin(32, 0);
						s_snake_head.setRotation(-90);
					}
					if (direction == "up") {
						s_snake_head.setOrigin(32, 32);
						s_snake_head.setRotation(-180);
					}
					if (direction == "down") {
						s_snake_head.setOrigin(0, 0);
						s_snake_head.setRotation(0);
					}
					s_snake_head.setPosition(snake[i].posX * 32, snake[i].posY * 32);
					window.draw(s_snake_head);
				}
			}

			s_fruit.setPosition(fruit.posX * 32, fruit.posY * 32);
			window.draw(s_fruit);

			t_score.setFillColor(Color::Red);
			t_score.setString("Score: " + std::to_string(score - 2));
			t_score.setPosition(VideoMode::getDesktopMode().width / 2 - t_score.getGlobalBounds().width / 2, 0);
			window.draw(t_score);
		}
		else {
			t_win.setStyle(Text::Bold);
			t_win.setPosition(VideoMode::getDesktopMode().width / 2 - t_win.getGlobalBounds().width / 2, VideoMode::getDesktopMode().height / 2 - VideoMode::getDesktopMode().height * 0.1);
			window.draw(t_win);
		}
		t_exit.setStyle(Text::Bold);
		t_exit.setPosition(VideoMode::getDesktopMode().width / 2 - t_exit.getGlobalBounds().width / 2, VideoMode::getDesktopMode().height - VideoMode::getDesktopMode().height * 0.1);
		window.draw(t_exit);
		window.display();
	}
	return 0;
}