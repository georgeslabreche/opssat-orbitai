import csv
import json

'''
    From Mochi directory: python3 src/accuracy.py 
'''

trackers = {
    'total': 0, # total predictions.
    'arow': {
        'correct': [0, 0, 0], # [1D, 2D, 3D]
        'accuracy': [0, 0, 0] 
    },
    'scw': {
        'correct': [0, 0, 0], # [1D, 2D, 3D]
        'accuracy': [0, 0, 0] 
    },
    'nherd': {
        'correct': [0, 0, 0], # [1D, 2D, 3D]
        'accuracy': [0, 0, 0] 
    },
    'pa': {
        'correct': [0, 0, 0], # [1D, 2D, 3D]
        'accuracy': [0, 0, 0] 
    }
}

# Read CSV file.
with open('logs/inference.csv') as csvfile:

    # Get rows of results.
    results = csv.reader(csvfile)

    # Iterate rows.
    for row in results:

        # Count number of predictions.
        trackers['total'] += 1

        # Get the expected label.
        expected_label = row[2]
        
        #####################################
        # ARROW: Count correct predictions. #
        #####################################
        
        # Using model trained with 1D feature space training data input.
        if row[3] == expected_label:
            trackers['arow']['correct'][0] += 1

        # Using model trained with 2D feature space training data input.
        if row[4] == expected_label:
            trackers['arow']['correct'][1] += 1

        # Using model trained with 3D feature space training data input.
        if row[5] == expected_label:
            trackers['arow']['correct'][2] +=1


        ###################################
        # SCW: Count correct predictions. #
        ###################################

        # Using model trained with 1D feature space training data input.
        if row[6] == expected_label:
            trackers['scw']['correct'][0] += 1

        # Using model trained with 2D feature space training data input.
        if row[7] == expected_label:
            trackers['scw']['correct'][1] += 1

        # Using model trained with 3D feature space training data input.
        if row[8] == expected_label:
            trackers['scw']['correct'][2] +=1


        #####################################
        # NHERD: Count correct predictions. #
        #####################################

        # Using model trained with 1D feature space training data input.
        if row[9] == expected_label:
            trackers['nherd']['correct'][0] += 1

        # Using model trained with 2D feature space training data input.
        if row[10] == expected_label:
            trackers['nherd']['correct'][1] += 1

        # Using model trained with 3D feature space training data input.
        if row[11] == expected_label:
            trackers['nherd']['correct'][2] +=1


        ##################################
        # PA: Count correct predictions. #
        ##################################

        # Using model trained with 1D feature space training data input.
        if row[12] == expected_label:
            trackers['pa']['correct'][0] += 1

        # Using model trained with 2D feature space training data input.
        if row[13] == expected_label:
            trackers['pa']['correct'][1] += 1

        # Using model trained with 3D feature space training data input.
        if row[14] == expected_label:
            trackers['pa']['correct'][2] +=1


#######################
# Calculate accuracy. #
#######################

# AROW
trackers['arow']['accuracy'][0] = trackers['arow']['correct'][0] / trackers['total']
trackers['arow']['accuracy'][1] = trackers['arow']['correct'][1] / trackers['total']
trackers['arow']['accuracy'][2] = trackers['arow']['correct'][2] / trackers['total']

# SCW
trackers['scw']['accuracy'][0] = trackers['scw']['correct'][0] / trackers['total']
trackers['scw']['accuracy'][1] = trackers['scw']['correct'][1] / trackers['total']
trackers['scw']['accuracy'][2] = trackers['scw']['correct'][2] / trackers['total']

# NHERD
trackers['nherd']['accuracy'][0] = trackers['nherd']['correct'][0] / trackers['total']
trackers['nherd']['accuracy'][1] = trackers['nherd']['correct'][1] / trackers['total']
trackers['nherd']['accuracy'][2] = trackers['nherd']['correct'][2] / trackers['total']

# PA
trackers['pa']['accuracy'][0] = trackers['pa']['correct'][0] / trackers['total']
trackers['pa']['accuracy'][1] = trackers['pa']['correct'][1] / trackers['total']
trackers['pa']['accuracy'][2] = trackers['pa']['correct'][2] / trackers['total']

############
# Results. #
############

# Pretty print the results.
print(json.dumps(trackers, indent=2))