//
//  input.hpp
//  2dCpp
//
//  Created by Дима Щипицын on 05/09/2022.
//

#ifndef input_h
#define input_h

#include "Export.h"
#include <glm/vec2.hpp>

class TWODCPP_EXPORT Input
{
public:
	void handle();

	enum Button
	{
		Quit,
		Fire1,
		Fire2,
		Fire3,
		MouseLeft,
		MouseRight,
		MouseMiddle,
		Total
	};

	enum Keys {
		A,
		B,
		C,
		D,
	};

	float get_vertical_axis();
	float get_horizontal_axis();
	float get_button(const Button& button);
	glm::vec2 get_mouse_position();

	bool pressed(Keys key);
	bool released(Keys key);

private:
	float vertical_axis_;
	float horizontal_axis_;
	float buttons_[Button::Total];

	glm::vec2 mouse_position_;
	bool key_pressed[100];
	bool key_released[100];
};

#endif
