#pragma once

namespace Debug
{
	bool debugMode = false;
	int debugFrames = 100;

	void time(const char funcName[], std::function<void()> func)
	{
		auto start = std::chrono::high_resolution_clock::now();
		func();
		auto stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout << funcName << " time:" << duration.count() << std::endl;
	}
	
	//TODO: logs
}