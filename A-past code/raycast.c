#include "../cub3d.h"

// void draw_line(t_var *data, t_line line, int color)
// {
//     int i;

//     i = 0;
//     line.dx = line.rx - data->player.px;
//     line.dy = line.ry - data->player.py;
//     if (fabs(line.dx) > fabs(line.dy))
//         line.steps =  fabs(line.dx);
//     else
//         line.steps =  fabs(line.dy);
//     line.x_inc = line.dx / line.steps;
//     line.y_inc = line.dy / line.steps;
//     while (i <= line.steps)
//     {
//         my_mlx_pixel_put(&data->image, (int)line.px, (int) line.py, color);
//         line.px += line.x_inc;
//         line.py += line.y_inc;
//         i++;
//     }
// }

// void cast_rays(t_var *data, t_ray *ray)
// {
//     t_line line;

//     if (ray->disH < ray->disV)
//         ray->side = 1; 
//     else
//     {
//         ray->disH = ray->disV;
//         ray->rx = ray->vx;
//         ray->ry = ray->vy;
//     }
//     line = (t_line){0};
//     line.px = (int)data->player.px;
//     line.py = (int)data->player.py;
//     line.rx = (int)ray->rx;
//     line.ry = (int)ray->ry;
//     draw_line(data, line, RAYCAST_COLOR);
// }

void draw_line(t_var *data, t_line line, int color)
{
    int i;

    i = 0;
    line.dx = line.rx - line.px;
    line.dy = line.ry - line.py;
    if (fabs(line.dx) > fabs(line.dy))
        line.steps = fabs(line.dx);
    else
        line.steps = fabs(line.dy);
    if (line.steps == 0)  // Prevent division by zero
        return;
    line.x_inc = line.dx / line.steps;
    line.y_inc = line.dy / line.steps;
    
    while (i <= line.steps)
    {
        int px = (int)line.px;
        int py = (int)line.py;
        // Only draw if within minimap bounds
        if (px >= 0 && px < MINIMAP_SIZE && py >= 0 && py < MINIMAP_SIZE) 
        {
            my_mlx_pixel_put(&data->image, px, py, color);
        }
        line.px += line.x_inc;
        line.py += line.y_inc;
        i++;
    }
}

void cast_rays(t_var *data, t_ray *ray)
{
    t_line line;
    float tile_size = data->minimap.tile_size;

    if (ray->disH < ray->disV) {
        ray->side = 1; 
    } else {
        ray->disH = ray->disV;
        ray->rx = ray->vx;
        ray->ry = ray->vy;
    }

    // Convert player position to minimap coordinates
    int p_tile_x = (int)(data->player.px / TILE_SIZE);
    int p_tile_y = (int)(data->player.py / TILE_SIZE);
    float p_offset_x = (data->player.px - p_tile_x * TILE_SIZE) / TILE_SIZE;
    float p_offset_y = (data->player.py - p_tile_y * TILE_SIZE) / TILE_SIZE;
    int draw_px = (p_tile_x - data->minimap_offset_x) * tile_size + p_offset_x * tile_size;
    int draw_py = (p_tile_y - data->minimap_offset_y) * tile_size + p_offset_y * tile_size;

    // Convert ray hit point to minimap coordinates
    int r_tile_x = (int)(ray->rx / TILE_SIZE);
    int r_tile_y = (int)(ray->ry / TILE_SIZE);
    float r_offset_x = (ray->rx - r_tile_x * TILE_SIZE) / TILE_SIZE;
    float r_offset_y = (ray->ry - r_tile_y * TILE_SIZE) / TILE_SIZE;
    int draw_rx = (r_tile_x - data->minimap_offset_x) * tile_size + r_offset_x * tile_size;
    int draw_ry = (r_tile_y - data->minimap_offset_y) * tile_size + r_offset_y * tile_size;

    // Set up line coordinates for minimap
    line = (t_line){0};
    line.px = draw_px + PLAYER_SIZE / 2;
    line.py = draw_py + PLAYER_SIZE / 2;
    line.rx = draw_rx;
    line.ry = draw_ry;

    draw_line(data, line, RAYCAST_COLOR);
}

// void draw_3d_helper(t_var *data, t_ray *ray, int lineOff, int lineH)
// {
//     int color = ray->side == 1 ? 0xCCCCCC : 0xAAAAAA;
//     int x, y, w;

//     w = 0;
//     while (w < STRIP_WIDTH)
//     {
//         x = ray->r * STRIP_WIDTH + (data->map.max_length * TILE_SIZE) + w;

//         if (x < WINDOW_WIDTH)
//         {
//             y = 0;
//             while (y < WINDOW_HEIGHT)
//             {
//                 if (y < lineOff)
//                     my_mlx_pixel_put(&data->image, x, y, CEILING_COLOR);
//                 else if (y >= lineOff && y < lineOff + lineH)
//                     my_mlx_pixel_put(&data->image, x, y, color);
//                 else
//                     my_mlx_pixel_put(&data->image, x, y, FLOOR_COLOR);
//                 y++;
//             }
//         }
//         w++;
//     }
// }


// void draw_3d_helper(t_var *data, t_ray *ray, int lineOff, int lineH)
// {
//     int color = ray->side == 1 ? 0xCCCCCC : 0xAAAAAA;
//     int x, y, w;

//     w = 0;
//     while (w < STRIP_WIDTH)
//     {
//         // Draw starting after the minimap
//         x = ray->r * STRIP_WIDTH + MINIMAP_WIDTH;

