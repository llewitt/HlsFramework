# Splitter
In this scenario you need to refactor an extremely simple monolithic kernel. The 
goal is to reimplement the kernel as a dataflow.

## Scenario
You have been provided a kernel which takes a stream of integers and multiplies 
each by two and three and then sums the results. Despite the obvious solution 
being to multiply the input by five and have done with it you have been asked to 
refactor it as a dataflow to allow for easier extension in the future.

All the usual rules apply, streams are terminated by negative numbers and these 
numbers should be written to the output unchanged.

