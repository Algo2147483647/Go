#ifndef GO_DEMO_BOARD_H
#define GO_DEMO_BOARD_H

#include <QtWidgets/QMainWindow>  
#include <QPainter>
#include <QWidget>
#include <QLabel>

class Board : public QWidget
{
public:
    static const short gridSize = 40, boardMargin = 30;
    short m_board_size;
    int windowSize;
    
    std::vector<QLabel*> m_index_labels_x, m_index_labels_y;

    Board(QWidget* parent, int board_size) : QWidget(parent), m_board_size(board_size) {
        windowSize = (m_board_size - 1) * gridSize + boardMargin * 2;

        QFont font("Times New Roman", 15, 50, true);

        for (int i = 0; i < m_board_size; i++) {
            char index = 'A' + (char)i;

            QLabel* index_label_x = new QLabel(this);
            QLabel* index_label_y = new QLabel(this);
            index_label_x->setText(QString(index));
            index_label_y->setText(QString(index));
            index_label_x->setGeometry(boardMargin + gridSize * (i - 0.15), 0, 30, 30);
            index_label_y->setGeometry(10, boardMargin + gridSize * (i - 0.3), 30, 30);
            index_label_x->setFont(font);
            index_label_y->setFont(font);

            m_index_labels_x.push_back(index_label_x);
            m_index_labels_y.push_back(index_label_y);
        }
    }

protected:
    void paintEvent(QPaintEvent* event) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        for (int i = 0; i < m_board_size; i++) {
            painter.drawLine(
                boardMargin + gridSize * i,
                boardMargin,
                boardMargin + gridSize * i,
                windowSize - boardMargin
            );
            painter.drawLine(
                boardMargin,
                boardMargin + gridSize * i,
                windowSize - boardMargin,
                boardMargin + gridSize * i
            );
        }
        painter.setPen(QPen(4)); 
        painter.setBrush(QColor(0, 0, 0));

        int starSize = gridSize / 4,
            star[][2] = {
                {-6,-6}, {-6, 0}, {-6, 6},
                { 0,-6}, { 0, 0}, { 0, 6},
                { 6,-6}, { 6, 0}, { 6, 6},
            };

        for (int i = 0; i < 9; i++) {
            painter.drawEllipse(
                boardMargin + gridSize * (star[i][0] + m_board_size / 2) - starSize / 2,
                boardMargin + gridSize * (star[i][1] + m_board_size / 2) - starSize / 2,
                starSize, starSize
            );
        }
    }
};


#endif
