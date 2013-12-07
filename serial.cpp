// Parallel Final Project
// Authors: Andrew Cook, Lucas Quintero
// Due date: 12/09/13
// TODO: (Optional) Add forces on the chamber
// TODO: (Optional) 3D problem

#include <iostream>
#include "time.h"
#include <cmath>
#include "Particle.cpp"
#include <fstream>

using namespace std;

// Function Prototypes
double inChamber(double);
double moveDist();
void collision(Particle*,int);

int main ()
{
	// Random seed (assume 2D problem to start)
	srand(time(NULL));
	int count = 0;
	double startTime, endTime;
	startTime = clock();


	// TODO: Remove this when comparing serial vs. parallel CPU times
	// Initializing file
	
	// Initializing particles
	int num_part(1);
	//cout<<"\n\nNumber of Particles:\t" << endl;
	//cin>>num_part;
				// Break loop
	cout<<"\n\nThere are " << num_part << " particles in this simulation.\n";
	cout<<"The total area is " << CHAMBER_WIDTH << " by " << CHAMBER_HEIGHT << "\n";
	cout<<"The escape window is of length " << 2*HALF_ESCAPE_WALL_WIDTH << "\n\n";

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
			collision(particleList,num_part);

			// Check if the particle has escaped
//			if(abs(particleList[i].getnewX())<escapeWidth/2.0 && abs(particleList[i].getnewY())<escapeHeight/2.0){
			if(particleList[i].getnewX() > CHAMBER_WIDTH/2) {
				// Break loop
				escape=true;
				cout<<"Iter #" << "\t" << "X loc" << "\t" << "Y loc" << endl;
				cout<<iter<<"\t"<<particleList[i].getnewX()<<"\t"<<particleList[i].getnewY()<<endl;
				break;
			}
		}

	} while(!escape);

	endTime = clock();
	cout << "Total CPU time was: " << (endTime-startTime)/CLOCKS_PER_SEC << " seconds.\n";
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
	double m1,m2; //Slopes
	double xval,yval;//absolute collision
	double A,B,C,negt,plust; //For Quadratic Eqn
	double yterm,yconst,xterm,xconst; //For simplifying *term is the variable coefficent
	double t,x1,x2,y1,y2; //For location of collision
	double tmin; //First collision

	for(int i=0 ; i<numlist ; i++){
		tmin=10000000.000;
//		//cout<<"ENTERED 1\n";
		// m=(y2-y1)/(x2-x1)
		//m2=(particleList[i].getnewY()-particleList[i].getoldY())/(particleList[i].getnewX()-particleList[i].getoldX());
		for(int j=i+1 ; j<numlist ; j++){		
//			//cout<<"ENTERED 2\n";
			if(i==j){
				continue;
			}
		
			yterm=((particleList[j].getnewY()-particleList[j].getoldY())-(particleList[i].getnewY()-particleList[i].getoldY()));
			yconst=(particleList[j].getoldY()-particleList[i].getoldY());
			xterm=((particleList[j].getnewX()-particleList[j].getoldX())-(particleList[i].getnewX()-particleList[i].getoldX()));
			xconst=particleList[j].getoldX()-particleList[i].getoldX();

			A=pow(yterm,2)+pow(xterm,2);
			B=2*(yterm*yconst+xterm*xconst);
			C=pow(yconst,2)+pow(xconst,2)-pow(1,2);
			if((pow(B,2)-4*A*C)<0)
				break;
			plust=(-B+sqrt(pow(B,2)-4*A*C))/(2*A);
			negt=(-B-sqrt(pow(B,2)-4*A*C))/(2*A);
			//We want t to be postive (as t is a time, so its trivially why)
			//We want the shortest t of the two t's as that is the initial collsion
			if(negt<0 && plust>0 && plust<=1){
				t=plust;
			} else if(plust<0 && negt>0 && negt<=1){
				t=negt;
			} else if(plust>0 && negt>0 && (plust<=1 || negt<=1)){
				if(plust<negt){
					t=plust;
				}
				else{
					t=negt;
				}
			} else{
				break;
			}
				cout<<"BINGO!!!\n";
			if(t<tmin){
				tmin=t;
			
}		}//cout<<particleList[0]<<endl;
		if(i!=numlist-1 && tmin<=1 && tmin>0){
			//cout<<tmin<<endl;
			particleList[i].setnewX(.95*tmin*(particleList[i].getnewX()-particleList[i].getoldX())+particleList[i].getoldX());
			particleList[i].setnewY(.95*tmin*(particleList[i].getnewY()-particleList[i].getoldY())+particleList[i].getoldY());
		}
		//cout<<particleList[0]<<endl;
		if(fabs(particleList[i].getnewX()) > CHAMBER_WIDTH/2){
			// Check if collision occurs on pos. wall
			//cout<<"hey"<<endl;
			if(particleList[i].getoldX()>0){
				//cout<<"move"<<endl;
				// Check if escaped
				double dist_from_wall = CHAMBER_WIDTH/2 -particleList[i].getoldX();
				double time_collision = dist_from_wall / (particleList[i].getnewX()-particleList[i].getoldX());
				double y_at_time = time_collision*(particleList[i].getnewX()-particleList[i].getoldX()) + particleList[i].getoldX();
			//	if(fabs(y_at_time) < HALF_ESCAPE_WALL_WIDTH) {
			//		setnewX(fromx+moveX); // X escapes
			//	} else {
					particleList[i].setnewX(CHAMBER_WIDTH-particleList[i].getnewX());
			//	}
			// Else collision on neg. wall (no escape possible)
			} else {
				particleList[i].setnewX(-CHAMBER_WIDTH-particleList[i].getnewX());
			}
		}
		// Else no collision
		else {	
			particleList[i].setnewX(particleList[i].getnewX());
		}
		// Check for Y wall collisions
		if(fabs(particleList[i].getnewY()) > CHAMBER_HEIGHT/2.0){
			if(particleList[i].getoldY()>0){
				particleList[i].setnewY(CHAMBER_HEIGHT-particleList[i].getnewY());
			} else {
				particleList[i].setnewY(-CHAMBER_HEIGHT-particleList[i].getnewY());
			}	
		} else {
			particleList[i].setnewY(particleList[i].getnewY());
		}
	}
}
