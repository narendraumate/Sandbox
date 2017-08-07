//
//  Platform.h
//
//
//  Created by Narendra Umate on 8/6/16.
//
//

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

// HACK This is a bad way to get the executable path until filesystem support lands.
#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <linux/limits.h>
#include <sstream>
#include <unistd.h>
#elif defined(__APPLE__)
#include <libproc.h>
#include <unistd.h>
#endif

std::string getExecutableDirectory();
std::string getShaderDirectory();
std::string getAssetDirectory();

bool fileExists(const std::string& filepath);
bool fileOpen(const std::string& filepath, std::ifstream& filehandle);
bool fileOpen(const std::string& filepath, std::ofstream& filehandle);
bool fileClose(std::ifstream& filehandle);
bool fileClose(std::ofstream& filehandle);
bool fileReadLine(std::ifstream& filehandle, std::string& line);
bool fileWriteLine(const std::ofstream& filehandle, std::string& line);
bool fileReadIntoString(const std::string& filepath, std::string& filecontent);
bool fileWriteFromString(const std::string& filepath, const std::string& filecontent);

std::string findBasePath(const std::string& strFilePath);
std::string findFileName(const std::string& strFilePath);
