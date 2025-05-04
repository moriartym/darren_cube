#include "cub3d.h"

void init_player(t_var *data)
{
    player_start(data, data->map.ypos, data->map.xpos);
    data->player.pa = P2;
    data->player.pdy = sinf(data->player.pa);
    data->player.pdx = cosf(data->player.pa);
}

void init_all(t_var *data, t_cub *cube)
{
    data->last_mouse_x = -1;
    data->move = (t_move) {0};
    data->map = cube->map;
    init_player(data);
    
    data->num_sprites = 1;
    data->sprites = malloc(sizeof(t_sprite) * data->num_sprites);
    *data->sprites = (t_sprite){0};
    data->sprites[0] = (t_sprite){.type = 1, .x =  48.0, .y = 48.0, .z = 0};// z not used
}





