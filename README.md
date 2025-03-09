# aurora-scicalc

How To Build:
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


