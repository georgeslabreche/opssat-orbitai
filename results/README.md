# OrbitAI Results
A summary of each training runs as well as the confusion matrices and classification metrics obtained by the trained models.

## Ground Operations
Ground operations are summarized in [OPERATIONS.md](OPERATIONS.md).

Note that the csv files in the `data` folder are downlinked from the spacecraft and have an invalid header becaues of [Issue #10](https://github.com/georgeslabreche/opssat-orbitai/issues/10). Each of these files have thus been manually fixed and saved as new files with the "_headerfix" suffix.
## Results


### Run #1
Initial training of models. The polled data pool param training inputs were not diverse enough during this run:
- The PD6 sun elevation angle training inputs ranged from 0 to 0.65 radians.
- Further training runs are required to cover inputs ranges from 1.05 to 1.57 radians.
### Run #2
Continued training.
### Run #3
Continued training