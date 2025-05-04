#include "cub3d.h"

void create_image_buffer(t_var *data)
{
    data->image.img = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    data->image.addr = mlx_get_data_addr(data->image.img,
            &data->image.bits_per_pixel,
            &data->image.line_length,
            &data->image.endian);
}

void my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
    char *dst;

    dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}

int close_window(t_var *data)
{
    if (data->mlx && data->win)
        mlx_destroy_window(data->mlx, data->win);
    if (data->mlx)
    {
        /* UNCOMMENT BEFORE SUBMISSION */
        // mlx_destroy_display(data->mlx);
        free(data->mlx);
    }
    exit(0);
    return 0;
}

int	key_hook(int keysym, t_var *data)
{
	if (keysym == XK_Escape)
		close_window(data);
	return (0);
}

int render(t_var *data)
{
    update_movement(data);
    create_minimap(data);
    draw_player(data);
    draw_rays(data);
    mlx_put_image_to_window(data->mlx, data->win, data->image.img, 0, 0);
}

int handle_mouse_move(int x, int y, t_var *data)
{
    if (data->mouse_state)
    {
        const float MOUSE_SENSITIVITY = 0.01f;
    
        if (data->last_mouse_x == -1)
        {
            data->last_mouse_x = x;
            return 0;
        }
    
        int delta_x = x - data->last_mouse_x;
        data->last_mouse_x = x;
    
        data->player.pa += delta_x * MOUSE_SENSITIVITY;
        data->player.pa = normalize_radians(data->player.pa);
    }
    return 0;
}

int create_visual(t_cub *cube)
{
    t_var data = {0};

    data.mlx = mlx_init();
    init_all(&data, cube);
    if (!data.mlx)
        return 1;
    data.win = mlx_new_window(data.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3d");
    if (!data.win)
        return 1;
    create_image_buffer(&data);    
    mlx_loop_hook(data.mlx, render, &data);
    mlx_hook(data.win, DestroyNotify, StructureNotifyMask, &close_window, &data);
    mlx_hook(data.win, KeyPress, KeyPressMask, &handle_keypress, &data);
    mlx_hook(data.win, KeyRelease, KeyReleaseMask, &handle_keyrelease, &data);
    mlx_hook(data.win, 6, (1L<<6), &handle_mouse_move, &data);
    mlx_loop(data.mlx);
    return 0;
}
