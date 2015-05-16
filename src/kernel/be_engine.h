#ifndef BE_ENGINE_H_INCLUDED
#define BE_ENGINE_H_INCLUDED

#include <string>
#include <iostream>

class BeEngine
{
        BeEngine( const BeGame* ) :
                m_fileSystem(),
                m_settingsSystem(m_fileSystem),
                m_graphicsSystem(m_fileSystem, m_settingsSystem)
                {
                }
        BeFileSystem                m_fileSystem;
        BeSettingsSystem         m_settingsSystem;
        BeGraphicsSystem        m_graphicsSystem;
};

#endif
