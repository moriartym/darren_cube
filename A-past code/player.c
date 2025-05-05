#include "../cub3d.h"

void player_start(t_var *data, int y, int x)
{
    data->player.py = TILE_SIZE * y + TILE_SIZE / 2 - PLAYER_SIZE / 2;
    data->player.px = TILE_SIZE * x + TILE_SIZE / 2 - PLAYER_SIZE / 2;
}

// void draw_player(t_var *data)
// {
//     int by;
//     int bx;

//     by = - PLAYER_SIZE/2;
//     while (by < PLAYER_SIZE)
//     {
//         bx = - PLAYER_SIZE/2;
//         while (bx < PLAYER_SIZE)
//         {
//             my_mlx_pixel_put(&data->image, (int)(data->player.px + bx), (int)(data->player.py + by), PLAYER_COLOR);
//             bx++;
//         }
//         by++;
//     }
// }
void draw_player(t_var *data)
{
    int tile_size = data->minimap.tile_size;

    int player_tile_x = (int)(data->player.px / TILE_SIZE);
    int player_tile_y = (int)(data->player.py / TILE_SIZE);

    int rel_x = player_tile_x - data->minimap_offset_x;
    int rel_y = player_tile_y - data->minimap_offset_y;

    float offset_x = (data->player.px - player_tile_x * TILE_SIZE) / (float)TILE_SIZE;
    float offset_y = (data->player.py - player_tile_y * TILE_SIZE) / (float)TILE_SIZE;

    int draw_x = rel_x * tile_size + offset_x * tile_size;
    int draw_y = rel_y * tile_size + offset_y * tile_size;

    for (int by = -1; by <= PLAYER_SIZE;by++)
    {
        for (int bx = -1; bx <= PLAYER_SIZE; bx++)
        {
            int px = draw_x + bx;
            int py = draw_y + by;
            if (px >= 0 && px < MINIMAP_SIZE && py >= 0 && py < MINIMAP_SIZE)
                my_mlx_pixel_put(&data->image, px, py, PLAYER_COLOR);
        }
    }
}
