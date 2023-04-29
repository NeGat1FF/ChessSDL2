#include "Board/Board.h"

Board::Board(): _isWhiteTurn(true), _halfMoveClock(0), _fullMoveNumber(1)
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

    _canWhiteCastleKingside = true;
    _canWhiteCastleQueenside = true;

    _canBlackCastleKingside = true;
    _canBlackCastleQueenside = true;
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

const Move &Board::GetLastMove() const
{
    return _lastMove;
}

std::string Board::GetFEN() const{
    std::string fen = "";
    for(int i = 0; i < 8; i++){
        int empty = 0;
        for(int j = 0; j < 8; j++){
            auto piece = _board[i][j]->GetPiece();
            if(piece){
                if(empty > 0){
                    fen += std::to_string(empty);
                    empty = 0;
                }
                fen += piece->GetFEN();
            }else{
                empty++;
            }
        }
        if(empty > 0){
            fen += std::to_string(empty);
        }
        if(i < 7){
            fen += "/";
        }
    }
    fen += " ";
    fen += _isWhiteTurn ? "w" : "b";
    fen += " ";

    // Add castling availability
    std::string castling = "";
    if (_canWhiteCastleKingside) castling += "K";
    if (_canWhiteCastleQueenside) castling += "Q";
    if (_canBlackCastleKingside) castling += "k";
    if (_canBlackCastleQueenside) castling += "q";

    if (castling.empty()) {
        fen += "-";
    } else {
        fen += castling;
    }

    fen += " ";
    fen += _enPassantSquare ? _enPassantSquare->GetPosition().ToString() : "-";
    fen += " ";
    fen += std::to_string(_halfMoveClock);
    fen += " ";
    fen += std::to_string(_fullMoveNumber);
    return fen;
}

void Board::Click(int x, int y)
{
    Position position(x / SQUARE_SIZE, y / SQUARE_SIZE);
    auto square = this->_board[position.y][position.x];

    if (square->IsSelected())
    {
        if (_selectedSquare && _selectedSquare != square)
        {
            MovePiece(_selectedSquare, square);
        }
        UnselectAll();
        return;
    }

    if (square->GetPiece())
    {
        auto piece = square->GetPiece();
        if(piece->GetColor() == Color::White && _isWhiteTurn || piece->GetColor() == Color::Black && !_isWhiteTurn)
        {
            SelectPiece(square);
        }
    }
}

void Board::SelectPiece(const std::shared_ptr<Square> &square)
{
    square->SetSelected(true);
    _selectedSquare = square;

    auto moves = square->GetPiece()->GetMoves(square->GetPosition(), *this);
    for (auto move : moves)
    {
        move->SetSelected(true);
    }
}

void Board::MovePiece(const std::shared_ptr<Square> &fromSquare, const std::shared_ptr<Square> &toSquare)
{
    std::shared_ptr<Piece> piece = fromSquare->GetPiece();
    Color color = piece->GetColor();
    
    // Update castling availability
    if (piece->GetType() == Type::King) {
        if (color == Color::White) {
            _canWhiteCastleKingside = false;
            _canWhiteCastleQueenside = false;
        } else {
            _canBlackCastleKingside = false;
            _canBlackCastleQueenside = false;
        }
    } else if (piece->GetType() == Type::Rook) {
        if (color == Color::White) {
            if (fromSquare->GetPosition() == Position("h1")) {
                _canWhiteCastleKingside = false;
            } else if (fromSquare->GetPosition() == Position("a1")) {
                _canWhiteCastleQueenside = false;
            }
        } else {
            if (fromSquare->GetPosition() == Position("h8")) {
                _canBlackCastleKingside = false;
            } else if (fromSquare->GetPosition() == Position("a8")) {
                _canBlackCastleQueenside = false;
            }
        }
    }


    _halfMoveClock++;

    if(piece->GetType() == Type::Pawn || toSquare->GetPiece() != nullptr){
        _halfMoveClock = 0;
    }

    toSquare->SetPiece(piece);
    piece->Move();

    fromSquare->SetPiece(nullptr);

    _lastMove._from = fromSquare->GetPosition();
    _lastMove._to = toSquare->GetPosition();
    _lastMove._piece = piece;

    // Handle promotion
    if(piece->GetType() == Type::Pawn && (toSquare->GetPosition().y == 0 || toSquare->GetPosition().y == 7)){
        toSquare->SetPiece(std::make_shared<Queen>(piece->GetColor()));
    }

    // Handle castling move
    if (piece->GetType() == Type::King && std::abs(toSquare->GetPosition().x - fromSquare->GetPosition().x) == 2)
    {
        std::shared_ptr<Square> rookFromSquare;
        std::shared_ptr<Square> rookToSquare;

        // King-side castling
        if (toSquare->GetPosition().x > fromSquare->GetPosition().x)
        {
            rookFromSquare = GetSquare(toSquare->GetPosition().x + 1, toSquare->GetPosition().y);
            rookToSquare = GetSquare(toSquare->GetPosition().x - 1, toSquare->GetPosition().y);
        }
        // Queen-side castling
        else
        {
            rookFromSquare = GetSquare(toSquare->GetPosition().x - 2, toSquare->GetPosition().y);
            rookToSquare = GetSquare(toSquare->GetPosition().x + 1, toSquare->GetPosition().y);
        }

        rookToSquare->SetPiece(rookFromSquare->GetPiece());
        rookToSquare->GetPiece()->Move();
        rookFromSquare->SetPiece(nullptr);

    }

    if(_enPassantSquare && toSquare->GetPosition() == _enPassantSquare->GetPosition()){
        std::shared_ptr<Square> pawnSquare = GetSquare(toSquare->GetPosition().x, toSquare->GetPosition().y + (piece->GetColor() == Color::White ? 1 : -1));
        pawnSquare->SetPiece(nullptr);
    }

    _enPassantSquare = nullptr;

    // Handle en passant move
    if (piece->GetType() == Type::Pawn && std::abs(toSquare->GetPosition().y - fromSquare->GetPosition().y) == 2)
    {
        _enPassantSquare = GetSquare(toSquare->GetPosition().x, toSquare->GetPosition().y + (piece->GetColor() == Color::White ? 1 : -1));
    }

    if(_isWhiteTurn){
        _isWhiteTurn = false;
    } else {
        _isWhiteTurn = true;
        _fullMoveNumber++;
    }
}

std::shared_ptr<Square> Board::GetEnPassantSquare() const
{
    return _enPassantSquare;
}

bool Board::IsTarget(const Position &pos, Color color)
{
    std::vector<std::shared_ptr<Square>> moves;
    for (auto row : this->_board)
    {
        for (auto square : row)
        {
            if (square->GetPiece() && square->GetPiece()->GetColor() != color)
            {
                auto piece = square->GetPiece();
                if (piece->GetType() == Type::King)
                {
                    auto king = std::dynamic_pointer_cast<King>(piece);
                    moves = king->GetMovesWithoutChecks(square->GetPosition(), *this);
                }
                else
                {
                    moves = piece->GetMoves(square->GetPosition(), *this);
                }
                for (auto move : moves)
                {
                    if (move->GetPosition() == pos)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
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