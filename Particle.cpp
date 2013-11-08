

#include "Particle.h"
Particle::Particle(double x_pos, double y_pos){
	x=x_pos;
	y=y_pos;
}


Particle Particle::moveParticle(double moveX , double moveY){
	setX(moveX);
	setY(moveY);
}
std::ostream& operator<<(std::ostream& outgo , const Particle& outpart){
	outgo << "(" << outpart.getX() << " , " << outpart.getY() << ")";
}
