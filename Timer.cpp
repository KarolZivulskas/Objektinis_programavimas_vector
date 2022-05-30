#include "Timer.hpp"

double Timer::elapsed() const {
	return duration(clock::now() - startTime).count();
}

void Timer::reset() {
	startTime = clock::now();
}

void Timer::start() {
	reset();
}
