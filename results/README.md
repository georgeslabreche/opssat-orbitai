# OrbitAI Results
The confusion matrices and classification metrics obtained by the trained models as well as a summary of each app runs.

## Results
_TODO_.

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

### Runs #6 to #19
To increase the chances of capturing training inputs for PD6 elevation angle values that are above the 1.05 radians threshold the app will only run while the spacecraft is illuminated. The spacecraft is illuminated for half of the orbit (45 min). Reduced the number of iterations from 850 to 540 (from ~71 min to 45 min). Uplinked the schedule to start the app at the following UTC times: 
 - **April 17 - Run \#6 to \#9**: 18:45, 19:45, 21:20, and 22:55.
 - **April 18 - Run \#10 to \#19**: 0:30, 2:05, 3:55*, 5:25*, 6:50, 8:25, 10:00, 11:35, 13:10, and 14:45.

App runs with an asterisk start time include both illuminated and eclipsed phases of the orbit. This is because the start time is delayed until after a ground pass is finished.

## Notes
The csv files in the `data` folder are downlinked from the spacecraft and their first column header are missing. This results in all headers shifted to the left by one column. This is due to [Issue #10](https://github.com/georgeslabreche/opssat-orbitai/issues/10).