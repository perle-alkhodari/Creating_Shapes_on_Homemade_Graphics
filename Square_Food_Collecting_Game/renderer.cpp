#include "helpers.cpp"

#define DEFAULT_COLOR 0xffffff


void Render();
void ClearScreen(int color);
unsigned int* SetPixelPointer(Point p);
void DrawPoint(Point p, int color);
void DrawLine(Point p1, Point p2, int color);
void DrawRect(Point p, int width, int height, int color);
void DrawRectDynamicPosition(float x, float y, float width, float height, int color);
void DrawRectDynamicPosAndSize(float x, float y, float width, float height, int color);
void DrawTriangle(Point p1, Point p2, Point p3, int color);


void Render(){ 
	unsigned int* pixel = (unsigned int*)renderBuffer.memory;
	for (int y = 0; y < renderBuffer.height; y++) {
		for (int x = 0; x < renderBuffer.width; x++) {
			// Do something to the pixel.
			// then increment to next.
			*pixel++ = (0Xff00ff * x) + (0x00ff00 * y);
		}
	}
}

unsigned int* SetPixelPointer(Point p)
{
	unsigned int* pixel = (unsigned int*)renderBuffer.memory;
	pixel += ((p.y - 1) * renderBuffer.width) + p.x;
	return pixel;
}

void ClearScreen(int color) {
	unsigned int* pixel = (unsigned int*)renderBuffer.memory;
	for (int y = 0; y < renderBuffer.height; y++) {
		for (int x = 0; x < renderBuffer.width; x++) {
			// Do something to the pixel.
			// then increment to next.
			*pixel++ = color;
		}
	}
} 

void DrawPoint(Point p, int color = DEFAULT_COLOR) {
	p.x = Bracket(0, renderBuffer.width, p.x);
	p.y = Bracket(0, renderBuffer.height, p.y);

	unsigned int* pixel = SetPixelPointer(p);
	*pixel = color;
}

void DrawLine(Point p1, Point p2, int color = DEFAULT_COLOR) {
	// Bracket for first point
	p1.x = Bracket(0, renderBuffer.width, p1.x);
	p1.y = Bracket(0, renderBuffer.height, p1.y);	

	// Bracket for second point
	p2.x = Bracket(0, renderBuffer.width, p2.x);
	p2.y = Bracket(0, renderBuffer.height, p2.y);

	// In case x2 < x1
	if (p2.x < p1.x) Swap(p1.x, p2.x), Swap(p1.y, p2.y);

	// Get line slope (m)
	float mSlope = (float)(p2.y - p1.y) / (float)(p2.x - p1.x);

	int y;
	Point p;
	for (int i = p1.x; i <= p2.x; i++) {
		y = (mSlope * (i - p1.x)) + p1.y;
		p = Point(i, y);
		DrawPoint(p, color);
	}
}

void DrawRect(Point p, int width, int height, int color = DEFAULT_COLOR) {

	p.x = Bracket(0, renderBuffer.width, p.x);
	int posX2 = Bracket(0, renderBuffer.width, p.x+width);
	p.y = Bracket(0, renderBuffer.height, p.y);
	int posY2 = Bracket(0, renderBuffer.height, p.y+height);

	for (int i = p.y; i < posY2; i++) {
		unsigned int* pixel = (unsigned int*)renderBuffer.memory + (((i)*renderBuffer.width) + p.x);
		for (int j = p.x; j < posX2; j++) {
			*pixel++ = color;
		}
	}
}

void DrawRectDynamicPosition(float x, float y, float width, float height, int color = DEFAULT_COLOR)
{
	x /= 100;
	y /= 100;
	x *= renderBuffer.width;
	y *= renderBuffer.height;
	x -= (width / 2);
	y -= (height / 2);
	Point p((int)x, (int)y);

	DrawRect(p, (int)width, (int)height, color);
}

void DrawRectDynamicPosAndSize(float x, float y, float width, float height, int color = DEFAULT_COLOR)
{
	Bracket(0, 100, (int)x);
	Bracket(0, 100, (int)y);
	Bracket(0, renderBuffer.width, (int)width);
	Bracket(0, renderBuffer.height, (int)height);

	x /= 100;
	y /= 100;
	x *= renderBuffer.width;
	y *= renderBuffer.height;
	x -= (width / 2);
	y -= (height / 2);
	Point p((int)x, (int)y);

	DrawRect(p, (int)width, (int)height, color);
}

void DrawTriangle(Point p1, Point p2, Point p3, int color = DEFAULT_COLOR, bool wireframe=true)
{
	if (wireframe) {
		DrawLine(p1, p2);
		DrawLine(p2, p3);
		DrawLine(p3, p1);
	}
}

