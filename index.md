---
layout: default
---

Welcome to Gopiandcode's C-projects Github page - this is just an experimental wrapper to the github page where I upload all my C code.

Now that you're here, why not stick around and listen to some of the projects you may find on this Github page?

**I assure you, it will be fun!**

# Snippets of Interest

## [Neural Network](https://github.com/Gopiandcode/C-Projects/tree/master/MatrixProgram/neural_networks)
Finally got round to building a neural network in C. A more complete description can be found at the link. Building this was quite enjoyable, particularly reaching the end and running it and watching the network train. The neural network structures in the library can be used in a variety of situations, and could be tweaked at a later date to support more architectures of networks - but the proof of concept program built by the application is intended to be used with a csv formatted version of the MNIST handwriting dataset. It als uses a matrix library I built as well(see below).

## [Andres_problem](https://github.com/Gopiandcode/C-Projects/blob/master/Andres%20problem.c)
This was one of the first things I had ever programmed in C - this was a program to calculate all the possible ways of summing a set of numbers to produce a specified value. At the time, I knew nothing of dynamic programming, or any other programming paradigm, and somehow crudely stuck together a solution using recursion. It worked given the question set my friend had presented me, however for any larger values was extremely slow.

## [Scope Testing](https://github.com/Gopiandcode/C-Projects/blob/master/ScopeofVariableTesting.c)
This was another one of my earlier code snippets - this was written while I was trying to get the hang of how scope worked in C and how the extern keyword was to be used - at the time I hadn't realized that the extern keyword would allow linking across files and had instead stumbled upon it's utility in allowing access to global scope variables that had been shadowed in local scope.

## [First Class Functions](https://github.com/Gopiandcode/C-Projects/blob/master/first_class_functions.c)
This was an experiment into how one might implement a first class functional system into C - numerous forums online had claimed it was impossible, yet I thought I could see a way to implement such a system - so I did. This isn't entirely complete, however, implementing it taught me numerous things about how such a task would be done - extensions of the knowledge I gained here can be found in the [list.c](https://github.com/Gopiandcode/C-Projects/blob/master/GraphUtils/list.c) file in my graph library. At the time I was building this, I was heavily involved in a Javascript project and was learning Haskell, so the resulting system had traits from both of those languages (see promises and thunks).

## [Monte Carlo](https://github.com/Gopiandcode/C-Projects/tree/master/MonteCarloStateSearch) 
This was a solution to a problem given as a university assignment - we were given a board of moles and had to identify the optimal selection of hits to clear the board in the minimum number of steps. My initial solution was using djikstra, and it worked fine - however a friend of mine stated that a better way to solve the algorithm was to use a montecarlo search. Eager to stretch my C muscles, and to learn this alternate method, I researched into the MonteCarlo method and implemented a version in C.

## [Graph Algorithms](https://github.com/Gopiandcode/C-Projects/tree/master/GraphAlgorithms)
This was my first attempt at implementing general purpose graph algorithms in C. It also documents my first experience with disjoint set data structures - however, my implementation didn't use a heap-like structure but a linked list implementation given that at the time I didn't know of alternate implementations.

## [Regex Parser and Evaluator](https://github.com/Gopiandcode/C-Projects/tree/master/FiniteAutomata)
I'm quite proud of this one, it certainly took me a while to fully wrap my head around how to parse the formula while at the same time trying to run Thomson's algorithm. It's part of one of my ongoing projects to implement a transpiler/compiler in C to transpile my own superset variant of C back into plain C - the first step is creating a lexical parser, and to do that, it would be easiest to describe such a system using Regular Expressions. As such I tried my hand at building a regular expression evaluator - currenlty it uses a NFA, and doesn't support the quality of life extension features found in standard regex - however implementing those should be easy as running a preprocessing command on the input.

# Library stuff

## [General Data Structure Library](https://github.com/Gopiandcode/C-Projects/tree/master/GopiandCode_C_Library)
This was one of the first library-like collection of components I made - having learnt how to program in C, and enthralled by the capabilities it provided in implementing your own data structures, I attempted to create my own C library. While all the components in the library function as intended and as expected, If I were to try implementing it again, the code style would be significantly different.


## [Matrix Library](https://github.com/Gopiandcode/C-Projects/tree/master/MatrixProgram)
---This is an ongoing project to implement a functional matrix library into C, with the hope that I may utilize it to implement ML algorithms in the summer when I have more free time.---
Edited: Rebuilt the library from the ground up. Over the summer I read C: Interfaces and Implementations, which is a highly recommended book for those attempting to build more reusable software in C. With that new knowledge in hand, I rebuilt the library following the pointers I had learn't from the book. Obviously, a matrix library is only as good as the projects it's used in, and so I built... a [neural network](https://github.com/Gopiandcode/C-Projects/tree/master/MatrixProgram/neural_networks)!


## [Graph Library](https://github.com/Gopiandcode/C-Projects/tree/master/GraphUtils)
One of the newest additions to my collection of C libraries - this was constructed by my desire to reinforce my knowledge of graph algorithms and to provide a general purpose library to use for graph related tasks. As one of my latest libraries, it has a particularly different code style to the others, and I feel, given my greater knowledge, has more reusability as compared to prior libraries. This fact was brought to my attention when I was implemnting more graph algorithms and vary rarely needed to construct new data types to represent the data.


