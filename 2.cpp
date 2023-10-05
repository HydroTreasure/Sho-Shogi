#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_sdlrenderer.h"

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int angle = 0, int h = 35, int w = 40) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopyEx(ren, tex, nullptr, &dst, angle, NULL, SDL_FLIP_NONE);
}

SDL_Texture *renderText(SDL_Renderer *renderer, const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize) {
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, message.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    return texture;
}

SDL_Texture *renderTextNoGlyph(SDL_Renderer *renderer, const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize) {
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    
    SDL_Surface *surface = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    return texture;
}

enum Piece: unsigned char {
    _0_,
    S_K,
    SDE,
    S_G,
    S_S,
    S_H,
    S_L,
    S_R,
    S_B,
    S_P,
    SCP,
    SPS,
    SPH,
    SPL,
    SDK,
    SDH,
    SPP,
    G_K,
    GDE,
    G_G,
    G_S,
    G_H,
    G_L,
    G_R,
    G_B,
    G_P,
    GCP,
    GPS,
    GPH,
    GPL,
    GDK,
    GDH,
    GPP
};

Piece board[9][9] = {
    {G_L,G_H,G_S,G_G,G_K,G_G,G_S,G_H,G_L},
    {_0_,G_R,_0_,_0_,GDE,_0_,_0_,G_B,_0_},
    {G_P,G_P,G_P,G_P,G_P,G_P,G_P,G_P,G_P},
    {_0_,_0_,_0_,_0_,_0_,_0_,_0_,_0_,_0_},
    {_0_,_0_,_0_,_0_,_0_,_0_,_0_,_0_,_0_},
    {_0_,_0_,_0_,_0_,_0_,_0_,_0_,_0_,_0_},
    {S_P,S_P,S_P,S_P,S_P,S_P,S_P,S_P,S_P},
    {_0_,S_B,_0_,_0_,SDE,_0_,_0_,S_R,_0_},
    {S_L,S_H,S_S,S_G,S_K,S_G,S_S,S_H,S_L}
};

struct SelectedPiece {
    Piece piece;
    int _x_;
    int _y_;
};

