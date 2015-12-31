#if !defined(WIN32_MAIN_H)

#include "SDL.h"

#define GAME_UPDATE_AND_RENDER(name) void name(GameInput *Input, GameState *State, SDL_Renderer *Renderer)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);

struct game_code
{
    HMODULE GameCodeDLL;
    FILETIME DLLLastWriteTime;
    game_update_and_render *UpdateAndRender;
};

#define WIN32_MAIN_H
#endif
