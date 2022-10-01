# OS-Project
This is the code for NYU Operating System Class Lab Project. In this project, I simulate the scheduling of n processes ${P_1}$ to $P_n$ on a
single CPU, with FIFO and RR scheduling algorithm.

## Language

C++

## Assumption
I assume for simplicity that all processes are created at time 0. I also assume that for the tie-breaking rule, the alphabetical order is favored.

## Input

There are generally four inputs. Number of processes (single number); SA, or Scheduling Algo (1 means FIFO, 2 means RR); The quantum for RR (only exists when SA = 2);
The running and blocking times for each process (array)

## Output
A pretty-printed table that shows at each cycle which processes are running, which are ready, and which are blocked.
