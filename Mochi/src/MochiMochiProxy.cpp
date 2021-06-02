
#include <map>
#include <vector>
#include <string>
#include <dirent.h>

#include "HyperParameters.hpp"
#include "MochiMochiProxy.hpp"

/**
 * Create the enabled algorithms via the Factory Pattern implemented in the MochiMochi library.
 */
void MochiMochiProxy::initAlgorithms(int dim, map<string, vector<string>>* pHpMap)
{
    for(map<string, vector<string>>::iterator it=pHpMap->begin(); it!=pHpMap->end(); ++it)
    {
        /* The name of the online ML algorithm. */
        string algorithmName = it->first;

        /* Check if the properties file indicates that this algorithm should be used. */
        int enable = m_pPropParser->getProperty<int>(algorithmName);
        if(enable == 1)
        {
            if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_ADAGRAD_RDA) == 0)
            {
                /* Get hyperparameter values. */
                const double eta = m_pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));
                const double lambda = m_pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(1));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm vector. */ 
                m_pBomlCreatorVector->push_back(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryADAGRADRDACreator(dim, eta, lambda)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_ADAM) == 0)
            {
                /* Instanciate the online ML algorithm class into an object and put it in the algorithm vector. */ 
                m_pBomlCreatorVector->push_back(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryADAMCreator(dim)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_AROW) == 0)
            {
                /* Get hyperparameter values. */
                const double r = m_pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm vector. */ 
                m_pBomlCreatorVector->push_back(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryAROWCreator(dim, r)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_NHERD) == 0)
            {
                /* Get hyperparameter values. */
                const double c = m_pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));
                const int diagonal = m_pPropParser->getHyperParameterProperty<int>(algorithmName, it->second.at(1));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm vector. */ 
                m_pBomlCreatorVector->push_back(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryNHERDCreator(dim, c, diagonal)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_PA) == 0)
            {
                /* Get hyperparameter values. */
                const int select = m_pPropParser->getHyperParameterProperty<int>(algorithmName, it->second.at(0));
                const double c = m_pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(1));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm vector. */ 
                m_pBomlCreatorVector->push_back(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryPACreator(dim, c, select)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_SCW) == 0)
            {
                /* Get hyperparameter values. */
                const double c = m_pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));
                const double eta = m_pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(1));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm vector. */ 
                m_pBomlCreatorVector->push_back(pair<string, BinaryOMLCreator*>(algorithmName, new BinarySCWCreator(dim, c, eta)));
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
    DIR* pDir;
    struct dirent* pEnt;

    if((pDir = opendir(DIR_PATH_MODELS)) != NULL)
    {   
        /* Loop through all the files inside the models directory. */
        while((pEnt = readdir(pDir)) != NULL)
        {
            /* Only process regular image files */
            if(pEnt->d_type == DT_REG)
            {
                string filname(string(DIR_PATH_MODELS) + "/" + string(pEnt->d_name));
                remove(filname.c_str());
            }
        }
    }

    /* Delete all log files. */
    remove(LOG_FILEPATH_TRAINING);
    remove(LOG_FILEPATH_INFERENCE);
    remove(LOG_FILEPATH_ORBITAI);
}
