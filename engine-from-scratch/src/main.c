#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glad/glad.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "engine/global.h"
#include "engine/config.h"
#include "engine/input.h"
#include "engine/time.h"
#include "engine/physics.h"
#include "engine/util.h"
#include "engine/entity.h"
#include "engine/render.h"

typedef enum collision_layer {
    COLLISION_LAYER_PLAYER = 1,
    COLLISION_LAYER_ENEMY = 1 << 1,
    COLLISION_LAYER_TERRAIN = 1 << 2, 
} Collision_Layer;

static bool should_quit = false;
static vec4 player_color = {0,1,1,1};
bool player_is_grounded = false;
static vec2 pos;

static void input_handle(Body *body_player) {
    if (global.input.escape) {
        should_quit = true;
    }
    
    f32 velx = 0;
    f32 vely = body_player->velocity[1];

    if (global.input.right) {
        velx += 600;
    }
    if (global.input.left) {
        velx -= 600;
    }
    if (global.input.up && player_is_grounded) {
        player_is_grounded = false;
        vely = 2000;
    }

    body_player->velocity[0] = velx;
    body_player->velocity[1] = vely;

}

void player_on_hit(Body *self, Body *other, Hit hit){
    if (other-> collision_layer == COLLISION_LAYER_ENEMY){
        player_color[0] = 1;
        player_color[2] = 0;
    }
}

void player_on_hit_static(Body *self, Hit hit){
    if (hit.normal[1] > 0) {
        player_is_grounded = true;
    }
}

void enemy_on_hit_static(Body *self, Hit hit) {
    if (hit.normal[0] > 0) {
        self->velocity[0] *= -1;
    } 
    
    if (hit.normal[0] < 0) {
        self->velocity[0] *= -1;
    }
}

int main(int argc, char *argv[]) {
    time_init(60);  
    config_init();
    SDL_Window *window = render_init();
    physics_init();
    entity_init();

    SDL_ShowCursor(false);

    u8 enemy_mask = COLLISION_LAYER_PLAYER | COLLISION_LAYER_TERRAIN;
    u8 player_mask = COLLISION_LAYER_ENEMY | COLLISION_LAYER_TERRAIN;

    usize player_id = entity_create((vec2){100, 200}, (vec2){24,24}, (vec2){0,0}, COLLISION_LAYER_PLAYER, player_mask, player_on_hit, player_on_hit_static);

    i32 window_width, window_height;
    SDL_GetWindowSize(window, & window_width, &window_height);
    f32 width = window_width / render_get_scale();
    f32 height = window_width / render_get_scale();

    u32 static_body_a_id = physics_static_body_create((vec2){width * 0.5 - 12.5, height - 12.5}, (vec2){width - 25, 25}, COLLISION_LAYER_TERRAIN);
    u32 static_body_b_id = physics_static_body_create((vec2){width - 12.5, height * 0.5 + 12.5}, (vec2){25,height - 25}, COLLISION_LAYER_TERRAIN);
    u32 static_body_c_id = physics_static_body_create((vec2){width *0.5 + 12.5, 12.5}, (vec2){width - 25,25}, COLLISION_LAYER_TERRAIN);
    u32 static_body_d_id = physics_static_body_create((vec2){12.5, height * 0.5 - 12.5}, (vec2){25,height - 25}, COLLISION_LAYER_TERRAIN);
    u32 static_body_e_id = physics_static_body_create((vec2){width * 0.5, height * 0.5}, (vec2){62,62}, COLLISION_LAYER_TERRAIN);

    usize entity_a_id = entity_create((vec2){200,200}, (vec2){25, 25}, (vec2){200, 0}, COLLISION_LAYER_ENEMY, enemy_mask, NULL, enemy_on_hit_static);
    usize entity_b_id = entity_create((vec2){300,300}, (vec2){25, 25}, (vec2){-200, 0}, COLLISION_LAYER_ENEMY, enemy_mask, NULL, enemy_on_hit_static);

    while (!should_quit) {
        time_update();

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                should_quit = true;
                break;
            default:
                break;
            }
        }

        Entity *player = entity_get(player_id);
        Body *body_player = physics_body_get(player->body_id);
        Static_Body *static_body_a = physics_static_body_get(static_body_a_id);
        Static_Body *static_body_b = physics_static_body_get(static_body_b_id);
        Static_Body *static_body_c = physics_static_body_get(static_body_c_id);
        Static_Body *static_body_d = physics_static_body_get(static_body_d_id);
        Static_Body *static_body_e = physics_static_body_get(static_body_e_id);

        input_update();
        input_handle(body_player);
        physics_update();

        render_begin();

        render_aabb((f32*)static_body_a, WHITE);
        render_aabb((f32*)static_body_b, WHITE);
        render_aabb((f32*)static_body_c, WHITE);
        render_aabb((f32*)static_body_d, WHITE);
        render_aabb((f32*)static_body_e, WHITE);
        render_aabb((f32*)body_player, player_color);

        render_aabb((f32*)physics_body_get(entity_get(entity_a_id)->body_id), WHITE);
        render_aabb((f32*)physics_body_get(entity_get(entity_b_id)->body_id), WHITE);

        render_end(window);
        player_color[0] = 0;
        player_color[2] = 1;

        time_update_late();
    }
    return 0;
}