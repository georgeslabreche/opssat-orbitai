import sys
import csv
import json
import math
from enum import Enum

'''
Calculates classification metrics for all models trained:
- Sensitivity
- Specificity
- Precision
- Negative Predictive Value
- Biased Accuracy
- Balanced Accuracy
- F1 Score

Usage: python3 analyze.py <string:inference_csv_log_file> <string:metrics_results_output_csv_file>
E.g.: python3 analyze.py logs/inference.csv metrics/performance.csv

References:

The 3 Most Important Composite Classification Metrics
https://towardsdatascience.com/the-3-most-important-composite-classification-metrics-b1f2d886dc7b

What is balanced accuracy?
https://statisticaloddsandends.wordpress.com/2020/01/23/what-is-balanced-accuracy/

Balanced accuracy: what and why?
http://mvpa.blogspot.com/2015/12/balanced-accuracy-what-and-why.html
'''

# Flag to indicate if classification metrics should be calculatd for models trained in 5D input space.
# The reason we make optional is because when testing with training on the ground we don't generate any
# valid 5D input space training data.
# TODO: externalize as a parameter
INCLUDE_5D_INPUT_SPACE = True

class Algorithm(Enum):
    ADAM = 1    # A Method for Stochastic Optimization
    RDA = 2     # Adaptive Subgradient Methods for Online Learning and Stochastic Optimization (Adagarad = Adaptive Gradian, RDA = Regularized Dual Averaging)
    AROW = 3    # Adaptive Regularization of Weight Vectors
    SCW = 4     # Exact Soft Confidence-Weighted Learning
    NHERD = 5   # Normal Herd (Learning via Gaussian Herding)
    PA = 6      # Passive Aggressive
    PA1 = 7     # Passive Aggressive I
    PA2 = 8     # Passive Aggressive II

ALGORITHMS = [
    Algorithm.ADAM, Algorithm.RDA, Algorithm.AROW, 
    Algorithm.SCW, Algorithm.NHERD,
    Algorithm.PA, Algorithm.PA1, Algorithm.PA2
]

# Model performance metrics tracker.
trackers = {
    'adam': [],
    'rda': [],
    'arow': [],
    'scw': [],
    'nherd': [],
    'pa': [],
    'pa1': [],
    'pa2': []
}

# Init the performance metrics tracker for all dimensions, i.e. 1D, 2D, 3D, and 5D.
for i in range(4):
    for algo_key in trackers:
        trackers[algo_key].append({
            'confusion_matrix': { # True Positive, False Negative, False Positive, and True Negative.
                'TP': 0, 'FN': 0, 'FP': 0, 'TN': 0 
            },
            'sensitivity': 0,
            'specificity': 0,
            'precision': 0,
            'neg_predictive_value': 0,
            'accuracy': {
                'biased': 0,
                'balanced': 0
            },
            'f1_score': 0
        })

# Init total prediction counter.
trackers['total'] = 0


def get_algo_name(algorithm):
    ''' Get the object key name of the given algorithm.
    
    The object key name is as defined in the trackers object.
    '''
    
    if algorithm is Algorithm.ADAM:
        return 'adam'

    elif algorithm is Algorithm.RDA:
        return 'rda'

    elif algorithm is Algorithm.AROW:
        return 'arow'

    elif algorithm is Algorithm.SCW:
        return 'scw'

    elif algorithm is Algorithm.NHERD:
        return 'nherd'

    elif algorithm is Algorithm.PA:
        return 'pa'

    elif algorithm is Algorithm.PA1:
        return 'pa1'

    elif algorithm is Algorithm.PA2:
        return 'pa2'

    else:
        print('ERROR: Invalid algorithm.')
        exit()


def build_confusion_matrix(algorithm, expected_label, row):
    ''' Build the confusion matrix of predictions made by each algorithm.
    
    Reminder: each algorihm produces 4 models using 4 different feature space dimensions (1D, 2D, 3D, and 5D).
    '''

    pred_label_index = -1
    algo_key = None

    if algorithm is Algorithm.ADAM:
        pred_label_index = 8

    elif algorithm is Algorithm.RDA:
        pred_label_index = 12

    elif algorithm is Algorithm.AROW:
        pred_label_index = 16

    elif algorithm is Algorithm.SCW:
        pred_label_index = 20

    elif algorithm is Algorithm.NHERD:
        pred_label_index = 24

    elif algorithm is Algorithm.PA:
        pred_label_index = 28

    elif algorithm is Algorithm.PA1:
        pred_label_index = 32

    elif algorithm is Algorithm.PA2:
        pred_label_index = 36

    else:
        print('ERROR: Invalid algorithm.')
        exit()

    # Dimension index in the tracker array for 1D, 2D, 3D, and 5D.
    # So from 0 to 4.
    dimension_index = 0
    
    # For each
    for i in range(pred_label_index, pred_label_index + 4):

        # The predicted label.
        predicted_label = int(row[i])

        # Increment counters in the confusion matrix.
        if expected_label == 1:
            if predicted_label == 1:
                # True Positive.
                trackers[get_algo_name(algorithm)][dimension_index]['confusion_matrix']['TP'] += 1

            elif predicted_label in [0, -1]:
                # False Negative.
                trackers[get_algo_name(algorithm)][dimension_index]['confusion_matrix']['FN'] += 1

            else:
                print("ERROR: Invalid value for predicted label: " + str(predicted_label))
                exit(1)

        elif expected_label in [0, -1]:
            if predicted_label == 1:
                # False Positive
                trackers[get_algo_name(algorithm)][dimension_index]['confusion_matrix']['FP'] += 1

            elif predicted_label in [0, -1]:
                # True Negative
                trackers[get_algo_name(algorithm)][dimension_index]['confusion_matrix']['TN'] += 1

            else:
                print("ERROR: Invalid value for predicted label: " + str(predicted_label))
                exit(1)

        else:
            print("ERROR: Invalid value for expected label: " + str(expected_label))
            exit(1)

        # Iteration to next dimension index.
        dimension_index += 1


