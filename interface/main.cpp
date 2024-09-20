#include <QtWidgets/QApplication>
#include "GoDemo.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    GoDemo w;
    w.show();
    return a.exec();
}
