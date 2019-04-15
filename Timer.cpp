#include "Timer.h"
using namespace std;

Timer::Timer() {
	start = chrono::high_resolution_clock::now();
}

Timer::~Timer() {
	end = chrono::high_resolution_clock::now();
	duration = end - start;
	float result = duration.count();
	cout << "Time is " << result << " seconds " << endl;
}