#include <stdlib.h> // rand() function
#include <unistd.h> // usleep() function
#include <string> // string type
#include "../headers/logic.h" // Logic class
#include "../headers/test_cam.h" // Camera class
#include <stdio.h> // printf(), fflush() functions
#include <wiringSerial.h> // serialOpen(), serialPutchar() functions
#include <fstream> // ofstream type, .open() function  
#include <iostream> // cout function
#include <ctime>


using namespace std;

using namespace raspicam;

Logic *logic = new Logic; // Includes class from car.h
Test_Cam *test_cam = new Test_Cam;


int serial_port; // Serial port number

int usb_port; // Usb port number

char stat[5] = {'0','\0','\0','\0','\0'}; /*
	 		   							  * Agent status:
	 		   							  * stat[0] - cross numeber. If it isn't cross, it sets 0 (1, 2, .., 5 - possible cross numbers) and next 3 characters is '\0';
										  * Next 3 characters:
										  * CRS - if it's on the cross  and don't go right;
										  * RGT - if it's on the cross and only go right;
										  * WT0, WT1, .., WT8 - if someone is on the cross with status CRS, where 0, 1, .., 8 - number in waiting list.
										  * Alwayes set stat[4] = '\0' (end of line).
										  */
int num = -1; // Number in line, when someone on the cross.
ofstream fout; // Out stream in file.
char stat_list[9][3]; // Statuses list.
int id = -1; // Own id.


// Arduino and xbee connection.
bool setup()
{
	cout << "Arduino connection ... ";
	if ((serial_port = serialOpen ("/dev/ttyS0", 9600)) < 0)   // Open serial0 port with 9600 baudrate.
	{
		cout << "Unable to connect\n";
		return false;
	}
	cout << "Successful" << endl;
/*
	cout << "Xbee connection ... ";
	if ((usb_port = serialOpen ("/dev/ttyUSB0", 9600)) < 0)   // Open ttyUSB0 port with 9600 baudrate.
	{
		if ((usb_port = serialOpen ("/dev/ttyUSB1", 9600)) < 0)   // Open ttyUSB1 port with 9600 baudrate.
		{
			cout << "Unable to connect\n";
			return false;
		}
	}
	cout << "Successful" << endl;
*/	return true;
}


/*
 * Gets own id in start of life.
 * Transmits messages "NEW!" , while don't recieves "idSET!" (id = 1, 2, .., 10).
 */
void getid()
{
	while (1)
	{
		serialFlush(usb_port);
		char info[6];
		int j = 0;
		cout << "Transmits: " << "NEW!" << endl;
		serialPutchar(usb_port, 'N');
		fflush(stdout);
		serialPutchar(usb_port, 'E');
		fflush(stdout);
		serialPutchar(usb_port, 'W');
		fflush(stdout);
		serialPutchar(usb_port, '!');
		fflush(stdout);
		usleep(500000);
		unsigned char rx;
		rx = serialGetchar(usb_port);
		if (rx - '0' >= 0 && rx - '0' <= 9)
		{
			info[j] = rx;
			fflush(stdout);
			j++;
			while (1)
			{
				rx = serialGetchar(usb_port);
				if (rx > 32 && rx < 127)
				{
					info[j] = rx;
					j++;
				}
				else break;
				fflush(stdout);
				if (j == 5) break;
			}
			info[5] = '\0';
			if (info[1] == 'S')
			{
				cout << "Recieves: " << info << endl;
				id = info[0] - '0';
				cout << "Set ID: " << id << endl;
				return;
			}
		}
		else cout << "Trying again\n";
		usleep(500000);
	}
}


/*
 * Sends command to arduino.
 */
void sendcmd(string str)
{
	int i = 0;
	cout << "Command: " << str << endl;
	while(1)
	{
		serialPutchar(serial_port, str[i]);
		fflush(stdout);
		if (str[i] == '!') return;
		i++;
	}
}

/*
 * Sends status to main XBEE.
 * Transmits message "idSTAT!", while don't recieves "idOK!" (where id - own id, STAT - status
 */
