#include <windows.h>
#include "structs.cpp"

bool isRunning = true;

// storing all the buffer variables in a struct
// to keep things organised.
//struct RenderBuffer {
//	int width, height, size;
//	void* memory;
//	BITMAPINFO bitmapInfo;
//};
RenderBuffer renderBuffer;
// for renderer to have access right click on file and
// exclude it from build.
#include "renderer.cpp"
#include "buttons.cpp"

//void* bufferMemory;
//int bufferWidth;
//int bufferHeight;
//int bufferSize;
//BITMAPINFO bufferBitmapInfo;

// Button input info
Input input = {};
#include "game.cpp"

// This is for the callback (window.lpfnWndProc)
LRESULT CALLBACK WindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	// Create Window Class
	WNDCLASS windowClass = {};
	// Makes sure you r drawing the window horizontal & vertical
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	// Class name
	windowClass.lpszClassName = L"Window Class";
	// Callback function (triggered when anything happens to the window)
	windowClass.lpfnWndProc = WindowCallback;
	// Get window handle
	windowClass.hInstance = hInst;


	// Register Class
	RegisterClass(&windowClass);


	// Create Window
	HWND window = CreateWindowEx(
		0,									// Optional window styles.
		windowClass.lpszClassName,				// Window class
		L"Perle's Shape Algos",		// Window text
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,   // Window style

											// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,								// Parent window    
		NULL,								// Menu
		windowClass.hInstance,					// Instance handle
		NULL								// Additional application data
	);
	// Device context for strechDIBits() function in render
	HDC hDeviceContext = GetDC(window);


	// Loop that keeps the window running
	while (isRunning) {

// Input
		MSG message;

		// Reset the button state because changed should only be true while its pressed
		for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
			input.buttons[i].changed = false;
			// is down will change itself.
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) { 
			TranslateMessage(&message);
			DispatchMessageW(&message);

			switch (message.message) {
			// if any key is released
			case WM_KEYUP:
			// if any key is pressed
			case WM_KEYDOWN: {
				unsigned int code = message.wParam;
				bool isDown = (message.wParam & (1 << 31)) == 0;

				// the key that has been either pressed or released
				switch (code) {
				case VK_UP:
					input.buttons[BUTTON_UP].isDown = isDown;
					input.buttons[BUTTON_UP].changed = true;
					break;
				case VK_DOWN:
					input.buttons[BUTTON_DOWN].isDown = isDown;
					input.buttons[BUTTON_DOWN].changed = true;
					break;
				case VK_LEFT:
					input.buttons[BUTTON_LEFT].isDown = isDown;
					input.buttons[BUTTON_LEFT].changed = true;
					break;
				case VK_RIGHT:
					input.buttons[BUTTON_RIGHT].isDown = isDown;
					input.buttons[BUTTON_RIGHT].changed = true;
					break;
				}
			} break;
			default:
				TranslateMessage(&message);
				DispatchMessageW(&message);
			}
		}

// Sim
		simulate_game(&input);

		// Testing out my shapes
		DrawRect(Point(3, 23), 234, 111, 0xf00fff, false);
		DrawSquare(Point(300, 400), 200, 0xf00fff, false);
		DrawRect(Point(899, 234), 133, 600, 0xfff00f);
		DrawTriangle(Point(23, 41), Point(245, 77), Point(234, 500), 0xf00fff);
		DrawRectDynamicPosAndSize(45, 45, 45, 45);
		int midpointX = renderBuffer.width / 2;
		int midpointY = renderBuffer.height / 2;
		DrawCircleFill(Point(midpointX,midpointY),30,0xffff0);
		DrawRect(Point(10, 234), 133, 600, 0xfff00f, false);
		DrawCircle(Point(800, 100), 10, 0x0fff0);
		DrawTriangle(Point(300, 410), Point(400, 300), Point(350, 500), 0xf00fff);

// Refresh
		// Need the Device Context, Buffer information, and the Bitmap Info
		StretchDIBits(hDeviceContext, 0, 0, renderBuffer.width, renderBuffer.height, 0, 0, renderBuffer.width, renderBuffer.height, renderBuffer.memory, &renderBuffer.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}
}

LRESULT CALLBACK WindowCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	// Switch that deals with the different call back messages
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY:
		isRunning = false;
		break;

	case WM_SIZE:						// Getting the new size of the window
		RECT rect;						// Create a rectangle struct
		GetClientRect(hwnd, &rect);		// And pass it into this function along with the window
		renderBuffer.width = rect.right - rect.left;
		renderBuffer.height = rect.bottom - rect.top;

									// number of pixels                   size of each pixel
		renderBuffer.size = (renderBuffer.width * renderBuffer.height) * sizeof(unsigned int);
		// Checking if bufferMemory contains stuff in it, if it does
		// we will free that space and make a new one in place of it
		if (renderBuffer.memory) VirtualFree(renderBuffer.memory, 0, MEM_RELEASE);
		// Reserving memory in the heap without specifying the type
		// because we need this space to store the buffer size.
		// bufferMemory is of type void*
		// PAGE_READWRITE because the content in the the space in this heap is
		// going to keep changing as the user resizes the window.
		renderBuffer.memory = VirtualAlloc(0, renderBuffer.size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		// Now send this to windows to use it in render.

		// You need the bitmap info header for render as well.
		renderBuffer.bitmapInfo.bmiHeader.biSize = sizeof(renderBuffer.bitmapInfo.bmiHeader);
		renderBuffer.bitmapInfo.bmiHeader.biWidth = renderBuffer.width;
		renderBuffer.bitmapInfo.bmiHeader.biHeight = renderBuffer.height;
		renderBuffer.bitmapInfo.bmiHeader.biPlanes = 1;
		renderBuffer.bitmapInfo.bmiHeader.biBitCount = 32;
		renderBuffer.bitmapInfo.bmiHeader.biCompression = BI_RGB;
		// using the struct below to know which attributes this struct
		// contains that we can change or need to fill in.
		// 
		//typedef struct tagBITMAPINFOHEADER {
		//	DWORD biSize;
		//	LONG  biWidth;
		//	LONG  biHeight;
		//	WORD  biPlanes;
		//	WORD  biBitCount;
		//	DWORD biCompression;
		//	DWORD biSizeImage;
		//	LONG  biXPelsPerMeter;
		//	LONG  biYPelsPerMeter;
		//	DWORD biClrUsed;
		//	DWORD biClrImportant;
		//} BITMAPINFOHEADER, * LPBITMAPINFOHEADER, * PBITMAPINFOHEADER;

		break;
	default:
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}