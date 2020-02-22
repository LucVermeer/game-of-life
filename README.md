In this repository you will find the Game of Life, an idea of Dr. Conway, programmed in C++.

To compile and run the game use the following commands:
>> sudo g++ -Wall -Wextra -o grid grid.cpp
>> ./grid

You will be presented with the grid and a menu with the following options:
• Stop, exit from program
• Clean, set all cells to be dead
• Randomize, set status of each cell to random dead or alive
• One, move the grid one timestap forward
• Go, move the grid 100 timesteps forward showing the state at each step
• Move, opens another menu where the user can move the viewport
• Parameter, opens another menu where the user can set parameters:
  ◦ horizontal step size of the viewport
  ◦ vertical step size of the viewport
  ◦ probability of a cell being alive when randomizing the grid
  ◦ character representation of a life cell
  ◦ character representation of a dead cell
  ◦ optionally others
• File, allows user to load a predefined pattern of cells from a file

Interesting example of a possible flow:
>> file
>> gliderGun.txt
>> go
