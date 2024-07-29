#define BACKGROUND_COLOR 0x237458

struct Player {
	float xPos = 50;
	float yPos = 50;
	float xSize = 60;
	float ySize = 60;
	float color = 0xF0C900;
} player;

bool isDown(int inputButton);
bool isPressed(int inputButton);
bool isReleased(int inputButton);
void simulate_game(Input* input);


void simulate_game(Input* input)
{
	ClearScreen(BACKGROUND_COLOR);

	if (isPressed(BUTTON_UP)) {
		player.yPos += 1;
	}
	if (isPressed(BUTTON_LEFT)) {
		player.xPos -= 1;
	}
	if (isPressed(BUTTON_DOWN)) {
		player.yPos -= 1;
	}
	if (isPressed(BUTTON_RIGHT)) {
		player.xPos += 1;
	}
	DrawRectDynamicPosAndSize(player.xPos, player.yPos, player.xSize, player.ySize, player.color);
}


bool isDown(int inputButton)
{
	return input.buttons[inputButton].isDown;
}

bool isPressed(int inputButton)
{
	return input.buttons[inputButton].isDown && input.buttons[inputButton].changed;
}

bool isReleased(int inputButton)
{
	return !input.buttons[inputButton].isDown && input.buttons[inputButton].changed;
}
