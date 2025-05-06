#include "../cub3d.h"

void draw_3d(t_var *data, t_ray *ray)
{
    int lineH;
    int lineOff;

    ray->disH *= cos(normalize_radians(data->player.pa - ray->ra));
    lineH = (TILE_SIZE * WINDOW_HEIGHT) / ray->disH;
    ray->ty_step = (float)TILE_SIZE_TEXTURE /(float)lineH;
    ray->ty_off = 0;
    if (lineH > WINDOW_HEIGHT)
    {
        ray->ty_off = (lineH - WINDOW_HEIGHT)/2.0;
        lineH = WINDOW_HEIGHT;
    }
    lineOff = (WINDOW_HEIGHT/2) - (lineH >> 1);
    ray->ty = ray->ty_off * ray->ty_step;
    set_tx(data, ray);
    draw_3d_helper(data, ray, lineOff, lineH);
}

void draw_3d_helper(t_var *data, t_ray *ray, int lineOff, int lineH)
{
    t_img *tex;
    int color;
    int y;
    int w;
    int x;
    
    tex = get_texture(data, ray);
    x = ray->r * STRIP_WIDTH;
    y = 0;
    while (y < WINDOW_HEIGHT)
    {
        if (y < lineOff)
            color = data->map.textures[CEILING].color;
        else if (y < lineH + lineOff)
        {
            color = get_color(ray, tex);
            ray->ty += ray->ty_step;
        }
        else
            color = data->map.textures[FLOOR].color;
        draw_wall_strip(&data->image, x, y, color);
        y++;
    }
}

void set_tx(t_var *data, t_ray *ray)
{
    if (ray->side == 1)
    {
        ray->tx = (int)(ray->rx) % TILE_SIZE;
        if (ray->ra > M_PI)
            ray->tx = TILE_SIZE - ray->tx;
    }
    else
    {
        ray->tx = (int)(ray->ry) % TILE_SIZE;
        if (ray->ra > M_PI/2 && ray->ra < 1.5 * M_PI)
            ray->tx = TILE_SIZE - ray->tx;
    }
    ray->tx = ray->tx * TILE_SIZE_TEXTURE / TILE_SIZE;
}

void    draw_wall_strip(t_img *img, int x, int y, int color)
{
    int w;

    w = 0;
    while (w < STRIP_WIDTH)
    {
        if (x + w >= WINDOW_WIDTH)
            return ;
        else if (x + w < MINIMAP_SIZE && y < MINIMAP_SIZE)
            return ;
        my_mlx_pixel_put(img, x + w, y, color);
        w++;   
    }
}
