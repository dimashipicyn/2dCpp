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
			case SDL_EVENT_QUIT:
				buttons_[Quit] += 1;
				break;
			case SDL_EVENT_MOUSE_MOTION:
				mouse_position_.x = event.motion.x;
				mouse_position_.y = event.motion.y;
				break;
			case SDL_EVENT_MOUSE_BUTTON_UP:
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
	auto keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_D]) {
		horizontal_axis_ += 1;
	}
	if (keys[SDL_SCANCODE_A]) {
		horizontal_axis_ -= 1;
	}
	if (keys[SDL_SCANCODE_W]) {
		vertical_axis_ += 1;
	}
	if (keys[SDL_SCANCODE_S]) {
		vertical_axis_ -= 1;
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

glm::vec2 Input::get_mouse_position() { 
	return mouse_position_;
}