void sendstat()
{
	while (1)
	{
		serialFlush(usb_port);
		char info[5];
		int j = 0;
		cout << "Transmits: " << id << stat << "!" << endl;
		fflush(stdin);
		serialPutchar(usb_port, id + '0');
		fflush(stdout);
		serialPutchar(usb_port, stat[0]);
		fflush(stdout);
		serialPutchar(usb_port, stat[1]);
		fflush(stdout);
		serialPutchar(usb_port, stat[2]);
		fflush(stdout);
		serialPutchar(usb_port, stat[3]);
		fflush(stdout);
		serialPuts(usb_port, "!");
		fflush(stdout);
		usleep(500000);
		unsigned char rx;
		rx = serialGetchar(usb_port);
		if (rx - '0' == id)
		{
			info[j] = rx;
			j++;
			while (1)
			{
				rx = serialGetchar(usb_port);
				if (rx > 32 && rx < 127)
				{
					info[j] = rx;
					j++;
				}
				else break;
				if (j == 4) break;
			}
			info[4] = '\0';
			if (info[1] == 'O')
			{
				cout << "Recieves: " << info << endl;
				return;
			}
			else
			{
				cout << "Can not translate: " << info << endl;
			}
		}
		else
		{
			cout << "Trying again\n";
		}
		usleep(500000);
	}
}

/*
 * Gets list of statuses
 * Transmits "idross_idINFO!", while don't recieves "idid_istat_i...!"
 * (where id - own id, cross_id - cross number (from 1 to 5), id_i - agent's id that on the cross or before right now and its status stat_i).
 * Returns true, if something changes and rewrite stat_list or if recieved only "id!" (means that cross is free), else return false.
 */
bool getstatlist(char cross)
{
	while (1)
	{
		serialFlush(usb_port);
		char info[35];
		int j = 0;
		int time = 0;
		cout << "Transmits: " << id << cross << "INFO!" << endl;
		serialPutchar(usb_port, id + '0');
		fflush(stdout);
		serialPutchar(usb_port, cross);
		fflush(stdout);
		serialPuts(usb_port, "INFO!");
		fflush(stdout);
		usleep(500000);
		unsigned char rx;
                rx = serialGetchar(usb_port);
                if (rx - '0' == id)
                {
                        info[j] = rx;
                        j++;
                        while (1)
                        {
                                rx = serialGetchar(usb_port);
                                if (rx > 32 && rx < 127)
                                {
                                        info[j] = rx;
                                        j++;
                                }
				else break;
                                if (j == 34 || rx == '!') break;
                        }
                        info[34] = '\0';
			cout << "Recieves: " << info << " ~~~ value = " << j <<  endl;
			if ((j - 2) % 4 == 0)
			{
			bool change = false;
			for (int k = 1; k < j - 1; k = k + 4)
			{
				if ((info[k+1] != stat_list[info[k] - '0' - 1][0]) || (info[k+2] != stat_list[info[k] - '0' - 1][1]) || (info[k+3] != stat_list[info[k] - '0' - 1][2]))
				{
					change = true;
					break;
				}
			}
			if (j == 2)
			{
				for (int k = 0; k < 10; k++)
				{
					stat_list[k][0] = '\0';
					stat_list[k][1] = '\0';
					stat_list[k][2] = '\0';
				}
				return true;
			}
			if (change)
			{
				int l = 1;
				for (int k = 0; k < 10; k++)
				{
					if (info[l] - '0' == k)
					{
						stat_list[k][0] = info[l+1];
						stat_list[k][1] = info[l+2];
						stat_list[k][2] = info[l+3];
						l = l + 4;
					}
					else
					{
						stat_list[k][0] = '\0';
						stat_list[k][1] = '\0';
						stat_list[k][2] = '\0';
					}
				}
				return true;
			}
			return false;
		}
		else
                {
                        cout << "Trying again\n";
                }

		}
		else
                {
                        cout << "Trying again\n";
                }
		usleep(500000);
	}
}

/*
 * Change status in case of moving through the cross.
 */
void changestat(char rgt, char cross)
{
	int i = 0;
	int max = -1;
	bool busy = false; // if someone on the cross - true
	bool right = false; // if someone turning right - true
	//if (!getstatlist(cross)) return; // if nothing changed return;
	stat[0] = cross;
	while (i < 9)
	{
		if (stat_list[i][0] != '\0')
		{
			if (stat_list[i][0] == 'C')
			{
				busy = true;
			}
			else if (stat_list[i][0] == 'W')
			{
				if (max < stat_list[i][2] - '0')
				{
					max = stat_list[i][2] - '0';
				}
			}
			else if (stat_list[i][0] == 'R')
			{
				right = true;
			}
		}
		i++;
	}
	if (num > 0) // If in line.
	{
		if (!busy) // If cross is free.
		{
			num--;
			stat[3] = num + '0';
		}
		return;
	}
	if (num == 0 && busy) return; // If it first in line, but cross still busy.
	if (busy) // if cross is busy and agent not in line.
	{
		num = max + 1;
		stat[1] = 'W';
		stat[2] = 'T';
		stat[3] = num + '0';
	}
	else if (right) // If someone turning right on the cross.
	{
		if (rgt == 'r') // If agent turn too.
		{
			num = -1;
			stat[1] = 'R';
			stat[2] = 'G';
			stat[3] = 'T';
		}
		else
		{
			num = max + 1;
			stat[1] = 'W';
			stat[2] = 'T';
			stat[3] = num + '0';
		}
	}
	else if (rgt == 'r')
	{
		num = -1;
                stat[1] = 'R';
                stat[2] = 'G';
                stat[3] = 'T';
	}
	else
	{
		num = -1;
		stat[1] = 'C';
		stat[2] = 'R';
		stat[3] = 'S';
	}
}


