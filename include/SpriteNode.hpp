//
//  SpriteNode.hpp
//  2dCpp
//
//  Created by Дима Щипицын on 15/03/2023.
//

#ifndef SpriteNode_hpp
#define SpriteNode_hpp

#include "Node.hpp"
#include "Sprite.h"

class Game;

class SpriteNode : public Sprite, public NodeBase
{
public:
	~SpriteNode();
	
	void render(Game& game) override;

	using NodeBase::set_position;
};

#endif /* SpriteNode_hpp */
