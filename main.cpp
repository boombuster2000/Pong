#include "raylib.h"

	enum Sides {
	LEFT = 0,
	RIGHT = 1
};

class Paddle {
private:
	Sides m_side;
	int m_points = 0;
	float m_speed = 300;
	Vector2 m_position;
	Color m_colour = WHITE;
	struct { float width = 10, height = 100; } m_size;
	struct { KeyboardKey up, down; } m_controls;

private:
	Rectangle GetRect() {
		return Rectangle{ m_position.x - (m_size.width / 2), m_position.y - (m_size.height / 2), m_size.width, m_size.height };
	}

public:
	Paddle(Sides side, Vector2 position) {
		m_side = side;
		m_position = position;

		if (side == LEFT) {
			m_controls.up = KEY_A;
			m_controls.down = KEY_D;
		}
		else if (side == RIGHT) {
			m_controls.up = KEY_UP;
			m_controls.down = KEY_DOWN;
		}
	}

	Sides GetSide() {
		return m_side;
	}

	int GetPoints() {
		return m_points;
	}

	void Render() {
		DrawRectangleRec(GetRect(), WHITE);
	}
};



int main() {
	InitWindow(800, 600, "Pong");
	SetTargetFPS(144);

	const Vector2 SCREEN_CENTRE = { GetScreenWidth() / 2 ,  GetScreenHeight() / 2 };

	Paddle leftPaddle(LEFT, {50, SCREEN_CENTRE.y});
	Paddle rightPaddle(RIGHT, { GetScreenWidth() - 50.0F, SCREEN_CENTRE.y });


	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		leftPaddle.Render();
		rightPaddle.Render();


		DrawFPS(5, 5);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}