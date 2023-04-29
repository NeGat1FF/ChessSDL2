#pragma once

#include <memory>
#include <utility>
#include "Square.h"
#include "Pieces/Piece.h"

struct Move{
    std::shared_ptr<Piece> _piece;
    Position _from;
    Position _to;
};