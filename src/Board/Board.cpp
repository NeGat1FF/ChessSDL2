#include "Board/Board.h"

Board::Board(Color playerColor, std::string fen) : _playerColor(playerColor)
{
    bool isWhite = false;
    for (int x = 0; x < 8; ++x)
    {
        std::vector<std::shared_ptr<Square>> row;
        for (int y = 0; y < 8; ++y)
        {
            row.push_back(std::make_shared<Square>(x, y));
        }
        this->_board.push_back(row);
    }

    LoadFEN(fen);
}

std::string Board::GetFEN() const
{
    std::string fen = "";
    for (int i = 0; i < 8; i++)
    {
        int empty = 0;
        for (int j = 0; j < 8; j++)
        {
            auto piece = _board[j][7 - i]->GetPiece();
            if (piece)
            {
                if (empty > 0)
                {
                    fen += std::to_string(empty);
                    empty = 0;
                }
                fen += piece->GetFEN();
            }
            else
            {
                empty++;
            }
        }
        if (empty > 0)
        {
            fen += std::to_string(empty);
        }
        if (i < 7)
        {
            fen += "/";
        }
    }
    fen += " ";
    fen += _turnColor == Color::White ? "w" : "b";
    fen += " ";

    // Add castling availability
    std::string castling = "";
    if (_canWhiteCastleKingside)
        castling += "K";
    if (_canWhiteCastleQueenside)
        castling += "Q";
    if (_canBlackCastleKingside)
        castling += "k";
    if (_canBlackCastleQueenside)
        castling += "q";

    if (castling.empty())
    {
        fen += "-";
    }
    else
    {
        fen += castling;
    }

    fen += " ";
    fen += _enPassantSquare ? _enPassantSquare->GetPosition().ToString() : "-";
    // fen += "-";
    fen += " ";
    fen += std::to_string(_halfMoveClock);
    fen += " ";
    fen += std::to_string(_fullMoveNumber);
    return fen;
}

std::vector<std::vector<std::shared_ptr<Square>>> Board::GetBoard() const
{
    return this->_board;
}

std::shared_ptr<Piece> Board::_getPieceFromFEN(char fenChar)
{
    Color color = std::isupper(fenChar) ? Color::White : Color::Black;
    char pieceType = std::tolower(fenChar);

    switch (pieceType)
    {
    case 'p':
        return std::make_shared<Pawn>(color);
    case 'r':
        return std::make_shared<Rook>(color);
    case 'n':
        return std::make_shared<Knight>(color);
    case 'b':
        return std::make_shared<Bishop>(color);
    case 'q':
        return std::make_shared<Queen>(color);
    case 'k':
        return std::make_shared<King>(color);
    default:
        return nullptr;
    }
}

void Board::LoadFEN(const std::string &fen)
{
    // Clear board
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; ++j)
        {
            _board[i][j]->SetPiece(nullptr);
        }
    }

    std::istringstream iss(fen);
    std::vector<std::string> fenComponents;
    std::string component;

    while (std::getline(iss, component, ' '))
    {
        fenComponents.push_back(component);
    }

    std::vector<std::string> ranks;
    std::string rank;
    std::istringstream iss2(fenComponents[0]);
    while (std::getline(iss2, rank, '/'))
    {
        ranks.push_back(rank);
    }

    for (int rows = 0; rows < 8; rows++)
    {
        int columns = 0;
        for (char c : ranks[rows])
        {
            if (std::isdigit(c))
            {
                columns += c - '0';
            }
            else
            {
                auto piece = _getPieceFromFEN(c);
                _board[columns][7 - rows]->SetPiece(piece);
                if (piece && piece->GetType() == Type::King)
                {
                    if (piece->GetColor() == Color::White)
                    {
                        _whiteKingSquare = _board[columns][7 - rows];
                    }
                    else
                    {
                        _blackKingSquare = _board[columns][7 - rows];
                    }
                }
                columns++;
            }
        }
    }

    _turnColor = fenComponents[1] == "w" ? Color::White : Color::Black;

    if (fenComponents[2] != "-")
    {
        for (char c : fenComponents[2])
        {
            switch (c)
            {
            case 'K':
                _canWhiteCastleKingside = true;
                break;
            case 'Q':
                _canWhiteCastleQueenside = true;
                break;
            case 'k':
                _canBlackCastleKingside = true;
                break;
            case 'q':
                _canBlackCastleQueenside = true;
                break;
            default:
                break;
            }
        }
    }

    if (fenComponents[3] != "-")
    {
        Position position(fenComponents[3]);
        _enPassantSquare = GetSquare(position.x, position.y);
    }

    _halfMoveClock = std::stoi(fenComponents[4]);

    _fullMoveNumber = std::stoi(fenComponents[5]);

    UpdateCheckStatus();
}

void Board::MakeMove(Position from, Position to)
{
    MakeMove(_board[from.x][from.y], _board[to.x][to.y]);
}

void Board::MakeMove(std::string from, std::string to)
{
    Position fromPosition(from);
    Position toPosition(to);

    MakeMove(_board[fromPosition.x][fromPosition.y], _board[toPosition.x][toPosition.y]);
}

