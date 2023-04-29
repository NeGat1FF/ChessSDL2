#include "Pieces/King.h"

#include "Board/Board.h"

King::King(Color color) : Piece(Type::King, color) {}

std::vector<std::shared_ptr<Square>> King::GetMoves(Position pos, Board &board) const {
    std::vector<std::shared_ptr<Square>> moves;
    int dx[] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};

    for (int i = 0; i < 8; ++i) {
        int newX = pos.x + dx[i];
        int newY = pos.y + dy[i];

        if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
            auto nextSquare = board.GetSquare(newX, newY);
            if (nextSquare->GetPiece()) {
                if (nextSquare->GetPiece()->GetColor() != GetColor()) {
                    moves.push_back(nextSquare);
                }
            } else {
                moves.push_back(nextSquare);
            }
        }
    }

    // TODO: Add castling logic if needed

    return moves;
}
