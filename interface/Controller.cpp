#include "Controller.h"

Controller::Controller(QWidget* parent) : QWidget(parent) {
    resize(Board::gridSize * (BOARD_SIZE + 0.5), Board::gridSize * (BOARD_SIZE + 0.5));
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFocusPolicy(Qt::StrongFocus);

    m_winLabel = new QLabel(this);

    Go::go_init();
    state = new Go::State();
    m_state_history.push_back(*state);
}

void Controller::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        signed char x = (event->x() - Board::boardMargin + Board::gridSize / 2) / Board::gridSize;
        signed char y = (event->y() - Board::boardMargin + Board::gridSize / 2) / Board::gridSize;

        Go::play(*state, y * BOARD_SIZE + x);
        m_state_history.push_back(*state);
        stone->displayStone(state->board);
    }
    else if (event->button() == Qt::MiddleButton) {
        if (m_state_history.size() >= 2) {
            *state = m_state_history[m_state_history.size() - 2];
            m_state_history.pop_back();
        }
        stone->displayStone(state->board);
    }
}

void Controller::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
    case Qt::Key_A: {
        QMessageBox pass(QMessageBox::NoIcon, "Confirm AI?", "Open AI?", QMessageBox::Ok | QMessageBox::Cancel);

        if (pass.exec() == QMessageBox::Ok) {
            analysis->openAI(*state);
        }
    } break;
    case Qt::Key_S: analysis->aiEvaluate(*state); break;
        //case Qt::Key_D: analysis->aiEvaluate_visit(); break;
    case Qt::Key_Q: analysis->displayQi(*state); break;
    case Qt::Key_M: analysis->displayMark(*state); break;
    case Qt::Key_N: analysis->displaySerialNumber(m_state_history); break;
    case Qt::Key_P: {
        QMessageBox pass(QMessageBox::NoIcon, "Confirm Pass?", "Do you want to Pass?", QMessageBox::Ok | QMessageBox::Cancel);

        if (pass.exec() == QMessageBox::Ok) {
            Go::play(*state, PASS);
            m_state_history.push_back(*state);
            displayWin();

            if (analysis->is_ai_open) {
                //GoAI::move_pos = -1;
            }
        }
    } break;
    }
}

void Controller::displayWin() {
    if (Go::isTermination(*state)) {
        Go::Color win = Go::computeReward(*state);

        // Use a more modern and appealing font, adjust size and boldness as needed
        QFont font("Arial", 60, QFont::Bold);
        m_winLabel->setFont(font);

        // Set the geometry to center the label in the window
        int labelWidth = 1000;
        int labelHeight = 100;
        int windowCenterX = this->width() / 2;
        int windowCenterY = this->height() / 2;
        m_winLabel->setGeometry(windowCenterX - labelWidth / 2, windowCenterY - labelHeight / 2, labelWidth, labelHeight);

        // Align text to the center
        m_winLabel->setAlignment(Qt::AlignCenter);

        // Set a background color and text color for better visibility
        m_winLabel->setStyleSheet("QLabel { background-color : black; color : white; }");

        if (win == Go::BLACK)
            m_winLabel->setText("BLACK  Wins!");
        else if (win == Go::WHITE)
            m_winLabel->setText("WHITE  Wins!");
    }
}
