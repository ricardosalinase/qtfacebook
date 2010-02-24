
#include <QtGui/QApplication>


#include "fbconnectwizard.h"
#include "qtfacebook.h"

#define FB_API_KEY 61cecf6f7ee5528d294e1d6bf675f424



int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    (new QtFacebook());


    return app.exec();


}
