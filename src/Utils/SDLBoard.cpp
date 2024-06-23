#include "Utils/SDLBoard.h"
#include "Utils/AudioManager.h"
#include "Utils/NetworkManager.h"

SDLBoard::SDLBoard(SDL_Color WhiteSquareColor, SDL_Color BlackSquareColor, SDL_Color SelectedSquareColor, unsigned int squareSize) : _squareSize(squareSize), _whiteSquareColor(WhiteSquareColor), _blackSquareColor(BlackSquareColor), _selectedSquareColor(SelectedSquareColor)
{
    auto squares = _board.GetBoard();

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
    SDL_Texture *boardTexture;
    if(_board.GetPlayerColor() == Color::Black)
    {
        boardTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 8 * _squareSize, 8 * _squareSize);
        SDL_SetRenderTarget(renderer, boardTexture);
    }


    for(auto row : _squares)
    {
        for(auto square : row)
        {
            square->Draw(renderer, _whiteSquareColor, _blackSquareColor, _selectedSquareColor, _board.GetPlayerColor() == Color::White);
        }
    }

    if(_board.GetPlayerColor() == Color::Black){
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopyEx(renderer, boardTexture, NULL, NULL, 180, NULL, SDL_FLIP_NONE);
        SDL_DestroyTexture(boardTexture);
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

std::optional<std::pair<Position, Position>> SDLBoard::Click(int x, int y)
{
    if (_board.GetPlayerColor() != _board.GetTurnColor())
        return {};

    Position position = (_board.GetPlayerColor() == Color::White)
                        ? Position(x / _squareSize, 7 - (y / _squareSize))
                        : Position(7 - (x / _squareSize), y / _squareSize);

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

            _board.MakeMove(_selectedSquare->GetSquare(), square->GetSquare());

            // Change Player color
            _board.SetPlayerColor((_board.GetPlayerColor() == Color::White) ? Color::Black : Color::White);

            DeselectAll();

            return std::make_pair(_selectedSquare->GetPosition(), square->GetPosition());
        }
    }
    else if (square->GetPiece() != nullptr && square->GetPiece()->GetColor() == _board.GetTurnColor())
    {
        SelectPiece(square);
    }

    return {};
}

void SDLBoard::SelectPiece(std::shared_ptr<SDLSquare> square)
{
    square->SetSelected(true);
    _selectedSquare = square;

    auto moves = _board.GetSquare(square->GetPosition())->GetPiece()->GetMoves(square->GetPosition(), _board);

    if (square->GetPiece()->GetType() != Type::King)
    {
        _board.FilterMoves(moves, square->GetPosition(), square->GetPiece()->GetColor());
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
    _board.LoadFEN(fen);

    _board.SetPlayerColor(_board.GetTurnColor());

    DeselectAll();
}

bool SDLBoard::IsPlayerTurn()
{
    return _board.GetPlayerColor() == _board.GetTurnColor();
}