//         if (x < WINDOW_WIDTH)
//         {
//             y = 0;
//             while (y < WINDOW_HEIGHT)
//             {
//                 if (y < lineOff)
//                     my_mlx_pixel_put(&data->image, x, y, CEILING_COLOR);
//                 else if (y >= lineOff && y < lineOff + lineH)
//                     my_mlx_pixel_put(&data->image, x, y, color);
//                 else
//                     my_mlx_pixel_put(&data->image, x, y, FLOOR_COLOR);
//                 y++;
//             }
//         }
//         w++;
//     }
// }

void draw_3d_helper(t_var *data, t_ray *ray, int lineOff, int lineH)
{
    int color = ray->side == 1 ? 0xCCCCCC : 0xAAAAAA;
    int x, y;

    x = ray->r * STRIP_WIDTH;
    
    for (int w = 0; w < STRIP_WIDTH; w++)
    {
        if (x + w >= WINDOW_WIDTH) continue;
        
        for (y = 0; y < WINDOW_HEIGHT; y++)
        {
            // Skip drawing in minimap area
            if (x + w < MINIMAP_SIZE && y < MINIMAP_SIZE) continue;
            
            if (y < lineOff)
                my_mlx_pixel_put(&data->image, x + w, y, CEILING_COLOR);
            else if (y >= lineOff && y < lineOff + lineH)
                my_mlx_pixel_put(&data->image, x + w, y, color);
            else
                my_mlx_pixel_put(&data->image, x + w, y, FLOOR_COLOR);
        }
    }
}

void draw_3d(t_var *data, t_ray *ray)
{
    double corrected_dist;
    int lineH;
    int lineOff;

    corrected_dist = ray->disH * cos(normalize_radians(data->player.pa - ray->ra));
    lineH = (TILE_SIZE * WINDOW_HEIGHT) / corrected_dist;
    lineH = lineH > WINDOW_HEIGHT ? WINDOW_HEIGHT : lineH;
    lineOff = (WINDOW_HEIGHT / 2) - (lineH / 2);
    
    draw_3d_helper(data, ray, lineOff, lineH);
}

// void draw_3d(t_var *data, t_ray *ray)
// {
//     double corrected_dist;
//     int lineH;
//     int lineOff;
//     int color;
//     int x, y, w;

//     corrected_dist = ray->disH * cos(normalize_radians(data->player.pa - ray->ra));
//     lineH = (TILE_SIZE * WINDOW_HEIGHT)/(corrected_dist);
//     if (lineH > WINDOW_HEIGHT)
//         lineH= WINDOW_HEIGHT;
//     lineOff = (WINDOW_HEIGHT / 2) - (lineH >> 1);
//     draw_3d_helper(data, ray, lineOff, lineH);
// }
void draw_sprites(t_var *data) {
    if (data->num_sprites == 0)
        return;

    t_sprite *sp = &data->sprites[0];

    // Relative position
    float sx = sp->x - data->player.px;
    float sy = sp->y - data->player.py;

    // View space rotation
    float sin_pa = sin(-data->player.pa);
    float cos_pa = cos(-data->player.pa);

    float dx = sx * cos_pa - sy * sin_pa; // Depth
    float dy = sx * sin_pa + sy * cos_pa; // Horizontal offset

    if (dx <= 0.1f) return;

    // Projection calculations
    float fov_scale = (WINDOW_WIDTH / 2.0f) / tan(FOV / 2);
    float proj_x = (dy * fov_scale / dx) + (WINDOW_WIDTH / 2.0f);
    float sprite_height = (SPRITE_SIZE * fov_scale) / dx;
    int sprite_size = (int)sprite_height;

    // Calculate vertical positioning
    float lineH = (TILE_SIZE * WINDOW_HEIGHT) / dx;
    lineH = lineH > WINDOW_HEIGHT ? WINDOW_HEIGHT : lineH;
    int floorY = (WINDOW_HEIGHT / 2) + (lineH / 2);

    // Sprite dimensions
    int screen_x = (int)proj_x - sprite_size / 2;
    int screen_y = floorY - sprite_size;

    // Draw sprite with depth check
    for (int x = 0; x < sprite_size; x++) {
        int px = screen_x + x;
        if (px < 0 || px >= WINDOW_WIDTH) continue;
        
        // Get corresponding ray index for depth check
        int ray_idx = (px * NUM_RAYS) / WINDOW_WIDTH;
        if (ray_idx < 0 || ray_idx >= NUM_RAYS) continue;
        
        // Depth comparison using z-buffer
        if (dx > data->zbuffer[ray_idx]) continue;

        for (int y = 0; y < sprite_size; y++) {
            int py = screen_y + y;
            if (py >= 0 && py < WINDOW_HEIGHT) {
                my_mlx_pixel_put(&data->image, px, py, 0xFFFF00);
            }
        }
    }
}


void draw_rays(t_var *data)
{
    t_ray ray;

    ray = (t_ray){0};
    ray.ra = normalize_radians(data->player.pa - (PI/6));
    ray.r = 0;
    while (ray.r < NUM_RAYS)
    {
        ray.disV = 100000;
        ray.disH = 100000;
        ray.side = 0;
        ray.dof = 0;
        ray.tan = - tan(ray.ra);
        cast_vertical(data, &ray);
        ray.dof = 0;
        ray.tan =  1 / ray.tan;
        cast_horizontal(data, &ray);
        data->zbuffer[ray.r] = fmin(ray.disH, ray.disV);
        cast_rays(data, &ray);
        draw_3d(data, &ray);
        ray.ra += (PI / 3) / NUM_RAYS;
        ray.ra = normalize_radians(ray.ra);
        ray.r++;
    }
    draw_sprites(data);
}
