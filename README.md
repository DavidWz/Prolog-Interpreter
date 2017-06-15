# Prolog-Interpreter
This is a simple prolog interpreter written in C++.  
It only supports the very basics of prolog, i.e. no `not`, `;`, etc.  
Currently, only 8-bit signed integer data types are natively supported. However they will simply be parsed to a function called `i8` with 8 arguments for every bit (either `b0` or `b1`). Basic functions on those integers are defined in `examples/int.pl`.  
Some other runnable example programs can be found in the `examples` folder as well.  

# Getting started
Compile the prolog interpreter by simply calling `make`.  
Then, run the interpreter on a prolog program like so `./PrologInterpreter examples/list.pl`.  
Within the interpreter, put in queries like so `?- bubbleSort([s(o), s(s(o)), o], X).`  
You will either get an answer of the form `X = ...` or simply `False.`. If there might be additional answers to your query, the interpreter will wait for you to press `;` followed by enter to continue evaluating the query. If you press anything else (e.g. simply pressing enter) the interpreter will stop evaluating that query and wait for your next query.  
You can quit the program by typing `:quit`.  
