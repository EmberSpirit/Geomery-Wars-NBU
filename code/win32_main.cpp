#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <windows.h>

#define BREAK __debugbreak
#define ASSERT(cond) if(!cond) BREAK()

#include "vector_intrinsics.h"
#include "game_structs.h"
#include "win32_main.h"
#include "texture.h"

static game_code GameCode;

FILETIME Win32GetLastWriteTime(char *Filename)
{
    FILETIME LastWriteTime = {};

    WIN32_FILE_ATTRIBUTE_DATA Data;
    if(GetFileAttributesEx(Filename, GetFileExInfoStandard, &Data))
    {
        LastWriteTime = Data.ftLastWriteTime;
    }

    return LastWriteTime;
}

int LoadGameCode(char *DLLName, char *TempDLLName)
{
    CopyFile(DLLName, TempDLLName, FALSE);
    GameCode.GameCodeDLL = LoadLibrary(TempDLLName);
    if(!GameCode.GameCodeDLL)
    {
        ASSERT(!"Failed to load game dll.");
        return 1;
    }

    GameCode.UpdateAndRender = (game_update_and_render *)
        GetProcAddress(GameCode.GameCodeDLL, "GameUpdateAndRender");
    GameCode.DLLLastWriteTime = Win32GetLastWriteTime(DLLName);
    return 0;
}

int UnloadGameCode()
{
    FreeLibrary(GameCode.GameCodeDLL);
    GameCode.UpdateAndRender = NULL;
    
    return 0;
}

int LoadResourses(GameState *State, SDL_Renderer *Renderer)
{
    SDL_Surface *Surface = IMG_Load("C:/Users/Yavor/Desktop/Geometry-Wars-NBU/data/art/level_background.bmp");
    State->Background = SDL_CreateTextureFromSurface(Renderer, Surface);
    SDL_FreeSurface(Surface);

    Surface = IMG_Load("C:/Users/Yavor/Desktop/Geometry-Wars-NBU/data/art/player/player_body.bmp");
    State->Player = SDL_CreateTextureFromSurface(Renderer, Surface);
    SDL_FreeSurface(Surface);

    Surface = IMG_Load("C:/Users/Yavor/Desktop/Geometry-Wars-NBU/data/art/player/player_pointer.bmp");
    State->Pointer = SDL_CreateTextureFromSurface(Renderer, Surface);
    SDL_FreeSurface(Surface);

    Surface = IMG_Load("C:/Users/Yavor/Desktop/Geometry-Wars-NBU/data/art/player/player_circle.bmp");
    State->Circle = SDL_CreateTextureFromSurface(Renderer, Surface);
    SDL_FreeSurface(Surface);

    return 0;
}

void ReloadGameCode(char *DLLName)
{
    FILETIME NewDLLWriteTime = Win32GetLastWriteTime(DLLName);
    if(CompareFileTime(&NewDLLWriteTime, &GameCode.DLLLastWriteTime) != 0)
    {
        UnloadGameCode();
        LoadGameCode(DLLName, "temp.dll");
    }
}

bool GetPlayerInput(GameInput *Input)
{
    bool Running = true;
    
    SDL_Event Event;
    
    while (SDL_PollEvent(&Event) != 0)
    {
        switch (Event.type)
        {
            case SDL_QUIT:   
                Running = false;
                break;
            case SDL_KEYDOWN:
                if(Event.key.keysym.sym == SDLK_w)
                    Input->IndividualButtons.W.Pressed = true;
                if(Event.key.keysym.sym == SDLK_s)
                    Input->IndividualButtons.S.Pressed = true;  
                if(Event.key.keysym.sym == SDLK_a)
                    Input->IndividualButtons.A.Pressed = true;
                if(Event.key.keysym.sym == SDLK_d)
                    Input->IndividualButtons.D.Pressed = true;
                break;
            case SDL_KEYUP:
                if(Event.key.keysym.sym == SDLK_w)
                    Input->IndividualButtons.W.Pressed = false;
                if(Event.key.keysym.sym == SDLK_s)
                    Input->IndividualButtons.S.Pressed = false;  
                if(Event.key.keysym.sym == SDLK_a)
                    Input->IndividualButtons.A.Pressed = false;
                if(Event.key.keysym.sym == SDLK_d)
                    Input->IndividualButtons.D.Pressed = false;
                break;
            default:
                break;
        }
    }

    int MousePosX, MousePosY;

    SDL_GetMouseState(&MousePosX, &MousePosY);
    Input->MousePos = {(float)MousePosX, float(MousePosY)};
    
    return Running;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    SDL_Init( SDL_INIT_EVERYTHING );

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);


    LARGE_INTEGER PrefFrequency;
    QueryPerformanceFrequency(&PrefFrequency);
    LARGE_INTEGER LastTimer;
    QueryPerformanceCounter(&LastTimer);
    LARGE_INTEGER CurrentTimer;

    char *GameCodeDLLName = "geometry_wars.dll";

    GameInput Input;
    GameState State;
    
    int WindowsWidth = 720;
    int WindowsHeight = 480;

    SDL_Window *Window = SDL_CreateWindow("Geometry Wars",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WindowsWidth,
                                          WindowsHeight,
                                          SDL_WINDOW_SHOWN);

    SDL_Renderer *Renderer = SDL_CreateRenderer(Window, -1,
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); 

    SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    bool Running = true;

    LoadResourses(&State, Renderer);    
    
    while(GetPlayerInput(&Input))
    {
        QueryPerformanceCounter(&CurrentTimer);

        Input.TimeDelta = (CurrentTimer.QuadPart - LastTimer.QuadPart) / (float)PrefFrequency.QuadPart;       

        ReloadGameCode(GameCodeDLLName);
                
        GameCode.UpdateAndRender(&Input, &State, Renderer);

        LastTimer = CurrentTimer;
    }
    
    SDL_Quit();
    
    return 0;    
}
