#ifndef GO_DEMO_ANALYSIS_H
#define GO_DEMO_ANALYSIS_H

#include <QMainWindow>
#include <QPainter>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <thread> 
#include <QMessageBox>
#include "Board.h"
#include "./core/Go.h"
#include "./ai/Go_AI_Multithread.h"

class Analysis : public QWidget
{
public:
    QWidget* w;
    array<QLabel*, BOARD_COLOR_NUM> m_labels;
    array<string, BOARD_COLOR_NUM> m_labels_string;

    bool is_ai_open = false;
    std::thread myThread;

    Analysis(QWidget* parent) : QWidget(parent) {
        w = parent;

        QFont font("Times New Roman", 15, 50);
        for (int i = 0; i < BOARD_COLOR_NUM; i++) {
            m_labels[i] = new QLabel(w);
            m_labels[i]->setFont(font);
            m_labels[i]->setStyleSheet("color:White");
            m_labels[i]->setAlignment(Qt::AlignCenter);
        }
    }

    void displayAnalysis();
    void displaySerialNumber(vector<Go::State>& ss);
    void displayQi(Go::State& s);
    void displayMark(Go::State& s);

    void openAI(Go::State& s);
    void aiEvaluate(Go::State& s);
};

#endif
