#include "Particle.h"


// 2-param constructor which takes x and y positions
Particle::Particle( double x_new, double y_new, double x_old, double y_old, double rad){
	setnewX(x_new);
	setnewY(y_new);
	setoldX(x_old);
	setoldY(y_old);
	setR(rad);
}

// Moves the particles according to given X and Y inputs
Particle Particle::moveParticle(double moveX , double moveY){
	setoldX(tox);
	setoldY(toy);

	// Check for X wall collisions
	if(fabs(fromx+moveX) > CHAMBER_WIDTH/2) {
		// Check if collision occurs on pos. wall
		if(fromx>0) {
			// Check if escaped
			double dist_from_wall = CHAMBER_WIDTH/2 -fromx;
			double time_collision = dist_from_wall / moveX;
			double y_at_time = time_collision*(moveY) + fromy;
			if(abs(y_at_time) < HALF_ESCAPE_WALL_WIDTH) {
				setnewX(fromx+moveX); // X escapes
			} else {
				setnewX(CHAMBER_WIDTH-moveX-fromx);
			}
		// Else collision on neg. wall (no escape possible)
		} else {
			setnewX(-CHAMBER_WIDTH-moveX-fromx);
		}
	// Else no collision
	} else {	
		setnewX(fromx + moveX);
	}	

	// Check for Y wall collisions
	if(fabs(fromy+moveY) > CHAMBER_HEIGHT/2.0) {
		if(fromy>0) {
			setnewY(CHAMBER_HEIGHT-moveY-fromy);
		} else {
			setnewY(-CHAMBER_HEIGHT-moveY-fromy);
		}	
	} else {
		setnewY(fromy + moveY);
	}
}

// Output a particle as an ordered pair of its location
std::ostream& operator<<(std::ostream& outgo , const Particle& outpart){
	outgo << "(" << outpart.getnewX() << " , " << outpart.getnewY() << ")";
}
