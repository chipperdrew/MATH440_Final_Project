// Parallel Final Project
// Authors: Andrew Cook, Lucas Quintero
// Due date: 12/09/13

#include <iostream>
#include "time.h"
#include <cmath>
#include "math.h"
#include "Particle.h"

using namespace std;

// Function Prototypes
double inChamber(double);
double moveDist();

int main ()
{
	// Declaration of vars (assume 2D problem to start)
	srand(time(NULL));
	
	// Initializing particles
	int num_part(1);
	cout<<"\n\nNumber of Particles:\t";
	cin>>num_part;
	Particle* particleList = new Particle[num_part];
	/* Made the center of the chamber to be a (0,0) and then allowed for particles to exist in any part of the rationals*/
	for( int i=0 ; i<num_part ; i++){
		particleList[i].setX(inChamber(chamber_width));
		particleList[i].setY(inChamber(chamber_height));
	}

	// Output 
	// Outputs the iteration when the first one escapes and its location to ensure that it did escape
	int iter=0;
	int j=0;
	do {
		iter++;
		for( int i=0 ; i<num_part ; i++ ){
			//cout<<"Particle "<<i<<"'s position is "<<particleList[i]<<".\n";
			particleList[i].moveParticle(moveDist(), moveDist());
			if(abs(particleList[i].getX()) > 400 || abs(particleList[i].getY()) > 400) {
				break;
			}			

			if(abs(particleList[i].getX())<1 && abs(particleList[i].getY())<1){
				j=1; // Break loop
				cout<<"Iter #" << "\t" << "X postion" << "\t" << "Y position" << endl;
				cout<<iter<<"\t"<<particleList[i].getX()<<"\t"<<particleList[i].getY()<<endl;
			}
		}
	} while(j != 1);

	// end main
	return 0;
}

// Given a chamber width/height, it returns a random value between -given/2 and given/2
double inChamber(double val){
	return ( (1.0*rand()/RAND_MAX) * val ) - (val*1.0/2.0);
}

// Returns a random number between -1 and 1 for particle movement
double moveDist(){
	return pow(-1.0,rand() % 2) * (5.0*rand()/RAND_MAX);
}
