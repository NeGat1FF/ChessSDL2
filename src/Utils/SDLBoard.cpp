#include "Utils/SDLBoard.h"
#include "Utils/TextureManager.h"

SDLBoard::SDLBoard(Board board, int squareSize, SDL_Renderer *renderer, SDL_Color whiteSqrColor, SDL_Color blackSqrColor, SDL_Color selSqrColor) : m_board(board), m_squareSize(squareSize), m_whiteSqrColor(whiteSqrColor), m_blackSqrColor(blackSqrColor), m_selSqrColor(selSqrColor), _renderer(renderer)
{
    // Initialize the board
}

void SDLBoard::Draw()
{
    // Draw squares
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            SDL_Rect rect = {i * m_squareSize, j * m_squareSize, m_squareSize, m_squareSize};
            if (IsSquareSelected(i, j))
            {
                SDL_SetRenderDrawColor(_renderer, m_selSqrColor.r, m_selSqrColor.g, m_selSqrColor.b, m_selSqrColor.a);
                SDL_RenderFillRect(_renderer, &rect);

                SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(_renderer, &rect);
            }
            else
            {
                SDL_Color color = (i + j) % 2 == 0 ? m_whiteSqrColor : m_blackSqrColor;
                SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(_renderer, &rect);
            }

            int boardPosition = (7 - j) * 8 + i;

            // Draw pieces
            if (m_board.getBit(m_board.GetWhitePawns(), boardPosition))
            {
                TextureManager::Instance().Draw("plt", i * m_squareSize, j * m_squareSize, m_squareSize, m_squareSize, _renderer);
            }
            if (m_board.getBit(m_board.GetWhiteKnights(), boardPosition))
            {
                TextureManager::Instance().Draw("nlt", i * m_squareSize, j * m_squareSize, m_squareSize, m_squareSize, _renderer);
            }
            if (m_board.getBit(m_board.GetWhiteBishops(), boardPosition))
            {
                TextureManager::Instance().Draw("blt", i * m_squareSize, j * m_squareSize, m_squareSize, m_squareSize, _renderer);
            }
            if (m_board.getBit(m_board.GetWhiteRooks(), boardPosition))
            {
                TextureManager::Instance().Draw("rlt", i * m_squareSize, j * m_squareSize, m_squareSize, m_squareSize, _renderer);
            }
            if (m_board.getBit(m_board.GetWhiteQueens(), boardPosition))
            {
                TextureManager::Instance().Draw("qlt", i * m_squareSize, j * m_squareSize, m_squareSize, m_squareSize, _renderer);
            }
            if (m_board.getBit(m_board.GetWhiteKing(), boardPosition))
            {
                TextureManager::Instance().Draw("klt", i * m_squareSize, j * m_squareSize, m_squareSize, m_squareSize, _renderer);
            }

            if (m_board.getBit(m_board.GetBlackPawns(), boardPosition))
            {
                TextureManager::Instance().Draw("pdt", i * m_squareSize, j * m_squareSize, m_squareSize, m_squareSize, _renderer);
            }
            if (m_board.getBit(m_board.GetBlackKnights(), boardPosition))
            {
                TextureManager::Instance().Draw("ndt", i * m_squareSize, j * m_squareSize, m_squareSize, m_squareSize, _renderer);
            }
            if (m_board.getBit(m_board.GetBlackBishops(), boardPosition))
            {
                TextureManager::Instance().Draw("bdt", i * m_squareSize, j * m_squareSize, m_squareSize, m_squareSize, _renderer);
            }
            if (m_board.getBit(m_board.GetBlackRooks(), boardPosition))
            {
                TextureManager::Instance().Draw("rdt", i * m_squareSize, j * m_squareSize, m_squareSize, m_squareSize, _renderer);
            }
            if (m_board.getBit(m_board.GetBlackQueens(), boardPosition))
            {
                TextureManager::Instance().Draw("qdt", i * m_squareSize, j * m_squareSize, m_squareSize, m_squareSize, _renderer);
            }
            if (m_board.getBit(m_board.GetBlackKing(), boardPosition))
            {
                TextureManager::Instance().Draw("kdt", i * m_squareSize, j * m_squareSize, m_squareSize, m_squareSize, _renderer);
            }
        }
    }

    SDL_RenderPresent(_renderer);
}

void SDLBoard::Click(int x, int y)
{
    IsSquareSelected(x / m_squareSize, y / m_squareSize) ? DeselectSquare(x / m_squareSize, y / m_squareSize) : SelectSquare(x / m_squareSize, y / m_squareSize);
}

inline void SDLBoard::SelectSquare(int x, int y)
{
    m_selectedSquares |= 1ULL << (x + y * 8);
}

inline void SDLBoard::DeselectSquare(int x, int y)
{
    m_selectedSquares &= ~(1ULL << (x + y * 8));
}

inline void SDLBoard::DeselectAllSquares()
{
    m_selectedSquares = 0;
}

inline bool SDLBoard::IsSquareSelected(int x, int y)
{
    return m_selectedSquares & (1ULL << (x + y * 8));
}