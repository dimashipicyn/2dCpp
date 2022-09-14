//
//  input.cpp
//  2dCpp
//
//  Created by Дима Щипицын on 05/09/2022.
//

#include "input.h"

#include <SDL.h>

void Input::handle() {

	horizontal_axis_ = 0;
	vertical_axis_ = 0;

	for (int i = 0; i < Total; i++) {
		buttons_[i] = 0;
	}

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
				switch (event.type)
				{
					case SDL_QUIT:
					case SDL_SCANCODE_ESCAPE:
						buttons_[Quit] += 1;
						break;
					case SDL_SCANCODE_W:
					case SDL_SCANCODE_UP:
						vertical_axis_ += 1;
						break;
					case SDL_SCANCODE_S:
					case SDL_SCANCODE_DOWN:
						vertical_axis_ -= 1;
						break;
					case SDL_SCANCODE_A:
					case SDL_SCANCODE_LEFT:
						horizontal_axis_ -= 1;
						break;
					case SDL_SCANCODE_D:
					case SDL_SCANCODE_RIGHT:
						horizontal_axis_ += 1;
						break;
					case SDL_SCANCODE_LCTRL:
					case SDL_SCANCODE_RCTRL:
						buttons_[Fire1] += 1;
						break;
					case SDL_SCANCODE_LALT:
					case SDL_SCANCODE_RALT:
						buttons_[Fire2] += 1;
						break;
		            case SDL_MOUSEMOTION:
						mouse_position_.x = event.motion.x;
						mouse_position_.y = event.motion.y;
		                break;
		            case SDL_MOUSEBUTTONUP:
						if (event.button.button == SDL_BUTTON_LEFT) {
							buttons_[MouseLeft] += 1;
						}
						if (event.button.button == SDL_BUTTON_RIGHT) {
							buttons_[MouseRight] += 1;
						}
						if (event.button.button == SDL_BUTTON_MIDDLE) {
							buttons_[MouseMiddle] += 1;
						}
		                break;
				}

	}
}

float Input::get_vertical_axis() { 
	return vertical_axis_;
}


float Input::get_horizontal_axis() { 
	return horizontal_axis_;
}


float Input::get_button(const Input::Button &button) {
	if (button < Button::Quit || button >= Button::Total) {
		return 0;
	}
	return buttons_[button];
}

Vec2i Input::get_mouse_position() { 
	return mouse_position_;
}


