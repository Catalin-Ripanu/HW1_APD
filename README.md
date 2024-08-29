# HW1_APD

Map-Reduce problem solved using Multithreading.

## Implementation

The main idea of the solution consists of creating a vector of dictionaries by the Mapper execution threads, which links the exponent to its set of perfect powers, the main purpose being for the Reducer execution threads to use this vector.

Mathematical formalization:

```
Let the vector of dictionaries be vector<unordered_map<int, unordered_set<int>>> vecMap.
Let an element of this be unordered_map<int, unordered_set<int>> elemMap.
Let a pair from elemMap be A = (x, y), where x represents an integer and y a set.
Then y = { n in N* | there exists a k in Z such that n ^ (1 / x) = k }.
```

Obviously, the length of this vecMap vector is equal to the number of Mapper threads since each Mapper will populate vecMap with its own processing, namely a dictionary.

A vector of strings is also used to more easily manipulate the test files from test.txt.

To respect the restriction from the assignment requirement, a single for loop was used to launch all Mapper and Reducer execution threads.

The algorithm that checks if a number is a perfect power uses a numerical method, namely the Newton-Raphson method. Moreover, to increase the acceleration of raising to power, a small algorithm is used that is based on the binary representation of the exponent passed as an argument. Of course, the verification uses a deviation (error) to obtain correct and optimal results.

After all these operations, the Mapper thread will populate its own dictionary with the corresponding values necessary further in the program.

Regarding the logic of thread synchronizations, a mutex is used so that only one thread accesses a file from the vector of files at a time, given by reference to the class that models the arguments of a Mapper thread.

The barrier at the end of the funcMapper function has the role of waiting for all Mapper threads, they will increment the counter of the shared semaphore so as to 'notify' the Reducer threads to start the next stage.

Each Reducer execution thread will exit the sem_wait() function and will increment, in turn, the semaphore counter in order to allow the other Reducer threads to work / write in the corresponding files.

Each Reducer completes its own unordered_set<int> by looking at the exponent stored in the key of each entry in the vector of dictionaries and fills in its own output file the size of the unordered_set conceived earlier (unordered_set was chosen as it eliminates duplicates) from the structure.

At the end, in main, all execution threads are closed respecting the format of their start.

Also, all dynamically allocated memory is rigorously freed.

For the bonus, an attempt was made to introduce templates for classes and methods. For the specific case of this assignment, all type variables (denoted T, E) are replaced with the primitive type 'int' in the main function. Additionally, the constructor of the ArgumentMapper class allows the assignment of any implemented algorithm through a method (again, for this assignment, the constructLists() method was chosen, which describes the algorithm for finding the Nth root of a given number using nthRoot() and IntPowFast()).

Some references:
https://www.codewithc.com/c-program-for-newton-raphson-method/
https://www.geeksforgeeks.org/find-root-of-a-number-using-newtons-method/
https://www.geeksforgeeks.org/find-nth-root-of-a-number-using-bisection-method/?ref=rp
