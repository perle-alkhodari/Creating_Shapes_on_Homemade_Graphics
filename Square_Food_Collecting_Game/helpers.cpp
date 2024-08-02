
#include <Math.h>

int Bracket(int minVal, int maxVal, int val);
bool IsInRange(int minVal, int maxVal, int val);
void Swap(int& a, int& b);
void Swap(Point& p1, Point& p2);
void Bracket(int screenWidth, int screenHeight, Point &p);
float GetMagnitude(Point p);

///
/// This function is supposed to make sure a value
/// stays within the allowed range.
///
int Bracket(int minVal, int maxVal, int val) {
	if (val <= minVal) val = minVal+1;
	if (val >= maxVal) val = maxVal-1;
	return val;
}

// Overloading Bracket to take points as arguments
void Bracket(int screenWidth, int screenHeight, Point& p) {
	p.x = Bracket(0, screenWidth, p.x);
	p.y = Bracket(0, screenHeight, p.y);
}

float GetMagnitude(Point p)
{
	return sqrt(pow(p.x, 2) + pow(p.y, 2));
}

bool IsInRange(int minVal, int maxVal, int val) {
	bool result = true;
	if (val >= maxVal || val <= minVal) result = false;
	return result;
}

void Swap(int& a, int& b) {
	int c = a;
	a = b;
	b = c;
}

// Overloading Swap to take points as arguments
void Swap(Point& p1, Point& p2) {
	Swap(p1.x, p2.x);
	Swap(p1.y, p2.y);
}

