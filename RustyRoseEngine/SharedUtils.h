#pragma once
#include <stdio.h>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <soloud_wav.h>
#include <RustyRenderWindow.h>

#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H

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