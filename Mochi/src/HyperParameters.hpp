#ifndef HYPER_PARAMETERS_H_
#define HYPER_PARAMETERS_H_

#define HYPER_PARAMETER_ETA                                             "eta"
#define HYPER_PARAMETER_LAMBDA                                       "lambda"
#define HYPER_PARAMETER_R                                                 "r"
#define HYPER_PARAMETER_C                                                 "c"
#define HYPER_PARAMETER_DIAGONAL                                   "diagonal"
#define HYPER_PARAMETER_VARIANT                                     "variant"

#include <map>
#include <string>
#include <list>
#include <vector>

using namespace std;

class HyperParameters
{
private:
    /* Map listing the hyperparameter names for each online ML algorithm. */
    map<string, vector<string>> m_hpMap;
 
public:
    /* Constants for property keys. */
    static const string ALGORITHM_NAME_ADAM;
    static const string ALGORITHM_NAME_ADAGRAD_RDA;
    static const string ALGORITHM_NAME_AROW;
    static const string ALGORITHM_NAME_SCW;
    static const string ALGORITHM_NAME_NHERD;
    static const string ALGORITHM_NAME_PA;

    /* Constructor. */
    HyperParameters();

    map<string, vector<string>> getHyperParamsMap(){ return m_hpMap; }

    /* Get the list of hyperparameter names for the given online ML algorithm. */
    vector<string> getHyperParamsNames(string algorithmName);
};

#endif // HYPER_PARAMETERS_H_