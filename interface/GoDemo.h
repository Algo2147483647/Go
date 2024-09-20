#ifndef GO_DEMO_H
#define GO_DEMO_H

#include <QtWidgets/QMainWindow>  
#include <QPainter>
#include <QWidget>
#include <QLabel>
#include "Board.h"
#include "Stone.h"
#include "Analysis.h"
#include "Controller.h"

class GoDemo : public QMainWindow
{
public:
    short windowSize; 
    Board* board;
    Stone* stone;
    Analysis* analysis;
    Controller* controller;
    QWidget* m_boardWidget;
    QWidget* m_stoneWidget;
    QWidget* m_analysisWidget;
    QWidget* m_controllerWidget;

    GoDemo(QWidget* parent = 0) : QMainWindow(parent) {
        windowSize = Board::gridSize * (BOARD_SIZE + 0.5);
        setMaximumSize(windowSize, windowSize);
        setMinimumSize(windowSize, windowSize);
        setWindowTitle("Go");
        setWindowOpacity(0.8);

        // Board 
        m_boardWidget = new QWidget(this);
        initializeWidget(m_boardWidget, Qt::white);
        board = new Board(m_boardWidget, BOARD_SIZE);
        board->setGeometry(0, 0, windowSize, windowSize);

        // Stone 
        m_stoneWidget = new QWidget(this);
        initializeWidget(m_stoneWidget, Qt::transparent);
        stone = new Stone(m_stoneWidget, BOARD_SIZE);
        stone->setGeometry(0, 0, windowSize, windowSize);

        // Analysis 
        m_analysisWidget = new QWidget(this);
        initializeWidget(m_analysisWidget, Qt::transparent);
        analysis = new Analysis(m_analysisWidget);
        analysis->setGeometry(0, 0, windowSize, windowSize);

        // Controller
        m_controllerWidget = new QWidget(this);
        initializeWidget(m_controllerWidget, Qt::transparent);
        controller = new Controller(m_controllerWidget);
        controller->stone = stone;
        controller->analysis = analysis;
    }
    ~GoDemo() { ; };

    void initializeWidget(QWidget* widget, const QColor& backgroundColor) {
        widget->setGeometry(0, 0, windowSize, windowSize);
        QPalette pal(widget->palette());
        pal.setColor(QPalette::Window, backgroundColor);
        widget->setAutoFillBackground(true);
        widget->setPalette(pal);
        widget->show();
    }
};

#endif