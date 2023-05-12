#include "raylib.h"

enum Sides {
LEFT = 0,
RIGHT = 1
};

struct Speed {
	float x, y;
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

	Rectangle GetRect() {
		return Rectangle{ m_position.x - (m_size.width / 2), m_position.y - (m_size.height / 2), m_size.width, m_size.height };
	}

	Vector2 GetPosition() {
		return m_position;
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

class Ball {
private:
	const float m_radius = 5; 
	Speed m_speed = {300, 300};
	Color m_colour = WHITE;
	Vector2 m_position;

public:
	Ball(Vector2 position) {
		m_position = position;
	}

	Vector2 GetPosition() {
		return m_position;
	}

	Speed GetSpeed() {
		return m_speed;
	}

	void SetPosition(Vector2 position) {
		m_position = position;
	}

	void SetSpeed(Speed speed) {
		m_speed = speed;
	}
	void Move() {
		m_position.x += m_speed.x * GetFrameTime();
		m_position.y += m_speed.y * GetFrameTime();
	}

	void Render() {
		DrawCircle((int) m_position.x, (int) m_position.y, m_radius, m_colour);
	}
};


void checkCollitions(Ball* ball, Paddle leftPaddle, Paddle rightPaddle) {
	Vector2 ballPosition = (*ball).GetPosition();
	Speed ballSpeed = (*ball).GetSpeed();
	Vector2 leftPaddlePosition = leftPaddle.GetPosition();
	Vector2 rightPaddlePosition = rightPaddle.GetPosition();

	if (ballPosition.y < 0) {
		(*ball).SetPosition({ ballPosition.x, 0 });
		(*ball).SetSpeed({ ballSpeed.x, -ballSpeed.y });
	}
	else if (ballPosition.y > GetScreenHeight()) {
		(*ball).SetPosition({ ballPosition.x, (float) GetScreenHeight()});
		(*ball).SetSpeed({ ballSpeed.x, -ballSpeed.y });
	}
}


int main() {
	InitWindow(800, 600, "Pong");
	SetTargetFPS(144);

	const Vector2 screenCentre = { GetScreenWidth() / 2.0F ,  GetScreenHeight() / 2.0F };

	Paddle leftPaddle(LEFT, {50, screenCentre.y});
	Paddle rightPaddle(RIGHT, { GetScreenWidth() - 50.0F, screenCentre.y });
	Ball ball(screenCentre);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		checkCollitions(&ball, leftPaddle, rightPaddle);
		ball.Move();

		ball.Render();
		leftPaddle.Render();
		rightPaddle.Render();

		DrawFPS(5, 5);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}