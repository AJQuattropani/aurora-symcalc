# Aurora Scientific Calculator

In-Dev Project for performing conventional and abstract calculations on input functions such as direct evaluation, graphing, symbolic differentiation, root-finding, and integration.
For when Desmos just isn't cutting it!

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

4. Run the output executable. You should get a response from the console.
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

[NOT IMPLEMENTED] Once you are in the environment, Aurora Scientific Calculator accepts commands from the user. These can be defining new functions, values, or operating on functions.

Additional documentation will come as features are added.

