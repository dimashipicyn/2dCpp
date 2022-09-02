#include "physics.h"

#include <stdlib.h>

static t_physic_world physic_world;

static int cmp(void* t, void* p)
{
    if (t == p) {
        return 0;
    }
    return 1;
}

static int intersect(t_body_rect lhs, t_body_rect rhs)
{
    float ax = lhs.pos.x - lhs.half_size.x;
    float ay = lhs.pos.y - lhs.half_size.y;
    float ax1 = lhs.pos.x + lhs.half_size.x;
    float ay1 = lhs.pos.y + lhs.half_size.y;

    float bx = rhs.pos.x - rhs.half_size.x;
    float by = rhs.pos.y - rhs.half_size.y;
    float bx1 = rhs.pos.x + rhs.half_size.x;
    float by1 = rhs.pos.y + rhs.half_size.y;

    return !(bx > ax1 || 
           bx1 < ax || 
           by > ay1 ||
           by1 < ay);
}

t_physic_world* new_physic_world()
{
    t_physic_world* w = calloc(1, sizeof(t_physic_world));
    w->static_bodies = NULL;
    w->dynamic_bodies = NULL;

    return w;
}

void free_physic_world(t_physic_world* w)
{
	typedef void (*foreach_callback)(void*);

    ut_list_foreach(w->static_bodies, (foreach_callback)free_physic_body);
    ut_list_foreach(w->dynamic_bodies, (foreach_callback)free_physic_body);
    ut_list_clear(&w->static_bodies);
    ut_list_clear(&w->dynamic_bodies);
    free(w);
}

void intersect_physic_body(t_physic_body* body_1)
{
	t_physic_world *w = &physic_world;

	body_1->contact = 0;
	body_1->contacted_body = NULL;

	for (t_list* it2 = w->dynamic_bodies; it2 != NULL; it2 = it2->next)
	{
		t_physic_body* body_2 = it2->content;

		if (body_1 == body_2) {
			continue;
		}

		if (body_1->layer & body_2->layer && intersect(body_1->rect, body_2->rect)) {
			body_1->contact = 1;
			body_1->contacted_body = body_2;
			break;
		}
	}
}

void step_physic_world()
{
    t_physic_world *w = &physic_world;

    for (t_list* it1 = w->dynamic_bodies; it1 != NULL; it1 = it1->next)
    {
        t_physic_body* body_1 = it1->content;
        t_body_rect move_rect = body_1->rect;
        
        if (body_1->velocity == 0) {
            continue;
        }

        body_1->contact = 0;
        body_1->contacted_body = NULL;

        move_rect.pos = vec2_add(body_1->rect.pos, vec2_scalar_num(body_1->dir, body_1->velocity));

        for (t_list* it2 = w->static_bodies; it2 != NULL; it2 = it2->next)
        {
            t_physic_body* body_2 = it2->content;

            if (body_1->layer & body_2->layer && intersect(move_rect, body_2->rect)) {
                body_1->contact = 1;
                body_1->contacted_body = body_2;
				body_1->velocity = 0;
                break;
            }
        }

		if (body_1->contact) {
			continue;
		}
        
        for (t_list* it2 = w->dynamic_bodies; it2 != NULL; it2 = it2->next)
        {
            if (it1 == it2) {
                continue;
            }
            
            t_physic_body* body_2 = it2->content;
            if (body_1->layer & body_2->layer && intersect(move_rect, body_2->rect)) {
                body_1->contact = 1;
                body_1->contacted_body = body_2;
				body_1->velocity = 0;
				//body_2->contact = 1;
				//body_2->contacted_body = body_1;
				//body_2->velocity = 0;
                break;
            }
        }

        if (!body_1->contact) {
            body_1->rect = move_rect;
        }
    }
}

t_physic_body* create_physic_body(t_physic_body_def def)
{
    t_physic_body* b = calloc(1, sizeof(t_physic_body));

    b->rect = (t_body_rect){def.pos, def.size};
    b->velocity = def.velocity;
    b->dir = def.dir;
    b->stop_on_contact = 1;
    b->contact = 0;
    b->owner = def.user_data;
    b->layer = def.layer;

    if (def.is_dynamic) {
        ut_list_push_back(&physic_world.dynamic_bodies, b);
    }
    else {
        ut_list_push_back(&physic_world.static_bodies, b);
    }

    return b;
}

void free_physic_body(t_physic_body* b)
{
	if (b) {
		ut_list_remove_if(&physic_world.static_bodies, b, cmp);
		ut_list_remove_if(&physic_world.dynamic_bodies, b, cmp);
		free(b);
	}
}
