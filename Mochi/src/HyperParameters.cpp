#include "HyperParameters.hpp"


/* Constants for algorithm names . */
const string HyperParameters::ALGORITHM_NAME_ADAM = "ADAM";
const string HyperParameters::ALGORITHM_NAME_ADAGRAD_RDA = "ADAGRAD_RDA";
const string HyperParameters::ALGORITHM_NAME_AROW = "AROW";
const string HyperParameters::ALGORITHM_NAME_SCW = "SCW";
const string HyperParameters::ALGORITHM_NAME_NHERD = "NHERD";
const string HyperParameters::ALGORITHM_NAME_PA = "PA";

/**
 * Constructor.
 */
HyperParameters::HyperParameters()
{
    /* List the names of the hyperparameters expected by each online ML algorithm. */
    vector<string> adamParams = {};
    vector<string> adagradRdaParams = {HYPER_PARAMETER_ETA, HYPER_PARAMETER_LAMBDA};
    vector<string> arowParams = {HYPER_PARAMETER_R};
    vector<string> scwParams = {HYPER_PARAMETER_C, HYPER_PARAMETER_ETA};
    vector<string> nherdParams = {HYPER_PARAMETER_C, HYPER_PARAMETER_DIAGONAL};
    vector<string> paParams = {HYPER_PARAMETER_VARIANT, HYPER_PARAMETER_C};

    /* Insert list of expected hyperparameters into a map. */
    m_hpMap.insert(pair<string, vector<string>>(HyperParameters::ALGORITHM_NAME_ADAM, adamParams));
    m_hpMap.insert(pair<string, vector<string>>(HyperParameters::ALGORITHM_NAME_ADAGRAD_RDA, adagradRdaParams));
    m_hpMap.insert(pair<string, vector<string>>(HyperParameters::ALGORITHM_NAME_AROW, arowParams));
    m_hpMap.insert(pair<string, vector<string>>(HyperParameters::ALGORITHM_NAME_SCW, scwParams));
    m_hpMap.insert(pair<string, vector<string>>(HyperParameters::ALGORITHM_NAME_NHERD, nherdParams));
    m_hpMap.insert(pair<string, vector<string>>(HyperParameters::ALGORITHM_NAME_PA, paParams));
}

/**
 * Get the list of hyperparameter names for the given online ML algorithm. 
 */
vector<string> HyperParameters::getHyperParamsNames(string algorithmName)
{
    /* List that will be returned. */
    vector<string> hyperParamNames;

    /* Fetch list of hyperparameters. */
    map<string, vector<string>>::iterator it = m_hpMap.find(algorithmName);

    if(it != m_hpMap.end())
    {
        /* Found element. */
        hyperParamNames = it->second;
    }

    /* Return list of hyperparameter names. */
    return hyperParamNames;
}