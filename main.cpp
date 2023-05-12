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
	  struct { float width, height; } m_size;
	  struct { KeyboardKey up, down; } m_controls;

  private:
	  Rectangle GetRect() {
		  return Rectangle{ m_position.x - (m_size.width / 2), m_position.y - (m_size.height / 2), m_size.width, m_size.height };
	  }

  public:
	  Paddle(Sides side, Vector2 position) {
		  m_side = side;
		  m_position = position;
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


	  return 0;
  }