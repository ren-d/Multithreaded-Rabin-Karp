**This project was created to study the performance effects of multithreading a string search algorithm such as Rabin Karp.**
# Multithreaded-Rabin-Karp
- Splits text into sections based on the amount of threads to be used.
- Thread waits for all text to be split into sections before starting the search
- Creates a thread for each section and searches for the pattern in the section
 Waits for all threads to be done and then calculates timings and shows matches.

## Parallel structure of the code
- Started a main thread to then create multiple threads splitting the searching work across them to join them together at the end (Known as the Fork-Join method) the joining occurs once all the text has been searched through and the matches can be shown.
- There is also a Producer-Consumer Relationship between multiple threads which is explained in further slides
- Across each thread as well as the Rabin Karp class mutexes were used in order to ensure that the same text wasn’t being searched through more than once or multiple matches weren’t incorrectly counted.

## Conclusion
- Rabin Karp performs slower the more threads that are being used at the same time.
- This may be because the creation of threads and mutex locking and unlocking progressively negatively affects the performance.
- The performance decreased based on the amount of threads being used is also because with the approach I used where Rabin Karp is effectively searching through multiple small sections of a larger text, Rabin Karp’s slow performance searching through these exerts stack increasing the time taken in an O(NM) time complexity, on top of Rabin Karp’s already existing O(NM) complexity meaning the algorithm I have created is far from efficient.

