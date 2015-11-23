#include "CTimer.h"

// Helper class for timing calculations
void CTimer::Start() {
	start = std::chrono::high_resolution_clock::now();
}

void CTimer::End() { 
	end = std::chrono::high_resolution_clock::now();
}

bool CTimer::DiffMs(double &ms) {
	std::chrono::duration<double, std::milli> elapsed = end - start;
	ms = elapsed.count();
	return ms < 0;
}

bool CTimer::Diff(double &seconds) {
	std::chrono::duration<double> elapsed = end - start;
	seconds = elapsed.count();
	return seconds < 0;
}
