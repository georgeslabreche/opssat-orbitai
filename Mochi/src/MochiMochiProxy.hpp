#ifndef MOCHI_MOCHI_PROXY_H_
#define MOCHI_MOCHI_PROXY_H_

#include <map>
#include <string>

/* The Binary Machine Learning Algorithm Factory. */
#include <mochimochi/classifier/factory/binary_oml_factory.hpp>

#include "Constants.hpp"
#include "PropertiesParser.hpp"

using namespace std;

/**
 * Instead of operating on a singly ML Algorithm this Proxy makes sure
 * that we operate on every ML Alagorithm enabled in the properties file.
 */ 
class MochiMochiProxy : public BinaryOMLCreatorInterface
{
private:
    /* Map pointer for the Binary ML algorithms creator classes. */
    map<string, BinaryOMLCreator*> *m_pBomlCreatorMap;

    /* Hide constructor. */
    MochiMochiProxy() {};
 
public:
    
    /* Constructor. */
    MochiMochiProxy(map<string, BinaryOMLCreator*> *pBomlCreatorMap) 
    { 
        m_pBomlCreatorMap = pBomlCreatorMap;
    }

    /**
     * Create the enabled algorithms via the Factory Pattern implemented in the MochiMochi library.
     */
    void initAlgorithms(int dim, PropertiesParser *pPropParser, map<string, vector<string>> *pHpMap);

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
    void train(string *pInput, int dim)
    {
        for(map<string, BinaryOMLCreator*>::iterator it=m_pBomlCreatorMap->begin(); it!=m_pBomlCreatorMap->end(); ++it)
        {
            it->second->train(pInput, dim);
        }
    }

    /**
     * Train/update the model with the given training input and save/serialize the model.
     * Note that for this proxy function the path argument is the parent directory path rather than the model file path.
     */
    void trainAndSave(string *pInput, size_t dim, const string modelDirPath)
    {
        for(map<string, BinaryOMLCreator*>::iterator it=m_pBomlCreatorMap->begin(); it!=m_pBomlCreatorMap->end(); ++it)
        {
            it->second->trainAndSave(pInput, dim, modelDirPath + "/" + it->second->name());
        }
    }

    /**
     * Infer/predict the label with the given input.
     * Note that for this proxy function the return value is not the prediction result.
     */
    int infer(string *pInput, size_t dim)
    {
        for(map<string, BinaryOMLCreator*>::iterator it=m_pBomlCreatorMap->begin(); it!=m_pBomlCreatorMap->end(); ++it)
        {
            // TODO: insert prediction result into map (or write them directly into file?)
            it->second->infer(pInput, dim);
        }
        
        /**
         * Multiple model predictions are invoked.
         * It makes no sense for this Proxy function to return a single predict result. 
         */
        return 0;
    }

    /**
     * Load a saved/serialized model.
     * Note that for this proxy function the path argument is the parent directory path rather than the model file path.
     */
    void load(const string modelDirPath)
    {
        for(map<string, BinaryOMLCreator*>::iterator it=m_pBomlCreatorMap->begin(); it!=m_pBomlCreatorMap->end(); ++it)
        {
            // TODO: Handle missing file exception (just skip and log the error?).
            it->second->load(modelDirPath + "/" + it->second->name());
        }
    }

    /**
     * Save/serialize the trained model.
     * Note that for this proxy function the path argument is the parent directory path rather than the model file path.
     */
    void save(const string modelDirPath)
    {
        for(map<string, BinaryOMLCreator*>::iterator it=m_pBomlCreatorMap->begin(); it!=m_pBomlCreatorMap->end(); ++it)
        {
            it->second->save(modelDirPath + "/" + it->second->name());
        }
    }
};

#endif // MOCHI_MOCHI_PROXY_H_