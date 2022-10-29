//Alexander Yu - CS302
//February 10th, 2022
//Lab 3: MIDI Files

/*This lab focuses on the implementation of a file converter from one MIDI format to another MIDI format. 
 * It uses maps, lists, and pointers to convert a MIDI file from Note format to Event format, as well as vice versa.*/

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <list>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include "cs302-midi.h"
using namespace std;

void CS302_Midi::el_to_nd() //converts from Event to Note
{
  vector <ND*> NDVector(128, NULL);	//stores incomplete entries before they are inserted into the Notes map
  double time; //stores total time for calculating start and stop times 
  size_t i; //loop variable
  ND *n, *d; //stores new NDs and the damper entry respectively
  list <Event *>::iterator eit;
  nd = new NDMap;
  d = NULL; //initializes variables for later
  time = 0;
  for(eit = el->begin(); eit != el->end(); eit++) //loops through Event list and processes each entry
  {
	  time += (double) (*eit)->time; //adds the time (how many 1/480 seconds since last entry) to total
	  if((*eit)->key == 'O') //if entry is O, creates a new ND object and sets everything 
	  {
		  if(NDVector[(*eit)->v1] == NULL || NDVector[(*eit)->v1]->stop != 0) //checks vector to ensure no incomplete entry is in the element
		  {
			  n = new ND; //creates and initializes new ND object
			  n->key = 'N';
			  n->start = time/480; //time is divided by 480 to get seconds
			  n->stop = 0; //a zero is used to denote an incomplete entry
			  n->pitch = (*eit)->v1;
			  n->volume = (*eit)->v2;
			  NDVector[n->pitch] = n; //sets vector at index of entry's pitch to incomplete entry
		  }
	  }
	  else if((*eit)->key == 'F') //if entry is F, completes ND object and inserts into ND map
	  {
		  if(NDVector[(*eit)->v1] != NULL) //checks if there is a incomplete entry (which would be the previously created object for an ON entry)
		  {
			  NDVector[(*eit)->v1]->stop = time/480; //sets stop time by dividing total time by 480
			  nd->insert(make_pair(NDVector[(*eit)->v1]->start, NDVector[(*eit)->v1])); //inserts into ND map using start time as key and ND object as val
		  }
	  }
	  else if((*eit)->key == 'D') //if entry is D, creates damper object
	  {  
		  if((*eit)->v1 == 1) //if v1 is 1, damper is down
		  {
			  if(d == NULL) //checks if a damper object has already been created
			  {
				 n = new ND; //creates and initializes damper object
				 n->key = 'D';
			     n->start = time/480;
			     n->stop = 0;
			     d = n;	
			  }
		  }
		  else if((*eit)->v1 == 0) //if v1 is 0, damper is up
		  {
			  if(d != NULL) //checks if incomplete damper entry exists
			  {
				  d->stop = time/480; 
				  nd->insert(make_pair(d->start, d)); //inserts damper object into ND map
				  d = NULL; //resets damper object for future damper entries
			  }
		  }
	  }
  }
  if(d != NULL) //if d isn't NULL after iteration, an incomplete damper object still exists (a damper down was created without a damper up)
  {
	  d->stop = time/480; //sets end time to last second
	  nd->insert(make_pair(d->start, d)); //inserts into map
  }
  for(i = 0; i < NDVector.size(); i++) //checks if any incomplete entries exist
  {
	  if(NDVector[i] != NULL)
	  {
		if(NDVector[i]->stop == 0) //if an entry's stop = 0, no OFF was set for an ON
		{
				NDVector[i]->stop = time/480; //sets end time to last second
				nd->insert(make_pair(NDVector[i]->start, NDVector[i])); //inserts into map
		}
	  }
  }
}

void CS302_Midi::nd_to_el() //converts from Note to Event
{
  Event *e; //stores new Event objects
  double start, stop; //stores start and stop times
  NDMap::iterator nit, tnit; //iterators for maps
  map<int, multimap < int, Event * > > tempMap; //stores entries ordered by start time; the multimap is ordered off, up, down, on, using 1-4 as keys
  map<int, multimap < int, Event * > >::iterator tit, ttit;
  multimap < int, Event * >::iterator mit;
  el = new EventList;
  for(nit = nd->begin(); nit != nd->end(); nit++) //iterates through map
  {
	    start = rint(nit->second->start * 480); //sets start and stop to nit's start and stop * 480, rounded to the nearest int
		stop = rint(nit->second->stop * 480);
		if(start == stop) //skips any entry whose start and stop are equal
		{
			nit++;
		}
		if(nit->second->key == 'N') //creates ON and OFF entry for a Note
		{
			e = new Event; //creates and initializes an object representing ON
			e->key = 'O';
			e->v1 = nit->second->pitch;
		    e->v2 = nit->second->volume;
			tit = tempMap.find(start); //attempts to find a key equal to start; creates a new one if not found
			if(tit == tempMap.end())
			{
				tempMap.insert(make_pair(start, multimap < int, Event * >()));				 
				tit = tempMap.find(start);
			}
			tit->second.insert(make_pair(4, e)); //inserts ON object into map; ON is 4th in order
			e = new Event; //creates and initializes an object representing OFF
			e->key = 'F';
			e->v1 = nit->second->pitch; //ignores v2 due to structure of Event
			tit = tempMap.find(stop); //attempts to find a key equal to stop; creates a new one if not found
			if(tit == tempMap.end())
			{
				tempMap.insert(make_pair(stop, multimap < int, Event * >()));
				tit = tempMap.find(stop);
			}
			tit->second.insert(make_pair(1, e)); //inserts OFF object into map; OFF is 1st in order
		}
		else if(nit->second->key == 'D') //creates DAMPER UP and DAMPER DOWN entry for a Note
		{
			e = new Event; //creates and initializes an object representing DAMPER DOWN
			e->key = 'D';
			e->v1 = 1; //damper ignores v2
			tit = tempMap.find(start);
			if(tit == tempMap.end())
			{
				tempMap.insert(make_pair(start, multimap < int, Event * >()));				 
				tit = tempMap.find(start);
			}
			tit->second.insert(make_pair(3, e)); //DAMPER DOWN is 3rd in order
			e = new Event; //creates and initializes an object representing DAMPER UP
			e->key = 'D';
			e->v1 = 0;
			tit = tempMap.find(stop);
			if(tit == tempMap.end())
			{
				tempMap.insert(make_pair(stop, multimap < int, Event * >()));
				tit = tempMap.find(stop);
			}
			tit->second.insert(make_pair(2, e)); //DAMPER UP is 2nd in order
		}
  }
  nit = nd->begin(); 
  for(tit = tempMap.begin(); tit != tempMap.end(); tit++)
  {
	  for(mit = tit->second.begin(); mit != tit->second.end(); mit++)
	  {
		  mit->second->time = 0;
		  if(mit == tit->second.begin() && tit == tempMap.begin())
		  {
			  mit->second->time = rint(nit->first * 480); //sets very first event's time to nd->begin's start time to account for rests
		  }
		  else if(mit == tit->second.begin() && tit != tempMap.begin()) //time is nonzero if entry is the first of its map but NOT in the first key of tempMap
		  {
			  ttit = tit; //time is equal to the difference between the time of the current map and previous map
			  ttit--;
			  mit->second->time = (tit->first - ttit->first);
		  }
		  el->push_back(mit->second); //inserts entry into Event list
	  }
  }
}
