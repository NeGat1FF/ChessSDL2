#include "Utils/AIManager.h"
#include <iostream>

AIManager::AIManager(Board* board) : _board(board)
{
}

void AIManager::MakeMove()
{
}

std::vector<std::pair<std::shared_ptr<Square>, std::vector<std::shared_ptr<Square>>>> AIManager::GenerateLegalMoves(){
    std::vector<std::pair<std::shared_ptr<Square>, std::vector<std::shared_ptr<Square>>>> moves;
    for(int x = 0;x < 8;x++){
        for(int y = 0;y < 8;y++){
            if(_board->GetSquare(x,y)->GetPiece() != nullptr){
                auto square = _board->GetSquare(x,y);
                if(square->GetPiece()->GetColor() == _board->GetTurnColor()){
                    std::vector<std::shared_ptr<Square>> possibleMoves = square->GetPiece()->GetMoves(Position(x,y),*_board);
                    if(possibleMoves.size() > 0){
                        moves.push_back(std::make_pair(square,possibleMoves));
                    }
                }
            }
        }
    }
    return moves;

}

void AIManager::generatePositions(int depth, int &positions) {
    if(depth == 0){
        positions++;
        return;
    }

    std::string fen = _board->GetFEN();

    std::vector<std::pair<std::shared_ptr<Square>, std::vector<std::shared_ptr<Square>>>> moves = GenerateLegalMoves();
    for(auto move : moves){
        for(auto square : move.second){
            _board->MovePiece(move.first,square);
            std::cout << "Moved " << move.first->GetPosition().ToString() << " to " << square->GetPosition().ToString() << std::endl;
            generatePositions(depth - 1,positions);
            _board->LoadFEN(fen);
        }
    }
}