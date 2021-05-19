#include "Timer.hpp"


using namespace std;

Timer::Timer(){
	m_Start = chrono::high_resolution_clock::now();
}

Timer::~Timer(){
	Stop();
}

void Timer::Stop()
{
	auto m_endTime = chrono::high_resolution_clock::now();

	auto start = chrono::time_point_cast<chrono::microseconds>(m_Start).time_since_epoch().count();
	auto end = chrono::time_point_cast<chrono::microseconds>(m_endTime).time_since_epoch().count();

	duration = end - start;

	//cout << "Temps d'execution " << duration << " us\n" << endl;


}

int Timer::getDuration(){
	return duration;
}