def calculate_classification_metrics(algorithm):
    ''' Calculate classification metrics for each models.'''

    # Get algorithm key.
    algo_key = get_algo_name(algorithm)

    # Dimension index in the tracker array for 1D, 2D, 3D, and 5D.
    for i in range(4):

        # Fetch confusion matrix values.

        # True Positives.
        TP = trackers[algo_key][i]['confusion_matrix']['TP']

        # False Negatives.
        FN = trackers[algo_key][i]['confusion_matrix']['FN']

        # False Positives.
        FP = trackers[algo_key][i]['confusion_matrix']['FP']

        # True Negatives.
        TN = trackers[algo_key][i]['confusion_matrix']['TN']

        # Calculate classification metrics.

        # Sensitivity = TP / (TP + FN)
        trackers[algo_key][i]['sensitivity'] = TP / (TP + FN) if (TP + FN) != 0 else math.nan

        # Specificity = TN / (TN + FP)
        trackers[algo_key][i]['specificity'] = TN / (TN + FP) if (TN + FP) != 0 else math.nan

        # Precision = TP / (TP + FP)
        trackers[algo_key][i]['precision'] = TP / (TP + FP) if (TP + FP) != 0 else math.nan
        
        # Negative Predictive Value = TN / (TN + FN)
        trackers[algo_key][i]['neg_predictive_value'] = TN / (TN + FN) if (TN + FN) != 0 else math.nan
        
        # Biased Accuracy =  (TP + TN) / (TP + FN + FP + TN)
        trackers[algo_key][i]['accuracy']['biased'] = (TP + TN) / (TP + FN + FP + TN)
        
        # Balanced Accuracy = (Sensitive + Specificity) / 2
        trackers[algo_key][i]['accuracy']['balanced'] = (trackers[algo_key][i]['sensitivity'] + trackers[algo_key][i]['specificity']) / 2

        # F1 Score = 2 * (Precision * Sensitivity) / (Precision + Sensitivity)
        trackers[algo_key][i]['f1_score'] = 2 * (trackers[algo_key][i]['precision'] * trackers[algo_key][i]['sensitivity']) / (trackers[algo_key][i]['precision'] + trackers[algo_key][i]['sensitivity'])


# Main application function.
if __name__ == "__main__":

    # Inference log file to process.
    INFERENCE_LOG_FILENAME = sys.argv[1]

    # Metrics output filename.
    OUTPUT_FILENAME = sys.argv[2]

    # Verbosity.
    print('Reading inference log file: ' + INFERENCE_LOG_FILENAME)

    # Read inference log CSV file and build confusion matrices for each models.
    with open(INFERENCE_LOG_FILENAME) as csvfile:

        # Get rows of results.
        results = csv.reader(csvfile)

        # Iterate rows.
        for row in results:

            # Count number of predictions made.
            trackers['total'] += 1

            # Get the expected label.
            expected_label = int(row[7])

            # Count number of correction predictions made by each algorithm.
            for algo in ALGORITHMS:
                build_confusion_matrix(algo, expected_label, row)


    # Calculate classification metrics.
    for algo in ALGORITHMS:
        calculate_classification_metrics(algo)

    # Verbosity.
    print('Writing classification metrics to file: ' + OUTPUT_FILENAME)

    # Write to results into a csv file.
    with open(OUTPUT_FILENAME, 'w') as csvfile:

        # Write headers row.
        headers = 'algorithm,dimension,TP,FN,FP,TN,sensitivity,specificity,precision,neg_predictive_value,accuracy_biased,accuracy_balanced,f1_score\n'
        csvfile.write(headers)

        for algo in ALGORITHMS:
            algo_key = get_algo_name(algo)

            # Dimension index in the tracker array for 1D, 2D, 3D, and 5D.
            for i in range(4):

                # Get the dimension we're on.
                dim = i + 1 if i != 3 else 5

                # Don't bother with models trained in 5D input space if we flagged not to include them.
                if dim == 5 and INCLUDE_5D_INPUT_SPACE is False:
                    break

                # tracker for the current dimension.
                t = trackers[algo_key][i]

                # Confusion matrix.
                cm = t['confusion_matrix']
                
                # CSV data row.
                row = '{0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12}\n'.format(algo_key, dim, \
                    cm['TP'], cm['FN'], cm['FP'], cm['TN'], \
                    t['sensitivity'], t['specificity'], t['precision'], t['neg_predictive_value'], \
                    t['accuracy']['biased'], t['accuracy']['balanced'], \
                    t['f1_score'])

                csvfile.write(row)
