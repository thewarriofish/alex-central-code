//Lab 8 - Dijkstra's Algorithm and the City Streets
//Alexander Yu - CS302
//April 2nd, 2022

/*This lab focuses on the implementation of Dijkstra's Algorithm in the use of pathfinding through a city map. There are three different implementations of the algorithm which differ based on what kind of path is being found: an optimal path, a suboptimal path, and an average path.*/

#include "city_map.hpp"
#include <list>
#include <map>
#include <vector> 
#include <iostream> //needed for cin
#include <cmath> //needed for sqrt and pow

City_Map::City_Map() //constructor for City_Map, which will take form of a graph
{
	Intersection *inter;
	Road_Segment *road;
	int str, ave, str_max, ave_max;
	double x, y, grn_str, grn_ave;
	vector <Intersection *> sorted_list; //holds a sorted list of Intersections, as standard input may not take Intersections in order
	list <Intersection *>::iterator iit;
	str_max = 0;
	ave_max = 0;
	cin >> str >> ave >> x >> y >> grn_str >> grn_ave;
	inter = new Intersection;
	while(!(cin.fail())) //iterates until no input is given
	{
		inter->street = str; //sets inter's parameters
		inter->avenue = ave;
		inter->x = x;
		inter->y = y;
		inter->green[0] = grn_str;
		inter->green[1] = grn_ave;
		inter->best_time = 0;
		inter->backedge = NULL;
		inter->bfsq_ptr = bfsq.end();
		if(str == 0 && ave == 0) //sets intersection 0, 0 as the first
		{
			first = inter;
		}
		if(str >= str_max && ave >= ave_max) //sets intersection as last if it is the highest street and avenue
		{
			last = inter;
			str_max = str;
			ave_max = ave;
		}
		all.push_back(inter); //adds to list
		cin >> str >> ave >> x >> y >> grn_str >> grn_ave;
		inter = new Intersection;
	}
	sorted_list.resize(all.size()); 
	for(iit = all.begin(); iit != all.end(); iit++) //sorts list by placing Intersection into correct index
	{
		sorted_list[(*iit)->street * (ave_max + 1) + (*iit)->avenue] = *iit; //follows [i*c+j]
	}
	for(iit = all.begin(); iit != all.end(); iit++) //iterates through the sorted list and creates adjacency lists
	{
		if((*iit)->street % 5 == 0) //creates edges pointed east and west if intersection's street number is a multiple of 5
		{
			if((*iit)->avenue != ave_max) //checks if intersection is on highest avenue
			{
				road = new Road_Segment; //object is created and parameters are set
				road->type = STREET;
				road->number = (*iit)->street;
				road->from = (*iit);
				road->to = sorted_list[(*iit)->street * (ave_max + 1) + (*iit)->avenue + 1]; //intersection east of current is destination 
				road->distance = sqrt(pow((road->to->x - road->from->x), 2) + pow((road->to->y - road->from->y), 2)); //pythagorean theorem 
				(*iit)->adj.push_back(road);
			}
			if((*iit)->avenue != 0) 
			{
				road = new Road_Segment;
				road->type = STREET;
				road->number = (*iit)->street;
				road->from = (*iit);
				road->to = sorted_list[(*iit)->street * (ave_max + 1) + (*iit)->avenue - 1]; //intersecction west of current is destination
				road->distance = sqrt(pow((road->to->x - road->from->x), 2) + pow((road->to->y - road->from->y), 2));
				(*iit)->adj.push_back(road);
			}
		}
		else
		{
			if((*iit)->street % 2 == 0) //even streets only have edges pointing east
			{
				if((*iit)->avenue != ave_max)
				{
					road = new Road_Segment;
					road->type = STREET;
					road->number = (*iit)->street;
					road->from = (*iit);
					road->to = sorted_list[(*iit)->street * (ave_max + 1) + (*iit)->avenue + 1];
					road->distance = sqrt(pow((road->to->x - road->from->x), 2) + pow((road->to->y - road->from->y), 2));
					(*iit)->adj.push_back(road);
				}
			}
			else //odd streets only have edges pointing west
			{
				if((*iit)->avenue != 0)
				{
					road = new Road_Segment;
					road->type = STREET;
					road->number = (*iit)->street;
					road->from = (*iit);
					road->to = sorted_list[(*iit)->street * (ave_max + 1) + (*iit)->avenue - 1];
					road->distance = sqrt(pow((road->to->x - road->from->x), 2) + pow((road->to->y - road->from->y), 2));
					(*iit)->adj.push_back(road);
				}
			}
		}
		if((*iit)->avenue % 5 == 0 || (*iit)->avenue == last->avenue) //intersection points north and south if its avenue is a multiple of 5 or it is the last avenue
		{
			if((*iit)->street != str_max)
			{
				road = new Road_Segment;
				road->type = AVENUE;
				road->number = (*iit)->avenue;
				road->from = (*iit);
				road->to = sorted_list[(*iit)->street * (ave_max + 1) + (*iit)->avenue + (ave_max + 1)]; //intersection north of current is destination
				road->distance = sqrt(pow((road->to->x - road->from->x), 2) + pow((road->to->y - road->from->y), 2));
				(*iit)->adj.push_back(road);
			}
			if((*iit)->street != 0)
			{
				road = new Road_Segment;
				road->type = AVENUE;
				road->number = (*iit)->avenue;
				road->from = (*iit);
				road->to = sorted_list[(*iit)->street * (ave_max + 1) + (*iit)->avenue - (ave_max + 1)]; //intersection south of current is destination
				road->distance = sqrt(pow((road->to->x - road->from->x), 2) + pow((road->to->y - road->from->y), 2));
				(*iit)->adj.push_back(road);
			}
		}
		else
		{
			if((*iit)->avenue % 2 == 0) //even avenues only point north
			{
				if((*iit)->street != str_max)
				{
					road = new Road_Segment;
					road->type = AVENUE;
					road->number = (*iit)->avenue;
					road->from = (*iit);
					road->to = sorted_list[(*iit)->street * (ave_max + 1) + (*iit)->avenue + (ave_max + 1)];
					road->distance = sqrt(pow((road->to->x - road->from->x), 2) + pow((road->to->y - road->from->y), 2));
					(*iit)->adj.push_back(road);
				}
			}
			else //odd avenues only point south
			{
				if((*iit)->street != 0)
				{
					road = new Road_Segment;
					road->type = AVENUE;
					road->number = (*iit)->avenue;
					road->from = (*iit);
					road->to = sorted_list[(*iit)->street * (ave_max + 1) + (*iit)->avenue - (ave_max + 1)];
					road->distance = sqrt(pow((road->to->x - road->from->x), 2) + pow((road->to->y - road->from->y), 2));
					(*iit)->adj.push_back(road);
				}
			}
		}
	}

}

