// Parallel Final Project
// Authors: Andrew Cook, Lucas Quintero
// Due date: 12/09/13
// Note: If using more than 128 cores, adjust the move_dist value from '5' to any
// number less than CHAMBER_WIDTH/num_cores.
// TODO: Particle locs should be doubles, not integers

#include <iostream>
#include "time.h"
#include <cmath>
#include "Particle.h"
#include <fstream>
#include "mpi.h"
#include <vector>

using namespace std;

// Function Prototypes
double inChamber(double);
double moveDist();
void collision(Particle*,int);

const int num_part = 10000;
const double MAX_MOVE_DIST = 5;
int main ()
{
	// Initialization of MPI, variables, time, seeding
	MPI::Init();
	int my_rank, num_cores;
	double startTime, endTime;
	my_rank = MPI::COMM_WORLD.Get_rank();
	num_cores = MPI::COMM_WORLD.Get_size();
	if(my_rank == 0) {
		startTime = clock();
	}
	srand(time(NULL));
	Particle* particleList = new Particle[num_part]; //Only used by master

	// Check to make sure particles can't move across multiple cores
	if(my_rank==0 && CHAMBER_WIDTH/num_cores < MAX_MOVE_DIST) {
		cout << "\n\n\nERROR MESSAGE: With the variables as is, this code will not work properly.\n" <<
			"The problem can be avoided by doing any of the following:\n" <<
			"\t 1) Increasing the chamber width (CHAMBER_WIDTH)\n" <<
			"\t 2) Decreasing the number of cores (num_cores)\n" <<
			"\t 3) Decreasing the maximum particle movement distance (MAX_MOVE_DIST)\n\n\n";
		MPI::COMM_WORLD.Abort(-1);
	}

	// Main core outputs basic info & initializes particle locations
	if(my_rank == 0) {
		//cout<<"\n\nNumber of Particles:\t" << endl;
		//cin>>num_part;
		cout<<"\n\nThere are " << num_part << " particles in this simulation.\n";
		cout<<"The total area is " << CHAMBER_WIDTH << " by " << CHAMBER_HEIGHT << "\n";
		cout<<"The escape window is of length " << 2*HALF_ESCAPE_WALL_WIDTH << "\n\n";

		// Center of the chamber is (0,0), particles can be any rational number
		for( int i=0 ; i<num_part ; i++){
			particleList[i].setnewX(inChamber(CHAMBER_WIDTH));
			particleList[i].setnewY(inChamber(CHAMBER_HEIGHT));
		}
	}

	double temp_locs[2];
	int recv_core;
	vector<Particle> core_part_list;
	// Assigning particles to each core. Each chamber has width CHAMBER_WIDTH/num_cores
	// Particles cannot be sent/received, so their x & y locs are sent/received
	for(int i=0; i<num_part; i++) {
		if(my_rank == 0) {
			double pos_0_to_max = particleList[i].getnewX() + CHAMBER_WIDTH/2;
			recv_core = pos_0_to_max * (num_cores/CHAMBER_WIDTH);
//cout << recv_core << endl;
			//cout << "X Pos:" << pos_0_to_max << " Core:" << recv_core << endl;
			temp_locs[0] = particleList[i].getnewX();
			temp_locs[1] = particleList[i].getnewY();
			MPI::COMM_WORLD.Send(&temp_locs, 2, MPI::DOUBLE, recv_core, 10);
		}
		MPI::COMM_WORLD.Bcast(&recv_core, 1, MPI::INT, 0); //Tells which core will receive
		MPI::COMM_WORLD.Barrier();
		// Appropriate core receive the x and y locs and reconstructs particle
		if(my_rank==recv_core) {
			MPI::COMM_WORLD.Recv(&temp_locs, 2, MPI::DOUBLE, 0, 10);
			Particle* p = new Particle(temp_locs[0], temp_locs[1]);
			core_part_list.push_back(*p);
		}
		MPI::COMM_WORLD.Barrier();
	}

	// TESTING SEND/RECEIVE -- REMOVE IF NEEDED
	if(core_part_list.size() > 0) {
		cout << "My rank is " << my_rank << " and my vector is size " << 
			core_part_list.size() << " and my 1st Y entry is " << 
			core_part_list.front() << endl;
	}

	// Loop runs until a particle has escaped
	// Outputs the iteration when the 1st one escapes & its location to ensure escape
	int iter=0;
	bool escape = false;
	do {
		// Map contains cores as keys and vector of indices corresponding to key
		map<int, vector<double> > send_map;
		map<int, vector<double> >::const_iterator map_iter;
		vector<int> indices_to_delete;
		iter++;
		for( int i=0 ; i< core_part_list.size() ; i++ ){
			//cout<<"My rank: " << my_rank << " & Particle "<<i<<"'s position is "<<core_part_list.at(i)<<".\n";
			core_part_list.at(i).moveParticle(moveDist(), moveDist());
			double tempX = core_part_list.at(i).getnewX();
			double tempY = core_part_list.at(i).getnewY();	

			// Collision Check
			collision(&core_part_list[0], core_part_list.size());

			// Check if the particle has escaped
			if(abs(tempX) > CHAMBER_WIDTH/2 && my_rank==num_cores-1) {
				escape = true; // Break loop
				cout << "\nA particle has escaped!!! Escape info on particle: " << endl;
				cout << "Iter #" << "\t" << "X loc" << "\t" << "Y loc" << endl;
				cout << iter << "\t"<< tempX << "\t"<< tempY << endl;
				break;
			}

			// Check for particles moving outside of left OR right core boundaries	
			if(tempX < CHAMBER_WIDTH*(double(my_rank)/num_cores - 0.5) || 
			   tempX > CHAMBER_WIDTH*(double(my_rank+1)/num_cores - 0.5)) {
				recv_core = (tempX + CHAMBER_WIDTH/2) * (num_cores/CHAMBER_WIDTH);
				send_map[recv_core].push_back(tempX);
				send_map[recv_core].push_back(tempY);
				indices_to_delete.push_back(i);
				//cout << "ESCAPE: Core " << my_rank << " has part with new loc: " << 
				//core_part_list.at(i) << " & should go to core " << recv_core << endl;
			}
		}

		// Bcast escape so cores know when to terminate loop
		MPI::COMM_WORLD.Bcast(&escape, 1, MPI::BOOL, num_cores-1);
		MPI::COMM_WORLD.Barrier();

/******** MOVING PARTICLES BETWEEN CORES VIA MPI *******/
		// 1st -- Check all of the particles that need to move LEFT a core
		// NEED to send the size so MPI Receive knows what to expect
		// If size is nonzero, send the vector of particle locations also
		int send_size, recv_size;
		// Sending
		if(my_rank!=0) {
			send_size = send_map[my_rank-1].size();
			MPI::COMM_WORLD.Send(&send_size, 1, MPI_INT, my_rank-1, 10);
			if(send_size > 0) {
				MPI::COMM_WORLD.Send(&send_map[my_rank-1].front(), send_size, MPI_DOUBLE, my_rank-1, 10);
			}
		}
		// Receiving and reconstruction
		if(my_rank!=num_cores-1) {
			MPI::COMM_WORLD.Recv(&recv_size, 1, MPI_INT, my_rank+1, 10);
			if(recv_size > 0) {
				double recv_array[recv_size];
				MPI::COMM_WORLD.Recv(&recv_array, recv_size, MPI_DOUBLE, my_rank+1, 10);
				//cout << "My rank is " << my_rank << " and I received particle with x loc " << recv_array[0] << endl;
				// Reconstruct particles & add on to back of particle vector
				for(int i=0; i<recv_size; i+=2) {
					Particle* p = new Particle(recv_array[i], recv_array[i+1]);
					core_part_list.push_back(*p);
				}
			}
		}
		MPI::COMM_WORLD.Barrier();

		// 2nd -- Check all particles that need to move RIGHT a core
		// Sending
		if(my_rank!=num_cores-1) {
			send_size = send_map[my_rank+1].size();
			MPI::COMM_WORLD.Send(&send_size, 1, MPI_INT, my_rank+1, 10);
			if(send_size > 0) {
				MPI::COMM_WORLD.Send(&send_map[my_rank+1].front(), send_size, MPI_DOUBLE, my_rank+1, 10);
			}
		}
		// Receiving and reconstruction
		if(my_rank!=0) {
			MPI::COMM_WORLD.Recv(&recv_size, 1, MPI_INT, my_rank-1, 10);
			if(recv_size > 0) {
				double recv_array[recv_size];
				MPI::COMM_WORLD.Recv(&recv_array, recv_size, MPI_DOUBLE, my_rank-1, 10);
				//cout << "My rank is " << my_rank << " and I received particle with x loc " << recv_array[0] << endl;
				// Reconstruct particles & add on to back of particle vector
				for(int i=0; i<recv_size; i+=2) {
					Particle* p = new Particle(recv_array[i], recv_array[i+1]);
					core_part_list.push_back(*p);
				}
			}
		}
		MPI::COMM_WORLD.Barrier();

		// 3rd -- Delete particles from loc vector that are no longer in core region
		for(int i=0; i<indices_to_delete.size(); i++) {
			core_part_list.erase(core_part_list.begin()+indices_to_delete.at(i));	
		}
/******** END MOVING PARTICLES BETWEEN CORES VIA MPI *******/
	} while(!escape);


	// Finalization and printing of CPU time
	MPI::COMM_WORLD.Barrier();
	if(my_rank == 0) {
		endTime = clock();
		cout << "Total CPU time was: " << (endTime-startTime)/CLOCKS_PER_SEC << " seconds.\n";
	}
	MPI::Finalize();
	return 0; //end main
}


//************FUNCTIONS***************
// Given a chamber width/height, it returns a random value between -given/2 and given/2
double inChamber(double val){
	double test = ( (1.0*rand()/RAND_MAX) * val ) - (val*1.0/2.0);
	return test;
}

// Returns a random number between -1 and 1 for particle movement
double moveDist(){
	double test = pow(-1.0,rand() % 2) * (MAX_MOVE_DIST*rand()/RAND_MAX);
	return test;
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
			C=pow(yconst,2)+pow(xconst,2)-pow(0,2);
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

			if(t<tmin){
				tmin=t;
			}
		}
	}
}
