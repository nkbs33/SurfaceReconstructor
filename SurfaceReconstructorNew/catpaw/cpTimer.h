#ifndef CPTIMER
#define CPTIMER

#include <chrono>
#include <time.h>
#include <stdio.h>

using namespace std::chrono;
namespace catpaw{

	struct cTime{
		steady_clock::time_point t1,t2;
		
		cTime(){}

		void tick(){
			t1 = steady_clock::now();
		}

		double tack(){
			t2 = steady_clock::now();
			duration<double> time_span = duration_cast<duration<double> >(t2-t1);
			return time_span.count();
		}

		double tack(const char* str) {
			t2 = steady_clock::now();
			duration<double> time_span = duration_cast<duration<double> >(t2-t1);
			printf("%s %f ms\n", str, time_span.count()*1000);
			return time_span.count();
		}
	};
	
}
#endif