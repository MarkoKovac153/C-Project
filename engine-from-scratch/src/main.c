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

static bool should_quit = false;
static vec2 pos;

static void input_handle(Body *body_player) {
    if (global.input.escape) {
        should_quit = true;
    }
    
    f32 velx = 0;
    f32 vely = body_player->velocity[1];

    if (global.input.right) {
        velx += 1000;
    }
    if (global.input.left) {
        velx -= 1000;
    }
    if (global.input.up) {
        vely = 4000;
    }
    if (global.input.down) {
        velx -= 800;
    }

    body_player->velocity[0] = velx;
    body_player->velocity[1] = vely;

}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    time_init(60);
    config_init();
    render_init();
    physics_init();

    printf("Key bind for LEFT: %d\n", global.config.keybinds[INPUT_KEY_LEFT]);
    printf("Key bind for RIGHT: %d\n", global.config.keybinds[INPUT_KEY_RIGHT]);
    printf("Key bind for UP: %d\n", global.config.keybinds[INPUT_KEY_UP]);
    printf("Key bind for DOWN: %d\n", global.config.keybinds[INPUT_KEY_DOWN]);
    printf("Key bind for ESCAPE: %d\n", global.config.keybinds[INPUT_KEY_ESCAPE]);

    SDL_ShowCursor(false);

    f32 width = global.render.width;
    f32 height = global.render.height;

    u32 body_a_id = physics_body_create((vec2){width*0.25,height*0.5}, (vec2){50,50});

    u32 static_body_a_id = physics_static_body_create((vec2){width * 0.5 - 25, height - 25}, (vec2){width - 50, 50});
    u32 static_body_b_id = physics_static_body_create((vec2){width - 25, height * 0.5 + 25}, (vec2){50,height - 50});
    u32 static_body_c_id = physics_static_body_create((vec2){width *0.5 + 25, 25}, (vec2){width - 50,50});
    u32 static_body_d_id = physics_static_body_create((vec2){25, height * 0.5 - 25}, (vec2){50,height - 50});
    u32 static_body_e_id = physics_static_body_create((vec2){width * 0.5, height * 0.5}, (vec2){150,150});

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

        Body *body_a_player = physics_body_get(body_a_id);
        Static_Body *static_body_a = physics_static_body_get(static_body_a_id);
        Static_Body *static_body_b = physics_static_body_get(static_body_b_id);
        Static_Body *static_body_c = physics_static_body_get(static_body_c_id);
        Static_Body *static_body_d = physics_static_body_get(static_body_d_id);
        Static_Body *static_body_e = physics_static_body_get(static_body_e_id);

        input_update();
        input_handle(body_a_player);
        physics_update();

        render_begin();

        render_aabb((f32*)static_body_a, WHITE);
        render_aabb((f32*)static_body_b, WHITE);
        render_aabb((f32*)static_body_c, WHITE);
        render_aabb((f32*)static_body_d, WHITE);
        render_aabb((f32*)static_body_e, WHITE);
        render_aabb((f32*)body_a_player, CYAN);


        render_end();
        time_update_late();
    }
    return 0;
}