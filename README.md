# robots
Port of the historic BSD robots game to MacOS

Robots is a historic ascii graphics game with origins unknown to me.
This code is from BSD4.2 and has been adapted to modern MacOS:
* K&R C converted to ANSI C: implicit declarations are now imported or declared, modern
argument lists, return types for all functions
* Some curses vars have been removed or renamed or turned into functions.

# Upcoming changes

## Safe teleports
More recent versions of this game introduced the concept of _safe teleports_:
They take you to a safe place but the number is limited. I would use the bonus
value earned for the number of safe teleports.

## Dynamic window size
I thought the game would adjust to the screen size but it doesn't. This needs to change.