#pragma once

#include <SDL.h>

#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int

SDL_Event event;
SDL_Renderer* renderer;
SDL_Rect rect, pixelRect;

void eventCook();

long time = 0;
long millis() { return time; }
void delay(int ms) { SDL_Delay(ms); }

int LCD_WIDTH = 512, LCD_HEIGHT = 512;
uint8_t keyValue = 0;

#include "LCD.h"
#include "Keypad.h"
void setup();
void loop();


SDL_Window* window;
SDL_Surface* icon;
SDL_Surface* surface;

int endFlag = 0, reset = 0;

void eventCook() {
	while (SDL_PollEvent(&event) != 0) {
		if (event.type == SDL_QUIT) {
			endFlag = 1;
		}
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				endFlag = 1;
				break;
			case SDLK_BACKSPACE:
				reset = 1;
				break;
			case SDLK_w:
				keyValue |= LPAD_UP;
				break;
			case SDLK_s:
				keyValue |= LPAD_DOWN;
				break;
			case SDLK_a:
				keyValue |= LPAD_LEFT;
				break;
			case SDLK_d:
				keyValue |= LPAD_RIGHT;
				break;
			case SDLK_i:
				keyValue |= RPAD_UP;
				break;
			case SDLK_k:
				keyValue |= RPAD_DOWN;
				break;
			case SDLK_j:
				keyValue |= RPAD_LEFT;
				break;
			case SDLK_l:
				keyValue |= RPAD_RIGHT;
				break;
			default:
				break;
			}
		}
		if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.sym) {
			case SDLK_w:
				keyValue &= ~LPAD_UP;
				break;
			case SDLK_s:
				keyValue &= ~LPAD_DOWN;
				break;
			case SDLK_a:
				keyValue &= ~LPAD_LEFT;
				break;
			case SDLK_d:
				keyValue &= ~LPAD_RIGHT;
				break;
			case SDLK_i:
				keyValue &= ~RPAD_UP;
				break;
			case SDLK_k:
				keyValue &= ~RPAD_DOWN;
				break;
			case SDLK_j:
				keyValue &= ~RPAD_LEFT;
				break;
			case SDLK_l:
				keyValue &= ~RPAD_RIGHT;
				break;
			default:
				break;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc > 1) {
		LCD_WIDTH = LCD_HEIGHT = atoi(argv[1]) * 128;
	}

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		"NGP Emulator",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		LCD_WIDTH, LCD_HEIGHT, SDL_WINDOW_SHOWN
	);

	surface = SDL_GetWindowSurface(window);
	icon = SDL_LoadBMP("res\\logo.bmp");
	if (icon != 0) SDL_SetWindowIcon(window, icon);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

RST:
	reset = 0;
	SDL_SetWindowSize(window, LCD_WIDTH, LCD_HEIGHT);
	pixelRect.w = (float)LCD_WIDTH / 127.0F;
	pixelRect.h = (float)LCD_HEIGHT / 127.0F;

	lcd = LCDInit();

	setup();

	while (!endFlag) {
		eventCook();
		if (reset) goto RST;

		loop();	

		time++;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
