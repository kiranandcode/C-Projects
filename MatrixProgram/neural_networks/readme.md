# Neural Network in C
## A simple feedforward neural network
### Project Description
Overall this project produces a small program for training and testing a neural network on the minst data. To avoid issues with binary files
etc. (look at my ongoing PNG encoder project for a ***deeper*** understanding of my disdain for binary files), I've used a csv formatted version
of the files.

The program built using the code has a fully featured cmdline interface, although minimal file error handling protocols.
You can serialize your neural networks and save them for later use, etc.

You can build the project by changing dir to this folder, and then running:

                                 gcc main.c neuron.c ../matrix.c -lm -o <executable-name>
                                 
Yes, I could have made a make file, but the number of files in this project are quite small, so I found it to be not worth the trouble.

### Reflections
Neural Networks... Personally I find that the overhyping of neural network research areas to be a counterproductive act.
Don't get me wrong. Neural networks are interesting no doubt, and I'm definitely not a person who feels that "the complexity" of the human
mind is something outside the reach of artificial agents, but I find the levels to which neural nets are presented these days to be an
exaggeration.

Despite these concerns, I thought I'd give building a neural network a go just to see what it felt like. 

And it felt good. Sooo goood.

While the level of research required to achieve this task was quite minimal, barely scraping the tip of the iceburg, I've found that the
actual act of just going through the various steps in the feedforward neural network algorithm has given me a better understanding of the
mechanics behind the algorithm.

I built this while following through Andrew NG's Coursera course, during which he advised the use of matlab to provide suitable levels of
abstraction to fully understand the concept. Unfortunately, my brain doesn't work like that, and I often find it hard to abstractly look at
a concept if I don't have at least a loose understanding of how the underlying implementation works.

I realize this trait is somewhat of a limit, but I find that working through concepts from the ground up provides me a more grounded and
permanent understanding of the mechanics behind the system.

Sure, I know how a matrix would be multiplied, and sure, I know how to take the dot product of two vectors etc. but somehow, while working
through the matlab assignments in Andrew's course, I found that my understanding of the mechanics would be somewhat superficial, and while
I could piece together how a particular matrix multiplication would work, shortly after I would lose that understanding. When building it
up from the ground, as I had manually programmed all the loops and iterations to perform the higher abstraction calculations, I could more
easily get a grasp on the concept.

Good stuff.
