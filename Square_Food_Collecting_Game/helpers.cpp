
int Bracket(int minVal, int maxVal, int val);


int Bracket(int minVal, int maxVal, int val) {
	val = (val < minVal) ? minVal : ((val > maxVal) ? maxVal : val);
	return val;
}