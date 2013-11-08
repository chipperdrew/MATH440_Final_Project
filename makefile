# Set compiler commands
#------------------------------------
CPP = g++
FLAGS = -O3
#------------------------------------
proj_files = serial.cpp
#q3_files = common.f90 Q3.f90

all: main_exe

main_exe: $(proj_files)
	$(CPP) $(FLAGS) *.cpp -o $@
#Q3_exe: $(q3_files)
#	$(FC90) $(FLAGS) $(q3_files) -o $@

clean:	
	@rm main_exe
