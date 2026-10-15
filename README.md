# Interactive chessboard

LED-Guided Smart Chessboard
An interactive, LED-guided smart chessboard designed for beginners. The board uses magnetic piece tracking via Hall effect sensors and provides real-time visual move guidance through an integrated LED matrix, all run by an ESP32.

# How it works:
All the chess pieces have small magnets taped on the bottom
On each square of the chessboard, there is a Hall effect sensor, which detects if a square is occupied or empty. 
So, we  calculate piece movement by polling the Hall effects twice:
1. If a Hall effect has changed from detecting something to not detecting something, then a piece has been lifted up
2. Then, if a Hall effect has changed from not detecting something to detecting something, then that piece has been placed down

As such, we have information about the location of all the pieces on the board on every move, which is stored in an 8x8 array.
From there, we iterate over every piece and run an algorithm to update its legal moves. These algorithms take advantage of the geometry of the chessboard. For example, a rook can move horizontally and vertically, so the algorithm to check rook moves checks elements in the array that are in the same row or column. 

Each square on the board is assigned a number, and the available moves of each piece are stored in an array containing these numbers. The number uniquely encodes information about the row and column of the square. The array exists as a field in the Piece object.

The numbers in that array are mapped to a 64-bit binary number that indicates which LED should be lit up. 
For example, the number 25 corresponds to the square with row 2 and column 5 (0-indexed). Each shift register controls the LEDS in a particular row, so the number 2 indicates which register ought to receive data, and the number 5 indicates which of the pins on that shift register ought to receive data. The data in question is the bit 0. The pins of the shift register are connected to the negative terminal of the LEDs. Since the positive end of the LED is always powered, upon receiving a bit 0, the LED is grounded and will light up. Otherwise, the LED is not connected and will not light up.

We iterate this algorithm over all the available moves for a piece that has been lifted up, so the player can see which moves exist in the position.
Upon checkmate, a similar algorithm is used to light up:
1. the checkmated king
2. the enemy pieces that contributed to the checkmate

Video of the Scholar's mate:


https://github.com/user-attachments/assets/231e7ca0-4020-4f57-ace3-88c942b9550d

