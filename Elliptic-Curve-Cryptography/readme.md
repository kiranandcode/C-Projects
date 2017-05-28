# Documentation of my exploration into Elliptic Curve Cryptography
## Code based off the brilliant book on the subject - "Implementing Elliptic Curve Cryptography"

None of that wishy washy RSA stuff. This is the real deal.

Ceasars shift? Psh. More like Ceasar's Salad.

Should be fun. Keep an eye on this page for updates.


## Update 1: 25th May 2017 - BigInt Implementation
Just the initial boilerplate to represent the large numbers used in this kind of cryptography.
Essentially, we're storing the values in chunks equal to the word size on the intended system.
(Why word size? - the word size is usually the natural base size for processor operations, so storing
values in terms of that size will optimize the efficiency). 


## Update 2: 26th May 2017 - BigInt functions
Added some more stuff from the book. Most of the code is unchanged, however have added my own comments.
Wrapping my head around this stuff certainly took a while - for the purpose of this book, I'll continue using
this BIGINT implementation, however, afterwards I want to implement my own arbitrary size version of the implementation.
Also have a look into how to optimize this kind of coding construct. We'll keep it simple for now to aid the digestion
of the more complex subject at hand, Elliptic curve cryptography.

## Update 3: 27th May 2017 - GCD Functionality
More code from the book, mainly implementing the extended GCD algorithm in terms of the BIGINT number system introduced earlier
into the book. While I have covered the GCD division algorithm in a First-Year CS Maths course, it was the first time I had met the 
computerized version. 

## Update 4: 28th May 2017 - Modular Inverses and Powers
Implemented more code from the book. This time added modular inverse and power functions. For the modular power function, I was able
to use loop invariants (Hooare Logic) to provide some extra proof behind it's correctness. At the momeent I don't fully understand the modular inverse code - I need to work through some examples on paper before I can gain a better understanding of the method behind that. Should be able
to add in the extra explanatory comments soon.
