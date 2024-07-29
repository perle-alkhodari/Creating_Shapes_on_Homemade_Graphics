struct ButtonState {
	bool isDown;
	bool changed;
};

enum {
	BUTTON_UP, BUTTON_DOWN,
	BUTTON_LEFT, BUTTON_RIGHT,

	NUMBER_OF_BUTTONS
};

struct Input {
	ButtonState buttons[NUMBER_OF_BUTTONS];
};