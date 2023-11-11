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
#include <Windows.h>

#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

static void RRE_OpenConsole()
{
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
}

static void RRE_CloseConsole()
{
	FreeConsole();
}

static void RRE_LogInfo(std::string text)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	printf("RRE_INFO: ");
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	printf(text.c_str());
	printf("\n");
}

static void RRE_LogWarning(std::string text)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
	printf("RRE_WARNING: ");
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	printf(text.c_str());
	printf("\n");
}

static void RRE_LogError(std::string text)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
	printf("RRE_ERROR: ");
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
	printf(text.c_str());
	printf("\n");
}

static void RRE_ScaleRectToScreenSize(RRW_ScreenSize* screenSize, RRW_ScreenSize* imageSize, SDL_Rect* srcRect, SDL_Rect* dstRect)
{
	dstRect->x = srcRect->x * screenSize->Width / imageSize->Width;
	dstRect->y = srcRect->y * screenSize->Height / imageSize->Height;

	dstRect->w = srcRect->w * screenSize->Width / imageSize->Width;
	dstRect->h = srcRect->h * screenSize->Height / imageSize->Height;
}

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