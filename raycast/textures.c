#include "../cub3d.h"

void    load_textures(t_var *data)
{
    t_id *tex = data->map.textures;
    int i;

    i = 0;
    while (i <= WEST)
    {
        if (extract_img(data->mlx, &tex[i].attr, tex[i].filename))
            close_window(data);
        i++;
    }
    while (i <= CEILING)
    {
        extract_rgb(&tex[i].color, tex[i].filename);
        i++;
    }
}

int extract_img(void *mlx, t_img *attr, char *filename)
{
    attr->img = mlx_xpm_file_to_image(mlx, filename, &attr->width, &attr->height);
    if (!attr->img)
        return (ft_error_return("mlx_xpm_file_to_image failed"));

    attr->addr = mlx_get_data_addr(attr->img, &attr->bits_per_pixel, &attr->line_length, &attr->endian);
    return (0);
}

int    extract_rgb(int *color, char *rgb)
{
    char    **colors;

    colors = ft_split(rgb, ',');
	if (!colors)
		return (ft_error_return("malloc failed"));

    *color = (ft_atoi(colors[0]) << 16) | (ft_atoi(colors[1]) << 8) | ft_atoi(colors[2]);
    ft_free_2d(&colors);
    return (0);
}

t_img *get_texture(t_var *data, t_ray *ray)
{
    if (ray->side == 0)
    {
        if (ray->rx > data->player.px)
            return (&data->map.textures[EAST].attr);  // Facing East
        else
            return (&data->map.textures[WEST].attr);  // Facing West
    }
    else
    {
        if (ray->ry > data->player.py)
            return (&data->map.textures[SOUTH].attr);  // Facing South
        else
            return (&data->map.textures[NORTH].attr);  // Facing North
    }
}

int    get_color(t_ray *ray, t_img *tex)
{
    int pixel_index;
    int color;

    pixel_index = (int)ray->ty * tex->line_length + (int)ray->tx * (tex->bits_per_pixel / 8);
    color = *(int *)(tex->addr + pixel_index);

    if ((color & 0x00FFFFFF) == 0x000000)  // transparent
        return -1;
    return color;
}
