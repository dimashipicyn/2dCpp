#ifndef PHYSICS_H
#define PHYSICS_H

#include "vector.h"

#include <vector>

struct AABB
{
    Vec2f pos;
    Vec2f size;
    
    bool has_intersection(const AABB& aabb) const;
};

struct Physic_body
{
    void*                       owner;
    std::vector<Physic_body*>   contacted_bodyes;
    AABB                        aabb;
	int32_t                     layer;
    Vec2f                       direction;
    float                       velocity;
    bool                        is_active;
};

struct Physic_body_def
{
    void*   owner;
    AABB    aabb;
    int32_t layer;
    Vec2f   direction;
    float   velocity;
    bool    is_static;
    bool    is_active;
};

class Physic
{
public:
    void step();
    bool has_intersection(const AABB& aabb);
    
    Physic_body* create_body(const Physic_body_def& def);
    
private:
    std::vector<Physic_body> static_body_;
    std::vector<Physic_body> dynamic_body_;
};

#endif
