//
//  Sample.cpp
//
//
//  Created by Narendra Umate on 8/18/15.
//
//

#include <chrono>

#include "Sample.h"

#if defined(_WIN32)
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
int main(int argc, char* argv[])
#endif
{
	Parameters parameters;
	parameters.title = "Sample";
	parameters.xOrigin = 0;
	parameters.yOrigin = 0;
	parameters.xSize = 800;
	parameters.ySize = 600;
	parameters.type = Metal;

	SampleWindow* sampleWindow = new SampleWindow(parameters);

	float dt = 0.0f;
	while (sampleWindow->isActive()) {
		auto startTime = std::chrono::high_resolution_clock::now();
		sampleWindow->update(dt);
		sampleWindow->swapBuffers();
		sampleWindow->pollEvents();
		auto stopTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
	}

	delete sampleWindow;

	return 0;
}
