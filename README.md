A simple naive implementation of memory leak detector.  
Each object is malloced() using a wrapper function xcalloc(), before that each structure should be registered with their information.  

All the objects are then treated as set of graphs , the node(object) that remains unvisited is where Memory leak happens.  

This is a very rudimentary implementation , with a lot of way around to create false Memory Leak.
