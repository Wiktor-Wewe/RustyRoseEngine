#pragma once
#include <stdio.h>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <soloud_wav.h>
#include <RustyRenderWindow.h>

#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

/*
std::fstream* RRE_LogFile = nullptr;

static void RRE_CreateAndOpenLogFile()
{
	time_t currentTime;
	struct tm localTime;
	time(&currentTime);
	localtime_s(&localTime, &currentTime);
	char result[24]; // "yyyy-mm-dd_hh-mm-ss.log" + 0x00 = 24
	memset(result, 0x00, sizeof(result));
	strftime(result, sizeof(result), "%Y-%m-%d_%H:%M:%S.log", &localTime);
	
	if (RRE_LogFile) {
		RRE_LogFile->close();
		delete RRE_LogFile;
		RRE_LogFile = nullptr;
	}

	RRE_LogFile = new std::fstream;
	RRE_LogFile->open(result, std::ios::in);
	
	if (RRE_LogFile->good() == false) {
		printf("Unable to create and open log file: %s\n", result);
	}
}

static void RRE_LogOpenFile(std::string path)
{
	if (RRE_LogFile->is_open()) {
		*RRE_LogFile << "O " << path << "\n";
	}
}

static void RRE_LogClosedFile(std::string path)
{
	if (RRE_LogFile->is_open()) {
		*RRE_LogFile << "C " << path << "\n";
	}
}

static void RRE_CloseLogFile()
{
	if (RRE_LogFile->is_open()) {
		RRE_LogFile->close();
	}

	delete RRE_LogFile;
	RRE_LogFile = nullptr;
}

static void RRE_VerifyFilesInputOutput()
{

}

*/

static std::vector<std::string> RRE_Split(std::string text, char separator)
{
	std::vector<std::string> result;
	size_t start = 0;
	size_t end = text.find(separator);

	while (end != std::string::npos)
	{
		result.push_back(text.substr(start, end - start));
		start = end + 1;
		end = text.find(separator, start);
	}

	result.push_back(text.substr(start));

	return result;
}

static std::string RRE_NormalizePath(std::string path)
{
	for (char& c : path) {
		if (c == '\\') {
			c = '/';
		}
	}

	for (int i = path.size() - 1; i >= 0; i--) {
		if (path[i] == '/') {
			break;
		}

		if (path[i] > 96 && path[i] < 123) {
			path[i] = path[i] - 32;
		}
	}

	return path;
}

#endif // SHARED_UTILS_H