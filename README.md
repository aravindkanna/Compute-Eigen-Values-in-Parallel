# Compute Eigen Values in Parallel

This is a project which is done as a part of learning Parallel Programming using openMP. If you found any error please kindly report by commenting there or you can privately mail me at [here](http://aravindkanna.github.io/#contact). Let us discuss about the project.

## How to run the code?<br>

There are two main files which we have to look at to make use of this project. They are `project.cpp` and `create.cpp`, the former being the main file and the latter being a utility file. To compile it run<br> 

`g++ project.cpp`<br>

Then it asks for the size of matrix which you want to work on. Let us say we want to work with size `nxn` and entered `n`. Now it searches for `n.txt` in the same folder. So where does this `n.txt` comes from? Here comes the savior `create.cpp`. It helps us to create a matrix of size of our choice. Just run<br>

`g++ create.cpp`<br>

then it asks for the size of matrix you want. After entering `n` it creates a matrix of dimension `nxn` and stores that in a file `n.txt` by creating it in the same folder. Now the other file uses this `.txt` file for input and does the remaining job for us.

I have plotted a sample graph by finding the total run times for different sizes of input. ![Graph](https://github.com/aravindkanna/Compute-Eigen-Values-in-Parallel/blob/master/plot.png) Here is that.
