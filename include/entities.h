#include <raylib.h>

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

struct RectangleSize {
	float width, height;
};


struct Win {
	const char* text;
	const Sides side;
};


class Paddle {
public:
	Sides m_side;
	int m_points = 0;
	float m_speed = 500;
	Vector2 m_position;
	Vector2 m_originalPosition;
	Color m_colour = WHITE;
	RectangleSize m_size = { 10, 100 };
	struct { KeyboardKey up, down; } m_controls;
	Texture2D m_texture = LoadTexture("assets/textures/Paddle.png");

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
		return Rectangle{ centrePostition.x, centrePostition.y, m_size.width, m_size.height };
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

	RectangleSize GetSize() {
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
		Rectangle source = { 0,0, m_texture.width, m_texture.height }; // What part of texture you want
		Rectangle destination = { m_position.x, m_position.y, m_size.width, m_size.height }; // Where it will be rendered
		DrawTexturePro(m_texture, source, destination, { m_size.width / 2, m_size.height / 2 }, 0, WHITE);
	}

	void Reset()
	{
		m_position = m_originalPosition;
		m_speed = 300;
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
		DrawCircle((int)m_position.x, (int)m_position.y, m_radius, m_colour);
	}

	void Reset() {
		m_speed = m_originalSpeed;
		m_position = m_originalPosition;
	}
};