void Board::MakeMove(const std::shared_ptr<Square> &fromSquare, const std::shared_ptr<Square> &toSquare)
{
    std::shared_ptr<Piece> piece = fromSquare->GetPiece();
    Color color = piece->GetColor();

    // Update castling availability
    if (piece->GetType() == Type::King)
    {
        if (color == Color::White)
        {
            _canWhiteCastleKingside = false;
            _canWhiteCastleQueenside = false;

            _whiteKingSquare = toSquare;
        }
        else
        {
            _canBlackCastleKingside = false;
            _canBlackCastleQueenside = false;

            _blackKingSquare = toSquare;
        }
    }
    else if (piece->GetType() == Type::Rook)
    {
        if (color == Color::White)
        {
            if (fromSquare->GetPosition() == Position("h1"))
            {
                _canWhiteCastleKingside = false;
            }
            else if (fromSquare->GetPosition() == Position("a1"))
            {
                _canWhiteCastleQueenside = false;
            }
        }
        else
        {
            if (fromSquare->GetPosition() == Position("h8"))
            {
                _canBlackCastleKingside = false;
            }
            else if (fromSquare->GetPosition() == Position("a8"))
            {
                _canBlackCastleQueenside = false;
            }
        }
    }

    _halfMoveClock++;

    if (piece->GetType() == Type::Pawn || toSquare->GetPiece() != nullptr)
    {
        _halfMoveClock = 0;
    }

    toSquare->SetPiece(piece);
    piece->Move();

    fromSquare->SetPiece(nullptr);

    _lastMove._from = fromSquare->GetPosition();
    _lastMove._to = toSquare->GetPosition();
    _lastMove._piece = piece;

    // Handle promotion
    if (piece->GetType() == Type::Pawn && (toSquare->GetPosition().y == 0 || toSquare->GetPosition().y == 7))
    {
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

    _enPassantSquare = nullptr;

    // Handle en passant move
    if (piece->GetType() == Type::Pawn)
    {
        if (_enPassantSquare && toSquare->GetPosition() == _enPassantSquare->GetPosition())
        {
            // Find the square of the captured pawn
            std::shared_ptr<Square> pawnSquare = GetSquare(toSquare->GetPosition().x, toSquare->GetPosition().y + (piece->GetColor() == Color::White ? -1 : 1));
            if (pawnSquare && pawnSquare->GetPiece() && pawnSquare->GetPiece()->GetType() == Type::Pawn)
            {
                pawnSquare->SetPiece(nullptr);
            }
        }
        if (std::abs(toSquare->GetPosition().y - fromSquare->GetPosition().y) == 2)
        {
            _enPassantSquare = GetSquare(toSquare->GetPosition().x, toSquare->GetPosition().y + (piece->GetColor() == Color::White ? -1 : 1));
        }
    }

    if (_turnColor == Color::White)
    {
        _turnColor = Color::Black;
    }
    else
    {
        _turnColor = Color::White;
        _fullMoveNumber++;
    }

    UpdateCheckStatus();
}

void Board::UndoMove()
{
    std::shared_ptr<Square> fromSquare = GetSquare(_lastMove._to);
    std::shared_ptr<Square> toSquare = GetSquare(_lastMove._from);

    fromSquare->SetPiece(_lastMove._piece);
}

std::shared_ptr<Square> Board::GetEnPassantSquare() const
{
    return _enPassantSquare;
}

bool Board::IsTarget(const Position &pos, Color color)
{
    std::vector<Position> moves;
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
                else if (piece->GetType() == Type::Pawn)
                {
                    auto pawn = std::dynamic_pointer_cast<Pawn>(piece);
                    moves = pawn->GetAttackSquares(square->GetPosition(), *this);
                }
                else
                {
                    moves = piece->GetMoves(square->GetPosition(), *this);
                }
                for (auto move : moves)
                {
                    if (move == pos)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void Board::FilterMoves(std::vector<Position> &moves, Position square, Color color)
{
    auto piece = GetSquare(square)->GetPiece();
    std::shared_ptr<Square> kingSquare;
    if (color == Color::White)
    {
        kingSquare = _whiteKingSquare;
    }
    else
    {
        kingSquare = _blackKingSquare;
    }
    for (auto it = moves.begin(); it != moves.end();)
    {
        auto move = *it;
        auto piece = GetSquare(move)->GetPiece();
        VirtualMove(square, move, nullptr);
        if (IsTarget(kingSquare->GetPosition(), color))
        {
            it = moves.erase(it);
        }
        else
        {
            it++;
        }
        VirtualMove(move, square, piece);
    }
}

void Board::UpdateCheckStatus()
{
    _isWhiteChecked = IsTarget(_whiteKingSquare->GetPosition(), Color::White);
    _isBlackChecked = IsTarget(_blackKingSquare->GetPosition(), Color::Black);
}

void Board::VirtualMove(Position fromPos, Position toPos, const std::shared_ptr<Piece> &piece)
{
    auto fromSquare = GetSquare(fromPos);
    auto toSquare = GetSquare(toPos);
    
    auto _piece = fromSquare->GetPiece();
    toSquare->SetPiece(_piece);
    fromSquare->SetPiece(piece);
}

std::shared_ptr<Square> Board::GetSquare(int x, int y)
{
    return IsValidCoordinate(x, y) ? this->_board[x][y]: nullptr;
}

std::shared_ptr<Square> Board::GetSquare(const Position &pos)
{
    return GetSquare(pos.x, pos.y);
}