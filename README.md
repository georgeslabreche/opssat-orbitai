![OPS-SAT Mission Patch](https://github.com/georgeslabreche/opssat-orbitai/blob/main/pub/images/ops-sat_mission_patch.png?raw=true)

# OPS-SAT OrbitAI
## Description
The use of Artificial Intelligence (AI) is of rising interest for space-segment applications despite limited technology demonstrators on-board flying spacecrafts. Past missions have restricted their experience with AI to inferring models that were trained on the ground prior to being uplinked to a spacecraft. The OrbitAI experiment pushes the envelope by breaking away from this trend and shifting ML training from a ground activity to an autonomous in-flight operation. Running on-board the OPS-SAT spacecraft, the experiment uses online Machine Learning (ML) algorithms to train fault detection, isolation, and recovery (FDIR) models that can be used to protect the on-board camera’s lens against exposure to sunlight. One model achieved 89% balanced accuracy in its predictions with an F1 score of 95%.

# Citation
We appreciate citations if you reference this work in our upcoming scientific publication. Thank you!

## APA
Labrèche, G., Evans, D., Marszk, D., Mladenov, T., Shiradhonkar, V., Soto, T., & Zelenevskiy, V. (2022). OPS-SAT Spacecraft Autonomy with TensorFlow Lite, Unsupervised Learning, and Online Machine Learning. _2022 IEEE Aerospace Conference._

## BibTex
```
@article{LabrecheIEEEAeroconf2022,
  title={OPS-SAT Spacecraft Autonomy with TensorFlow Lite, Unsupervised Learning, and Online Machine Learning},
  author={Georges Labrèche and David Evans and Dominik Marszk and Tom Mladenov and Vasundhara Shiradhonkar and Tanguy Soto and Vladimir Zelenevskiy},
  journal={2022 IEEE Aerospace Conference},
  year={2022}
}
```

## Repository Structure
- **Mochi**: C++ implementation of online machine learning algorithms.
- **RandomForest**: C++ implementation of the Random Forest algorithm.
- **ipk**: Script and guideline to generate a ZIP file and then an IPK file to install the experiment on the Satellite Experimental Processing Platform (SEPP) on-board OPS-SAT.
- **nmf/space-app**: The OrbitAI [NMF](https://github.com/esa/nmf-mission-ops-sat) App.
- **pub**: Publication assest, i.e. R scripts, plots, and diagram files.
- **results**: Mission flight logs, logged training data, and serialized models from the experiment's initial runs on-board the OPS-SAT spacecraft.
- **sandbox**: A collection of scripts and experiments conducted while the project was being scoped.
## Getting Started
1. Initialize and update the Git submodules: `git submodule init && git submodule update`.
2. Read the READMEs in the `nmf/space-app`, `Mochi`, and `ipk` directories.
