#include <stdlib.h> // random() functon
#include <string> // string type
#include <sstream> // stringstream - in/out stream
#include <ctime>

using namespace std;

class Logic
{
	private:

		bool checkin[64]; // For avoidance secondary passing and looping. True, if passed.
		string coord[64]; // Keep coordinates in string with format: (x,y).
		bool arr[64][2]; // Indication of sons (2 or less): left(0) and right(1). True, if it is.
		int sons[64][2]; // Keep numbers of node's sons.
		int sum, minsum; // Passed length and the minimum length.
		string list, newlist; // Passed path and minimum path.
		char direction; // Car's direction regarding static map U - up, D - down, L - left, R - right (start point - (1,1) on the left and up corner).
		string cmds; // Commands' list.
		int first,last; // First and last points of path.

		/*
		 * Graph inicialization.
		 * Images field 10*10 squares like graph, where each nude have coordinates.
		 * Sets nudes' sons.
		 */
		void inicialization()
		{
			std::stringstream ss;
			const int count = 64;
			int k = 0; // Nude counter.
			int x = 0; // X of coordinate.
			int y = 0; // Y of coordinate.

			// Filling nudes by coordinates.
			for (int i = 0; i < 100; i++)
			{
				if (!(((x >= 1 && x <= 3) || (x >= 6 && x <= 8)) && ((y >= 1 && y <= 3) || (y >= 6 && y <= 8))))
				{
					coord[k] = "(,)";
					ss << x;
					coord[k].insert(1, ss.str());
					ss.str("");
					ss << y;
					coord[k].insert(3, ss.str());
					ss.str("");
					k++;
				}
				y++;
				if (y == 10)
				{
					y = 0;
					x++;
				}
			}

			for (int i = 0; i < count; i++)
			{
				checkin[i] = false;
				for (int j = 0; j < 2; j++)
				{
					arr[i][j] = false;
				}
			}

			// Creation of nudes' sons.
			for (int i = 0; i < count; i++)
			{
				if ((i >= 0 && i <= 4) || (i >= 6 && i <= 8) || (i >= 23 && i <= 25) || (i >= 28 && i <= 30)) sons[i][0] = i + 1;
				if ((i >= 33 && i <= 35) || (i >= 38 && i <= 40) || (i >= 55 && i <= 57) || (i >= 59 && i <= 63)) sons[i][0] = i - 1;
				if (i == 9 || i == 13 || i == 17 || i == 45 || i == 49 || i == 12 || i == 16 || i == 44 || i == 48) sons[i][0] = i + 4;
				if (i == 15 || i == 19 || i == 47 || i == 51 || i == 14 || i == 18 || i == 46 || i == 54 || i == 50) sons[i][0] = i - 4;
				if (i == 10 || i == 32 || i == 42) sons[i][0] = i - 10;
				if (i == 21 || i == 31 || i == 53) sons[i][0] = i + 10;
				if (i == 11 || i == 43) sons[i][0] = i - 7;
				if (i == 20 || i == 52) sons[i][0] = i + 7;
				arr[i][0] = true;
			}
			arr[5][0] = true;
			sons[5][0] = 6;
			arr[5][1] = true;
			sons[5][1] = 12;
			arr[41][0] = true;
			sons[41][0] = 45;
			arr[41][1] = true;
			sons[41][1] = 40;
			arr[58][0] = true;
			sons[58][0] = 57;
			arr[58][1] = true;
			sons[58][1] = 51;
			arr[22][0] = true;
			sons[22][0] = 18;
			arr[22][1] = true;
			sons[22][1] = 23;
			arr[26][0] = true;
			sons[26][0] = 19;
			arr[26][1] = true;
			sons[26][1] = 27;
			arr[27][0] = true;
			sons[27][0] = 28;
			arr[27][1] = true;
			sons[27][1] = 37;
			arr[36][0] = true;
			sons[36][0] = 35;
			arr[36][1] = true;
			sons[36][1] = 26;
			arr[37][0] = true;
			sons[37][0] = 36;
			arr[37][1] = true;
			sons[37][1] = 44;
		}

		/*
		 * Recursive function.
		 * Find minimal path out.
		 * a - where path start, b - where path finish (a,b - coordinate index in coord array)
		 */
		void path(int a, int b)
		{
			if (checkin[a]) return;
			checkin[a] = true;
			sum++;
			newlist.insert(newlist.length(), coord[a]);
			newlist.insert(newlist.length(), " -> ");
			if (arr[a][0])
			{
				path(sons[a][0],b);
			}
			if (arr[a][1])
			{
				path(sons[a][1],b);
			}
			newlist.erase(newlist.length()-4, 4);
			if (a == b)
			{
				if (sum < minsum)
				{
					minsum = sum;
					list = newlist;
				}
			}
			newlist.erase(newlist.length()-5, 5);
			sum--;
			checkin[a] = false;
		}


