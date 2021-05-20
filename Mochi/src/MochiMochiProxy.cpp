
#include <map>
#include <string>
#include <dirent.h>

#include "HyperParameters.hpp"
#include "MochiMochiProxy.hpp"

/**
 * Create the enabled algorithms via the Factory Pattern implemented in the MochiMochi library.
 */
void MochiMochiProxy::initAlgorithms(int dim, PropertiesParser *pPropParser, map<string, vector<string>> *pHpMap)
{
    for(map<string, vector<string>>::iterator it=pHpMap->begin(); it!=pHpMap->end(); ++it)
    {
        /* The name of the online ML algorithm. */
        string algorithmName = it->first;

        /* Check if the properties file indicates that this algorithm should be used. */
        int enable = pPropParser->getProperty<int>(algorithmName);
        if(enable == 1)
        {
            if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_ADAGRAD_RDA) == 0)
            {
                /* Get hyperparameter values. */
                const double eta = pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));
                const double lambda = pPropParser->getHyperParameterProperty<double>(algorithmName,it->second.at(1));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm map. */ 
                m_pBomlCreatorMap->insert(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryADAGRADRDACreator(dim, eta, lambda)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_ADAM) == 0)
            {
                /* Instanciate the online ML algorithm class into an object and put it in the algorithm map. */ 
                m_pBomlCreatorMap->insert(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryADAMCreator(dim)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_AROW) == 0)
            {
                /* Get hyperparameter values. */
                const double r = pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm map. */ 
                m_pBomlCreatorMap->insert(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryAROWCreator(dim, r)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_NHERD) == 0)
            {
                /* Get hyperparameter values. */
                const double c = pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));
                const int diagonal = pPropParser->getHyperParameterProperty<int>(algorithmName,it->second.at(1));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm map. */ 
                m_pBomlCreatorMap->insert(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryNHERDCreator(dim, c, diagonal)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_PA) == 0)
            {
                /* Get hyperparameter values. */
                const double c = pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));
                const int select = pPropParser->getHyperParameterProperty<int>(algorithmName,it->second.at(1));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm map. */ 
                m_pBomlCreatorMap->insert(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryPACreator(dim, c, select)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_SCW) == 0)
            {
                /* Get hyperparameter values. */
                const double c = pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));
                const double eta = pPropParser->getHyperParameterProperty<double>(algorithmName,it->second.at(1));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm map. */ 
                m_pBomlCreatorMap->insert(pair<string, BinaryOMLCreator*>(algorithmName, new BinarySCWCreator(dim, c, eta)));
            }
        }
    }
}

/**
 * Delete all model and log files.
 */
void MochiMochiProxy::reset()
{
    /* Delete all model files. */
    DIR *dir;
    struct dirent *ent;

    if((dir = opendir(DIR_PATH_MODELS)) != NULL)
    {   
        /* Loop through all the files inside the models directory. */
        while((ent = readdir(dir)) != NULL)
        {
            /* Only process regular image files */
            if(ent->d_type == DT_REG)
            {
                string filname(string(DIR_PATH_MODELS) + "/" + string(ent->d_name));
                remove(filname.c_str());
            }
        }
    }

    /* Delete all log files. */
    remove(LOG_FILEPATH_TRAINING);
    remove(LOG_FILEPATH_INFERENCE);
    remove(LOG_FILEPATH_ORBITAI);
}
