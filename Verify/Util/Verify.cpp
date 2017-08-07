//
//  Verify.cpp
//
//
//  Created by Narendra Umate on 7/5/17.
//
//

#include "Verify.h"

TEST_CASE("System") {
	std::string executableDirectory = getExecutableDirectory();
	std::string shaderDirectory = getShaderDirectory();
	std::string assetDirectory = getAssetDirectory();
#if defined(_WIN32)
	CHECK(findFileName(executableDirectory).substr(0, strlen("Windows")) == "Windows");
#elif defined(__linux__)
	CHECK(findFileName(executableDirectory).substr(0, strlen("Linux")) == "Linux");
#elif defined(__APPLE__)
	CHECK(findFileName(executableDirectory).substr(0, strlen("MacOS")) == "MacOS");
#endif
	CHECK(findFileName(shaderDirectory) == "Shaders");
	CHECK(findFileName(assetDirectory) == "Assets");
}
