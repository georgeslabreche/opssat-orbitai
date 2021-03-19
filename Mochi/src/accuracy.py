import csv
import json
from enum import Enum

'''
    From Mochi directory: python3 src/accuracy.py 
'''

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

trackers = {
    'total': 0, # total predictions.
    'adam': {
        'correct': [0, 0, 0, 0], # [1D, 2D, 3D, 5D]
        'accuracy': [0, 0, 0, 0] 
    },
    'rda': {
        'correct': [0, 0, 0, 0],
        'accuracy': [0, 0, 0, 0]
    },
    'arow': {
        'correct': [0, 0, 0, 0],
        'accuracy': [0, 0, 0, 0]
    },
    'scw': {
        'correct': [0, 0, 0, 0],
        'accuracy': [0, 0, 0, 0]
    },
    'nherd': {
        'correct': [0, 0, 0, 0],
        'accuracy': [0, 0, 0, 0] 
    },
    'pa': {
        'correct': [0, 0, 0, 0],
        'accuracy': [0, 0, 0, 0] 
    },
    'pa1': {
        'correct': [0, 0, 0, 0],
        'accuracy': [0, 0, 0, 0] 
    },
    'pa2': {
        'correct': [0, 0, 0, 0],
        'accuracy': [0, 0, 0, 0] 
    }
}

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


def count_correct_predictions(algorithm, expected_label, row):
    ''' Count number of correction predictions made by each algorithm.
    
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

        # Increment the prediction counter if the prediction is correct
        # for the given algorithm at the current feature space dimension.
        if row[i] == expected_label:
            trackers[get_algo_name(algorithm)]['correct'][dimension_index] += 1

        # Iteration to next dimension index.
        dimension_index += 1


def calculate_accuracies(algorithm, total_predictions):
    ''' Calculate prediction accuracies for each models.'''

    algo_key = get_algo_name(algorithm)

    for i in range(4):
        trackers[algo_key]['accuracy'][i] = trackers[algo_key]['correct'][i] / total_predictions


# Read inference log CSV file.
with open('logs/inference.csv') as csvfile:

    # Get rows of results.
    results = csv.reader(csvfile)

    # Iterate rows.
    for row in results:

        # Count number of predictions made.
        trackers['total'] += 1

        # Get the expected label.
        expected_label = row[7]

        # Count number of correction predictions made by each algorithm.
        for algo in ALGORITHMS:
            count_correct_predictions(algo, expected_label, row)


# Calculate accuracies.
for algo in ALGORITHMS:
    calculate_accuracies(algo, trackers['total'])

# Pretty print the results.
print(json.dumps(trackers, indent=2))