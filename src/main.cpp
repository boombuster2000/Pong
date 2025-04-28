#include "raylib.h"
#include "entities.h"


void checkCollitions(Ball* ball, Paddle leftPaddle, Paddle rightPaddle) {
	const Vector2 ballPosition = (*ball).GetPosition();
	const Speed ballSpeed = (*ball).GetSpeed();
	const float ballRadius = (*ball).GetRadius();

	const Vector2 leftPaddlePosition = leftPaddle.m_position;
	const RectangleSize leftPaddleSize = leftPaddle.GetSize();

	const Vector2 rightPaddlePosition = rightPaddle.GetPosition();
	const RectangleSize rightPaddleSize = rightPaddle.GetSize();

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

void renderMenu(MenuOption selected) {

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
	
	renderMenu(selected);

	if (IsKeyDown(KEY_W) && selected != TWO_PLAYER) {
		selected = TWO_PLAYER;
	}
	else if (IsKeyDown(KEY_S) && selected != EXIT) {
		selected = EXIT;
	}

	return selected;
}

void runGame() {
	const Vector2 screenCentre = { GetScreenWidth() / 2.0F ,  GetScreenHeight() / 2.0F };

	Paddle leftPaddle(LEFT, { 50, screenCentre.y });
	Paddle rightPaddle(RIGHT, { GetScreenWidth() - 50.0F, screenCentre.y });
	Ball ball(screenCentre, Speed{ 300,300 });
	bool hasPointAdded = false; // prevents points being added each frame after someone won

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		Win win = checkWin(ball);

		if (win.side != NONE and win.text != NULL) {

			// Adds the points
			if (!hasPointAdded) {
				if (win.side == leftPaddle.m_side) leftPaddle.m_points++;
				else if (win.side == rightPaddle.m_side) rightPaddle.m_points++;
				hasPointAdded = true;
			}

			Text winText = { win.text, 60 }; // win message
			winText.Render(screenCentre, YELLOW);

			Text playAgainText = { "Press space to play again", 25 };
			playAgainText.Render({ screenCentre.x, screenCentre.y + 75 }, YELLOW);

			Text exitText = { "Press backspace to exit", 25 };
			exitText.Render({ screenCentre.x, screenCentre.y + 110 }, YELLOW);

			// Reset Game
			if (IsKeyPressed(KEY_SPACE)) {
				ball.Reset();
				leftPaddle.Reset();
				rightPaddle.Reset();
				hasPointAdded = false;

			}
			else if (IsKeyPressed(KEY_BACKSPACE)) {
				break;
			}
		}

		else { // freezes movement if there is a winner
			checkCollitions(&ball, leftPaddle, rightPaddle);
			ball.Move();
			leftPaddle.Move();
			rightPaddle.Move();
		}

		//Render objects
		ball.Render();
		leftPaddle.Render();
		rightPaddle.Render();


		// Render Points
		Text leftPaddlePointsText = { TextFormat("%i", leftPaddle.m_points), 30 };
		Text rightPaddlePointsText = { TextFormat("%i", rightPaddle.m_points), 30 };

		leftPaddlePointsText.Render({ 30,40 }, YELLOW);
		rightPaddlePointsText.Render({ (float)GetScreenWidth() - 30,40 }, YELLOW);


		DrawFPS(5, 5);
		EndDrawing();

	}


}

int main() {
	InitWindow(800, 600, "Pong");
	SetTargetFPS(144);

	MenuOption selected = TWO_PLAYER;

	// main appliccation
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		if (!IsKeyPressed(KEY_ENTER)) {
			selected = menu(selected);
		}
		else {
			if (selected == TWO_PLAYER) {
				runGame();
			}
			else if (selected == EXIT) {
				break;
			}
		}

		DrawFPS(5, 5);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}