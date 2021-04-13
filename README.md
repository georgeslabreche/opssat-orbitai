![OPS-SAT Mission Patch](https://raw.githubusercontent.com/georgeslabreche/opssat-orbitai/main/pub/images/ops-sat_mission_patch.png?raw=true)

# OPS-SAT OrbitAI
The OrbitAI experiment on the European Space Agency's OPS-SAT spacecraft demonstrates how on-board Machine Learning (ML) can be achieved given the availability of high processing power and access to the on-board software data pool. Past missions have limited the use of ML to that of in-flight inferences of models trained on the ground. OrbitAI breaks from this trend and pushes ML training from that of a pre-launch activity to that of an in-flight operation. To the authors' knowledge, the experiment is the first time that ML models are trained on-board a spacecraft in-orbit with real-time sensor inputs as training data.

## Repository Structure
- **Boost**: C++ implementation of Online Multi-Class LPBoost, Online Multi-Class Gradient Boost, and Online Random Forest algorithms. These were not included in the OrbitAI in the end due to unresolved serialization issues.
- **Mochi**: C++ implementation of ADAM, Adagarad RDA, AROW, SCW, NHERD, and PA algorithms. These are used by the OrbitAI app for online ML.
- **ipk**: Script and guideline to generate a ZIP file and then an IPK file to install the experiment on the Satellite Experimental Processing Platform (SEPP) on-board OPS-SAT.
- **nmf/space-app**: The OrbitAI [NMF](https://github.com/esa/nmf-mission-ops-sat) App.
- **pub**: Publication assest, i.e. R scripts, plots, and diagram files.
- **sandbox**: A collection of scripts and experiments conducted while the project was being scoped.
## Getting Started
1. Initialize and update the Git submodules: `git submodule init && git submodule update`.
2. Read the READMEs in the `nmf/space-app`, `Mochi`, and `ipk` directories.