# /**
# * @File        Makefile
# * @Author      Jiri Jaros, Vojtech Nikl, Marta Cudova
# * @Affiliation FIT, Brno University of Technology
# * @Email       jarosjir@fit.vutbr.cz, inikl@fit.vutbr.cz, icudova@fit.vutbr.cz
# * @Comments    Linux makefile for Salomon
# * 
# * @Tool        ARC project 2017
# * @Created     15 February  2015, 15:25
# * @LastModif   01 March  2019, 10:30
#
# */

#Load following modules first
# for processor:   module load intel/2018a HDF5/1.10.4-intel-2018a-serial
#   or             module load intel/2017a HDF5/1.8.18-intel-2017a-serial
#            

#SET PATHS 
HDF5_DIR=${EBROOTHDF5}

CXX  = icpc

CXXFLAGS       = -std=c++11 -O3 -qopenmp -I. 
CXXFLAGS_NOPHI = -xhost -I$(HDF5_DIR)/include
CXXFLAGS_PHI   = -mmic -I$(HDF5_PHI_DIR)/include

TARGET     = arc_generator
TARGET_PHI = arc_generator_phi

LDFLAGS       = -std=c++11 -O3 -qopenmp 
LDFLAGS_NOPHI = -xhost -L$(HDF5_DIR)/lib/ -Wl,-rpath,$(HDF5_DIR)/lib/
LDFLAGS_PHI   = -mmic -L$(HDF5_PHI_DIR)/lib/ -Wl,-rpath,$(HDF5_PHI_DIR)/lib/


all:    $(TARGET) 

#---------------------------------------------------------

$(TARGET):  $(TARGET).o          
	$(CXX) $(LDFLAGS) $(LDFLAGS_NOPHI) $(TARGET).o \
    -lhdf5_hl -lhdf5 \
    -o $@


$(TARGET).o : $(TARGET).cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_NOPHI) -c $(TARGET).cpp

#---------------------------------------------------------

phi:  $(TARGET_PHI).o           
	$(CXX) $(LDFLAGS) $(LDFLAGS_PHI) $(TARGET_PHI).o \
    -lhdf5_hl -lhdf5 \
    -o $(TARGET_PHI)


$(TARGET_PHI).o : $(TARGET).cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_PHI) -c $(TARGET).cpp -o $(TARGET_PHI).o

#---------------------------------------------------------

test:
	./$(TARGET) -o material.h5 -N 512 -H 100 -C 20

clean:
	rm -f *.o  $(TARGET) $(TARGET_PHI) material.h5

