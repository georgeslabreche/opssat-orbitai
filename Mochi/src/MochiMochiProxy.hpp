#ifndef MOCHI_MOCHI_PROXY_H_
#define MOCHI_MOCHI_PROXY_H_

#include <vector>
#include <string>

/* The Binary Machine Learning Algorithm Factory. */
#include <mochimochi/classifier/factory/binary_oml_factory.hpp>

#include "Constants.hpp"
#include "Utils.hpp"
#include "PropertiesParser.hpp"

using namespace std;

/**
 * Instead of operating on a singly ML Algorithm this Proxy makes sure
 * that we operate on every ML Alagorithm enabled in the properties file.
 */ 
class MochiMochiProxy : public BinaryOMLInterface
{
private:
    /* Vector pointer for the Binary ML algorithms creator classes. */
    vector<pair<string, BinaryOMLCreator*>> m_bomlCreatorVector;
    PropertiesParser* m_pPropParser;

    /* Hide constructor. */
    MochiMochiProxy() {};
 
public:
    
    /* Constructor. */
    MochiMochiProxy(PropertiesParser* pPropParser)
    {
        m_pPropParser = pPropParser;
    }

    /* Destructor. */
    ~MochiMochiProxy()
    {
        /* Destroy the BinaryOMLCreator pointers in the Creator map. */
        /* Do not increment "it" in the for loop because calling ->erase(it) on a vector we are iterating through is problematic. */
        for(vector<pair<string, BinaryOMLCreator*>>::iterator it=m_bomlCreatorVector.begin(); it!=m_bomlCreatorVector.end();)
        {
            it = m_bomlCreatorVector.erase(it);
        }
    }

    /**
     * Create the enabled algorithms via the Factory Pattern implemented in the MochiMochi library.
     */
    void initAlgorithms(int dim, map<string, vector<string>>* pHpMap);

    /**
     * Delete all model and log files.
     */
    void reset();

    /**
     * For the proxy just return the name of the proxy.
     */
    string name()
    {
        return string("MochiMochiProxy");
    }

    /**
     * Train/update the model with the given training input.
     */
    void train(string* pInput, int dim)
    {
        for(vector<pair<string, BinaryOMLCreator*>>::iterator it=m_bomlCreatorVector.begin(); it!=m_bomlCreatorVector.end(); ++it)
        {
            it->second->train(pInput, dim);
        }

        /* Log the training data. */
        if(m_pPropParser->isTrainingDataLogEnabled() == 1)
        {
            logTrainingData(m_pPropParser->getInputParamNames(), pInput);
        }
    }

    /**
     * Train/update the model with the given training input and save/serialize the model.
     * Note that for this proxy function the path argument is the parent directory path rather than the model file path.
     */
    void trainAndSave(string* pInput, size_t dim, const string modelDirPath)
    {
        for(vector<pair<string, BinaryOMLCreator*>>::iterator it=m_bomlCreatorVector.begin(); it!=m_bomlCreatorVector.end(); ++it)
        {
            it->second->trainAndSave(pInput, dim, modelDirPath + "/" + it->second->name());
        }

        /* Log the training data. */
        if(m_pPropParser->isTrainingDataLogEnabled() == 1)
        {
            logTrainingData(m_pPropParser->getInputParamNames(), pInput);
        }
    }

    /**
     * Infer/predict the label with the given input.
     * Note that for this proxy function the return value is not the prediction result.
     */
    int infer(string* pInput, size_t dim)
    {
        /* This vector will contain the predictions made by the trained algorithms. */
        vector<pair<string, int>> inferences;

        for(vector<pair<string, BinaryOMLCreator*>>::iterator it=m_bomlCreatorVector.begin(); it!=m_bomlCreatorVector.end(); ++it)
        {
            int label = it->second->infer(pInput, dim);
            inferences.push_back(pair<string, int>(it->second->name(), label));
        }

        /* Log the inference results. */
        logInferenceResult(m_pPropParser->getInputParamNames(), pInput, &inferences);
        
        /**
         * Multiple model predictions are invoked.
         * It makes no sense for this Proxy function to return a single prediction result. 
         */
        return 0;
    }

    /**
     * Load a saved/serialized model.
     * Note that for this proxy function the path argument is the parent directory path rather than the model file path.
     */
    void load(const string modelDirPath)
    {
        string modelFilePath;

        for(vector<pair<string, BinaryOMLCreator*>>::iterator it=m_bomlCreatorVector.begin(); it!=m_bomlCreatorVector.end(); ++it)
        {
            /* The file path to the serialized model. */
            modelFilePath = modelDirPath + "/" + it->second->name();

            /* If the serialized model file exists then load it. */
            if(exists(modelFilePath) == 1)
            {
                /* Load the model. */
                it->second->load(modelFilePath);

                /* Log that the model has been loaded. */
                logInfo("Loaded model: " + modelFilePath);
            }
            else
            {
                /* Log that serialized model file is missing file exception. */
                /* When this happens the model will be retrained from scratch. */
                logError(ERROR_SERIALIZED_MODE_NOT_EXIST, "Serialized model file does not exist, training it from sratch instead of loading: " + modelFilePath);
            }
            
        }
    }

    /**
     * Save/serialize the trained model.
     * Note that for this proxy function the path argument is the parent directory path rather than the model file path.
     */
    void save(const string modelDirPath)
    {
        for(vector<pair<string, BinaryOMLCreator*>>::iterator it=m_bomlCreatorVector.begin(); it!=m_bomlCreatorVector.end(); ++it)
        {
            it->second->save(modelDirPath + "/" + it->second->name());
        }
    }
};

#endif // MOCHI_MOCHI_PROXY_H_