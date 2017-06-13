# Prolog-Interpreter
This is a simple prolog interpreter written in C++.  
It only supports the very basics of prolog, i.e. no `not`, `;`, no lists (you have to define lists yourself), etc.  
Oh, and did I forget to mention that it also doesn't support numbers? You have to define them yourself too :)  
Some runnable example programs can be found in the `examples` folder.  

# Getting started
Compile the prolog interpreter by simply calling `make`.  
Then, run the interpreter on a prolog program like so `./PrologInterpreter examples/list.pl`.  
Within the interpreter, put in queries like so `?- bubbleSort(ls(s(o), ls(o, nil)), X).`  
You will either get an answer of the form `True. X = ...` or simply `False.`. If there might be additional answers to your query, the interpreter will wait for you to press `,` followed by enter to continue evaluating the query. If you press anything else (e.g. simply pressing enter) the interpreter will stop evaluating that query and wait for your next query. You can quit the program by typing `:quit`.  
