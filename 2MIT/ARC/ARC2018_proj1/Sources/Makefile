# /**
# * @File        Makefile
# * @Author      Jiri Jaros, Vojtech Nikl, Marta Cudova
# * @Affiliation FIT, Brno University of Technology
# * @Email       jarosjir@fit.vutbr.cz, inikl@fit.vutbr.cz, icudova@fit.vutbr.cz
# * @Comments    Linux makefile for Salomon
# * 
# * @Tool        ARC project 2017
# * @Created     15 February  2015, 15:25
# * @LastModif   05 March  2018, 20:00
#
# */

#Load following modules first
# for processor:   module load intel/2018a HDF5/1.10.4-intel-2018a-serial
#   or             module load intel/2017a HDF5/1.8.18-intel-2017a-serial  



#SET PATHS
HDF5_DIR=${EBROOTHDF5}


CXX  = icpc
#CXX = g++

CXXFLAGS       = -W -Wall -Wextra -pedantic -std=c++11 -fast -xhost -qopenmp -I. 

TARGET     = arc_proj01
TARGET_PHI = arc_proj01_phi

LDFLAGS       = -std=c++11 -O3 -qopenmp -xhost
LDFLAGS_NOPHI = -xhost -L$(HDF5_DIR)/lib/ -Wl,-rpath,$(HDF5_DIR)/lib/
LDFLAGS_PHI   = -mmic -L$(HDF5_PHI_DIR)/lib/ -Wl,-rpath,$(HDF5_PHI_DIR)/lib/


all:    $(TARGET) 

#---------------------------------------------------------

$(TARGET):  $(TARGET).o MaterialProperties.o BasicRoutines.o         
	$(CXX) $(LDFLAGS) $(LDFLAGS_NOPHI) \
  $(TARGET).o MaterialProperties.o BasicRoutines.o \
    -lhdf5_hl -lhdf5 \
    -o $@


$(TARGET).o : proj01.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_NOPHI) -c proj01.cpp -o $(TARGET).o

MaterialProperties.o: MaterialProperties.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_NOPHI) -c MaterialProperties.cpp

BasicRoutines.o: BasicRoutines.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_NOPHI) -c BasicRoutines.cpp

#---------------------------------------------------------

phi:  $(TARGET_PHI).o MaterialProperties_phi.o BasicRoutines_phi.o         
	$(CXX) $(LDFLAGS) $(LDFLAGS_PHI) \
  $(TARGET_PHI).o MaterialProperties_phi.o BasicRoutines_phi.o \
    -lhdf5_hl -lhdf5 \
    -o $(TARGET_PHI)


$(TARGET_PHI).o : proj01.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_PHI) -c proj01.cpp -o $(TARGET_PHI).o
  
MaterialProperties_phi.o: MaterialProperties.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_PHI) -c MaterialProperties.cpp -o MaterialProperties_phi.o

BasicRoutines_phi.o: BasicRoutines.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_PHI) -c BasicRoutines.cpp -o BasicRoutines_phi.o

#---------------------------------------------------------

test:
	./$(TARGET) -n 1000 -m 1 -w 10 -t 16 -i ../DataGenerator/material.h5 -o test.h5
	
clean:
	rm -f *.o  $(TARGET) $(TARGET_PHI) result_seq.h5



