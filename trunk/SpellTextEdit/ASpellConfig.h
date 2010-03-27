#ifndef ASPELLCONFIG_H
#define ASPELLCONFIG_H

#include <aspell.h>

class ASpellConfig
{
public:
    static ASpellConfig * instance();

    AspellConfig * getConfig();

private:
    ASpellConfig();
    ~ASpellConfig();
    AspellConfig *m_aConfig;
    static ASpellConfig *m_instance;

};

#endif // ASPELLCONFIG_H
