# Hls Tutorial
The aim of this tutorial is to allow developers to bypass the complexities of 
creating a framework for using hls skipping straight to the fun stuff, 
optimising hls kernels.

The only prerequisite is a basic knowledge c/c++ (if you can write a hello world 
program from scratch you are probably good enough to get started).

The content is broken up into two categories, plus helper code that is not 
explicitly discussed in the tutorial.

## Examples
The examples are intended to be covered first to provide developers with context 
that will be needed in the hands on portion of the tutorial. This is by no means 
a rule and depending on the developers experience they could be skipped entirely, 
though I reccommend at least skimming them.

Each example demonstrates a feature of hls and what situtations these features 
are best applied. With the exception of 00_AllInOne, which I do not reccommend as 
a design pattern except as a starting point for further development, the 
examples do not represent an idealised development pathway. For example, a 
sequence of free running kernels could be refactored into a dataflow contained 
in a single kernel. The goal is to provide an opinionated set of 
reccommendations for developing firmware using hls that can be easily refactored 
to improve performance and maintability.

## Scenarios
The scenarios are intended to make up the remainder of the tutorial. Each 
scenario comes with either an open ended task, e.g. parallelise this kernel with 
a factor of eight, or a specific defect which can be fixed, e.g. fix a failing 
kernel build. For the former, at least one solution is provided and for the 
latter a single solution is provided. 

# Getting Started
## Installation
TBD

## Setup
```
mkdir HlsTutorial
cd HlsTutorial
git clone https://github.com/llewitt/HlsFramework.git
mkdir Build
mkdir Run
```

## Build
```
cd Build
cmake -DDO_SAVE_TEMPS=ON -DEMULATION_MODE_hw_emu=ON ../HlsFramework
cmake --build . --parallel 4
```

## Run
```
ctest
cd ../Run
echo $(seq 1 8190) -1 | sed "s/ /\n/g" > input.txt
XCL_EMULATION_MODE=hw_emu ../Build/Host/host.exe \
    ../Build/Targets/AllInOne_hw_emu.xclbin 
    ../HlsFramework/Examples/00_AllInOne/interfaces.json \
    ../HlsFramework/Examples/00_AllInOne/sequence.json

cat output.txt
```

