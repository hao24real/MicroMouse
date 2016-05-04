There is a trouble with the code when I trying to reset the value of encoder count.
The encoder only reset when the motor stop. The code is doing this task is in the pid.c
So try to make the mouse stop some where to reset the encoder. Other wise we can get encoder count overflow.