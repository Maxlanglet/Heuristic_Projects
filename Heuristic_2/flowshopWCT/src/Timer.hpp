#include <chrono>
#include <stdio.h>
#include <iostream>


using namespace std;

class Timer{
	public:
		Timer();
		~Timer();
		void Stop();
		int getDuration();
	private:
		chrono::time_point<chrono::high_resolution_clock> m_Start;
		int duration;
};