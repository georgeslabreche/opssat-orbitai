# OrbitAi Onboard Online Machine Learning
Using Online ML to train FDIR models for the OPS-SAT spacecraft.

## Development Environment
1. Install ARM cross-compilers: `sudo apt-get install gcc-arm-linux-gnueabihf`
2. Include boost libraries: serizaliazation, math, and archive.
3. Fetch source code from submodules: `git submodule update --init`

## Compile
```
./clean.sh
cmake .
make
```

## rRun
`./orbita`
