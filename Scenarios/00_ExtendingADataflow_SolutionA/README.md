# Extending a dataflow
Previously we saw how to create a dataflow and we hoped that such a structure 
would lend itself to extension. Now we will demonstrate this power!

## Scenario
You have been provided a free running kernel that multiplies numbers by three.

We have been asked to change this kernel so that only keep numbers (still 
multiplying by three but checking the result and only forwarding the resulting 
number if it is even).

We will keep the rule that negative numbers should be passed to the next stage 
unchanged for consistency.

