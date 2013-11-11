// Parallel Final Project
// Authors: Andrew Cook, Lucas Quintero
// Due date: 12/09/13
// TODO: (Optional) Add forces on the chamber
// TODO: (Optional) 3D problem
// TODO: (Optional) Output the data to file so particle movement can be plotted


#include <iostream>
#include "time.h"
#include <cmath>
#include "math.h"
#include "Particle.h"

using namespace std;

// Function Prototypes
double inChamber(double);
double moveDist();
void collision(Particle*,int);

int main ()
{
	// Random seed (assume 2D problem to start)
	srand(time(NULL));
	
	// Initializing particles
	int num_part(6400);
	cout<<"\n\nNumber of Particles:\t";
	//cin>>num_part;
	Particle* particleList = new Particle[num_part];
	/* Made the center of the chamber to be a (0,0) and then allowed for particles to exist in any part of the rationals*/
	for( int i=0 ; i<num_part ; i++){
		particleList[i].setnewX(inChamber(CHAMBER_WIDTH));
		particleList[i].setnewY(inChamber(CHAMBER_HEIGHT));
	}

	// Output 
	// Outputs the iteration when the first one escapes and its location to ensure that it did escape
	int iter=0;
	bool escape = false;
	do {
		iter++;
		for( int i=0 ; i<num_part ; i++ ){
			//cout<<"Particle "<<i<<"'s position is "<<particleList[i]<<".\n";
			particleList[i].moveParticle(moveDist(), moveDist());
			// TODO: Partcile collisions

			// Check if the particle has escaped
			if(abs(particleList[i].getnewX())<escapeWidth/2.0 && abs(particleList[i].getnewY())<escapeHeight/2.0){
				escape = true; // Break loop
				cout<<"Iter #" << "\t" << "X postion" << "\t" << "Y position" << endl;
				cout<<iter<<"\t"<<particleList[i].getnewX()<<"\t"<<particleList[i].getnewY()<<endl;
			}
		}
		collision(particleList,num_part);
	} while(!escape);

	return 0; //end main
}

// Given a chamber width/height, it returns a random value between -given/2 and given/2
double inChamber(double val){
	return ( (1.0*rand()/RAND_MAX) * val ) - (val*1.0/2.0);
}

// Returns a random number between -1 and 1 for particle movement
double moveDist(){
	return pow(-1.0,rand() % 2) * (5.0*rand()/RAND_MAX);
}

void collision(Particle *particleList, int numlist){
	double xval,m1,m2, t, x1,x2,y1,y2;

	for(int i=0 ; i<numlist ; i++){
		m1=(particleList[i].getnewY()-particleList[i].getoldY())/(particleList[i].getnewX()-particleList[i].getoldX());
		for(int j=i ; j<numlist ; j++){
			//xval is the value of x for the intersection of two lines
			//this is found by (y2-y1)=m(x2-x1) for two sets of two points,
			//solving for m of each system then setting y equal for the
			//respective y=(y2-y1)/(x2-x1)*(x2-x1)+y1
			m2=(particleList[j].getnewY()-particleList[j].getoldY())/(particleList[j].getnewX()-particleList[j].getoldX());
			xval=(particleList[j].getoldY()-particleList[j].getoldX()*m1)-(particleList[i].getoldY()-particleList[i].getoldX()*m2);
			xval=xval/(m1-m2);

	  	
	  	if((0<(particleList[i].getoldX()-xval) && 0>(particleList[i].getnewX()-xval))||
	  			(0>(particleList[i].getoldX()-xval) && 0<(particleList[i].getnewX()-xval))){
	  		if((0<(particleList[j].getoldX()-xval) && 0>(particleList[j].getnewX()-xval))||
	  			(0>(particleList[j].getoldX()-xval) && 0<(particleList[j].getnewX()-xval))){
					//Thus a collision is possible, so lets find where each particle was at time t
					t=(xval-particleList[j].getoldX())/(particleList[j].getnewX()-particleList[j].getoldX());
					x1=t*(particleList[i].getoldX()+particleList[i].getnewX());
					y1=m1*t+particleList[i].getoldX();
			  	x2=t*(particleList[j].getoldX()+particleList[j].getnewX());
					y2=m2*t+particleList[j].getoldX();
					if((2*particleList[i].getR()>fabs(x1-x2)) &&(2*particleList[i].getR()>fabs(y1-y2)))
				  	cout<<"COLLISION\t"<<particleList[i].getnewX()<<"\n";
	  		}
	  	}
	  	
		}
	}
}
