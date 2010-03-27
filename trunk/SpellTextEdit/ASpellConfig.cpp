#include "ASpellConfig.h"

ASpellConfig * ASpellConfig::m_instance = 0;

ASpellConfig::ASpellConfig()
{
    m_aConfig = new_aspell_config();
    aspell_config_replace(m_aConfig, "lang", "en_US");
}

ASpellConfig * ASpellConfig::instance()
{
    if (!m_instance)
        m_instance = new ASpellConfig();

    return m_instance;

}

AspellConfig * ASpellConfig::getConfig()
{
    return m_aConfig;
}
