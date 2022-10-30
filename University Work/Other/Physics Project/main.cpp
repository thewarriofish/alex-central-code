#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;
#include<vector>
#include<string>
#include<stdlib.h>
#include"SerialPort.h"

char output[MAX_DATA_LENGTH];
char incomingData[MAX_DATA_LENGTH];
char trigger[MAX_DATA_LENGTH] = "showtime";
char trigger2[MAX_DATA_LENGTH] = "ex";

vector <string> cmds;
vector <string> times;
vector <string> notes;

string port = "\\\\.\\COM";
string data;
string num;
size_t i;

stringstream ss;

int main(){
    string fileName;
    cout << "Enter file name:" << endl;
    cin >> fileName;
    fstream fin;
    fin.open(fileName.c_str());
    if(fin.is_open())
    {
        while(getline(fin, data))
        {
            string cmd, time, note;
            ss.str(data);
            ss >> cmd >> time >> note;
            cmd = 'C'+cmd;
            time = 'T'+time;
            note = 'N'+note;
            cmds.push_back(cmd);
            times.push_back(time);
            notes.push_back(note);
            ss.clear();
        }
        cout << "File loaded." << endl << endl;
        fin.close();
    }
    else
    {
        cout << "File not found." << endl << endl;
        return -1;
    }
    cout << "Enter port number." << endl;
    cin >> num;
    port += num;
	SerialPort arduino(port.c_str());
	if(arduino.isConnected()){
		cout<<"Connection made"<<endl<<endl;
	}
	else{
		cout<<"Error in port name"<<endl<<endl;
	}
	Sleep(100);
	arduino.readSerialPort(output, 8);
	while(strcmp(output,trigger) != 0)
    {
        Sleep(100);
        arduino.readSerialPort(output, 8);
    }
    memset(output, 0, MAX_DATA_LENGTH);
	while(arduino.isConnected()){
		for(i = 0; i < cmds.size(); i++)
        {
            Sleep(100);
            arduino.writeSerialPort((cmds[i]).c_str(), (unsigned int) cmds[i].size());
            arduino.writeSerialPort((times[i]).c_str(), (unsigned int) times[i].size());
            arduino.writeSerialPort((notes[i]).c_str(), (unsigned int) notes[i].size() + 1);
            arduino.readSerialPort(output, MAX_DATA_LENGTH);
           if(output[0] != '\0')
           {
            cout << output << endl;
           }
            memset(output, 0, MAX_DATA_LENGTH);
        }
	}
	return 0;
}
