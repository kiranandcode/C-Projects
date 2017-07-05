# Async Functions in C
## JS inspired Construct

So recently, I've been programming a lot in Node.js for a work project. In preparation for this task,
I'd been looking around at the basics of node, and was suprised to find that it was single threaded. Unline the thread pooled server frameworks I had used prior (Django and Tomcat), Node js used a single thread.

This was immensely interesting to me as the thought of restricting workflow to 1 worker thread was not something I had expected to increase speed.

Now, while I haven't yet implemented any cross-platform multithreading in C (although 2 side projects I'm working on currently are heading in that direction) I thought it would be fun to try and dabble in some async programming in C and setting up the constructs to possibly use it as a reusable section of code.

So essentially the threading in this system will consist of a protothread like system, where a sequence of task objects are passed to the thread manager.

The thread manager will keep the "task" in the task pool while each time the task is executed, it doesn't return an exit code.

Once a task returns, the task manager will run the next section of code in the thread pool, repeating this process.

Also, I can definitely feel the benefits of reading through Interfaces and Implementations. While this is nowhere near a level where it would be worthwhile for others to use, It definitely seems like a piece of reusable code that I wouldn't mind using in other projects. The structure hides the complexity away and allows the library to be easily implemented in a variety of situations.

This is unlike my other "library" like functions, which alongside having tedious apis, feel more restricted to individual use cases and I feel like I'll be unlikely to reuse them.

## Example
The example file shows how this library could be used to perform 2 "hypothetically" long tasks simultaneously, by passing control between them.

When Executed this is the result.
~~~~
Printing to 20: 0
Printing to 20: 1
Printing to 20: 2
Printing to 20: 3
Printing to 20: 4
Printing to 20: 5
Printing to 20: 6
Printing to 20: 7
Printing to 20: 8
Printing to 20: 9
Printing to 20: 10
Printing to 20: 11
Printing to 20: 12
Printing to 20: 13
Printing to 20: 14
Printing to 20: 15
completed fibnc
Printing to 20: 16
Printing to 20: 17
Printing to 20: 18
Printing to 20: 19
Fibb result: 165580141
~~~~

# Beautiful!
