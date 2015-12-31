#if !defined(TEXTURE_H)
#define TEXTURE_H

class LTexture
{
public:
    LTexture();
    ~LTexture();
    bool LoadFromFile(SDL_Renderer *Renderer, char *Path);
    bool LoadFromRenderedText(SDL_Renderer *Renderer, char *TextureText,
                              SDL_Color TextColor, TTF_Font *Font);
    void Free();
    void SetAlpha(Uint8 alpha);
    void Render(SDL_Renderer *Renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, 
        SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
    int GetWidth();
    int GetHeight();


private:
    SDL_Texture* Texture;
    int Width;
    int Height;
};

#endif
