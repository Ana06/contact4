# Contact 4

The [Connect Four game](https://en.wikipedia.org/wiki/Connect_Four) implemented in C++ using the minimax algorithm, different heuristics and alpha–beta pruning. Heuristics  has been proved to be essential to be able to play in big boards moving in a reasonable period of time. Alpha-beta pruning has also been proved to reduce significantly the amount of time needed to select a movement. 

The executable used for the analysis is **contact4.cpp**. There are three types of game: human against human, human against machine and machine again machine. Taking into account all the analysis made, a playable version of the game (**contact4Juego.cpp**) has been implemented to play the human against machine version with different levels of difficulty. The machine chooses the best move that it can find in a reasonable period of time. In the case that several moves are considered to be equally good, one of them is choosen randomnly to avoid playing always the same game.

The number of rows and columns can also be choosen. Typically this game is played with 6 rows and 7 columns, but in that case the player who starts alsways win if he plays well. For example, with 7 rows and 7 columns the second player can achive a draw even if the fisrt player plays well. 

The project was Implemented in May 2014. Comments are in Spanish. **contact4-AnaMariaMartinez.pdf** is a detailed report in Spanish about the implementation and the analysis made.



## Authors

This project was developed by Ana María Martínez Gómez.



## Licence

Code published under MIT license (see [LICENSE](LICENSE)).
