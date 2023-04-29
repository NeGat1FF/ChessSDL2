#include "Board/Board.h"

Board::Board()
{
    bool isWhite = true;
    for (int i = 0; i < 8; i++)
    {
        std::vector<std::shared_ptr<Square>> row;
        for (int j = 0; j < 8; j++)
        {
            row.push_back(std::make_shared<Square>(j, i, isWhite));
            isWhite = !isWhite;
        }
        this->_board.push_back(row);
        isWhite = !isWhite;
    }
}

void Board::InitPieces()
{
    for (int i = 0; i < 8; i++)
    {
        this->_board[1][i]->SetPiece(std::make_shared<Pawn>(Color::Black));
        this->_board[6][i]->SetPiece(std::make_shared<Pawn>(Color::White));
    }

    this->_board[0][0]->SetPiece(std::make_shared<Rook>(Color::Black));
    this->_board[0][1]->SetPiece(std::make_shared<Knight>(Color::Black));
    this->_board[0][2]->SetPiece(std::make_shared<Bishop>(Color::Black));
    this->_board[0][3]->SetPiece(std::make_shared<Queen>(Color::Black));
    this->_board[0][4]->SetPiece(std::make_shared<King>(Color::Black));
    this->_board[0][5]->SetPiece(std::make_shared<Bishop>(Color::Black));
    this->_board[0][6]->SetPiece(std::make_shared<Knight>(Color::Black));
    this->_board[0][7]->SetPiece(std::make_shared<Rook>(Color::Black));

    this->_board[7][0]->SetPiece(std::make_shared<Rook>(Color::White));
    this->_board[7][1]->SetPiece(std::make_shared<Knight>(Color::White));
    this->_board[7][2]->SetPiece(std::make_shared<Bishop>(Color::White));
    this->_board[7][3]->SetPiece(std::make_shared<Queen>(Color::White));
    this->_board[7][4]->SetPiece(std::make_shared<King>(Color::White));
    this->_board[7][5]->SetPiece(std::make_shared<Bishop>(Color::White));
    this->_board[7][6]->SetPiece(std::make_shared<Knight>(Color::White));
    this->_board[7][7]->SetPiece(std::make_shared<Rook>(Color::White));
}

const Move& Board::GetLastMove() const
{
    return _lastMove;
}

void Board::Click(int x, int y)
{
    Position position(x / SQUARE_SIZE, y / SQUARE_SIZE);
    auto square = this->_board[position.y][position.x];

    if(square->IsSelected()) {
        if (_selectedSquare && _selectedSquare != square) {
            MovePiece(_selectedSquare, square);
        }
        UnselectAll();
        return;
    }

    if (square->GetPiece()) {
        SelectPiece(square);
    }
}

void Board::SelectPiece(const std::shared_ptr<Square>& square)
{
    square->SetSelected(true);
    _selectedSquare = square;

    auto moves = square->GetPiece()->GetMoves(square->GetPosition().x, square->GetPosition().y, *this);
    for (auto move : moves) {
        move->SetSelected(true);
    }
}

void Board::MovePiece(const std::shared_ptr<Square>& fromSquare, const std::shared_ptr<Square>& toSquare)
{
    toSquare->SetPiece(fromSquare->GetPiece());
    toSquare->GetPiece()->Move();

    fromSquare->SetPiece(nullptr);

    _lastMove._from = fromSquare->GetPosition();
    _lastMove._to = toSquare->GetPosition();
    _lastMove._piece = toSquare->GetPiece();
}


void Board::UnselectAll()
{
    for (auto row : this->_board)
    {
        for (auto square : row)
        {
            square->SetSelected(false);
        }
    }

    _selectedSquare = nullptr;
}

bool Board::IsValidCoordinate(int x, int y) const
{
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

std::shared_ptr<Square> Board::GetSquare(int x, int y)
{
    if (x < 0 || x > 7 || y < 0 || y > 7)
    {
        return nullptr;
    }
    return this->_board[y][x];
}

Board::~Board()
{
}

void Board::Draw(SDL_Renderer *renderer)
{
    for (auto row : this->_board)
    {
        for (auto square : row)
        {
            square->Draw(renderer);
        }
    }
}