		/*
		 * Command defenition.
		 * Makes list of commands includes ct0. and like this.
		 * ct0. sets after cross. ct1., ct2., and others before crosses
		 */
		void parser()
		{
			std::stringstream ss;
			int x, y;
			int xold = list[1] - '0';
			int yold = list[3] - '0';
			for (unsigned int i = 9; i < list.length(); i = i + 6)
			{
				if ((xold == 3 && yold == 5) || (xold == 5 && yold == 6) || (xold == 4 && yold == 3) || (xold == 6 && yold == 4))
				{
					cmds.insert(cmds.length(), "ct1.");
				}
				else if ((xold == 1 && yold == 4) || (xold == 0 && yold == 3))
				{
					cmds.insert(cmds.length(), "ct2.");
				}
				else if ((xold == 4 && yold == 8) || (xold == 3 && yold == 9))
				{
					cmds.insert(cmds.length(), "ct3.");
				}
				else if ((xold == 8 && yold == 5) || (xold == 9 && yold == 6))
				{
					cmds.insert(cmds.length(), "ct4.");
				}
				else if ((xold == 5 && yold == 1) || (xold == 6 && yold == 0))
				{
					cmds.insert(cmds.length(), "ct5.");
				}
				else if ((xold == 0 && yold == 6) || (xold == 1 && yold == 5) || (xold == 5 && yold == 8) || (xold == 6 && yold == 9) || (xold == 3 && yold == 0) || (xold == 4 && yold == 1)  || (xold == 9 && yold == 3) || (xold == 8 && yold == 4) || (xold == 3 && yold == 4) || (xold == 4 && yold == 6) || (xold == 6 && yold == 5) || (xold == 5 && yold == 3))
				{
					cmds.insert(cmds.length(), "ct0.");
				}
				i++;
				x = list[i] - '0';
				i = i + 2;
				y = list[i] - '0';
				if (direction == 'U')
				{
					if (xold < x)
					{
						cmds.insert(cmds.length(), "rgt.fwd(1).");
						direction = 'R';
					}
					else if (xold > x)
					{
						cmds.insert(cmds.length(), "lft.fwd(1).");
						direction = 'L';
					}
					else
					{
						if (cmds[cmds.length()-2] == ')')
						{
							ss << cmds[cmds.length() - 3] - '0' + 1;
							cmds.erase(cmds.length() - 3, 1);
							cmds.insert(cmds.length() - 2, ss.str());
							ss.str("");
						}
						else
						{
							cmds.insert(cmds.length(), "fwd(1).");
						}
					}
				}
				else if (direction == 'D')
				{
					if (xold < x)
					{
						cmds.insert(cmds.length(), "lft.fwd(1).");
						direction = 'R';
					}
					else if (xold > x)
					{
						cmds.insert(cmds.length(), "rgt.fwd(1).");
						direction = 'L';
					}
					else
					{
						if (cmds[cmds.length()-2] == ')')
						{
							ss << cmds[cmds.length() - 3] - '0' + 1;
							cmds.erase(cmds.length() - 3, 1);
							cmds.insert(cmds.length() - 2, ss.str());
							ss.str("");
						}
						else
						{
							cmds.insert(cmds.length(), "fwd(1).");
						}
					}
				}
				else if (direction == 'L')
				{
					if (yold > y)
					{
						cmds.insert(cmds.length(), "rgt.fwd(1).");
						direction = 'U';
					}
					else if (yold < y)
					{
						cmds.insert(cmds.length(), "lft.fwd(1).");
						direction = 'D';
					}
					else
					{
						if (cmds[cmds.length()-2] == ')')
						{
							ss << cmds[cmds.length() - 3] - '0' + 1;
							cmds.erase(cmds.length() - 3, 1);
							cmds.insert(cmds.length() - 2, ss.str());
							ss.str("");
						}
						else
						{
							cmds.insert(cmds.length(), "fwd(1).");
						}
					}
				}
				else if (direction == 'R')
				{
					if (yold < y)
					{
						cmds.insert(cmds.length(), "rgt.fwd(1).");
						direction = 'D';
					}
					else if (yold > y)
					{
						cmds.insert(cmds.length(), "lft.fwd(1).");
						direction = 'U';
					}
					else
					{
						if (cmds[cmds.length()-2] == ')')
						{
							ss << cmds[cmds.length() - 3] - '0' + 1;
							cmds.erase(cmds.length() - 3, 1);
							cmds.insert(cmds.length() - 2, ss.str());
							ss.str("");
						}
						else
						{
							cmds.insert(cmds.length(), "fwd(1).");
						}
					}
				}
				xold = x;
				yold = y;
			}
		}

	public:

		/*
		 * Life start: moving from start point (1,1) to road (0,1).
		 */
		void start()
		{
			inicialization();
			cmds = "fwd(1).lft.";
			list = "(1,1) -> (0,1)";
			direction = 'D';
			first = 1;
		}

		void finish()
		{
			minsum = 64;
			sum = -1;
			list = "";
			newlist = "";
			cmds = "";
			last = 10;
			path(first,last);
			first = last;
			parser();
		}

		/*
		 * It rerturns list of commands.
		 */
		string getcmds()
		{
			return cmds;
		}

		/*
		 * It returns list of coordinates.
		 */
		string getcoord()
		{
			return list;
		}

		/*
		 * Creation of new path, therefore, it creates lists of coordinates and commands.
		 */
		void newpath()
		{
			minsum = 64;
			sum = -1;
			list = "";
			newlist = "";
			cmds = "";
			srand(time(0));
			while (1)
			{
				last = rand() % 64;
				if (last != 4 && last != 5 && last != 22 && last != 26 && last != 27 && last != 31 && last != 32 && last != 36 && last != 37 && last != 41 && last != 58 && last != 59)
				{
					break;
				}
			}
			path(first,last);
			first = last;
			parser();
		}
};
