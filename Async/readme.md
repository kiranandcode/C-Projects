# Async Functions in C
## JS inspired Construct

So recently, I've been programming a lot in Node.js for a work project. In preparation for this task,
I'd been looking around at the basics of node, and was suprised to find that it was single threaded. Unline the thread pooled server frameworks I had used prior (Django and Tomcat), Node js used a single thread.

This was immensely interesting to me as the thought of restricting workflow to 1 worker thread was not something I had expected to increase speed.

Now, while I haven't yet implemented any cross-platform multithreading in C (although 2 side projects I'm working on currently are heading in that direction) I thought it would be fun to try and dabble in some async programming in C and setting up the constructs to possibly use it as a reusable section of code.

So essentially the threading in this system will consist of a protothread like system, where a sequence of task objects are passed to the thread manager.

The thread manager will keep the "task" in the task pool while each time the task is executed, it doesn't return an exit code.

Once a task returns, the task manager will run the next section of code in the thread pool, repeating this process.