void drawBoard(SDL_Renderer *renderer) {
    for (int i = 0; i <= 8; i++) {
        for (int j = 0; j <= 8; j++) {
            SDL_Rect rect = {x: j * 70, y: i * 70, w: 70, h: 70};

            SDL_SetRenderDrawColor(renderer, 242, 170, 107, 255);
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &rect);
            
            SDL_Texture *piece;
            
            switch (board[i][j]) {
                case S_K:
                    piece = renderText(renderer, "王", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 0, 40);
                    piece = renderText(renderer, "将", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 40, 0, 30);
                    break;
                case S_G:
                    piece = renderText(renderer, "金", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "将", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 50);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case S_S:
                    piece = renderText(renderer, "銀", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "将", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 50);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case S_H:
                    piece = renderText(renderer, "桂", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "馬", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case S_L:
                    piece = renderText(renderer, "香", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 0, 40);
                    piece = renderText(renderer, "車", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 40, 0, 30);
                    break;
                case S_B:
                    piece = renderText(renderer, "角", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "行", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case S_R:
                    piece = renderText(renderer, "飛", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "車", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 45);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case SDE:
                    piece = renderText(renderer, "酔", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 60);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "象", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 50);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case S_P:
                    piece = renderText(renderer, "歩", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "兵", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case SPS:
                    piece = renderText(renderer, "成", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "銀", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case SPH:
                    piece = renderText(renderer, "成", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "桂", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case SPL:
                    piece = renderText(renderer, "成", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "香", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case SDH:
                    piece = renderText(renderer, "龍", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 45);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "馬", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case SDK:
                    piece = renderText(renderer, "龍", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 45);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "王", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case SCP:
                    piece = renderText(renderer, "太", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "子", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case SPP:
                    piece = renderText(renderer, "と", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70);
                    piece = renderText(renderer, "金", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35);
                    break;
                case G_K:
                    piece = renderText(renderer, "玉", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 30, 180, 40);
                    piece = renderText(renderer, "将", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180, 30);
                    break;
                case G_G:
                    piece = renderText(renderer, "金", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "将", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 50);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
                case G_S:
                    piece = renderText(renderer, "銀", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "将", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 50);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
                case G_H:
                    piece = renderText(renderer, "桂", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "馬", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
                case G_L:
                    piece = renderText(renderer, "香", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 30, 180, 40);
                    piece = renderText(renderer, "車", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180, 30);
                    break;
                case G_B:
                    piece = renderText(renderer, "角", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "行", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
                case G_R:
                    piece = renderText(renderer, "飛", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "車", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 45);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
                case GDE:
                    piece = renderText(renderer, "酔", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 60);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "象", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 50);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
                case G_P:
                    piece = renderText(renderer, "歩", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "兵", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){0, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
                case GPS:
                    piece = renderText(renderer, "成", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "銀", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
                case GPH:
                    piece = renderText(renderer, "成", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "桂", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
                case GPL:
                    piece = renderText(renderer, "成", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "香", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
                case GDH:
                    piece = renderText(renderer, "龍", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 45);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "馬", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
                case GDK:
                    piece = renderText(renderer, "龍", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 45);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "王", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
                case GCP:
                    piece = renderText(renderer, "太", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "子", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
                case GPP:
                    piece = renderText(renderer, "と", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 75);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70 + 35, 180);
                    piece = renderText(renderer, "金", "./res/NotoSerifCJKjp-Bold.ttf", (SDL_Color){200, 0, 0}, 55);
                    renderTexture(piece, renderer, j * 70 + 15, i * 70, 180);
                    break;
            }
        }
    }
}

bool isLegal(SelectedPiece start, SelectedPiece end, bool& isSentePlaying) {
    bool legal = false;
    switch(start.piece) {
        case SCP:
        case S_K:
            if ((abs(end._x_ - start._x_) == 1) && (end._y_ == start._y_)) legal = true;
            if (hypot(end._y_ - start._y_, end._x_ - start._x_) == pow(2, 0.5)) legal = true;
            if ((abs(end._y_ - start._y_) == 1) && (end._x_ == start._x_)) legal = true;
            break;
        case S_L:
            if ((end._x_ - start._x_ == 0) && (end._y_ - start._y_ < 0)) {
                for (int i = start._y_ - 1; i >= end._y_; i--) {
                    if ((board[i][end._x_] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            break;
        case SDH:
        case GDH:
            if ((abs(end._x_ - start._x_) == 1) && (end._y_ == start._y_)) legal = true;
            if (hypot(end._y_ - start._y_, end._x_ - start._x_) == pow(2, 0.5)) legal = true;
            if ((abs(end._y_ - start._y_) == 1) && (end._x_ == start._x_)) legal = true;
            if (hypot(end._y_ - start._y_, end._x_ - start._x_) == pow(2, 0.5)) {
                legal = true;
                break;
            }
            end._x_ -= (end._x_ - start._x_ > 0) ? 1 : -1;
            end._y_ -= (end._y_ - start._y_ > 0) ? 1 : -1;
            if (-(end._y_ - start._y_) == end._x_ - start._x_) {
                for (int i = 1; i <= start._y_ - end._y_; i++) {
                    if ((board[start._y_ - i][start._x_ + i] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            if (end._y_ - start._y_ == -(end._x_ - start._x_)) {
                for (int i = 1; i <= end._y_ - start._y_; i++) {
                    if ((board[start._y_ + i][start._x_ - i] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            if (-(end._y_ - start._y_) == -(end._x_ - start._x_)) {
                for (int i = 1; i <= start._y_ - end._y_; i++) {
                    if ((board[start._y_ - i][start._x_ - i] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            if (end._y_ - start._y_ == end._x_ - start._x_) {
                for (int i = 1; i <= end._y_ - start._y_; i++) {
                    if ((board[start._y_ + i][start._x_ + i] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            end._x_ += (end._x_ - start._x_ > 0) ? 1 : -1;
            end._y_ += (end._y_ - start._y_ > 0) ? 1 : -1;
            break;
        case S_B:
        case G_B:
            if (hypot(end._y_ - start._y_, end._x_ - start._x_) == pow(2, 0.5)) {
                legal = true;
                break;
            }
            end._x_ -= (end._x_ - start._x_ > 0) ? 1 : -1;
            end._y_ -= (end._y_ - start._y_ > 0) ? 1 : -1;
            if (-(end._y_ - start._y_) == end._x_ - start._x_) {
                for (int i = 1; i <= start._y_ - end._y_; i++) {
                    if ((board[start._y_ - i][start._x_ + i] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            if (end._y_ - start._y_ == -(end._x_ - start._x_)) {
                for (int i = 1; i <= end._y_ - start._y_; i++) {
                    if ((board[start._y_ + i][start._x_ - i] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            if (-(end._y_ - start._y_) == -(end._x_ - start._x_)) {
                for (int i = 1; i <= start._y_ - end._y_; i++) {
                    if ((board[start._y_ - i][start._x_ - i] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            if (end._y_ - start._y_ == end._x_ - start._x_) {
                for (int i = 1; i <= end._y_ - start._y_; i++) {
                    if ((board[start._y_ + i][start._x_ + i] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            end._x_ += (end._x_ - start._x_ > 0) ? 1 : -1;
            end._y_ += (end._y_ - start._y_ > 0) ? 1 : -1;
            break;
        case SDK:
        case GDK:
            if ((abs(end._x_ - start._x_) == 1) && (end._y_ == start._y_)) legal = true;
            if (hypot(end._y_ - start._y_, end._x_ - start._x_) == pow(2, 0.5)) legal = true;
            if ((abs(end._y_ - start._y_) == 1) && (end._x_ == start._x_)) legal = true;
            if ((end._x_ - start._x_ == 0) && (end._y_ - start._y_ < 0)) {
                for (int i = start._y_ - 1; i >= end._y_; i--) {
                    if ((board[i][end._x_] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            if ((end._x_ - start._x_ == 0) && (end._y_ - start._y_ > 0)) {
                for (int i = start._y_ + 1; i <= end._y_; i++) {
                    if ((board[i][end._x_] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            if ((end._y_ - start._y_ == 0) && (end._x_ - start._x_ < 0)) {
                for (int i = start._x_ - 1; i >= end._x_; i--) {
                    if ((board[end._y_][i] != _0_) && (i != end._x_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            if ((end._y_ - start._y_ == 0) && (end._x_ - start._x_ > 0)) {
                for (int i = start._x_ + 1; i <= end._x_; i++) {
                    if ((board[end._y_][i] != _0_) && (i != end._x_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            break;
        case S_R:
        case G_R:
            if ((end._x_ - start._x_ == 0) && (end._y_ - start._y_ < 0)) {
                for (int i = start._y_ - 1; i >= end._y_; i--) {
                    if ((board[i][end._x_] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            if ((end._x_ - start._x_ == 0) && (end._y_ - start._y_ > 0)) {
                for (int i = start._y_ + 1; i <= end._y_; i++) {
                    if ((board[i][end._x_] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            if ((end._y_ - start._y_ == 0) && (end._x_ - start._x_ < 0)) {
                for (int i = start._x_ - 1; i >= end._x_; i--) {
                    if ((board[end._y_][i] != _0_) && (i != end._x_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            if ((end._y_ - start._y_ == 0) && (end._x_ - start._x_ > 0)) {
                for (int i = start._x_ + 1; i <= end._x_; i++) {
                    if ((board[end._y_][i] != _0_) && (i != end._x_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            break;
        case S_H:
            if ((end._y_ - start._y_ == -2) && (abs(end._x_ - start._x_) == 1)) legal = true;
            break;
        case S_S:
            if (hypot(end._y_ - start._y_, end._x_ - start._x_) == pow(2, 0.5)) legal = true;
            if ((end._y_ - start._y_ == -1) && (end._x_ == start._x_)) legal = true;
            break;
        case S_G:
        case SPS:
        case SPH:
        case SPL:
        case SPP:
            if ((abs(end._x_ - start._x_) == 1) && (end._y_ == start._y_)) legal = true;
            if ((abs(end._y_ - start._y_) == 1) && (end._x_ == start._x_)) legal = true;
            if ((hypot(end._y_ - start._y_, end._x_ - start._x_) == pow(2, 0.5)) && (end._y_ - start._y_ == -1)) legal = true;
            break;
        case SDE:
            if ((abs(end._x_ - start._x_) == 1) && (end._y_ == start._y_)) legal = true;
            if (hypot(end._y_ - start._y_, end._x_ - start._x_) == pow(2, 0.5)) legal = true;
            if ((end._y_ - start._y_ == -1) && (end._x_ == start._x_)) legal = true;
            break;
        case S_P:
            if ((end._y_ - start._y_ == -1) && (end._x_ == start._x_)) legal = true;
            break;
        case GCP:
        case G_K:
            if ((abs(end._x_ - start._x_) == 1) && (end._y_ == start._y_)) legal = true;
            if (hypot(end._y_ - start._y_, end._x_ - start._x_) == pow(2, 0.5)) legal = true;
            if ((abs(end._y_ - start._y_) == 1) && (end._x_ == start._x_)) legal = true;
            break;
        case G_L:
            if ((end._x_ - start._x_ == 0) && (end._y_ - start._y_ > 0)) {
                for (int i = start._y_ + 1; i <= end._y_; i++) {
                    if ((board[i][end._x_] != _0_) && (i != end._y_)) {
                        legal = false;
                        break;
                    }
                    else legal = true;
                }
            }
            break;
        case G_H:
            if ((end._y_ - start._y_ == 2) && (abs(end._x_ - start._x_) == 1)) legal = true;
            break;
        case G_S:
            if (hypot(end._y_ - start._y_, end._x_ - start._x_) == pow(2, 0.5)) legal = true;
            if ((end._y_ - start._y_ == 1) && (end._x_ == start._x_)) legal = true;
            break;
        case G_G:
        case GPS:
        case GPH:
        case GPL:
        case GPP:
            if ((abs(end._x_ - start._x_) == 1) && (end._y_ == start._y_)) legal = true;
            if ((abs(end._y_ - start._y_) == 1) && (end._x_ == start._x_)) legal = true;
            if ((hypot(end._y_ - start._y_, end._x_ - start._x_) == pow(2, 0.5)) && (end._y_ - start._y_ == 1)) legal = true;
            break;
        case GDE:
            if ((abs(end._x_ - start._x_) == 1) && (end._y_ == start._y_)) legal = true;
            if (hypot(end._y_ - start._y_, end._x_ - start._x_) == pow(2, 0.5)) legal = true;
            if ((end._y_ - start._y_ == 1) && (end._x_ == start._x_)) legal = true;
            break;
        case G_P:
            if ((end._y_ - start._y_ == 1) && (end._x_ == start._x_)) legal = true;
            break;
    }
    if (end.piece != _0_) {
        if (end.piece < G_K && start.piece < G_K) legal = false;
        if (end.piece > SPP && start.piece > SPP) legal = false;
    }
    if (isSentePlaying) {
        if (start.piece > SPP) legal = false;
    }
    else {
        if (start.piece < G_K) legal = false;
    }
    isSentePlaying = legal ? !(isSentePlaying) : isSentePlaying;
    return legal;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init ERROR:\n" << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() != 0) {
        std::cerr << "TTF_Init ERROR: \n" << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SHO SHOGI - 小将棋", 100, 100, 630, 630, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) puts(SDL_GetError());
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
    
    bool PRESSEDBTN = 0;
    bool RENDERPROMOTE = 0;
    int PROMOTECOORDS[2];
    Piece arupiece;
    bool SENTE_TURN = 1, SENTE_ROYAL, GOTE_ROYAL;

    SelectedPiece selectedpiece;
    SelectedPiece newpiece;
    selectedpiece.piece = _0_;

#define IF(condition) if (condition) {
#define ENDIF }

    SDL_Window *msgWin;
    while (1) {
        SENTE_ROYAL = 0;
        GOTE_ROYAL = 0;
        for (int j = 0; j < 9; j++) {
            for (int i = 0; i < 9; i++) {
                if ((board[j][i] == S_K) || (board[j][i] == SCP)) {
                    SENTE_ROYAL = 1;
                }
                if ((board[j][i] == G_K) || (board[j][i] == GCP)) {
                    GOTE_ROYAL = 1;
                }
            }
        }

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);
            switch (e.type) {
                case SDL_QUIT:
                    ImGui_ImplSDLRenderer_Shutdown();
                    ImGui_ImplSDL2_Shutdown();
                    ImGui::DestroyContext();
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return 0;
                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        if (e.button.state == SDL_PRESSED) {
                            PRESSEDBTN = 1;
                            int xpos = (e.button.x - (e.button.x % 70))/ 70;
                            int ypos = (e.button.y - (e.button.y % 70))/ 70;
                            selectedpiece.piece = board[ypos][xpos];
                            selectedpiece._x_ = xpos;
                            selectedpiece._y_ = ypos;
                        }
                    }
                case SDL_MOUSEBUTTONUP:
                    if (e.button.button == SDL_BUTTON_LEFT && e.button.state == SDL_RELEASED) {
                        if (PRESSEDBTN) {
                            PRESSEDBTN = 0;
                            if (selectedpiece.piece != _0_) {
                                int xpos = (e.button.x - (e.button.x % 70))/ 70;
                                int ypos = (e.button.y - (e.button.y % 70))/ 70;
                                if (!((selectedpiece._x_ == xpos) && (selectedpiece._y_ == ypos))) {
                                    Piece piece = board[ypos][xpos];
                                    newpiece = {piece: piece, _x_: xpos, _y_: ypos};
                                    if (isLegal(selectedpiece, newpiece, SENTE_TURN)) {
                                        if (newpiece._y_ <= 2) {
                                            switch (selectedpiece.piece) {
                                                case S_S:
                                                case S_H:
                                                case S_L:
                                                case S_R:
                                                case S_B:
                                                case SDE:
                                                case S_P:
                                                    //msgWin = SDL_CreateWindow("Promotion", 200, 200, 100, 50, SDL_WINDOW_SHOWN);
                                                    //SDL_SetWindowBordered(msgWin, SDL_TRUE);
                                                    RENDERPROMOTE = 1;
                                                    PROMOTECOORDS[0] = ypos;
                                                    PROMOTECOORDS[1] = xpos;
                                                    arupiece = selectedpiece.piece;
                                                    break;
                                            }
                                        }
                                        if (newpiece._y_ >= 6) {
                                            switch (selectedpiece.piece) {
                                                case G_S:
                                                case G_H:
                                                case G_L:
                                                case G_R:
                                                case G_B:
                                                case GDE:
                                                case G_P:
                                                    RENDERPROMOTE = 1;
                                                    PROMOTECOORDS[0] = ypos;
                                                    PROMOTECOORDS[1] = xpos;
                                                    arupiece = selectedpiece.piece;
                                                    break;
                                            }
                                        }
                                        board[ypos][xpos] = selectedpiece.piece;
                                        board[selectedpiece._y_][selectedpiece._x_] = _0_;
                                        selectedpiece.piece = _0_;
                                    }
                                }
                            }
                        }
                    }
            }
        }
        
        if ((SENTE_ROYAL == 0) || (GOTE_ROYAL == 0)) {
            SDL_Texture *winlose = renderTextNoGlyph(renderer, "GAME END", "./res/Ubuntu-L.ttf", (SDL_Color){255, 255, 255}, 45);
            renderTexture(winlose, renderer, 75, 75, 0, 35, 170);
            winlose = renderTextNoGlyph(renderer, std::string((GOTE_ROYAL == 0) ? "Sente" : "Gote") + std::string(" Wins"), "./res/Ubuntu-L.ttf", (SDL_Color){255, 255, 255}, 45);
            renderTexture(winlose, renderer, 75, 110, 0, 35, 170);
            winlose = renderText(renderer, std::string((GOTE_ROYAL == 0) ? "先手" : "後手") + std::string("が勝った"), "./res/NotoSansCJKjp-VF.ttf", (SDL_Color){255, 255, 255}, 45);
            renderTexture(winlose, renderer, 75, 145, 0, 35, 170);
            SDL_RenderPresent(renderer);
            continue;
        }

        SDL_RenderClear(renderer);
        
        drawBoard(renderer);

        if (RENDERPROMOTE) {
            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            ImGui::SetNextWindowSize(ImVec2(100, 75));
            ImGui::Begin("Promotion?");
            if (ImGui::Button("Yes", ImVec2(75, 20))) {
                RENDERPROMOTE = false;
                Piece naripiece;
                switch (arupiece) {
                    case S_B:
                        naripiece = SDH;
                        break;
                    case S_R:
                        naripiece = SDK;
                        break;
                    case S_S:
                        naripiece = SPS;
                        break;
                    case S_H:
                        naripiece = SPH;
                        break;
                    case S_L:
                        naripiece = SPL;
                        break;
                    case SDE:
                        naripiece = SCP;
                        break;
                    case S_P:
                        naripiece = SPP;
                        break;
                    case G_B:
                        naripiece = GDH;
                        break;
                    case G_R:
                        naripiece = GDK;
                        break;
                    case G_S:
                        naripiece = GPS;
                        break;
                    case G_H:
                        naripiece = GPH;
                        break;
                    case G_L:
                        naripiece = GPL;
                        break;
                    case GDE:
                        naripiece = GCP;
                        break;
                    case G_P:
                        naripiece = GPP;
                        break;
                    default:
                        naripiece = arupiece;
                }
                board[PROMOTECOORDS[0]][PROMOTECOORDS[1]] = naripiece;
            }
            if (ImGui::Button("No", ImVec2(75, 20))) {
                RENDERPROMOTE = false;
            }
            ImGui::End();
            ImGui::Render();
            SDL_SetRenderDrawColor(renderer, 0.45 * 255, 0.55 * 255, 0.60 * 255, 255);
            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyWindow(window);
    SDL_Quit();
}

#include "imgui/imgui.cpp"
#include "imgui/imgui_impl_sdl.cpp"
#include "imgui/imgui_impl_sdlrenderer.cpp"
#include "imgui/imgui_tables.cpp"
#include "imgui/imgui_widgets.cpp"
#include "imgui/imgui_draw.cpp"
