class Particle{
	private:
		double x, y;
	public:
		Particle(double x_pos=0, double y_pos=0);
		Particle moveParticle(double moveX, double moveY);
		// Getters 
		// TODO: Remove after testing (unless need arsises)
		double getX() const { return x; }
		double getY() const { return y; }
		void setX(double x_new) { x = x_new; }
		void setY(double y_new) { y = y_new; }
		
};
ostream& operator<<(ostream& outgo , const Particle& outpart);