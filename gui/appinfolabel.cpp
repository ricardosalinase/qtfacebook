#include "appinfolabel.h"

namespace GUI {

AppInfoLabel::AppInfoLabel(DATA::AppInfo *ai, QWidget *parent) :
            QLabel(parent),
            m_appInfo(ai)
{
    setPixmap(*(ai->getIconPixmap()));
}

AppInfoLabel::~AppInfoLabel() {
    delete m_appInfo;
}

void AppInfoLabel::myIconPixmap(QPixmap *p, bool display) {
    if (display)
        setPixmap(*p);
    m_appInfo->setIconPixmap(p);
}

void AppInfoLabel::myLogoPixmap(QPixmap *p, bool display) {
    if (display)
        setPixmap(*p);
    m_appInfo->setLogoPixmap(p);
}

DATA::AppInfo * AppInfoLabel::getAppInfo() {
    return m_appInfo;
}

} // namespace GUI
