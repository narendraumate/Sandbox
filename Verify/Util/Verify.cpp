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
	std::string projectDirectory = getProjectDirectory();
	std::string shaderDirectory = getShaderDirectory();
	std::string assetDirectory = getAssetDirectory();
	CHECK(findFileName(executableDirectory) == "VerifyUtil");
	CHECK(findFileName(projectDirectory) == "Sandbox");
	CHECK(findFileName(shaderDirectory) == "Shaders");
	CHECK(findFileName(assetDirectory) == "Assets");
}
