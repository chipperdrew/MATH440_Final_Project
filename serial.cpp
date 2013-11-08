// Parallel Final Project
// Authors: Andrew Cook, Lucas Quintero
// Due date: 12/09/13

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "Particle.cpp"
using namespace std;

// Class for particles to store x and y position of each

double inChamber(double);

int main ()
{
	// Declaration of vars (assume 2D problem to start)
	int chamber_height = 800;
	int chamber_width = 800;
	srand(time(NULL));
	
	// Initializing particles
	int num_part(1);
//	cout<<"\n\nNumber of Particles:\t";
//	cin>>num_part;
	Particle* particleList = new Particle[num_part];
	/* Made the center of the chamber to be a (0,0) and then allowed for particles to exist in any part of the rationals*/
	for( int i=0 ; i<num_part ; i++){
		particleList[i].setX(inChamber(1.0*chamber_width));
		particleList[i].setY(inChamber(1.0*chamber_height));
	}

	// Output 
	// Outputs the iteration when the first one escapes and its location to ensure that it did escape
	int iter=0;
	for( int j=0 ; j!=1 ; ){
		iter++;
	for( int i=0 ; i<num_part ; i++ ){
		//	cout<<"Particle "<<i<<"'s position is "<<particleList[i]<<".\n";
			particleList[i].moveParticle(inChamber(1.0*chamber_width), inChamber(1.0*chamber_height));
			if(abs(particleList[i].getX())<8 && abs(particleList[i].getY())<8){
				j=1;
				cout<<iter<<"\t"<<particleList[i].getX()<<"\t"<<particleList[i].getY()<<endl;

		}
		}
	}

	return 0;
}

double inChamber(double val){
	return ( (1.0*rand()/RAND_MAX) * val ) - (val*1.0/2.0);
}