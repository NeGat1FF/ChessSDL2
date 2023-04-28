#pragma once

#include <vector>
#include <memory>

#include "SDL2/SDL_render.h"

#include "Square.h"

#include "Pieces/Piece.h"
#include "Pieces/Pawn.h"
#include "Pieces/Rook.h"
#include "Pieces/Knight.h"
#include "Pieces/Bishop.h"
#include "Pieces/Queen.h"
#include "Pieces/King.h"


class Board
{
public:
    Board();
    ~Board();

    void Draw(SDL_Renderer *renderer);
    void InitPieces();

    bool IsValidCoordinate(int x, int y) const;

    std::shared_ptr<Square> GetSquare(int x, int y);
    void Click(int x, int y);

private:
    void UnselectAll();
    std::vector<std::vector<std::shared_ptr<Square>>> _board;
};
