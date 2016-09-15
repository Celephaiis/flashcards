//#include <QtWidget>
#include <QApplication>
#include "window.h"
#include "adddeck.h"
#include "cardview.h"

using namespace std;

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    Window window;
    window.show();
    return app.exec();
}
