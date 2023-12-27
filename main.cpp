#include <iostream>
#include <fstream>
#include <math.h>
#include <chrono>
#include <thread>
#include <vector>

bool startFlag = false;

char grey[10] = {'@', '%', '#', '*', '+', '=', '-', ':', '.', ' '};


double gammaExpr(int c){
	double cl = c / 255.0;

	if(cl <= 0.04045) return cl/12.92;
	
	return pow((cl+0.055)/1.055,2.4);
}


void job(int* tabIn, double* tabOut, int start, int end){
	
	while (!startFlag) {}
	
	for(int i = start; i < end; i++){
		tabOut[i] = gammaExpr(tabIn[i*3])*0.2126+gammaExpr(tabIn[i*3 + 1])*0.7152+gammaExpr(tabIn[i*3 + 2])*0.0722;
	}
}

int main(int argc, char** argv){

	int numOfThreads = 0;
	int width = 4000;
	int height = 2000;
	int testCount = 20;

	for(int i = 1; i < argc; i++){

		std::string arg = argv[i];

		if(arg == "--core"){
			i++;
			numOfThreads = std::atoi(argv[i]);
			std::cout << "thread count: " << numOfThreads << "\n";
		}
		else if(arg == "--test"){
			i++;
			testCount = std::atoi(argv[i]);
			std::cout << "test count: " << testCount << "\n";
		}
		else if(arg == "--size"){
			i++;
			width = std::atoi(argv[i]);
			i++;
			height = std::atoi(argv[i]);
			std::cout << "custom size: " << width << " " << height << "\n";
		}
		else{
			std::cout << "unknown argument\n";
			exit(0);
		}
	}

	int* tabIn;
	int tabOutSize;
	double* tabOut;

	if(numOfThreads == 0) numOfThreads = std::thread::hardware_concurrency();

	tabIn = new int[width * height * 3];
	tabOut = new double[width * height];

	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			tabIn[(y * width + x) * 3] = x * (255.0 / width);
			tabIn[(y * width + x) * 3 + 1] = y * (255.0 / height);
			tabIn[(y * width + x) * 3 + 2] = x * (255.0 / width) * y * (255.0 / height) / 255.0;
		}
	}

	double average = 0;

	for(int test = 0; test < testCount; test++){

		std::vector<std::thread>jobs;

		for(int i = 0; i < numOfThreads;i++){
			int start = i * (width * height / numOfThreads);
			int end = start + (width * height / numOfThreads);

			jobs.push_back(std::thread(job, tabIn, tabOut, start, end));
		}

		auto timeStart = std::chrono::high_resolution_clock::now();
		startFlag = true;

		for(int i = 0; i < numOfThreads;i++) jobs[i].join();	

		auto timeStop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(timeStop - timeStart);

		std::cout << test << " done in " << duration.count() / 1000000.0 << "\n";
		average += duration.count() / 1000000.0;
	}

	average /= testCount;

	std::cout << "average time: " << average << "\n";

	return 0;
}
