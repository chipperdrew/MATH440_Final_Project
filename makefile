# Set compiler commands
#------------------------------------
CPP = mpiCC #g++
FLAGS = -O3
#------------------------------------
proj_files = parallel.cpp
#q3_files = common.f90 Q3.f90

all: main_exe

main_exe: $(proj_files)
#	$(CPP) $(FLAGS) parallel.cpp Particle.cpp -o $@
	$(CPP) $(FLAGS) serial.cpp Particle.cpp -o $@

clean:	
	@rm main_exe particle_locs.txt
