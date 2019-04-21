#This program was developed for autonomus unmanned transport vechicles (UTVs).
#It's include next steps.
#Field inicialization - field's size 10*10 sqaures, each square have unique coordinate
#looks like (x,y). Also, field have "forbidden" squares that don't have coordinates,
#becouse UTVs cannot use it. All not "forbidden" squares images as oriented graph have,
#where each square is node and it have 1 or 2 sons. All of it need for finding out of
#minimal path.
#Finding out of minimal path - it's recursive function that move from node to node and
#process minimal count of nodes for path from first coordinate to last coordinate. When
#function stops, program have list of coordinates with view:
#(x_0,y_0) -> (x_1,y_1) -> .. -> (x_n,y_n).
#Commands list creation - it's time to create commads for arduino, when we have
#coordinates list. In case, when it knows where stay now and where it must move, it can
#process if it need to turn left or right, or should it move straight. So, it analizes
#coordinates list and comapre each pair of coordinates. After all, it have list of
#commands. By the way, if coordinates is part of cross, before command set cross
#indicator. The same, in situation with 4 squares, that used for moving on the cross (UTVs
#use right side moving roles, therefore, we have only 4 squares).
#After all commands list translates to commands that used by arduino and each alone 
#command transmits to arduino.
    
raspicam_cv.h instalation:

cd raspicam-0.1.6
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig


#compile maslab_cars:
make

#run maslab_cars:
./maslab_cars

