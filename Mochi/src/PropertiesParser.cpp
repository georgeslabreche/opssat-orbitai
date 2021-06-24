#include <fstream>
#include <iostream>

#include "PropertiesParser.hpp"

/* Constants for property keys. */
const string PropertiesParser::PROPS_PREFIX = "esa.mo.nmf.apps.OrbitAI.";
const string PropertiesParser::PROPS_PREFIX_MOCHI = PropertiesParser::PROPS_PREFIX + "mochi.";
const string PropertiesParser::PROPS_LOG_TRAINING_DATA = "log.data.training";
const string PropertiesParser::PROPS_PORT_NUMBER  = "port";
const string PropertiesParser::PROPS_MODE  = "mode";
const string PropertiesParser::PROPS_INPUTS  = "inputs";

/**
 * Constructor.
 */
PropertiesParser::PropertiesParser(char* propertiesFilePath)
{
    loadProperties(propertiesFilePath);

    /* Create string stream from the string of comma separated param names. */
    string paramNamesString = getProperty<string>(PropertiesParser::PROPS_PREFIX, PropertiesParser::PROPS_INPUTS);
    stringstream s_stream(paramNamesString);

    /* Create a vector of param names. */
    while(s_stream.good())
    {
        string substr;
        getline(s_stream, substr, ',');
        m_paramNames.push_back(substr);
    }

    /* Set dimension class member. */
    m_dimension = m_paramNames.size();
}

/**
 * Load the NMF app properties file and put <key, value> property pairs into a map.
 */ 
void PropertiesParser::loadProperties(char* propertiesFilePath)
{
    std::ifstream propertiesFile(propertiesFilePath);

    std::string line;
    while(std::getline(propertiesFile, line))
    {
        if (line.find(PROPS_PREFIX) == 0)
        {
            m_propsMap.insert({line.substr(0, line.find("=")), line.substr(line.find("=") + 1, line.length())});
        }
    }
}
