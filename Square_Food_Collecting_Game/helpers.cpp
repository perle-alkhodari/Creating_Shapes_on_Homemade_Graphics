
int Bracket(int minVal, int maxVal, int val);
bool IsInRange(int minVal, int maxVal, int val);
void Swap(int& a, int& b);
void Swap(Point& p1, Point& p2);


int Bracket(int minVal, int maxVal, int val) {
	if (val <= minVal) val = minVal+1;
	if (val >= maxVal) val = maxVal-1;
	return val;
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

void Swap(Point& p1, Point& p2) {
	Swap(p1.x, p2.x);
	Swap(p1.y, p2.y);
}