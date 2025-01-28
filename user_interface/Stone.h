#ifndef GO_DEMO_STONE_H
#define GO_DEMO_STONE_H

#include <QMainWindow>
#include <QPainter>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QThread> 
#include <QMessageBox>
#include "Board.h"
#include "./core/Go.h"

class Stone : public QWidget
{
public:
    const int stoneSize = Board::gridSize * 0.8;
    int m_board_size;
    std::vector<QLabel*> m_stones;
    QLabel* m_stoneMarks = new QLabel(this);

    Stone(QWidget* parent, int board_size) : QWidget(parent), m_board_size(board_size){
        m_stoneMarks->setStyleSheet("QLabel{background:#FF0000;}");
        m_stoneMarks->setGeometry(0, 0, 0, 0);

        for (int i = 0; i < m_board_size * m_board_size; i++)
            m_stones.push_back(new QLabel(this));
    }

    void displayStone(std::array<Go::Color, Go::BOARD_COLOR_NUM>& board) {
        for (int i = 0; i < m_board_size * m_board_size; i++) {
            if (board[i] != 0) {
                int x = i % m_board_size;
                int y = i / m_board_size;
                x = Board::boardMargin + x * Board::gridSize - stoneSize / 2;
                y = Board::boardMargin + y * Board::gridSize - stoneSize / 2;

                m_stones[i]->setGeometry(x, y, stoneSize, stoneSize);

                if (board[i] == Go::BLACK)
                    m_stones[i]->setStyleSheet("QLabel{background:#000000; border-radius: 16px; border:0px solid black;}");
                else
                    m_stones[i]->setStyleSheet("QLabel{background:#FF0000; border-radius: 16px; border:0px solid #FF0000;}");

                m_stones[i]->show();
            }
            else {
                m_stones[i]->hide();
            }
        }
    }

    void displayStoneMark(int p) {
        int x = p % m_board_size;
        int y = p / m_board_size;

        m_stoneMarks->setGeometry(
            Board::boardMargin + x * Board::gridSize - stoneSize / 2,
            Board::boardMargin + y * Board::gridSize - stoneSize / 2,
            4, 4
        );
    }
};
#endif