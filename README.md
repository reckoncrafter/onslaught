# Onslaught!

![Gameplay Screenshot](gameplay.png)
*Onslaught running on [cool-retro-term](https://github.com/Swordfish90/cool-retro-term)*

Onslaught is a retro-style shooter using ncurses as it's engine.

It only requires an ncurses-compatible terminal emulator, and the ncurses library;

---

**This currently only works on Linux.**

I want to rewrite it in JS so it can be played in a web browser, but I don't know JS all that well.

If you have Windows 10, and you really want to play my stupid game, try using the [Windows Subsystem for Linux](https://docs.microsoft.com/en-us/windows/wsl/install)

---

## Building
Clone the git repository

`git clone https://github.com/reckoncrafter/onslaught.git`

`cd onslaught`

Build the binary

`make`

Launch

`./onslaught`

## Gameplay

The `P>` is the player's avatar.
Using the arrow keys allows the player to move left and right, and to jump, where the player will then gradually decend to the floor.

The spacebar shoots pellets in whatever direction the player is facing.

The `!`'s are the enemies that move across the screen. The `SCORE` count goes up for each `!` hit, and the `FAILS` count goes up for each `!` that makes it to the other side of the screen.
