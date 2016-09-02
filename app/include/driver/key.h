#ifndef __KEY_H__
#define __KEY_H__

#include "gpio_func.h"

typedef void (* key_function)(void);

struct single_key_param 
{
    u8				key_level;
    u8				gpio_id;
    u8				gpio_func;
    u32				gpio_name;
    os_timer_t		key_5s;
    os_timer_t		key_50ms;
    key_function	short_press;
    key_function	long_press;
};

struct keys_param 
{
    u8 key_num;
    struct single_key_param **single_key;
};

struct single_key_param *key_init_single (u8 gpio_id, u32 gpio_name, u8 gpio_func, key_function long_press, key_function short_press);
void	key_init	(struct keys_param *key);

#endif
