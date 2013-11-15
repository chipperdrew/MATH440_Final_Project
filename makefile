# Set compiler commands
#------------------------------------
CPP = mpiCC #g++
FLAGS = -O3
#------------------------------------
all: main_exe

#main_exe: $(parallel.cpp)
#	$(CPP) $(FLAGS) parallel.cpp Particle.cpp -o $@
main_exe: $(serial.cpp)
	$(CPP) $(FLAGS) serial.cpp Particle.cpp -o $@

clean:	
	@rm main_exe particle_locs.txt
