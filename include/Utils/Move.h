#pragma once

#include <memory>
#include <utility>
#include "Board/Square.h"
#include "Pieces/Piece.h"

struct Move{
    std::shared_ptr<Piece> _piece;
    Position _from;
    Position _to;

    Move() = default;

    Move(std::shared_ptr<Piece> piece, Position from, Position to) : _piece(std::move(piece)), _from(from), _to(to) {}

    std::string ToString() const {
        return _piece->ToString() + " " + _from.ToString() + " " + _to.ToString();
    }
};