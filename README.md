# Battleship — 2D Arcade Shooter

> Shoot down incoming ships, dodge enemy fire, and collect health pickups to stay in the fight.

**Battleship** is a Programming Fundamentals project built with **C++17** and **SFML**. It brings together real-time movement, collision detection, scoring, and audio in a 2D arcade shooter.

## How to play

Click **PLAY** to take control of your ship. Enemies spawn directly into the action and fire at you. Return fire to score points, dodge incoming shots to protect your health, and move into health pickups to recover lost health.

| Input | Action |
| --- | --- |
| Mouse click on **PLAY** | Start playing |
| Arrow keys | Move the ship up, down, left, or right |
| Hold **Space** | Fire repeatedly |
| **Esc** | Exit in the linked repository build |

## Game features

- **Enemy spawning and combat** — Enemy ships spawn during play. Aim your shots while steering clear of incoming fire.
- **Health and recovery** — Enemy shots deduct health points on impact. Collecting a health pickup restores health.
- **Scoring and a local high score** — The linked scoring build awards 50 points per enemy hit and saves new high scores in `high.txt`.
- **Audio feedback** — Sound effects accompany the action, and the linked scoring build includes menu music and a firing sound.

## Programming concepts used

| Concept | How it applies to Battleship |
| --- | --- |
| Variables and constants | Track health, score, ship speed, and shot timing. |
| Loops and event handling | Keep the game running, process input, and update and draw game objects each frame. |
| Conditional logic | Decide when a shot can fire, whether a collision causes damage or restores health, and when to save a new high score. |
| Functions and structs | Organize enemy data and separate tasks such as spawning ships and loading or saving scores. |
| Vectors | Store active projectiles and remove them when they hit an enemy or leave the screen. |
| Randomization | Vary where enemies appear instead of spawning them at a fixed horizontal position. |
| Collision detection | Detect overlapping objects for projectile hits and health pickups. |
| Delta time and timers | Keep movement tied to elapsed time and control how often shots can be fired. |
| File I/O | Load and update the local high score with C++ file streams. |
| SFML graphics and audio | Draw sprites and text, handle keyboard and mouse input, and play music and sound effects. |

## Build the linked repository on Windows

The [GitHub repository](https://github.com/MuhammadAyyan789/PF_Project) includes a scoring build in [`battleship.cpp`](https://github.com/MuhammadAyyan789/PF_Project/blob/main/battleship.cpp). Its configuration uses **MinGW-w64** and **SFML 3.1.0**. Install matching compiler and SFML builds, then adjust these paths for your setup:

```powershell
git clone https://github.com/MuhammadAyyan789/PF_Project.git
cd PF_Project

$sfmlPath = 'C:\libraries\SFML-3.1.0'
$env:Path = "$sfmlPath\bin;C:\mingw64\bin;$env:Path"
g++ -std=c++17 battleship.cpp -o battleship.exe -I "$sfmlPath\include" -L "$sfmlPath\lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
if ($LASTEXITCODE -eq 0) { .\battleship.exe }
```

Run the game from the repository root so it can find its images, font, and audio files. Make sure any SFML DLLs beside the executable match the libraries used to build it.

> **Version note:** The health, enemy-fire, and pickup mechanics above describe the expanded version reported by the team. They were not present in the linked scoring source when it was reviewed. The repository's default VS Code task targets a separate menu demo, `battleship_menu.cpp`.

## Team

| Team member | Student ID | University email |
| --- | --- | --- |
| Muhammad Ayyan | 26L-0754 | [l260754@lhr.nu.edu.pk](mailto:l260754@lhr.nu.edu.pk) |
| Muhammad Roshaan | 24L-0992 | — |
| Jawad Ahmad | 26L-0768 | [l260768@lhr.nu.edu.pk](mailto:l260768@lhr.nu.edu.pk) |
