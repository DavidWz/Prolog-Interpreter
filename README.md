# Prolog-Interpreter
This is a simple prolog interpreter written in C++.  
It only supports the very basics of prolog, i.e. no `not`, `;`, no lists (you have to define lists yourself), etc.  
Some runnable example programs can be found in the `examples` folder.  

# Getting started
Compile the prolog interpreter by simply calling `make`.  
Then, run the interpreter on a prolog program like so `./PrologInterpreter examples/list.pl`.  
Within the interpreter, put in queries like so `?- bubbleSort(ls(s(o), ls(o, nil)), X).`  
Note however, that it currently is not usable for larger programs since it takes a really long time to finish evaluating. 
