//
//  Platform.cpp
//
//
//  Created by Narendra Umate on 8/6/16.
//
//

#include "Platform.h"

std::string getExecutableDirectory() {
#if defined(_WIN32)
	unsigned int bufferSize = MAX_PATH;
#else
	unsigned int bufferSize = PATH_MAX;
#endif

#if defined(_WIN32)
	std::vector<char> buffer(bufferSize);
	::GetModuleFileNameA(NULL, LPSTR(&buffer[0]), bufferSize);
	std::string s(buffer.begin(), buffer.end());
	unsigned found = s.find_last_of("/\\");
	s = s.substr(0, found + 1); // base directory with ending /
#elif defined(__linux__)
	std::vector<char> buffer(bufferSize);
	int pid = getpid();
	// Construct a path to the symbolic link pointing to the process executable.
	// This is at /proc/<pid>/exe on Linux systems (we hope).
	std::ostringstream oss;
	oss << "/proc/" << pid << "/exe";
	std::string link = oss.str();
	// Read the contents of the link.
	int count = readlink(link.c_str(), &buffer[0], bufferSize);
	buffer[count] = '\0';
	std::string s(buffer.begin(), buffer.begin() + count);
#elif defined(__APPLE__)
	// TODO Clean up this garbage that is required because older path failed when verify system was run from mac terminal
	char pathbuf[bufferSize];
	pid_t pid = getpid();
	__unused int ret = proc_pidpath(pid, pathbuf, bufferSize);
	std::string s(pathbuf);
#else
	std::string s;
#endif
	return s;
}

std::string getProjectDirectory() {
	std::string executableDirectory = getExecutableDirectory();
	unsigned found1 = executableDirectory.find_last_of("/\\");
	executableDirectory = executableDirectory.substr(0, found1);
	unsigned found2 = executableDirectory.find_last_of("/\\");
	executableDirectory = executableDirectory.substr(0, found2);
	unsigned found3 = executableDirectory.find_last_of("/\\");
	executableDirectory = executableDirectory.substr(0, found3);
	unsigned found4 = executableDirectory.find_last_of("/\\");
	executableDirectory = executableDirectory.substr(0, found4);
	unsigned found5 = executableDirectory.find("/Build");
	executableDirectory = executableDirectory.substr(0, found5);
	return executableDirectory;
}

std::string getShaderDirectory() {
	return getProjectDirectory() + "/Shaders";
}

std::string getAssetDirectory() {
	return getProjectDirectory() + "/Assets";
}

bool fileExists(const std::string& filepath) {
	std::fstream filehandle(filepath.c_str());
	return filehandle.good();
}

bool fileOpen(const std::string& filepath, std::ifstream& filehandle) {
	filehandle.open(filepath.c_str());
	if (filehandle.is_open()) {
		filehandle.close();
		return true;
	}
	return false;
}

bool fileOpen(const std::string& filepath, std::ofstream& filehandle) {
	filehandle.open(filepath.c_str());
	if (filehandle.is_open()) {
		filehandle.close();
		return true;
	}
	return false;
}

bool fileClose(std::ifstream& filehandle) {
	if (filehandle.is_open()) {
		filehandle.close();
		return true;
	}
	return false;
}

bool fileClose(std::ofstream& filehandle) {
	if (filehandle.is_open()) {
		filehandle.close();
		return true;
	}
	return false;
}

bool fileReadLine(std::ifstream& filehandle, std::string& line) {
	return static_cast<bool>(std::getline(filehandle, line));
}

bool fileWriteLine(std::ofstream& filehandle, std::string& line) {
	return static_cast<bool>(filehandle << line);
}

std::string findBasePath(const std::string& strFilePath) {
	unsigned found = strFilePath.find_last_of("/\\");
	return strFilePath.substr(0, found + 1); // base directory with ending /
	// return strFilePath.substr(found + 1);	// file name without starting /
}

std::string findFileName(const std::string& strFilePath) {
	unsigned found = strFilePath.find_last_of("/\\");
	// return strFilePath.substr(0, found + 1); // base directory with ending /
	return strFilePath.substr(found + 1); // file name without starting /
}

bool fileReadIntoString(const std::string& filepath, std::string& filecontent) {
	std::ifstream fin(filepath, std::ios::binary);
	if (!fin) {
		fin.close();
		return false;
	}

	fin.seekg(0, std::ios::end);
	std::streamsize filesize = fin.tellg();
	fin.seekg(0, std::ios::beg);

	filecontent.resize(filesize);
	if (!fin.read(&filecontent[0], filesize)) {
		fin.close();
		return false;
	}

	fin.close();
	return true;
}

bool fileWriteFromString(const std::string& filepath, const std::string& filecontent) {
	std::ofstream fout(filepath.c_str());
	if (fout.fail()) {
		fout.close();
		return false;
	}
	fout.write(filecontent.c_str(), filecontent.size());
	fout.close();
	return true;
}
