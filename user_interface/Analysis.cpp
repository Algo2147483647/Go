#include "Analysis.h"

void Analysis::displayAnalysis() {
    static bool display = true;

    if (display) {
        for (int i = 0; i < BOARD_COLOR_NUM; i++) {
            int x = i % BOARD_SIZE,
                y = i / BOARD_SIZE;

            m_labels[i]->setText(QString::fromStdString(m_labels_string[i]));
            m_labels[i]->setGeometry(
                Board::boardMargin + Board::gridSize * (x - 0.36),
                Board::boardMargin + Board::gridSize * (y - 0.35), 30, 30);
            m_labels[i]->show();
        }
    } else {
        for (int i = 0; i < BOARD_COLOR_NUM; i++)
            m_labels[i]->hide();
    }
    display = !display;
}

void Analysis::displaySerialNumber(vector<Go::State>& ss) {
    m_labels_string.fill("");

    int i = 0;
    for (auto& s : ss) {
        if (s.action != PASS && s.action != -2)
            m_labels_string[s.action] = to_string(i);
        i++;
    }
    displayAnalysis();
}

/*
 *  显示气数
 */
void Analysis::displayQi(Go::State& s) {
    m_labels_string.fill("");

    for (int i = 0; i < BOARD_COLOR_NUM; i++) {
        if (s.board[i] != 0) {
            m_labels_string[i] = to_string(s.qi[s.mark[i]]);
        }
    }
    displayAnalysis();
}

/*
 *  显示棋块标记
 */
void Analysis::displayMark(Go::State& s) {
    m_labels_string.fill("");

    for (int i = 0; i < BOARD_COLOR_NUM; i++) {
        if (s.mark[i] >= 0) {
            m_labels_string[i] = to_string(s.mark[i]);
        }
    }
    displayAnalysis();
}

void Analysis::openAI(Go::State& s) {
    if (is_ai_open == false) {
        is_ai_open = true;
        myThread = std::thread(GoAI::MonteCarloTreeSearch, &s);
        myThread.detach();
    }
    return;
}

/*
 *  显示AI评估结果
 */
void Analysis::aiEvaluate(Go::State& s) {
    m_labels_string.fill("");

    for (int i = 0; i < BOARD_COLOR_NUM; i++) {
        if (s.mark[i] == -1) {
            m_labels_string[i] = to_string(GoAI::evaluate_result[i] * 100);
        }
    }

    displayAnalysis();
}