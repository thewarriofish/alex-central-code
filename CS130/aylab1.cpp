/* Alex Yu */
/* Review Lab */
/* TAs: Chris Muncey and Zach McMichael */
/* Completed 1/24/2021 */
//https://www.youtube.com/watch?v=Rz8AyXhR0RU

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

/* creates a struct for a single room*/
struct room
{
	string title;
	string descript;
	string directions[4];
	int roomNumber[4];
};

/* creates a struct for multiple rooms*/
struct rooms
{
	int numRooms;
	room* dungeon;
};

int main(int argc, char** argv)
{
/*variables to be used*/
	ifstream fin;
	string tempStr, direction, command, descript;
	stringstream ss;
	rooms x;
	int i, index, currentRoom;

/*checks for at least two arguments*/
	if(argc < 2)
	{
		cerr << "Usage: " << argv[0] << " [inputfile]" << endl;
		return(0);
	}

/*opens the roomfile named by the second argument*/
	fin.open(argv[1]);
	if(!fin.is_open())
	{
		/*automatically returns an error if file cannot be opened*/
		perror(argv[1]);
		return(1);
	}

/*counts how many rooms are in the roomfile*/
	x.numRooms = 0; //numRooms was not set to zero, so this makes sure it does
	while(getline(fin, tempStr))
	{
		if(tempStr == "~")
		{
			++x.numRooms;
		}
	}
		x.numRooms /= 3;

/*memory allocation*/
	x.dungeon = new room[x.numRooms];
	
/*resets file for further use*/
	fin.close();
	fin.open(argv[1]);

/*initializes rooms of the dungeon*/
	for(i = 0; i < x.numRooms; ++i)
	{
/*sets name of room*/
		getline(fin, tempStr);
		x.dungeon[i].title = tempStr + "\n";
		getline(fin, tempStr);

/*sets description of room*/
		while(getline(fin, tempStr))
		{
			if(tempStr == "~")
			{
				break;
			}
			descript += tempStr + "\n";
		}
		x.dungeon[i].descript = descript;
		descript = "";	

/*sets exits of room*/
		x.dungeon[i].directions[0] = "n";
		x.dungeon[i].roomNumber[0] = -1;
		x.dungeon[i].directions[1] = "e";
		x.dungeon[i].roomNumber[1] = -1;
		x.dungeon[i].directions[2] = "s";
		x.dungeon[i].roomNumber[2] = -1;
		x.dungeon[i].directions[3] = "w";
		x.dungeon[i].roomNumber[3] = -1;
		while(getline(fin, tempStr)) //ensures every exit is accounted for
		{
			if(tempStr == "~")
			{
				break;
			}
			ss.str(tempStr);
			ss >> direction >> index;
			if(direction == "n")
			{
				x.dungeon[i].roomNumber[0] = index;
			}
			if(direction == "e")
			{
				x.dungeon[i].roomNumber[1] = index;
			}
			if(direction == "s")
			{
				x.dungeon[i].roomNumber[2] = index;
			}
			if(direction == "w")
			{
				x.dungeon[i].roomNumber[3] = index;
			}
			ss.clear();
		}
	}

/*lets the player know the code is working*/	
	cout << "Dungeon initialized with " << x.numRooms << " rooms" << '\n'; //indicates to player dungeon is ready
	cout << "n, s, e, w to move, l to look, q to quit. Have fun!" << '\n'; //every game needs a tutorial
	currentRoom = 0; //starting room

/*the code to take input from player and run game*/
	while(true)
	{
		cin >> command;
		if(command == "q") //code to quit game
		{
			cout << "Thanks for playing!" << endl;
			break;
		}
		else if(command == "l") //code to examine room
		{
			cout << x.dungeon[currentRoom].title << "\n" << x.dungeon[currentRoom].descript << "\n";
			for(i = 0; i < 4; ++i) //checks to see if exits exist, i.e. isn't -1
			{
				/*only prints existing exits*/
				if(x.dungeon[currentRoom].roomNumber[i] != -1)
				{
					cout << x.dungeon[currentRoom].directions[i] << " " << x.dungeon[currentRoom].roomNumber[i] << "\n";
				}
			}
		}
		else if(command == "n") //code to move north
		{
			if(x.dungeon[currentRoom].roomNumber[0] != -1) //moves player to different room
			{
				currentRoom = x.dungeon[currentRoom].roomNumber[0];
			}
			else //error message if exit doesn't exist
			{
				cout << "That's a wall." << endl;
			}
		}
		else if(command == "e") //code to move east
		{
			if(x.dungeon[currentRoom].roomNumber[1] != -1) //moves player to different room
			{
				currentRoom = x.dungeon[currentRoom].roomNumber[1];
			}
			else //error message if exit doesn't exist
			{
				cout << "That's a wall." << endl;
			}
		}
		else if(command == "s") //code to move south
		{
			if(x.dungeon[currentRoom].roomNumber[2] != -1) //moves player to different room
			{
				currentRoom = x.dungeon[currentRoom].roomNumber[2];
			}
			else //error message if exit doesn't exist
			{
				cout << "That's a wall." << endl;
			}
		}
		else if(command == "w") //code to move west
		{
			if(x.dungeon[currentRoom].roomNumber[3] != -1) //moves player to different room
			{
				currentRoom = x.dungeon[currentRoom].roomNumber[3];
			}
			else //error message if exit doesn't exist
			{
				cout << "That's a wall." << endl;
			}
		}
		else //error message if anything other than allowed commands is inputed
		{
			cout << "Invalid command" << endl;
		}
	}

	delete[] x.dungeon; //deletes allocated memory to prevent leaks
	return(0); 
}
