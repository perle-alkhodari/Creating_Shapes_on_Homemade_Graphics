#include "helpers.cpp"
#include <math.h>
#include <vector>

#define DEFAULT_COLOR 0xffffff


void Render();
void ClearScreen(int color);
unsigned int* SetPixelPointer(Point p);
void DrawPoint(Point p, int color);
void DrawLine(Point p1, Point p2, int color);
void DrawRect(Point p, int width, int height, int color, bool fill);
void DrawRectDynamicPosition(float x, float y, float width, float height, int color, bool fill);
void DrawRectDynamicPosAndSize(float x, float y, float width, float height, int color, bool fill);
void DrawTriangle(Point p1, Point p2, Point p3, int color, bool fill);
void DrawSquare(Point p, int size, int color, bool fill);
void DrawCircle(Point p, int radius, int color, int smoothness);


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
	// Making sure nothing is out of range
	Bracket(renderBuffer.width, renderBuffer.height, p1);
	Bracket(renderBuffer.width, renderBuffer.height, p2);

	// Bresenham's line drawing algo
	int dx = abs(p2.x - p1.x);
	int sx = p1.x < p2.x ? 1 : -1;
	int dy = -abs(p2.y - p1.y);
	int sy = p1.y < p2.y ? 1 : -1;
	int err = dx + dy;

	while (true) {
		DrawPoint(p1);
		if (p1.x == p2.x && p1.y == p2.y) break;
		int e2 = 2 * err;
		if (e2 >= dy) {
			if (p1.x == p2.x) break;
			err += dy;
			p1.x += sx;
		}
		if (e2 <= dx) {
			if (p1.y == p2.y) break;
			err += dx;
			p1.y += sy;
		}
	}
	
}

void DrawRect(Point p, int width, int height, int color = DEFAULT_COLOR, bool fill = true) {

	p.x = Bracket(0, renderBuffer.width, p.x);
	int posX2 = Bracket(0, renderBuffer.width, p.x + width);
	p.y = Bracket(0, renderBuffer.height, p.y);
	int posY2 = Bracket(0, renderBuffer.height, p.y + height);

	if (fill) {
		for (int i = p.y; i < posY2; i++) {
			unsigned int* pixel = (unsigned int*)renderBuffer.memory + (((i)*renderBuffer.width) + p.x);
			for (int j = p.x; j < posX2; j++) {
				*pixel++ = color;
			}
		}
	}
	else {
		DrawLine(p, Point(posX2, p.y));
		DrawLine(p, Point(p.x, posY2));
		DrawLine(Point(p.x, posY2), Point(posX2, posY2));
		DrawLine(Point(posX2, posY2), Point(posX2, p.y));
	}
}

void DrawSquare(Point p, int size, int color = DEFAULT_COLOR, bool fill = true) {

	if (fill) {
		for (int y = p.y; y <= p.y + size; y++) {
			DrawLine(Point(p.x, y), Point(p.x + size, y));
		}
	}
	else {
		int x2 = p.x + size;
		int y2 = p.y + size;

		DrawLine(p, Point(x2, p.y));				// Horizontal top
		DrawLine(p, Point(p.x, y2));				// Vertical left
		DrawLine(Point(p.x, y2), Point(x2, y2));	// Horizontal bottom
		DrawLine(Point(x2, p.y), Point(x2, y2));	// Vertical right

	}

}

void DrawRectDynamicPosition(float x, float y, float width, float height, int color = DEFAULT_COLOR, bool fill = true)
{
	x /= 100;
	y /= 100;
	x *= renderBuffer.width;
	y *= renderBuffer.height;
	x -= (width / 2);
	y -= (height / 2);
	Point p((int)x, (int)y);

	DrawRect(p, (int)width, (int)height, color, fill);
}

void DrawRectDynamicPosAndSize(float x, float y, float width, float height, int color = DEFAULT_COLOR, bool fill = true)
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

	DrawRect(p, (int)width, (int)height, color, fill);
}

void DrawTriangle(Point p1, Point p2, Point p3, int color = DEFAULT_COLOR, bool fill=true)
{
	DrawLine(p1, p2);
	DrawLine(p2, p3);
	DrawLine(p3, p1);

	if (fill) {


		// Sort y points
		if (p1.y < p2.y) Swap(p1, p2);
		if (p1.y < p3.y) Swap(p1, p3);
		// Barycentric algo
		// drawing like an imaginary box around the triangle, going through each pixel in the
		// box and determining whether that pixel is in the triangle. If it is, plot it.

		float y1 = p1.y;
		float y2 = p2.y;
		float y3 = p3.y;

		float x1 = p1.x;
		float x2 = p2.x;
		float x3 = p3.x;

		// Making imaginary box
		int minx = (int)min(x1, min(x2, x3));
		int maxx = (int)max(x1, max(x2, x3));
		int miny = (int)min(y1, min(y2, y3));
		int maxy = (int)max(y1, max(y2, y3));

		// Square containing triangle
		for (int y = miny; y <= maxy; y++)
		{
			for (int x = minx; x <= maxx; x++)
			{
				// half space function calcs
				if ((x1 - x2) * (y - y1) - (y1 - y2) * (x - x1) < 0 &&
					(x2 - x3) * (y - y2) - (y2 - y3) * (x - x2) < 0 &&
					(x3 - x1) * (y - y3) - (y3 - y1) * (x - x3) < 0)
				{
					DrawPoint(Point(x, y), color);
				}
			}

		}
	}
}


// My own circle drawing algorithm. To date this might be my biggest pride lol.
// I save 4 points in each iteration, one point for each quadrant, all based on the first point, 
// calculated with sin and cos. Then I draw a line connected the previous point of each quadrant
// to the current point of each quadrant. :).
void DrawCircle(Point origin, int radius, int color = DEFAULT_COLOR, int smoothness = 500) {

	// Bracketing the radius and smoothness.
	smoothness = Bracket(3, 1501, smoothness);
	int maxRadius = min(renderBuffer.width - origin.x, min(renderBuffer.height - origin.y, min(origin.x, origin.y)));
	radius = Bracket(0, maxRadius, radius);


	float half_pi = 1.57;
	float inc = half_pi / smoothness;

	Point previousQ1;
	Point previousQ2;
	Point previousQ3;
	Point previousQ4;

	for (float a = 0; a <= half_pi; a += inc) {
		int x = radius * cos(a) + origin.x;
		int y = radius * sin(a) + origin.y;

		Point p(x, y);		
		Point pQ2(2 * origin.x - p.x, p.y);
		Point pQ3(2 * origin.x - p.x, 2 * origin.y - p.y);
		Point pQ4(p.x, 2 * origin.y - p.y);

		if (a != 0) {
			DrawLine(previousQ1, p, color);
			DrawLine(previousQ2, pQ2, color);
			DrawLine(previousQ3, pQ3, color);
			DrawLine(previousQ4, pQ4, color);
		}

		previousQ1 = p;
		previousQ2 = pQ2;
		previousQ3 = pQ3;
		previousQ4 = pQ4;
	}
}

