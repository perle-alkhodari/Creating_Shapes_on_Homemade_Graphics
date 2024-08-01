


struct RenderBuffer {
	int width, height, size;
	void* memory;
	BITMAPINFO bitmapInfo;
};

struct Point {
	int x;
	int y;
	Point(int x1, int y1) : x(x1), y(y1) {}
	Point() { x = -1, y = -1; }
};