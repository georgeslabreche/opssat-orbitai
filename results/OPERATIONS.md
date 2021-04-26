# OrbitAI Ground Operations
SpaceShell commands were sent to the spacecraft's Satellite Experimental Processing Platform (SEPP) in order to configure and run the OrbitAI app. These commands are written files referred to as "macros" by the Flight Control Tream (FCT). Their execution is summarized here. The macro executions and their logs are archived in the `scripts` and `logs` directories. Missing macro logs indicate that the macro failed to run (e.g., due to not bitlock). The files operate on OrbitAI's `exp144` directory in the SEPP's file system.
## Summary
This is a summary of FCT operations to run the OrbitAI app.
### Pass 7330
#### 7330.OMAC.sh
- The NMF was updated on the SEPP. This update enables NMF app access to the OBSW data pool parameters and thus was a necessary pre-requisite to run the OrbitAI app.
- The app's config file was updated to reduce the number of training iterations from 1,000 to 850. This was to make sure that the experiment would be finished before the next pass (i.e reducing the execution time from ~83 minutes to ~71 minutes).
- Other operations unrelated to the OrbitAI experiment were executed.
#### SCOS
Run \#1: A time-tagged command was sent to the spacecraft to start the OrbitAI app after the pass had finished, at 17:17:00 UTC in "continue_train" mode
### Pass 7331
#### 7331.OMAC.sh
- Peeked at the app's toGround folder to check if the app ran correctly prior to changing the app mode from "train" to "continue_train".
- Tarred the app's toGround folder for downlink and ground analysis.

#### 1441.OMAC.sh
- Changed the app mode from "train" to "continue_train". 

#### 1440.OMAC.sh
- Peeked the app's toGround folder again, just for good measure.

#### SCOS
Time-tagged commands were prepared to start the the OrbitAI app during the next three passes. Release times were:
- **Run \#2**: 2021-04-16 18:46:34.710 UTC. Executed.
- **Run \#3**: 2021-04-17 03:53:01.124 UTC. Not executed due to no bitlock.
- **Run \#4**: 2021-04-17 05:26:30.186 UTC. Not executed due to no bitlock.

### Pass 7337
Via the 7337.OMAC.sh macro:
- Fetched the content of the the app's toGround folder to check if the app ran correctly.
- Tarred the app's toGround folder for downlink and ground analysis.

### Pass 7338
Via the 7338.OMAC.sh macro:
- Fetched the content of the the app's toGround folder to check if the app ran correctly.
- Tarred the app's toGround folder for downlink and on-ground analysis.

### Pass 7345
To increase the chances of capturing training inputs for PD6 elevation angle values that go above the 1.05 radians threshold the app will only run while the spacecraft is illuminated. The spacecraft is illuminated for half of the orbit (45 min).

#### 7345.OMAC.sh
- Reduced the number of iterations from 850 to 540 (from ~71 min to 45 min).
- Fetched the content of the the app's toGround folder to check if the app ran correctly.
- Other operations unrelated to the OrbitAI experiment were executed.

#### Matis
Uplinked the schedule to start the app at the following times (UTC): 
 - **April 17 - Run \#6 to \#8**: 19:45, 21:20, and 22:55.
 - **April 18 - Run \#9 to \#16**: 0:30, 2:05, 6:50, 8:25, 10:00, 11:35, 13:10, and 14:45.

### SCOS
Run \#5: A StartExperiment command was sent to start the app. For reasons not investigated, the app did not run so no new data was created. The previous pass had SCOS/Matis delay problems which could be related to the issue.

### Pass 7346
Via the 7346.OMAC.sh macro:
- Tarred the app's toGround folder for downlink and ground analysis. 
- Other operations unrelated to the OrbitAI experiment were executed.

### Pass 7352
The following was scheduled via the 7353.OMAC.sh macro but was not executed due to ground segment problems:
- Fetched the content of the the app's toGround folder to check if the app ran correctly.
- Tarred the app's toGround folder for downlink and ground analysis.
- Other operations unrelated to the OrbitAI experiment were executed.

### Pass 7353
The following was scheduled via the 7353.OMAC.sh macro but was not executed due to ground segment problems:
- Fetched the content of the the app's toGround folder to check if the app ran correctly.
- Tarred the app's toGround folder for downlink and ground analysis.
- Moved log files for later deletion to manage disk usage.

### Pass 7360
Moved to from primary (Chain-A) to secondary (Chain-B) ground station due to problems in primary.

#### MATIS
- Uplinked the schedule to start the app at the following times (UTC): 
 - **April 18 - Run \#18 to \#20**: 19:30, 21:05, and 22:40.
 - **April 19 - Run \#21 to \#31**: 0:15, 1:50, 3:25, 5:00, 6:35, 8:10, 9:45, 11:20, 12:55, 14:30, and 16:05.
- Switched from SpaceWire to CAN to attempt downlink in next pass.

### SCOS
Run \#17 at 18:14 UTC. Live StartExperiment command to start the app.
#### 7360.OMAC.sh
- Fetched the content of the the app's toGround folder to check if the app ran correctly.
- Tarred the app's toGround folder. Cound not downlink due to SpaceWire not being configured for Chain-B.
- Moved log files for later deletion to manage disk usage.
- Used the `cat` command on the serialized model files so that they could manually be recreated on the ground (because downlinking was not working in Chain-B).

### Pass 7361
#### 7361.OMAC.sh
The following commands were not necessary because they had been done in the previous pass and the app had not run since.
- Fetched the content of the the app's toGround folder to check if the app ran correctly.
- Used the `cat` command on the serialized model files.

#### SCOS
- Sent command at 18:14 UTC to start the app.

### Pass 7391

#### MATIS
Uplinked the schedule to start the app for runs \#33 to\# 35 at the following times on April 20: 20:35, 22:10, and 23:45.

#### SCOS
Run \#32: Time tagged StartExperiment command was sent to start the app at 19:00 UTC.

### Remain Passes
No other experiment runs were scheduled and the remaining macros were to inspect and fetch experiment outputs.