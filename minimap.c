#include "cub3d.h"

void draw_tile(t_img *img, int x, int y, int color)
{
    int by;
    int bx;

    by = 0;
    while (by < TILE_SIZE)
    {
        bx = 0;
        while (bx < TILE_SIZE)
        {
            my_mlx_pixel_put(img, x + bx, y + by, color);
            bx++;
        }
        by++;
    }
}

// void draw_map(t_var *data, int x, int y)
// {
//     int draw_x;
//     int draw_y;
//     char tile;
//     int color;

//     tile = data->map.arr[y][x];
//     color = EMPTY_COLOR;
//     if (tile == '0')
//         color = EMPTY_COLOR;
//     else if (tile == '1')
//         color = WALL_COLOR;
//     else if (tile == '2')
//         color = DOOR_COLOR;
//     else if (tile == '3')
//         color = DOOR_OPEN_COLOR;
//     else
//         color = EMPTY_COLOR;
//     draw_x = x * TILE_SIZE;
//     draw_y = y * TILE_SIZE;
//     draw_tile(&data->image, draw_x, draw_y, color);
// }

// void    create_minimap(t_var *data)
// {
//     int y;
//     int x;
    
//     y = 0;
//     while (y < data->map.height)
//     {
//         x = 0;
//         while (x < data->map.max_length - 1)
//         {
//             draw_map(data, x, y);
//             x++;
//         }
//         y++;
//     }
// }


void draw_map(t_var *data, int x, int y, int draw_x, int draw_y)
{
    int color;
    char tile = data->map.arr[y][x];
    
    // Determine tile color based on the type
    if (tile == '0')
        color = EMPTY_COLOR;
    else if (tile == '1')
        color = WALL_COLOR;
    else if (tile == '2')
        color = DOOR_COLOR;
    else if (tile == '3')
        color = DOOR_OPEN_COLOR;
    else
        color = EMPTY_COLOR;
    
    // Draw the tile in the specified position
    draw_tile(&data->image, draw_x, draw_y, color);
}

void create_minimap(t_var *data)
{
    int draw_size = 8;
    int minimap_size = 256;
    int tile_size_minimap = minimap_size / draw_size;

    int player_tile_x = (int)(data->player.px / TILE_SIZE);
    int player_tile_y = (int)(data->player.py / TILE_SIZE);

    // Check if player is outside current minimap window
    int margin = 2;
    int shift = 4;

    if (player_tile_x < data->minimap_offset_x + margin)
        data->minimap_offset_x -= shift;
    else if (player_tile_x >= data->minimap_offset_x + draw_size - margin)
        data->minimap_offset_x += shift;

    if (player_tile_y < data->minimap_offset_y + margin)
        data->minimap_offset_y -= shift;
    else if (player_tile_y >= data->minimap_offset_y + draw_size - margin)
        data->minimap_offset_y += shift;


    // Clamp offsets to map size
    if (data->minimap_offset_x < 0)
        data->minimap_offset_x = 0;
    if (data->minimap_offset_y < 0)
        data->minimap_offset_y = 0;
    if (data->minimap_offset_x + draw_size > data->map.max_length)
        data->minimap_offset_x = data->map.max_length - draw_size;
    if (data->minimap_offset_y + draw_size > data->map.height)
        data->minimap_offset_y = data->map.height - draw_size;

    // Draw tiles from current minimap offset
    for (int y = 0; y < draw_size; y++)
    {
        for (int x = 0; x < draw_size; x++)
        {
            int map_x = data->minimap_offset_x + x;
            int map_y = data->minimap_offset_y + y;
            int draw_x = x * tile_size_minimap;
            int draw_y = y * tile_size_minimap;

            draw_map(data, map_x, map_y, draw_x, draw_y);
        }
    }

    // Store tile size for player rendering
    data->minimap.tile_size = tile_size_minimap;
}

