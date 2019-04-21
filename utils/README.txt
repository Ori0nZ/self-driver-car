commands.cpp - send commads to arduino.
You can write FWD, STP, RGT, LFT (sign '!' sets automatically before transmition).

listen.cpp - prints all that is transmited by arduino.

compile:

gcc -Wall file.cpp -o file.exe -lwiringPi

run:

./file.exe