# Aurora Scientific Calculator

In-Dev Project for performing conventional and abstract calculations on input functions such as direct evaluation, graphing, symbolic differentiation, root-finding, integration, and more via a command-line interface.

## How To Build:
This project currently requires compilation with GCC.
1. Make a new directory and cd into new directory.
```bash
mkdir Aurora && cd Aurora
```

2. Clone the repository inside the folder.
```bash
git clone https://github.com/AJQuattropani/aurora-scicalc.git
```

3. Make and cd into a build directory. Build the project using at least CMake v3.30. For example in current project folder: 
```bash
mkdir build && cd build
cmake ../aurora-scicalc
cmake --build .
```

4. Run the output executable to enter the command environment.
```bash
./aurora-scicalc
```

5. [ALTERNATIVE] Use quickbuild script to make debug and release builds in source directory. Ensure the the shell script has the necessary privileges.
```bash
chmod u+x quickbuild.sh
./quickbuild.sh
```

## How To Use:
Aurora Scientific Calculator is a script environment. Open program via the terminal with `./aurora-scicalc [optional-files]`. List addition files with the handle `.ask` to run them on startup. `.ask` files are script documents containing commands the calculator can perform.

Once you are in the environment, Aurora Scientific Calculator accepts commands from the user. These can be defining new functions, values, or operating on functions.

| Command    | Arguments                     |  Description                                                            |
|:-----------|:------------------------------|:------------------------------------------------------------------------|
| exit       | none                          |Closes the program.                                                      |
| return     | none                          |Closes current file scope.                                               |
| reset      | none                          |Resets environment variables                                             |
| set        | name VECTOR vars...           |Define a vector.                                                         |
|            | name SCALAR value             |Define a scalar.                                                         |
|            | name LINSPACE a, b, n         |Define a vector of n equal partitions between [a,b].                     |
|            | name COUNT a, b, dx           |Define a vector by counting by dx from a to b.                           |
|            | name FUNC(args) definition... |Define a function.                                                       |
|            | name PACK [function]          |Copy the function into a packed buffer.                                  |
|            | name REDUCE [function]        |Copy the function with redundancies removed.                             |
|            | name GRAD [function] [index]  |Evaluate the partial derivative of a function from the index.            |
|            | name = [function]             |Copy a function.                                                         |
|            | name = [function] (inputs...) |Evaluate the function and set variable to output.                        |
| delete     | names...                      |Flags the symbols for deletion.                                          |
| open       | "filepath"                    |Read and execute a .ask.                                                 |
| peak       | names...                      |Prints the value of the symbols from a list of names.                    |
| printenv   | none                          |Prints all environment variables.                                        |
| [function] | none                          |Prints function as string representation.                                |
|            | (input...)                    |Evaluate with the given scalar/vector arguments, print output to console.|
| [vector]   | none                          |Prints the value of the vector.                                          |
| [other]    | none                          |prints the value of the token, if it exists.                             |

## How it Works:
Aurora Scientific Calculator is a simple environment for doing math problems from the command line. It is a lightweight engine for constructing mathematical expressions which can be used for symbolic manipulation or numeric computation. The program accepts input from the console, parses the commands for keywords which are then loaded into a map to symbols. Symbols are an abstract representation of different things the program can do, or objects the program constructs. Context-modifying procedures like `set`, `delete`, or `printenv` allow for the construction of new and destruction of objects in the map, switches in environment context such as files to read from, or making global modifications to state. Readers are key words that allow for `set` to explicitly know how to read remaining arguments in the command, such as reading to construct a vector from a definition, a function from a definition, a function from another function, or a modification to an existing function.

Types of Objects:
- VECTOR: buffers for storing numerical values (SCALAR is a special type of vector with no size)
- FUNC: program representation of ordered mappings of vectors to vectors. Functions are stored as syntax trees. Each node in the tree has 0-2 children depending on the type of operation it performs, being unary (1 input), binary (2 inputs), constant (1 input), or identity (base input). Binary and unary operations are composed together in order to build the full function. When the function is evaluated with an input, operations are executed in order to leaves first to the root. Outputs of the child operations are provided as inputs to the parent, for which each node contains a pointer to a program-defined procedure for operating on inputs to generate its output. This cascades upward to the root, whose output is the complete envaluation of the function from the provided input. Storing the function as a syntax tree has the benefit of being able to do symbolic computation as well. Simplification algorithms and symbolic derivatives can be derived from the syntax tree, making the program more than a simple numeric tool, now having analytic capabilities.

More information on current and upcoming features is listed in TODO.
