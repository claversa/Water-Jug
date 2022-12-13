# Water-Jug
Solves the Water Jug Problem
The motivation for the Water Jug Problem comes from the general problem: given an 8-gallon jug full of water and two empty jugs of 5- and 3-gallon capacity, get exactly 4  gallons of water in one of the jugs by completely filling up and/or emptying jugs into others.
My version of this problem uses 3 jugs--A, B, and C--of variable capacity and implements a breadth-first search in its solution. Jug C always starts at capacity.
The algorithm results in the goal state (a,b,c) of Jug A with a gallons, Jug B with b gallons, and Jug C with c gallons and prints the order of pouring that accomplishes this goal state. If the state is impossible, the program will throw an error.
This breadth first search relies on the following traversal:
Pour from C to A 
Pour from B to A 
Pour from C to B 
Pour from A to B 
Pour from B to C 
Pour from A to C 
Enjoy!
