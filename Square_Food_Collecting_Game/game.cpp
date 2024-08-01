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
	bool xInRange = IsInRange(0, renderBuffer.width, player.xPos);
	bool yInRange = IsInRange(0, renderBuffer.height, player.yPos);
	if (isPressed(BUTTON_UP)) {
		if (yInRange) player.yPos += 2;
	}
	if (isPressed(BUTTON_LEFT)) {
		if (xInRange)
		player.xPos -= 2;
	}
	if (isPressed(BUTTON_DOWN)) {
		if (yInRange)
		player.yPos -= 2;
	}
	if (isPressed(BUTTON_RIGHT)) {
		if (xInRange)
		player.xPos += 2;
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
