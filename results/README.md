# OrbitAI Results
The confusion matrices and classification metrics obtained by the trained models as well as a summary of each app runs. The PA, PA1, and PA2 algorithms were disabled due to serialization issues identified during experiment validation on the Engineering Model (EM) flatsat. This eliminated the risk of an unhandled error preventing the models from being loaded to resume training rather than being re-initialized at every run. 

## Results
Only 6 of the attempted 35 runs succeeded in starting the experiment. This is due to a series of ground segment problems as well as an issue with the NMF Supervisor. Despite these setbacks, at least one model received enough training inputs that resulted in 89% balanced accuracy and a F1 score of 91%. This model in particular was the one trained with the AROW alogirthm in 3D input space.<br><br>The [serialized models](https://github.com/georgeslabreche/opssat-orbitai/tree/main/Mochi/tools/models_space/2021-04-18) were downlinked and their [classificaiton metrics](https://github.com/georgeslabreche/opssat-orbitai/blob/main/Mochi/tools/metrics/space/inferences_2021-04-18.csv) calculated on the ground with an exhaustive sun elevation angle dataset ranging from 0 to 1.57 radians in steps of 0.01. Although this could have been evaluated on-board with the OrbitAI app's inference mode, it was not done to prioritize training the models given the limited amount successuful app runs.

### Balanced accuracies of predictions made by models trained in-flight
|       | 1D    | 2D    | 3D    |
|-------|-------|-------|-------|
| Adam  | 0.495 | 0.495 | 0.495 |
| RDA   | 0.495 | 0.495 | 0.495 |
| AROW  | 0.500 | 0.561 | **0.891** |
| SCW   | 0.500 | 0.500 | 0.500 |
| NHERD | 0.495 | 0.495 | 0.665 |

### Confusion matrix for AROW 3D
|      | **OFF*** | **ON*** |
|------|------|-----|
| **OFF**** | 43   | 11  |
| **ON****  | 1    | 104 |

\* Labels predicted by the mode.<br>
\** Expected labels included in the test data set.

## Ground Operations
- All passes in which OrbitAI related ground operations took place are listed in [passes.txt](passes.txt).
- Ground operations at every pass are detailed in [OPERATIONS.md](OPERATIONS.md).

## Runs
All app runs are summarized in this section.
### Run #1
Initial training of models. The training inputs were not diverse enough:
- The PD6 sun elevation angle training inputs ranged from 0 to 0.65 radians.
- Further training runs are required to cover inputs ranges from 1.05 to 1.57 radians.
### Run #2
Continued training the models. The training inputs were not diverse enough:
- The PD6 sun elevation angle training inputs ranged from 0 to 0.23 radians.
- Further training runs are required to cover inputs ranges from 1.05 to 1.57 radians.
### Run #3
Was not triggered due to no bitlock during the time scheduled to send the StartExperiment telecommand (TC) to the spacecraft.

### Run #4
Was not triggered due to no bitlock during the time scheduled to send the StartExperiment TC to the spacecraft.

### Run #5
For reasons not investigated the app did not run so no new data was created. The previous pass from which the StartExperiment TC was sent had SCOS/Matis delay problems which could be related to the issue.

### Runs #6 to #16
To increase the chances of capturing training inputs for PD6 elevation angle values that are above the 1.05 radians threshold the app will only run while the spacecraft is illuminated. The spacecraft is illuminated for half of the orbit (45 min). Reduced the number of iterations from 850 to 540 (from ~71 min to 45 min). Uplinked the schedule to start the app at the following UTC times: 
 - **April 17 - Run \#6 to \#8**: 19:45, 21:20, and 22:55.
 - **April 18 - Run \#9 to \#16**: 0:30, 2:05, 6:50, 8:25, 10:00, 11:35, 13:10, and 14:45.

The runs at the following start times failed to start: 19:45, 6:50, 8:25, 10:00, 11:35, 13:10, and 14:45. Downlinked logs confirmed a problem with the NMF Supervisor which prevented the experiment from running.

### Runs #17 to #31
 - **April 18 - Run \#17 to \#20**: 18:14, 19:30, 21:05, and 22:40.
 - **April 19 - Run \#21 to \#31**: 0:15, 1:50, 3:25, 5:00, 6:35, 8:10, 9:45, 11:20, 12:55, 14:30, and 16:05.

The app failed to start for all scheduled runs. Suspected issues with NMF Supervisor but not investigated.

### Runs #32 to #35
**April 20**: 19:00, 20:35, 22:10, and 23:45.

The app failed to start for all scheduled runs. Suspected issues with NMF Supervisor but not investigated.