double City_Map::Dijkstra(int avg_best_worst) //implementation of Dijkstra's Algorithm
{
	Intersection *current;
	int nextNode;
	double totalDistance, travelTime, avgTime;
	vector <double> dist; //holds distances from 0,0; B uses it as distances, A and W uses it as times
	list <class Road_Segment *>::iterator rit;
	bfsq.insert(make_pair(0, first)); //inserts intersection 0, 0, distance 0
	current = bfsq.begin()->second; //sets initial starting points 
	current->bfsq_ptr = bfsq.begin();
	totalDistance = 0;
	dist.resize(all.size(), -1);
	dist[0] = 0;
	if(avg_best_worst == 'B') //algorithm if optimal route is chosen, i.e. all green lights
	{
		while(bfsq.size() != 0)
		{
			for(rit = current->adj.begin(); rit != current->adj.end(); rit++)
			{
				nextNode = (*rit)->to->street * (last->avenue + 1) + (*rit)->to->avenue; //follows [i*c+j]; represents n2
				if(dist[nextNode] == -1 || (*rit)->distance + current->bfsq_ptr->first < dist[nextNode]) //if n2 hasn't been visited or its distance is not as optimal, replaces distance
				{
					if((*rit)->to->bfsq_ptr != bfsq.end()) //deletes entry from map if it is already in it
					{
						bfsq.erase((*rit)->to->bfsq_ptr);
					}
					dist[nextNode] = (*rit)->distance + current->bfsq_ptr->first; //distance set as edge's length plus distance of n
					(*rit)->to->backedge = (*rit); //sets n2's backedge to current edge
					(*rit)->to->best_time = (*rit)->from->best_time + (((*rit)->distance / 30) * 3600); //sets n2's new time in seconds
					bfsq.insert(make_pair((*rit)->distance + current->bfsq_ptr->first, (*rit)->to)); //inserts into multimap
					(*rit)->to->bfsq_ptr = bfsq.find((*rit)->distance + current->bfsq_ptr->first); //n2's pointer is set to its location in the map
				}
			}
			bfsq.erase(bfsq.begin()); //removes first entry in multimap
			current = bfsq.begin()->second; //sets n to next value
			current->bfsq_ptr = bfsq.begin();
		}
	}
	else if(avg_best_worst == 'W') //algorithm if suboptimal route is chosen, i.e. all red lights
	{
		while(bfsq.size() != 0)
		{
			for(rit = current->adj.begin(); rit != current->adj.end(); rit++)
			{
				nextNode = (*rit)->to->street * (last->avenue + 1) + (*rit)->to->avenue;
				if((*rit)->type == STREET) //if edge is a STREET, add the destination intersection's avenue green light time to travelTime
				{
					travelTime = ((*rit)->distance / 30) * 3600 + (*rit)->to->green[1];
				}
				else //if edge is an AVENUE, adds the street green light time instead
				{
					travelTime = ((*rit)->distance / 30) * 3600 + (*rit)->to->green[0];
				}
				if(dist[nextNode] == -1 || (*rit)->from->best_time + travelTime < dist[nextNode]) //similar to B, except time is used as distance from 0
				{
					if((*rit)->to->bfsq_ptr != bfsq.end())
					{
						bfsq.erase((*rit)->to->bfsq_ptr);
					}
					(*rit)->to->backedge = (*rit);
					(*rit)->to->best_time = (*rit)->from->best_time + travelTime;
					dist[nextNode] = (*rit)->to->best_time;
					bfsq.insert(make_pair((*rit)->to->best_time, (*rit)->to));
					(*rit)->to->bfsq_ptr = bfsq.find((*rit)->to->best_time);
				}
			}	
			bfsq.erase(bfsq.begin());
			current = bfsq.begin()->second;
			current->bfsq_ptr = bfsq.begin();
		}
	}
	else if(avg_best_worst == 'A') //algorithm for average route
	{
		while(bfsq.size() != 0)
		{
			for(rit = current->adj.begin(); rit != current->adj.end(); rit++)
			{
				nextNode = (*rit)->to->street * (last->avenue + 1) + (*rit)->to->avenue;
				/*R is green light time of perpendicular direction, G is green light time of same direction*/
				if((*rit)->type == STREET) //if edge is a STREET, add the destination intersection's average avenue green light time to travelTime
				{
					avgTime = (pow((*rit)->to->green[1], 2))/(2 * ((*rit)->to->green[1]+(*rit)->to->green[0])); //average time at stop light is R^2/(2(R+G))
					travelTime = ((*rit)->distance / 30) * 3600 + avgTime;
				}
				else //if edge is an AVENUE, adds the average street green light time instead
				{
					avgTime = (pow((*rit)->to->green[0], 2))/(2 * ((*rit)->to->green[1]+(*rit)->to->green[0]));
					travelTime = ((*rit)->distance / 30) * 3600 + avgTime;
				}
				if(dist[nextNode] == -1 || (*rit)->from->best_time + travelTime < dist[nextNode])
				{
					if((*rit)->to->bfsq_ptr != bfsq.end())
					{
						bfsq.erase((*rit)->to->bfsq_ptr);
					}
					(*rit)->to->backedge = (*rit);
					if((*rit)->type == STREET)
					{
						(*rit)->to->best_time = (*rit)->from->best_time + travelTime;
					}		
					else
					{
						(*rit)->to->best_time = (*rit)->from->best_time + travelTime; 
					}
					dist[nextNode] = (*rit)->to->best_time;
					bfsq.insert(make_pair((*rit)->to->best_time, (*rit)->to)); //like 'W', 'A' uses time as distance from 0
					(*rit)->to->bfsq_ptr = bfsq.find((*rit)->to->best_time);
				}
			}
			bfsq.erase(bfsq.begin());
			current = bfsq.begin()->second;
			current->bfsq_ptr = bfsq.begin();
		}
	}
	current = last;
	while(current != first) //iterates backward, following backedges
	{
		path.push_front(current->backedge);
		totalDistance += current->backedge->distance; //adds distance
		current = current->backedge->from;
	}
	return totalDistance;
}
