# OrbitAI Ground Operations
SpaceShell commands were sent to the spacecraft's Satellite Experimental Processing Platform (SEPP) in order to configure and run the OrbitAI app. These commands are written files referred to as "macros" by the Flight Control Tream (FCT). Their execution is summarized here. The macro executions and their logs are archived in the `scripts` and `logs` directories. The files operate on OrbitAI's `exp144` directory in the SEPP's file system.

## Passes
### 07330 2021-04-16 17:04 UTC

```
ESOC1        AOS_5DEG        17:06:09        37 deg        08min05s (11min47s RX)
ESOC1        LOS_5DEG        17:14:14        37 deg
TUG          AOS_5DEG        17:06:28        82 deg        09min29s (12min04s RX)
TUG          LOS_5DEG        17:15:57        82 deg
```

### 07331 2021-04-16 18:40 UTC
```
ESOC1        AOS_5DEG        18:40:31        22 deg        08min10s (11min03s RX)
ESOC1        LOS_5DEG        18:48:41        22 deg
TUG          AOS_5DEG        18:41:49        10 deg        05min45s (09min24s RX)
TUG          LOS_5DEG        18:47:34        10 deg
```

## Summary
This is a summary of FCT operations to run the OrbitAI app.

### Pass 7330
#### Macros
- The NMF was updated on the SEPP. This update enables NMF app access to the OBSW data pool parameters and thus was a necessary pre-requisite to run the OrbitAI app.
- The app's config file was updated to reduce the number of training iterations from 1,000 to 850. This was to make sure that the experiment would be finished before the next pass (i.e reducing the execution time from ~83 minutes to ~71 minutes).
- Other operations unrelated to the OrbitAI experiment were executed.
#### SCOS
- A time-tagged command was sent to the spacecraft to start the OrbitAI app after the pass had finished, at 17:17:00 UTC in "continue_train"mode.

### Pass 7331
#### 7331.OMAC.sh
- Peeked at the app's toGround folder to check if the app ran correctly prior to changing the app mode from "train" to "continue_train".
- Tarred the app's toGround folder for downlink and ground analysis.

#### 1441.OMAC.sh
- Changed the app mode from "train" to "continue_train". 

#### 1440.OMAC.sh
- Peeked the app's toGround folder again, just for good measure.

#### SCOS
Time-tagged commands were sent to the spacecraft to start the OrbitAI app at:
- 2021-04-16 XX:XX:00 UTC in "continue_train" mode.
- 2021-04-17 YY:YY:00 UTC in "continue_train" mode.
- 2021-04-17 ZZ:ZZ:00 UTC in "continue_train" mode.

### Pass 7337
#### 7337.OMAC.sh
- Peeked at the app's toGround folder to check if the app ran correctly.
- Tarred the app's toGround folder for downlink and ground analysis.

### Pass 7338
#### 7338.OMAC.sh
- Peeked at the app's toGround folder to check if the app ran correctly.
- Tarred the app's toGround folder for downlink and on-ground analysis.