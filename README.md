# Aurora Scientific Calculator

In-Dev Project for performing conventional and abstract calculations on input functions such as direct evaluation, graphing, symbolic differentiation, root-finding, integration, and more via a command-line interface.

## How To Build:
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

5. [OPTIONAL] To generate unit test files, make a new folder at the location of the project/test directory (.../Aurora/). Follow steps to build, but instead of building from the project folder, build from the 'test' subdirectory. CMake will build multiple executable files, each for running its own tests.
```bash
mkdir test && cd test
cmake ../aurora-scicalc/test
cmake --build .
```

## How To Use:
Aurora Scientific Calculator is a script environment. Open program via the terminal with `./aurora-scicalc [optional-files]`. List addition files with the handle `.ask` to run them on startup. `.ask` files are script documents containing commands the calculator can perform.

Once you are in the environment, Aurora Scientific Calculator accepts commands from the user. These can be defining new functions, values, or operating on functions.

[TODO add list of currently implemented commands]
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

Types of Variables:
- VECTOR: buffers for storing numerical values (SCALAR is a special type of vector with no size)
- FUNC: program representation of ordered mappings of vectors to vectors
- PFUNC: memory-packed version of function

To test or experiment with interplay of elements in the program, or perform unit tests, new test files can be generated in `./test/`. CMake will generate a separate output executable for each file in this directory.


Additional documentation will come as features are added.

