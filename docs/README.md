# CPS (Clicks-Per-Second) Trainer Game
**Developer**: Robert Logan

![CPS Trainer Demo](docs/CPSTrainerGamePlayClip.gif)

## Project Overview
This project serves as a technical demonstration of game loops and object-oriented design using C++.
There are a few goals that this project covers:
1. Scalable architecture through a Game State Machine using enumeration data types
1. Availability in the **CIA Triad** via 3-2-1 security strategy (*see note below*)
1. Building my own game after reading John Horton's Beginning C++ Game Programming

## Tech Stack
* **Language**: C++20
* **Library**: SFML 3.0.2
* **Integrated Development Environment (IDE)**: Visual Studio 2022, v. 17.4
* **Version Control**: Git

## Project Structure
* `assets/` - contains any graphical, audio, and font assets used in the game
* `bin/` - contains necessary *.dll* files from SFML and the *.exe* file
* `docs/` - contains supporting documentation for the project
* `include/` - contains SFML header files
* `lib/` - contains all of the SFML *.lib* files needed for a successful compile to run the game
* `obj/` - contains all the unnecessary files that Visual Studio creates when compiling the game
* `src/` - contains all *.cpp* files created, including `main.cpp`

### Security Strategy
* **3 Copies** - GitHub, Local Drive, External Flash Drive
* **2 Media Types** - Cloud, Physical/Solid State Drive
* **1 Off-site** - GitHub

## Game Concept
The title of this game is called Clicks-Per-Second (CPS) Trainer. This game has different game modes
that allow the player to test their skills in speed and precision. There are currently (as of May 10, 
2026) 4 game modes:
1. `Growing Circle` - With each successive click, the circle will grow.
1. `Shrinking Circle` - With each successive click, the circle will shrink.
1. `Alternating Click` - A left-click must be captured on the circle before a right-click can be captured.
1. `Random Spawn` - Circles spawn in random locations for the player to click on them as fast as possible.

A timer begins when the player chooses a game mode from the menu. The player must then try to click
as fast as they possibly can. Once the timer reaches 0, an average clicks-per-second is recorded and
presented to the player.

The game can be considered more of a utility, where PC gamers can utilize this tool to track and even 
train their clicks-per-second. The game is scalable and allows for another game mode to be created in order to 
pivot for other buttons, such as the `Q`, `W`, `E`, or `R` keys that are needed for game genres, such as MOBAs.
While this current version does not contain a game for the aforementioned keys, the architecture allows for easy 
addition of these keys.

## Lessons Learned
**Valuable Troubleshooting Skills**
* The IDE's breakpoint function to easily track which variables are changing as the game is running.
* Placement of various functions to allow for optimal game performance (i.e. srand outside of game loop)
* Reading the documentation for the updated SFML event handling. Horton's book covers SFML 2.6, but I chose 
to use the updated version SFML 3.0, which changed the way event handling is done.
* Using a flag variable to maintain specific sets of game logic.

## Installation
1. **Clone the repository:**
`https://github.com/RLogan-CSDev/CPS-Trainer.git`
1. Open `CPS-Trainer.sln`, set configuration to Release / x64, press `F5`

A few notes: Ensure `assets/` directory remains where it is located so that
all assets load correctly. This repo already includes the necessary SFML headers 
and libraries. This project is configured with all relative path macros in the IDE,
so it should compile straight "out of the box" once cloned.