//
//  SpriteNode.cpp
//  2dCpp
//
//  Created by Дима Щипицын on 15/03/2023.
//

#include "SpriteNode.hpp"

#include "game.h"
#include "log.h"

#include <glm/gtx/compatibility.hpp>
#include <glm/common.hpp>

SpriteNode::~SpriteNode()
{

}

void SpriteNode::render(Game& game)
{
	float a = game.get_lag() / game.get_tick();

	glm::vec2 curr_pos = get_position();

	glm::vec2 pos = glm::round(glm::lerp(
		curr_pos
		, curr_pos + curr_pos - prev_pos
		, glm::clamp(a, 0.0f, 1.0f)
	));

	Sprite::set_position(pos.x, pos.y);
	draw(game.get_graphics());
}