/*
 * Extract command from cmds.
 * If ct0. - sets stat "0". If ctN. where N != 0 it call changestat() function.
 * When extratc command seems like lft. call  test_cam->...
 */
void cmd(string str)
{
	while (str.length() > 0)
	{
		string com;
		int i = 0;
		do
		{
			com[i] = str[i];
			i++;
		}
		while (str[i] != '.');
		str.erase(0, i + 1);
		if (com[0] == 'c' && com[1] == 't')
		{
			char cross_num = com[2];
			com = "";
			int i = 0;
			do
			{
				com[i] = str[i];
				i++;
			}
			while (str[i] != '.');
			str.erase(0, i + 1);
			if (cross_num == '0')
			{
				stat[0] = cross_num;
				stat[1] = '\0';
				stat[2] = '\0';
				stat[3] = '\0';
				cout << "                                                             STATUS:  " << stat << endl;
				//sendstat();
			}
			else
			{
				do
				{
					changestat(str[0], cross_num);
					cout << "                                                              STATUS:  " << stat << endl;
					//sendstat();
					usleep(500000);
				}
				while (stat[1] == 'W');
			}
		}
		else
		{
			cout << "                                                         STATUS:  " << stat << endl;
		}
		if (com[0] == 'l')
		{
			test_cam->left(serial_port);
		}
		else if (com[0] == 'r')
		{
			test_cam->right(serial_port);
		}
		else if (com[0] == 'f')
		{
			test_cam->forward(serial_port, com[4] - '0');
		}
	}
}

/*
 * Loop.
 * When path passed wait 2 seconds.
 */
int main()
{
	fout.open("logs.txt");
	cout << "Setup ... " << endl;
	if (!setup())
	{
		cout << "ERROR\n";
		return 0;
	}
	logic->start();
	cout << "Done" << endl;
//	test_cam->line_detection();
	//cout << "Recieves own id ..." << endl;
	//getid();
	//cout << "Done" << endl;
	time_t t_start = time(0);
	time_t life_time = 0;
	cout << "Path: \n" << logic->getcoord() << endl;
	cout << "Commands: \n" << logic->getcmds() << endl;
	cmd(logic->getcmds());
	usleep (2000000);

	while (true)
	{
		cout << endl << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << endl << endl;
		if ((time(0) - t_start) / 60 >= 10 || life_time >= 30)
		{
			logic->finish();
			cout << "Path: \n" << logic->getcoord() << endl;
			cout << "Commands: \n" << logic->getcmds() << endl;
			cmd(logic->getcmds());
			cmd("lft.fwd(1).rgt.");
			cout << endl << endl << "FUNCTIONING TIME: " << life_time + (time(0) - t_start) / 60 << " minutes" << endl << endl;
			if (life_time >= 30) break;
			else life_time += (time(0) - t_start) / 60;
			usleep (180000000);
			logic->start();
			t_start = time(0);
			cout << "Path: \n" << logic->getcoord() << endl;
			cout << "Commands: \n" << logic->getcmds() << endl;
			cmd(logic->getcmds());
			cout << endl << endl << "FUNCTIONING TIME: " << life_time + (time(0) - t_start) / 60 << " minutes" << endl << endl;
			usleep (2000000);
		}
		else
		{
			logic->newpath();
			cout << "Path: \n" << logic->getcoord() << endl;
			cout << "Commands: \n" << logic->getcmds() << endl;
			cmd(logic->getcmds());
			cout << endl << endl << "FUNCTIONING TIME: " << life_time + (time(0) - t_start) / 60 << " minutes" << endl << endl;
			usleep (2000000);
			if (life_time + (time(0) - t_start) / 60 >= 30) life_time += (time(0) - t_start) / 60;
		}
	}
	return 0;
}

