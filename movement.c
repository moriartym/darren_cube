#include "cub3d.h"

int handle_keypress(int keysym, t_var *data)
{
    if (keysym == XK_Escape)
        close_window(data);
    else if (keysym == XK_a)
        data->move.move_a = 1;
    else if (keysym == XK_d)
        data->move.move_d = 1;
    else if (keysym == XK_w)
        data->move.move_w = 1;
    else if (keysym == XK_s)
        data->move.move_s = 1;
    else if (keysym == XK_Left)
        data->move.move_al = 1;
    else if (keysym == XK_Right)
        data->move.move_ar = 1;
    else if (keysym == XK_m)
    {
        if (!data->mouse_state)
            data->mouse_state = 1;
        else
        {
            data->last_mouse_x = -1;
            data->mouse_state = 0;
        }
    }
    else if (keysym == XK_c)
    {
        int curr_tile_x = data->player.px / TILE_SIZE;
        int curr_tile_y = data->player.py / TILE_SIZE;
        
        int dir_x = (int)roundf(cosf(data->player.pa));
        int dir_y = (int)roundf(sinf(data->player.pa));
        
        int tile_in_front_x = curr_tile_x + dir_x;
        int tile_in_front_y = curr_tile_y + dir_y;
        
        // Bounds check to avoid segfault
        if (tile_in_front_x >= 0 && tile_in_front_x < data->map.width &&
            tile_in_front_y >= 0 && tile_in_front_y < data->map.height)
        {
            char *tile = &data->map.arr[tile_in_front_y][tile_in_front_x];
            if (*tile == '0')
            {
                *tile = '2';
                printf("Door Created\n");
            }
            else if (*tile == '2' || *tile == '3')
            {
                *tile = '0';
                printf("Door Destroyed\n");
            }
        }
        else
        {
            printf("Invalid tile position (outside map bounds)\n");
        }
    }
    else if (keysym == XK_o)
    {
        int curr_tile_x = data->player.px / TILE_SIZE;
        int curr_tile_y = data->player.py / TILE_SIZE;
        
        int dir_x = (int)roundf(cosf(data->player.pa));
        int dir_y = (int)roundf(sinf(data->player.pa));
        
        int tile_in_front_x = curr_tile_x + dir_x;
        int tile_in_front_y = curr_tile_y + dir_y;
        
        // Bounds check
        if (tile_in_front_x >= 0 && tile_in_front_x < data->map.width &&
            tile_in_front_y >= 0 && tile_in_front_y < data->map.height)
        {
            char *tile = &data->map.arr[tile_in_front_y][tile_in_front_x];
            if (*tile == '2')
            {
                *tile = '3';
                printf("Door Opened\n");
            }
            else if (*tile == '3')
            {
                *tile = '2';
                printf("Door Closed\n");
            }
        }
        else
        {
            printf("Invalid tile position (outside map bounds)\n");
        }
    }

    return 0;
}

int handle_keyrelease(int keysym, t_var *data)
{
    if (keysym == XK_a)
        data->move.move_a = 0;
    else if (keysym == XK_d)
        data->move.move_d = 0;
    else if (keysym == XK_w)
        data->move.move_w = 0;
    else if (keysym == XK_s)
        data->move.move_s = 0;
    else if (keysym == XK_Left)
        data->move.move_al = 0;
    else if (keysym == XK_Right)
        data->move.move_ar = 0;
    return 0;
}

double get_delta_time(t_var *data)
{
    struct timeval current_time;
    double delta_time;

    gettimeofday(&current_time, NULL);
    delta_time = (current_time.tv_sec - data->last_time.tv_sec) +
                 (current_time.tv_usec - data->last_time.tv_usec) / 1000000.0;
    data->last_time = current_time;
    if (delta_time > MAX_DELTA_TIME)
        delta_time = MAX_DELTA_TIME;
    return delta_time;
}

float normalize_radians(float angle)
{
    while (angle < 0)
        angle += 2 * PI;
    while (angle >= 2 * PI)
        angle -= 2 * PI;
    return angle;
}

void update_movement(t_var *data)
{
    double delta_time;

    delta_time = get_delta_time(data);
    if (data->move.move_al)
        data->player.pa -= ROTATION_SPEED * delta_time;
    if (data->move.move_ar)
        data->player.pa += ROTATION_SPEED * delta_time;
    data->player.pa = normalize_radians(data->player.pa);
    data->player.pdy = sinf(data->player.pa) * MOVEMENT_SPEED * delta_time;
    data->player.pdx = cosf(data->player.pa) * MOVEMENT_SPEED * delta_time;
    
    int xo = 0;
    if (data->player.pdx < 0) { xo = -10; } else { xo = 10; }
    int yo = 0;
    if (data->player.pdy < 0) { yo = -10; } else { yo = 10; }
    
    int ipx = data->player.px / TILE_SIZE;
    int ipx_add_xo = (data->player.px + xo) / TILE_SIZE;
    int ipx_sub_xo = (data->player.px - xo) / TILE_SIZE;
    
    int ipy = data->player.py / TILE_SIZE;
    int ipy_add_yo = (data->player.py + yo) / TILE_SIZE;
    int ipy_sub_yo = (data->player.py - yo) / TILE_SIZE;
    
    if (data->move.move_w)
    {
        if (data->map.arr[ipy][ipx_add_xo] == '0' || data->map.arr[ipy][ipx_add_xo] == '3')
            data->player.px += data->player.pdx;
        if (data->map.arr[ipy_add_yo][ipx] == '0' || data->map.arr[ipy_add_yo][ipx] == '3')
            data->player.py += data->player.pdy;       
    }
    if (data->move.move_s)
    {
        if (data->map.arr[ipy][ipx_sub_xo] == '0' || data->map.arr[ipy][ipx_sub_xo] == '3')
            data->player.px -= data->player.pdx;
        if (data->map.arr[ipy_sub_yo][ipx] == '0' || data->map.arr[ipy_sub_yo][ipx] == '3')
            data->player.py -= data->player.pdy;    
    }

    if (data->move.move_d)
    {
        int strafe_px = data->player.px - yo;
        int strafe_py = data->player.py + xo;

        int strafe_ipx = strafe_px / TILE_SIZE;
        int strafe_ipy = strafe_py / TILE_SIZE;

        
        if (data->map.arr[ipy][strafe_ipx] == '0' || data->map.arr[ipy][strafe_ipx] == '3')
            data->player.px -= data->player.pdy;
        if (data->map.arr[strafe_ipy][ipx] == '0' || data->map.arr[strafe_ipy][ipx] == '3')
            data->player.py += data->player.pdx;
    }
    if (data->move.move_a)
    {
        int strafe_px = data->player.px + yo;
        int strafe_py = data->player.py - xo;
    
        int strafe_ipx = strafe_px / TILE_SIZE;
        int strafe_ipy = strafe_py / TILE_SIZE;

        if (data->map.arr[ipy][strafe_ipx] == '0' || data->map.arr[ipy][strafe_ipx] == '3')
            data->player.px += data->player.pdy;
        if (data->map.arr[strafe_ipy][ipx] == '0' || data->map.arr[strafe_ipy][ipx] == '3')
            data->player.py -= data->player.pdx;
    }
}

