// Parallel Final Project
// Authors: Andrew Cook, Lucas Quintero
// Due date: 12/09/13

#include <iostream>
#include "time.h"
#include <cmath>
#include "Particle.h"
#include <fstream>
#include "mpi.h"

using namespace std;

// Function Prototypes
double inChamber(double);
double moveDist();
void collision(Particle*,int);

int main ()
{
	MPI::Init();
	int my_rank, num_cores;
	my_rank = MPI::COMM_WORLD.Get_rank();
	num_cores = MPI::COMM_WORLD.Get_size();

	// Random seed (assume 2D problem to start)
	srand(time(NULL));

	// Initializing particles
	int num_part(10);
	if(my_rank == 0) {
		//cout<<"\n\nNumber of Particles:\t" << endl;
		//cin>>num_part;
		cout<<"\n\nThere are " << num_part << " particles in this simulation.\n";
		cout<<"The total area is " << CHAMBER_WIDTH << " by " << CHAMBER_HEIGHT << "\n";
		cout<<"The escape window is of length " << 2*HALF_ESCAPE_WALL_WIDTH << "\n\n";
	}
	Particle* particleList = new Particle[num_part];
	/* Made the center of the chamber to be a (0,0) and then allowed for particles to exist in any part of the rationals*/
	// Main core sets location of all initial particles
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
			//collision(particleList,num_part);

			// Check if the particle has escaped
//			if(abs(particleList[i].getnewX())<escapeWidth/2.0 && abs(particleList[i].getnewY())<escapeHeight/2.0){
			if(abs(particleList[i].getnewX()) > CHAMBER_WIDTH/2) {
				escape = true; // Break loop
				cout<<"Iter #" << "\t" << "X loc" << "\t" << "Y loc" << endl;
				cout<<iter<<"\t"<<particleList[i].getnewX()<<"\t"<<particleList[i].getnewY()<<endl;
				break;
			}
		}
	} while(!escape);

	MPI::Finalize();
	return 0; //end main
}

// Given a chamber width/height, it returns a random value between -given/2 and given/2
double inChamber(double val){
	double test = ( (1.0*rand()/RAND_MAX) * val ) - (val*1.0/2.0);
	return test;
}

// Returns a random number between -1 and 1 for particle movement
double moveDist(){
	return pow(-1.0,rand() % 2) * (5.0*rand()/RAND_MAX);
}

void collision(Particle *particleList, int numlist){
	double xval,yterm,xterm,yconst,xconst,plust,negt;
	double m1,m2,t;
	double x1,x2,y1,y2;
	for(int i=0 ; i<numlist ; i++){
		m1=(particleList[i].getnewY()-particleList[i].getoldY())/(particleList[i].getnewX()-particleList[i].getoldX());
		for(int j=i+1 ; j<numlist-1 ; j++){
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
					//Thus there should exist 2 points where the two points are separated by the two radius
					//so we look where (y2-y1)^2+(x2-x1)^2=(2R)^2
					//after some analysis, we get (yterm*t+yconst)^2-(xterm*t+xconst)^2=(radius1+radius2)^2

//WAKA WAKA LUCAS CHECK THE DAMN CODE, YOU MAY HAVE DRANK 2 TOO MANY
//WHAT TO DO: FIND THE PROJECTION OF ONE PATH ON TO THE OTHER, THEN FIND THE INVERSE TANGENT, THEN BISECT, THEN REFLECT BY INTERIOR ANGLES, AND CONTINUE
//ALSO, CONSIDER FINDING THE CLOSEST INTERSECTION (VALLUE WHERE X-XVAL EXISTS AND IS SMALL), THEN RETURNING THE J VALUE, THEN DOING THE MOVEMENT,
//THEN DOING THE MOVEMENT AND THEN SETTNIG A BOOL SO THAT THE PARTICLE HAS TO CHECK THAT THERE ARE NO OTHER COLLISIONS, ONCE THERE ARE NUM_PARTS-1-J
// "NO" COLLSIONS THEN GO TO THE NEXT PARTICLE (NOTE:P1 CHECKS 2->NUM-1, P2 CHECKS 3->NUM-1...)
					yterm=(particleList[j].getnewY()-particleList[j].getoldY())-(particleList[i].getnewY()-particleList[i].getoldY());
					yconst=(particleList[j].getoldX()/(particleList[j].getnewX()-particleList[j].getoldX()))-(particleList[i].getoldX()/(particleList[i].getnewX()-particleList[i].getoldX()))+particleList[j].getoldY()-particleList[i].getoldY();
					xterm=(particleList[j].getnewX()-particleList[j].getoldX())-(particleList[i].getnewX()-particleList[i].getoldX());
					xconst=(particleList[j].getoldX()-particleList[i].getoldX());
					plust=-(2*(yconst*yterm+xterm*xconst))+sqrt(pow(2*(yconst*yterm+xterm*xconst),2)-4*(pow(yterm,2)+pow(xterm,2))*(pow(yconst,2)*pow(xconst,2)));
					negt=-(2*(yconst*yterm+xterm*xconst))-sqrt(pow(2*(yconst*yterm+xterm*xconst),2)-4*(pow(yterm,2)+pow(xterm,2))*(pow(yconst,2)*pow(xconst,2)));
					//We want t to be postive (as t is a time, so its trivially why)
					//We want the shortest t of the two t's as that is the initial collsion
					if(negt<0 && plust>0){
						t=plust;
					} else if(plust<0 && negt<0){
						t=negt;
					} else if(plust>0 && negt>0){
						if(plust<negt){
							t=plust;
						}
						else{
							t=negt;
						}
						cout<<t<<endl;
					//x1=t*x()
					}

	  		}
	  	}
		}
	}
}
