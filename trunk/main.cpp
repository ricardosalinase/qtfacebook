
#include <QtGui/QApplication>

#include "facebookconnector.h"
#include "fbconnectwizard.h"
#include "qtfacebook.h"

#define FB_API_KEY 61cecf6f7ee5528d294e1d6bf675f424



int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    // load session_key, uid, and secret


    // If we don't have those, launch the connector
    //FaceBookConnector fbc("61cecf6f7ee5528d294e1d6bf675f424");
    //fbc.setApiKey("61cecf6f7ee5528d294e1d6bf675f424");
    //fbc.show();

    //FBConnectWizard fbc("61cecf6f7ee5528d294e1d6bf675f424", "qtFacebook");
    //fbc.show();

    QtFacebook *fb = new QtFacebook();
    fb->show();

    return app.exec();


}
