#ifndef PROPERTIES_PARSER_H_
#define PROPERTIES_PARSER_H_

#include <vector>
#include <map>
#include <string>
#include <sstream>

using namespace std;

class PropertiesParser
{
private:
    char* m_propertiesFilePath;
    map<string, string> m_propsMap;
    size_t m_dimension;
    vector<string> m_paramNames;

    /* Hide constructor */
    PropertiesParser();

    void loadProperties(char* propertiesFilePath);
 
public:

    /* Constants for property keys. */
    static const string PROPS_PREFIX;
    static const string PROPS_LOG_TRAINING_DATA;
    static const string PROPS_PORT_NUMBER;
    static const string PROPS_MODE;
    static const string PROPS_INPUTS;

    PropertiesParser(char* propertiesFilePath);

    template<typename T>
    T getProperty(string key)
    {   
        istringstream stringValue(m_propsMap[PropertiesParser::PROPS_PREFIX + key]);
        T castValue;
        stringValue >> castValue;

        return castValue;
    }

    template<typename T>
    T getHyperParameterProperty(string algorithmName, string hyperParamName)
    {
        istringstream stringValue(m_propsMap[PropertiesParser::PROPS_PREFIX + algorithmName + ".hparam." + hyperParamName]);
        T castValue;
        stringValue >> castValue;

        return castValue;
    }

    int isTrainingDataLogEnabled()
    {
        return getProperty<int>(PropertiesParser::PROPS_LOG_TRAINING_DATA);
    }

    int getPortNumber()
    {
        return getProperty<int>(PropertiesParser::PROPS_PORT_NUMBER);
    }

    /* Get mode: 0 - new training, 1 - continue training, or 2 - inference. */
    int getMode()
    {
        return getProperty<int>(PropertiesParser::PROPS_MODE);
    }

    vector<string>* getInputParamNames()
    {
        return &m_paramNames;
    }

    size_t getInputDimension()
    {
        return m_dimension;
    }

};

#endif // PROPERTIES_PARSER_H_