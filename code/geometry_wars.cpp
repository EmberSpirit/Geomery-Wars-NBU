#include <windows.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "vector_intrinsics.h"
#include "game_structs.h"
#include "win32_main.h"
#include <stdio.h>

int UpdatePlayer(GameInput *Input, GameState *State)
{
    Entity *Player = &State->Entities[State->PlayerEntity];

    vec2 Acceleration = {0, 0};

    if(Input->IndividualButtons.W.Pressed)
        Acceleration.y -= 1.0f;
    if(Input->IndividualButtons.S.Pressed)
        Acceleration.y += 1.0f;
    if(Input->IndividualButtons.A.Pressed)
        Acceleration.x -= 1.0f;
    if(Input->IndividualButtons.D.Pressed)
        Acceleration.x += 1.0f;

    if(Acceleration.x || Acceleration.y)
        Acceleration = NormalizeVector(Acceleration);

    Acceleration = Acceleration * Player->Speed;

    Player->Vel = Player->Vel + (Acceleration - Player->Vel * 4) * Input->TimeDelta;

    Player->Pos = Player->Pos + (Player->Vel * Input->TimeDelta) +
        (Acceleration * Input->TimeDelta * Input->TimeDelta * 0.5);

    Player->Direction = NormalizeVector(Input->MousePos - Player->Pos);
    
    return 0;
}

int RenderScene(GameState *State, SDL_Renderer *Renderer)
{
    SDL_RenderClear(Renderer);

    SDL_RenderCopy(Renderer, State->Background, NULL, NULL);

    Entity Player = State->Entities[State->PlayerEntity];
                                    
    SDL_Rect DescRect;
    float Width = Player.Width * State->MeterToPixels;
    float Height = Player.Height * State->MeterToPixels;
    float CenterX = Player.Pos.x - 0.5f * Width;
    float CenterY = Player.Pos.y - 0.5f * Height;
    DescRect.x = CenterX;
    DescRect.y = CenterY;
    DescRect.w = Width;
    DescRect.h = Height;
    SDL_Point Center;
    Center.x = CenterX;
    Center.y = CenterY;


    vec2 NaturalDirection = {0, -1};
    float Angle = AngleBetweenVectors(Player.Direction, NaturalDirection);

    char buffer[128];
    sprintf(buffer, "Angle: %f\n", Angle);
    OutputDebugString(buffer);

    SDL_RenderCopyEx(Renderer, State->Pointer, NULL, &DescRect, Angle, NULL, SDL_FLIP_NONE);

    Angle = AngleBetweenVectors(Player.Vel, NaturalDirection);
    SDL_RenderCopyEx(Renderer, State->Player, NULL, &DescRect, Angle, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(Renderer, State->Circle, NULL, &DescRect, Angle, NULL, SDL_FLIP_NONE);
                    
    SDL_RenderPresent(Renderer);

    return 0;
}

extern "C" GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
    char buffer[256];
    sprintf(buffer, "FPS: %d Time per frame: %fseconds\n",
            (int)(1.0 / Input->TimeDelta), Input->TimeDelta);
    OutputDebugString(buffer);

    static bool NotInitiated = true;

    if(NotInitiated)
    {
        NotInitiated = false;

        State->MeterToPixels = 50;
        State->PlayerEntity = 1;
        State->Entities[1].Width = 2;
        State->Entities[1].Height = 2;
        State->Entities[1].Speed = 1000;
    }

    UpdatePlayer(Input, State);

    RenderScene(State, Renderer);
}
