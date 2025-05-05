#include "../cub3d.h"

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
