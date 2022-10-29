//Alexander Yu - CS302
//January 26, 2022
//Lab 1: Music Files

//This lab serves as a review of 202 material. Via implementation with maps, we can take input and organize it into a structured output.

#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstdio>
using namespace std;

//class implementations, taken from the lab writeup
class Song{
	public:
		string title;
		int time;
		int track;
};

class Album{
	public:
		map <int, Song *> songs;
		string name;
		int time;
};

class Artist{
	public:
		map <string, Album *> albums;
		string name;
		int time;
		int nsongs;
};


int main(int argc, char** argv)
{
	map <string, Artist *> artists; //holds all artists found in input file
	string songTitle, songArtist, songAlbum, songGenre, songTime, fileName, x; 
	int min, sec, songTrack, totalTime;
	size_t i;
	map <string, Artist *>::iterator ait; //iterators for maps
	map <string, Album *>::iterator alit;
	map <int, Song *>::iterator sit;
	if(argc != 2) //checks if command line is correct
	{
		cout << "usage: /lib_info filename.txt" << endl;
		return -1;
	}
	ifstream fin;
	stringstream ss;

	fileName = argv[1]; //sets file name

	fin.open(fileName.c_str());
	if(fin.is_open())
	{
		while(getline(fin, x)) //takes one line from input file
		{
			ss.str(x); //passes line through string stream
			ss >> songTitle >> songTime >> songArtist >> songAlbum >> songGenre >> songTrack; //places data into respective variables
			sscanf(songTime.c_str(), "%d:%d", &min, &sec); //grabs the minutes and seconds from the time
			totalTime = (min * 60) + sec; //calculates the total time of the song
			for(i = 0; i < songArtist.size(); i++) //replaces all underscores with spaces
			{
				if(songArtist.at(i) == '_')
				{
					songArtist.replace(i, 1, " ");
				}
			}
			for(i = 0; i < songAlbum.size(); i++)
			{
				if(songAlbum.at(i) == '_')
				{
					songAlbum.replace(i, 1, " ");
				}
			}
			for(i = 0; i < songTitle.size(); i++)
			{
				if(songTitle.at(i) == '_')
				{
					songTitle.replace(i, 1, " ");
				}
			}
			ait = artists.find(songArtist); //checks if artist is in map
			if(ait == artists.end()) //if artist is not found in map, creates new artist, album and song
			{
				Artist *a; //initializes new Artist object
				a = new Artist;
				map <string, Album *> als; //initializes list of albums
				Album *al; //initializes new album object
				al = new Album;
				map <int, Song *> so; //initializes list of songs
				Song *s; //initializes new song object
				s = new Song;
				s->title = songTitle; //sets data into respective variables
				s->time = totalTime;
				s->track = songTrack;
				so.insert(make_pair(songTrack, s)); //inserts song into song list, with the track number as the key
				al->songs = so; //sets list of songs to the album
				al->name = songAlbum; //sets data into respective variables
				al->time = totalTime; 
				als.insert(make_pair(songAlbum, al)); //inserts album into album list, using the album name as key
				a->albums = als; //sets list of albums to artist
				a->name = songArtist;
				a->time = totalTime; //total time at this point is length of song
				a->nsongs = 1; //one song has been added
				artists.insert(make_pair(songArtist, a)); //inserts artist into map, using name as key
			}
			else //if artist is found in map, accesses data of said artist
			{
				alit = ait->second->albums.find(songAlbum); //attempts to find album in artist's album list
				if(alit == ait->second->albums.end()) //if album isn't found, creates a new one with only the song
				{
					Album *al; //initializes new album object
					al = new Album;
					map <int, Song *> so; //initializes list of songs
					Song *s; //initializes new song object
					s = new Song;
					s->title = songTitle; //sets data into respective variables
					s->time = totalTime;
					s->track = songTrack;
					so.insert(make_pair(songTrack, s)); //inserts song into song list, with the track number as the key
					al->songs = so; //sets list of songs to the album
					al->name = songAlbum; //sets data into respective variables
					al->time = totalTime; 
					ait->second->albums.insert(make_pair(songAlbum, al)); //inserts album into album list of artist
					ait->second->time += totalTime; //adds length of album to artist's total time
					ait->second->nsongs++; //increments number of songs
				}
				else //if album is found, accesses its data and adds the song
				{
					Song *s; //initializes new song object
					s = new Song;
					s->title = songTitle; //sets data into respective variables
					s->time = totalTime;
					s->track = songTrack;
					alit->second->songs.insert(make_pair(songTrack, s)); //inserts song into song list of album, using track no. as key
					alit->second->time += totalTime; //adds length of song into album's total time
					ait->second->time += totalTime; //adds length of song into artist's total time
					ait->second->nsongs++; //increments artist's no. of songs
				}
			}
			ss.clear(); //clears stream for later use
		}
	}
	fin.close(); //closes file as it is no longer needed

	for(ait = artists.begin(); ait != artists.end(); ait++) //prints out contents of artists map in a format
	{
		min = ait->second->time / 60; //converts total time to minutes and seconds (m:ss)
		sec = ait->second->time % 60;
		printf("%s: %i, %i:%02i\n", ait->second->name.c_str(), ait->second->nsongs, min, sec); //prints artist's name, no. of songs, and length of all songs
		for(alit = ait->second->albums.begin(); alit != ait->second->albums.end(); alit++) //prints artist's albums
		{
			min = alit->second->time / 60; //converts total time to minutes and seconds
			sec = alit->second->time % 60;
			printf("        %s: %i, %i:%02i\n", alit->second->name.c_str(), (int) alit->second->songs.size(), min, sec); //prints padding, album name, no. of songs and length of album
			for(sit = alit->second->songs.begin(); sit != alit->second->songs.end(); sit++) //prints album's songs
			{
				min = sit->second->time / 60; //converts total time to minutes and seconds
				sec = sit->second->time % 60;
				printf("                %i. %s: %i:%02i\n", sit->second->track, sit->second->title.c_str(), min, sec); //padding, track no., song title, length
			}
		}
	}
	for(ait = artists.begin(); ait != artists.end(); ait++) //deletes all pointers to prevent memory leaks
	{
		for(alit = ait->second->albums.begin(); alit != ait->second->albums.end(); alit++)
		{
			for(sit = alit->second->songs.begin(); sit != alit->second->songs.end(); sit++) 
			{
				delete sit->second;
			}
			delete alit->second;
		}
		delete ait->second;
	}

	

	return 0;
}
