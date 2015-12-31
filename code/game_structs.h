#if !defined(GAME_STRUCTS_H)

#include "SDL.h"

struct Button
{
    bool Pressed;
};

struct Buttons
{
    Button A;
    Button S;
    Button D;
    Button W;
};

struct GameInput
{
    float TimeDelta;
    union
    {
        Button buttons[4];
        Buttons IndividualButtons;
    };
    vec2 MousePos;
};

enum entity_type
{
    Player
};

struct Entity
{
    entity_type Type;
    vec2 Pos;
    vec2 Vel;
    vec2 Direction;
    float Speed;
    float Width;
    float Height;
};

struct GameState
{
    SDL_Texture *Background;
    SDL_Texture *Player;
    SDL_Texture *Pointer;
    SDL_Texture *Circle;
    
    float MeterToPixels;
    int PlayerEntity;
    Entity Entities[2048];
};



#define GAME_STRUCTS_H
#endif
