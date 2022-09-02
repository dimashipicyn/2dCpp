#include "keys.h"

int			key_init(int keycode, t_keys* keys)
{
    keys->pressed[keycode] = 1;
	return (0);
}

int			key_destroy(int keycode, t_keys* keys)
{
    keys->pressed[keycode] = 0;
	return (0);
}
