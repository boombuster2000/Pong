#include "raylib.h"

enum Sides {
LEFT = 0,
RIGHT = 1,
NONE = -1
};

enum MenuOption {
	OPT_NONE = -1,
	TWO_PLAYER = 1,
	EXIT = 2
};

struct Text {
	const char* text;
	int fontSize;
	
	void Render(Vector2 position, Color colour) {
		const int textSize = MeasureText(text, fontSize);
		DrawText(text, position.x - textSize / 2, position.y - fontSize / 2, fontSize, colour);
	}
};

struct Speed {
	float x, y;
};

struct Win {
	const char* text;
	const Sides side;
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
	Vector2 m_originalPosition;
	Color m_colour = WHITE;
	Size m_size = { 10, 100 };
	struct { KeyboardKey up, down; } m_controls;
	Texture2D m_texture = LoadTexture("resources/textures/Paddle.png");

public:
	Paddle(Sides side, Vector2 position) {
		m_side = side;
		m_position = position;
		m_originalPosition = position;

		if (side == LEFT) {
			m_controls.up = KEY_W;
			m_controls.down = KEY_S;
		}
		else if (side == RIGHT) {
			m_controls.up = KEY_UP;
			m_controls.down = KEY_DOWN;
		}
	}

	Rectangle GetRect() { //rect version of texture to keep collitions in sync.
		Vector2 centrePostition = { m_position.x - (m_size.width / 2), m_position.y - (m_size.height / 2) };
		return Rectangle{centrePostition.x, centrePostition.y, m_size.width, m_size.height };
	}

	void ResetPosition() {
		m_position = m_originalPosition;
	}

	Vector2 GetPosition() {
		return m_position;
	}

	Sides GetSide() {
		return m_side;
	}

	void AddPoint() {
		m_points++;
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
		Rectangle source = {0,0, m_texture.width, m_texture.height}; // What part of texture you want
		Rectangle destination = {m_position.x, m_position.y, m_size.width, m_size.height}; // Where it will be rendered
		DrawTexturePro(m_texture, source, destination, {m_size.width/2, m_size.height/2}, 0, WHITE);
	}
};

class Ball {
private:
	const float m_radius = 5; 
	Speed m_speed;
	Speed m_originalSpeed;
	Color m_colour = WHITE;
	Vector2 m_position;
	Vector2 m_originalPosition;

public:
	Ball(Vector2 position, Speed speed) {
		m_position = position;
		m_originalPosition = position;
		m_speed = speed;
		m_originalSpeed = speed;
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

	void Reset() {
		m_speed = m_originalSpeed;
		m_position = m_originalPosition;
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
			//changes angle depending where hit on paddle, closer to middle of paddle less of angle it bounces off (slower y speed)
			(*ball).SetSpeed({ ballSpeed.x * -1.1f, (ballPosition.y - leftPaddlePosition.y) / (leftPaddleSize.height / 2) * ballSpeed.x});
		}
	}
	else if (CheckCollisionCircleRec(ballPosition, ballRadius, rightPaddle.GetRect())) {
		if (ballSpeed.x > 0) {
			//changes angle depending where hit on paddle, closer to middle of paddle less of angle it bounces off (slower y speed)

			(*ball).SetSpeed({ ballSpeed.x * -1.1f, (ballPosition.y - rightPaddlePosition.y) / (rightPaddleSize.height / 2) * -ballSpeed.x });
		}
	}
}

Win checkWin(Ball ball) {
	const Vector2 ballPosition = ball.GetPosition();

	if (ballPosition.x <= 0) {
		return Win{ "Right Side Wins!", RIGHT };
	}
	else if (ballPosition.x >= GetScreenWidth()) {
		return Win{ "Left Side Wins!", LEFT };
	}
	else {
		return Win{ nullptr, NONE };
	}
}

void display_menu(MenuOption selected) {

	Text title = { "Menu", 100 };
	Text two_player = { "2 Player", 40 };
	Text exit = { "Exit", 40 };

	if (selected == TWO_PLAYER) {
		two_player.text = "> 2 Player";
	} 
	else if (selected == EXIT) {
		exit.text = "> Exit";
	}

	title.Render({GetScreenWidth() / 2.0F ,75 }, YELLOW);
	two_player.Render({ GetScreenWidth() / 2.0F ,250 }, YELLOW);
	exit.Render({ GetScreenWidth() / 2.0F ,500 }, YELLOW);


}


MenuOption menu(MenuOption selected) {
	
	if (IsKeyDown(KEY_W) && selected != TWO_PLAYER) {
		selected = TWO_PLAYER;
	}
	else if (IsKeyDown(KEY_S) && selected != EXIT) {
		selected = EXIT;
	}

	display_menu(selected);

	return selected;
}

int main() {
	InitWindow(800, 600, "Pong");
	SetTargetFPS(144);

	const Vector2 screenCentre = { GetScreenWidth() / 2.0F ,  GetScreenHeight() / 2.0F };

	Paddle leftPaddle(LEFT, {50, screenCentre.y});
	Paddle rightPaddle(RIGHT, { GetScreenWidth() - 50.0F, screenCentre.y });
	Ball ball(screenCentre, Speed {300,300});

	bool won = false;
	bool reseted = true;
	MenuOption selected = TWO_PLAYER;

	// main game
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		selected = menu(selected);





		//if (!won) { // freezes movement if there is a winner
		//	checkCollitions(&ball, leftPaddle, rightPaddle);
		//	ball.Move();
		//	leftPaddle.Move();
		//	rightPaddle.Move();
		//}

		////Render objects
		//ball.Render();
		//leftPaddle.Render();
		//rightPaddle.Render();


		//// Render Points
		//Text leftPaddlePointsText = { TextFormat("%i", leftPaddle.GetPoints()), 30};
		//Text rightPaddlePointsText = { TextFormat("%i", rightPaddle.GetPoints()), 30 };
		//
		//leftPaddlePointsText.Render({ 30,40 }, YELLOW);
		//rightPaddlePointsText.Render({ (float) GetScreenWidth() - 30,40}, YELLOW);


		//Win win = checkWin(ball);

		//if (win.side != NONE and win.text != NULL) {
		//	won = true;

		//	// Adds the points
		//	if (reseted) {
		//		if (win.side == leftPaddle.GetSide()) leftPaddle.AddPoint();
		//		else if (win.side == rightPaddle.GetSide()) rightPaddle.AddPoint();
		//		reseted = false;
		//	}
		//	
		//	Text winText = {win.text, 60}; // win message
		//	winText.Render(screenCentre, YELLOW);

		//	Text playAgainText = { "Press space to play again", 25 };
		//	playAgainText.Render({ screenCentre.x, screenCentre.y + 50 }, YELLOW);

		//	// Reset Game
		//	if (IsKeyPressed(KEY_SPACE)) {
		//		ball.Reset();
		//		leftPaddle.ResetPosition();
		//		rightPaddle.ResetPosition();
		//		win.text = nullptr;
		//		won = false;
		//		reseted = true;
		//	}
		//}

		DrawFPS(5, 5);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}