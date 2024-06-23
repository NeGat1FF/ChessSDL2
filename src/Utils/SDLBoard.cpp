#include "Utils/SDLBoard.h"
#include "Utils/AudioManager.h"

SDLBoard::SDLBoard(std::shared_ptr<Board> board, SDL_Color WhiteSquareColor, SDL_Color BlackSquareColor, SDL_Color SelectedSquareColor, unsigned int squareSize) : _board(board), _squareSize(squareSize), _whiteSquareColor(WhiteSquareColor), _blackSquareColor(BlackSquareColor), _selectedSquareColor(SelectedSquareColor)
{
    auto squares = _board->GetBoard();

    bool isWhite = false;
    for (int i = 0; i < squares.size(); i++)
    {
        std::vector<std::shared_ptr<SDLSquare>> row;
        for (int j = 0; j < squares[i].size(); j++)
        {
            row.push_back(std::make_shared<SDLSquare>(squares[i][j], i, j, _squareSize, isWhite));
            isWhite = !isWhite;
        }
        _squares.push_back(row);
        isWhite = !isWhite;
    }
}

void SDLBoard::Draw(SDL_Renderer *renderer)
{
    for(auto row : _squares)
    {
        for(auto square : row)
        {
            square->Draw(renderer, _whiteSquareColor, _blackSquareColor, _selectedSquareColor);
        }
    }
}

void SDLBoard::Resize(unsigned int squareSize)
{
    _squareSize = squareSize;

    for (auto row : _squares)
    {
        for (auto square : row)
        {
            square->Resize(_squareSize);
        }
    }
}

void SDLBoard::Click(int x, int y)
{
    if (_board->GetPlayerColor() != _board->GetTurnColor())
        return;

    // Position position = (_board->GetPlayerColor() == Color::White)
    //                     ? Position(x / _squareSize, 7 - (y / _squareSize))
    //                     : Position(7 - (x / _squareSize), y / _squareSize);

    Position position = Position(x / _squareSize, 7 - (y / _squareSize));

    auto square = GetSquare(position);

    if (_selectedSquare)
    {
        if (square == _selectedSquare)
        {
            DeselectAll();
        }
        else if (square->IsSelected())
        {
            if (square->GetPiece())
            {
                AudioManager::Instance().PlaySound("capture");
            }
            else
            {
                AudioManager::Instance().PlaySound("move-self");
            }

            _board->MakeMove(_selectedSquare->GetSquare(), square->GetSquare());

            // Change Player color
            _board->SetPlayerColor((_board->GetPlayerColor() == Color::White) ? Color::Black : Color::White);

            DeselectAll();
        }
    }
    else if (square->GetPiece() != nullptr && square->GetPiece()->GetColor() == _board->GetTurnColor())
    {
        SelectPiece(square);
    }
}

std::shared_ptr<SDLSquare> SDLBoard::GetSquare(const Position &position)
{
    return _squares[position.x][position.y];
}

void SDLBoard::SelectPiece(std::shared_ptr<SDLSquare> square)
{
    square->SetSelected(true);
    _selectedSquare = square;

    auto moves = _board->GetSquare(square->GetPosition())->GetPiece()->GetMoves(square->GetPosition(), *(_board.get()));

    if (square->GetPiece()->GetType() != Type::King)
    {
        _board->FilterMoves(moves, square->GetPosition(), square->GetPiece()->GetColor());
    }

    for (auto move : moves)
    {
        GetSquare(move)->SetSelected(true);
    }
}

void SDLBoard::DeselectAll()
{
    for (auto row : _squares)
    {
        for (auto square : row)
        {
            square->SetSelected(false);
        }
    }

    _selectedSquare = nullptr;
}

void SDLBoard::LoadFEN(std::string fen)
{
    _board->LoadFEN(fen);

    _board->SetPlayerColor(_board->GetTurnColor());
}
