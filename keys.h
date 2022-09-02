#ifndef KEYS_H
# define KEYS_H

#include <stdint.h>

typedef enum {
    K_CODE_A = 0,
    K_CODE_B = 11,
    K_CODE_C = 8,
    K_CODE_D = 2,
    K_CODE_E = 14,
    K_CODE_F = 3,
    K_CODE_G = 5,
    K_CODE_H = 4,
    K_CODE_I = 34,
    K_CODE_J = 38,
    K_CODE_K = 40,
    K_CODE_L = 37,
    K_CODE_M = 46,
    K_CODE_N = 45,
    K_CODE_O = 31,
    K_CODE_P = 35,
    
    K_CODE_Q = 12,
    K_CODE_R = 15,
    K_CODE_S = 1,
    K_CODE_T = 17,
    K_CODE_U = 32,
    K_CODE_V = 9,
    K_CODE_W = 13,
    K_CODE_X = 7,
    K_CODE_Y = 16,
    K_CODE_Z = 6,
    
    K_CODE_L_CTRL = 59,
    K_CODE_L_SHIFT = 57,
    K_CODE_L_ALT = 58,
    K_CODE_L_CMD = 55,
    
    K_CODE_R_CTRL = 59,
    K_CODE_R_SHIFT = 60,
    K_CODE_R_ALT = 61,
    K_CODE_R_CMD = 55,
    
    K_CODE_ENTER = 36,
    
    K_CODE_LEFT = 123,
    K_CODE_RIGHT = 124,
    K_CODE_TOP = 126,
    K_CODE_BOTTOM = 125,
    
    
    K_CODE_1 = 18,
    K_CODE_2 = 19,
    K_CODE_3 = 20,
    K_CODE_4 = 21,
    K_CODE_5 = 23,
    K_CODE_6 = 22,
    K_CODE_7 = 26,
    K_CODE_8 = 28,
    K_CODE_9 = 25,
    K_CODE_0 = 29,
} t_key_code;


typedef struct s_keys
{
    int8_t pressed[300];
} t_keys;

typedef struct s_mouse
{
	int x;
	int y;
	int is_press_l;
	int is_press_r;
} t_mouse;

int		key_init(int keycode, t_keys* keys);
int		key_destroy(int keycode, t_keys* keys);

#endif
