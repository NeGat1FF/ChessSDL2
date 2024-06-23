#include "Utils/AIManager.h"
#include <stack>

std::vector<std::pair<Position, std::vector<Position>>> AIManager::GenerateLegalMoves(Board &board)
{
    std::vector<std::pair<Position, std::vector<Position>>> moves;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (board.GetSquare(x, y)->GetPiece() != nullptr)
            {
                auto square = board.GetSquare(x, y);
                if (square->GetPiece()->GetColor() == board.GetTurnColor())
                {
                    std::vector<Position> possibleMoves = square->GetPiece()->GetMoves(Position(x, y), board);
                    board.FilterMoves(possibleMoves, {x, y}, square->GetPiece()->GetColor());
                    if (possibleMoves.size() > 0)
                    {
                        moves.push_back(std::make_pair(Position{x, y}, possibleMoves));
                    }
                }
            }
        }
    }
    return moves;
}

void AIManager::generatePositions(int depth, int &positions, std::string fen)
{
    struct State {
        int depth;
        std::string fen;
    };

    std::stack<State> stack;
    stack.push({depth, fen});

    while (!stack.empty()) {
        State current = stack.top();
        stack.pop();

        if (current.depth == 0) {
            positions++;
            continue;
        }

        Board board;
        board.LoadFEN(current.fen);

        auto moves = GenerateLegalMoves(board);
        for (auto move : moves) {
            for (auto square : move.second) {
                board.MakeMove(move.first, square);
                stack.push({current.depth - 1, board.GetFEN()});
                board.LoadFEN(current.fen);
            }
        }
    }
}