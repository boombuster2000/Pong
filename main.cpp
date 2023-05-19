#include "raylib.h"

enum Sides {
LEFT = 0,
RIGHT = 1
};

struct Speed {
	float x, y;
};

struct Size {
	float width, height;
};

class Paddle {
private:
	Sides m_side;
	int m_points = 0;
	float m_speed = 300;
	Vector2 m_position;
	Color m_colour = WHITE;
	Size m_size = { 10, 100};
	struct { KeyboardKey up, down; } m_controls;

public:
	Paddle(Sides side, Vector2 position) {
		m_side = side;
		m_position = position;

		if (side == LEFT) {
			m_controls.up = KEY_W;
			m_controls.down = KEY_S;
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

	Size GetSize() {
		return m_size;
	}

	void Move() {
		if (IsKeyDown(m_controls.up)) {
			m_position.y -= m_speed * GetFrameTime();
		}
		else if (IsKeyDown(m_controls.down)) {
			m_position.y += m_speed * GetFrameTime();
		}
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

	const float GetRadius() {
		return m_radius;
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
	const Vector2 ballPosition = (*ball).GetPosition();
	const Speed ballSpeed = (*ball).GetSpeed();
	const float ballRadius = (*ball).GetRadius();

	const Vector2 leftPaddlePosition = leftPaddle.GetPosition();
	const Size leftPaddleSize = leftPaddle.GetSize();

	const Vector2 rightPaddlePosition = rightPaddle.GetPosition();
	const Size rightPaddleSize = rightPaddle.GetSize();

	// checks collitions on top and bottom of screen
	if (ballPosition.y < 0) {
		(*ball).SetPosition({ ballPosition.x, 0 });
		(*ball).SetSpeed({ ballSpeed.x, -ballSpeed.y });
	}
	else if (ballPosition.y > GetScreenHeight()) {
		(*ball).SetPosition({ ballPosition.x, (float) GetScreenHeight()});
		(*ball).SetSpeed({ ballSpeed.x, -ballSpeed.y });
	}

	// checks collitions with paddles
	if (CheckCollisionCircleRec(ballPosition, ballRadius, leftPaddle.GetRect())) {
		if (ballSpeed.x < 0) {
			(*ball).SetSpeed({ ballSpeed.x * -1.1f, (ballPosition.y - leftPaddlePosition.y) / (leftPaddleSize.height / 2) * ballSpeed.x});
		}
	}
	else if (CheckCollisionCircleRec(ballPosition, ballRadius, rightPaddle.GetRect())) {
		if (ballSpeed.x > 0) {
			(*ball).SetSpeed({ ballSpeed.x * -1.1f, (ballPosition.y - rightPaddlePosition.y) / (rightPaddleSize.height / 2) * -ballSpeed.x });
		}
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
		leftPaddle.Move();
		rightPaddle.Move();

		ball.Render();
		leftPaddle.Render();
		rightPaddle.Render();

		DrawFPS(5, 5);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}