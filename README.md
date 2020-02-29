# PinballSimulation

This program is a simulation of a  "ball" moving through a space 
with boundaries and obstacles(think about the ball in a pinball 
machine but much much more simplified).

# RULES OF MOVEMENT
=> This ball can only move sideways  and every time
it bumps into an obstacle, its direction changes 
(left -> right/ right -> left). 

=> When it reaches the edge of the grid then it hops to the next or 
previous line (according to the user's input which specifies the 
initial movement of the ball "upward or downward")
and changes its direction again(left -> right/ right -> left) .

=>This continues until it reaches the bottom or upper most row of the
grid. In that case,  
If it reaches the bottom row its movement will change to upward
and when it reaches the upper most row its movement will change to
downward.


# CHARACTERISTICS
=> The dimensions of grid are 6-by-6, the empty space is denoted by '_'

=> It contains 2 obstacles, denoted by 'O'

=> The ball is denoted by '1'


# INPUT
=>If the user gives no arguments the program will run its own testing.

=>To run the program with specific arguments the user have to 
enter 5 arguments next to ./ball

These 5 arguments would be:
1. The initial position of the ball e.g. in the form of "b2"
2. The initial direction of the ball: 'L' for left/ 'R' for right
3. The initial movement of the ball: 'U' for Upward/ 'D' for Downward
4. The positin of first obstacle e.g. in the form of "c4"
5. The positin of second obstacle e.g. in the form of "e2"

Note that the obstacles can't be in the boundaries of the grid
i.e. 1st or last row and 1st or last column

please look at the bottom printed grid to actually see the movement
as a sequence as it updates itself. In every step the program shows the updated grid 
and stops for the user to observe. 

The program will run forever until the user presses CTRL+C

Example: ./ball a3 R D b2 e4
