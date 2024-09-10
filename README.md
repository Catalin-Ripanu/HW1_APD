# HW1_APD

## Implementation

The main idea of the solution consists of creating a vector of dictionaries by the Mapper execution threads, which links the exponent to its set of perfect powers. The main purpose is for the Reducer execution threads to use this vector.

### Mathematical Formalization

```
Let the vector of dictionaries be vector<unordered_map<int, unordered_set<int>>> vecMap.
Let an element of this be unordered_map<int, unordered_set<int>> elemMap.
Let a pair from elemMap be A = (x, y), where x represents an integer and y a set.
Then y = { n in N* | there exists a k in Z such that n ^ (1 / x) = k }.
```

The length of this `vecMap` vector is equal to the number of Mapper threads since each Mapper will populate `vecMap` with its own processing, namely a dictionary.

### Key Components

1. A vector of strings is used to easily manipulate the test files from `test.txt`.
2. A single for loop launches all Mapper and Reducer execution threads to respect the assignment restriction.
3. The algorithm for checking if a number is a perfect power uses the Newton-Raphson method.
4. A small algorithm based on the binary representation of the exponent is used to accelerate power raising.
5. Verification uses a deviation (error) to obtain correct and optimal results.

### Thread Synchronization

- A mutex ensures only one thread accesses a file from the vector of files at a time.
- A barrier at the end of the `funcMapper` function waits for all Mapper threads.
- Mapper threads increment a shared semaphore counter to 'notify' Reducer threads.
- Reducer threads use `sem_wait()` and increment the semaphore counter to allow other Reducer threads to work.

### Reducer Operation

Each Reducer:
1. Completes its own `unordered_set<int>` by looking at the exponent stored in each dictionary entry.
2. Fills its output file with the size of the conceived `unordered_set`.

### Main Function and Memory Management

- All execution threads are closed respecting their start format.
- All dynamically allocated memory is rigorously freed.

### Bonus Implementation

- Templates for classes and methods were introduced.
- For this assignment, all type variables (T, E) are replaced with the primitive type 'int' in the main function.
- The `ArgumentMapper` class constructor allows assignment of any implemented algorithm through a method.

## References

1. [C Program for Newton Raphson Method](https://www.codewithc.com/c-program-for-newton-raphson-method/)
2. [Find root of a number using Newton's method](https://www.geeksforgeeks.org/find-root-of-a-number-using-newtons-method/)
3. [Find Nth root of a number using Bisection method](https://www.geeksforgeeks.org/find-nth-root-of-a-number-using-bisection-method/?ref=rp)
