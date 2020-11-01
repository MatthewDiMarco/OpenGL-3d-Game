To compile the program, create a directory called “build”. Inside this directory,
Enter the commands “cmake ..” followed by “make”. The final executable should be
located inside bin/main and should be titled “main__v1”.

Source Files:
game.cpp - this file contains the main algorithm, game loop and entities.
entity.hpp - this file contains a series of classes describing entities that can
	     exist in the game.

Controls:
	WASD: Move
	Mouse: Look around
	E: Interact with items, when close enough
	R: Restart
	O: Toggle light-mode
	P: Toggle perspective and orthographic projection
	K-L: increase and decrease light attenuation, respectively

Game Objective:
	Collect the floating wooden boxes to unlock the big metal door.
	There are 4 in total to be collected. Interact with them using 'E'.

Enemy: 
	There is a tall dark figure in pursuit of you. If you come into contact
	with it, you die. Press 'R' to restart.
