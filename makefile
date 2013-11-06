# Set compiler commands
#------------------------------------
CPP = mpic++ #g++
FLAGS = -O3
#------------------------------------
proj_files = serial.cpp
#q3_files = common.f90 Q3.f90

all: Main_exe

Main_exe: $(proj_files)
	$(CPP) $(FLAGS) $(proj_files) -o $@
#Q3_exe: $(q3_files)
#	$(FC90) $(FLAGS) $(q3_files) -o $@

clean:	
	@rm Main_exe
