# Extending a dataflow
Now that we've seen how to chain multiple firmware blocks together we will 
explore automating this procedure.

## Scenario
You have been provided a very simple sorting kernel. It takes a stream of 
intergers terminated by a negative number and puts them in order from smallest 
to largest. The problem is that it only works with an input of at most 16 
numbers, not including the terminating negative number.

We have been asked to change this kernel so that it can work with an input of at 
most 128 numbers.

The only other requirement is that is that because this upper limit might need 
to be changed later we must not hard code the logic but instead parameterise 
this limit with a compile time constant (e.g. a `static constexpr std::size_t 
upperLimit = 128;` or a macro `#define UPPER_LIMIT 128`).

