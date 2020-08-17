/**
 * @file        proj01.cpp
 * @author      Jiri Jaros, Vojtech Nikl and Marta Cudova\n
 *              Faculty of Information Technology \n
 *              Brno University of Technology \n
 *              {jarosjir,inikl,icudova}@fit.vutbr.cz
 *
 * @brief       Parallelisation of Heat Distribution Method in Heterogenous
 *              Media using OpenMP
 *
 * @version     2018
 * @date        19 February 2015, 16:22 (created) \n
 *              01 March 2019, 09:30 (last revised)
 *
 * @detail
 * This is the main file of the project. Add all code here.
 */


#include <string.h>
#include <string>
#include <ios>

#ifdef _OPENMP
  #include <omp.h>
#endif

#include <hdf5.h>
#include <sstream>
#include <immintrin.h>

#include "MaterialProperties.h"
#include "BasicRoutines.h"



using namespace std;


//----------------------------------------------------------------------------//
//---------------------------- Global variables ------------------------------//
//----------------------------------------------------------------------------//

/// Temperature data for sequential version.
float* seqResult = nullptr;
/// Temperature data for parallel method.
float* parResult = nullptr;

/// Parameters of the simulation
TParameters parameters;


//----------------------------------------------------------------------------//
//------------------------- Function declarations ----------------------------//
//----------------------------------------------------------------------------//

/// Sequential implementation of the Heat distribution
void SequentialHeatDistribution(float*                     seqResult,
                                const TMaterialProperties& materialProperties,
                                const TParameters&         parameters,
                                string                     outputFileName);

/// Parallel Implementation of the Heat distribution (Non-overlapped file output)
void ParallelHeatDistributionNonOverlapped(float*                     parResult,
                                           const TMaterialProperties& materialProperties,
                                           const TParameters&         parameters,
                                           string                     outputFileName);

/// Parallel Implementation of the Heat distribution Using Tasks (Non-overlapped file output)
void ParallelHeatDistributionNonOverlappedTasks(float*                     parResult,
                                                const TMaterialProperties& materialProperties,
                                                const TParameters&         parameters,
                                                string                     outputFileName);
                                           
/// Parallel Implementation of the Heat distribution (Overlapped file output)
void ParallelHeatDistributionOverlapped(float*                     parResult,
                                        const TMaterialProperties& materialProperties,
                                        const TParameters&         parameters,
                                        string                     outputFileName);                                           

/// Store time step into output file
void StoreDataIntoFile(hid_t        h5fileId,
                       const float* data,
                       const size_t edgeSize,
                       const size_t snapshotId,
                       const size_t iteration);


/// Bring data into cache (useful for small domains)
void WarmUp(float*                     seqResult,
            const TMaterialProperties& materialProperties,
            const TParameters&         parameters);


//----------------------------------------------------------------------------//
//------------------------- Function implementation  -------------------------//
//----------------------------------------------------------------------------//


/**
 * Bring data into cache
 * @param [out] seqResult          - Final heat distribution
 * @param [in]  materialProperties - Material properties
 * @param [in]  parameters         - parameters of the simulation
 */
void WarmUp(float*                     seqResult,
            const TMaterialProperties& materialProperties,
            const TParameters&         parameters)
{

  TParameters warmUpParams(parameters);
  warmUpParams.nIterations = 1;  
  warmUpParams.batchMode   = false;
  warmUpParams.warmUp      = true;
  
  SequentialHeatDistribution(seqResult,
                             materialProperties,
                             warmUpParams,
                             "");

}// end of WarmUp
//------------------------------------------------------------------------------




/**
 * Sequential version of the Heat distribution in heterogenous 2D medium
 * @param [out] seqResult          - Final heat distribution
 * @param [in]  materialProperties - Material properties
 * @param [in]  parameters         - parameters of the simulation
 * @param [in]  outputFileName     - Output file name (if NULL string, do not store)
 *
 */
void SequentialHeatDistribution(float*                     seqResult,
                                const TMaterialProperties& materialProperties,
                                const TParameters&         parameters,
                                string                     outputFileName)
{

  // [1] Create a new output hdf5 file
  hid_t file_id = H5I_INVALID_HID;
  
  if (outputFileName != "")
  {
    if (outputFileName.find(".h5") == string::npos)
      outputFileName.append("_seq.h5");
    else
      outputFileName.insert(outputFileName.find_last_of("."), "_seq");
    
    file_id = H5Fcreate(outputFileName.c_str(),
                        H5F_ACC_TRUNC,
                        H5P_DEFAULT,
                        H5P_DEFAULT);
    if (file_id < 0) 
      throw(ios::failure("Cannot create output file"));
  }


  // [2] A temporary array is needed to prevent mixing of data form step t and t+1
  float* tempArray = (float*) _mm_malloc(materialProperties.nGridPoints * 
                                         sizeof(float), DATA_ALIGNMENT);
                                           
  // [3] init of arrays
  for  (size_t i = 0; i < materialProperties.nGridPoints; i++)
  {
    tempArray[i] = materialProperties.initTemp[i];
    seqResult[i] = materialProperties.initTemp[i];
  }

  // [4] t+1 values
  float* newTemp = tempArray;
  // t - values
  float* oldTemp = seqResult;

 
  if (!(parameters.warmUp || parameters.batchMode)) 
    printf("Starting sequential simulation... \n");
  
  //---------------------- [5] press the stop watch ------------------------------//
  double elapsedTime = omp_get_wtime();
  size_t i, j;
  size_t iteration, printCounter = 1;
  float  middleColAvgTemp = 0.0f;

  // [6] Start the iterative simulation
  for (iteration = 0; iteration < parameters.nIterations; iteration++)
  {
    // calculate one iteration of the heat distribution
    // We skip the grid points at the edges
    for (i = 2; i < materialProperties.edgeSize - 2; i++)
    {
      #pragma omp simd
      for (j = 2; j < materialProperties.edgeSize - 2; j++)
      {
        // [a)] Calculate neighbor indices
        const size_t center    =  i * materialProperties.edgeSize + j;
        const size_t top[2]    =  {center - materialProperties.edgeSize,
                                   center - 2 * materialProperties.edgeSize};
        const size_t bottom[2] =  {center + materialProperties.edgeSize,
                                   center + 2 * materialProperties.edgeSize};
        const size_t left[2]   =  {center - 1, center - 2};
        const size_t right[2]  =  {center + 1, center + 2};

        // [b)] The reciprocal value of the sum of domain parameters for normalization
        const float frec = 1.0f / (materialProperties.domainParams[top[0]]    +
                                   materialProperties.domainParams[top[1]]    +
                                   materialProperties.domainParams[bottom[0]] +
                                   materialProperties.domainParams[bottom[1]] +
                                   materialProperties.domainParams[left[0]]   +
                                   materialProperties.domainParams[left[1]]   + 
                                   materialProperties.domainParams[right[0]]  +
                                   materialProperties.domainParams[right[1]]  +
                                   materialProperties.domainParams[center]    );

        // [c)] Calculate new temperature in the grid point
        float pointTemp = 
              oldTemp[top[0]]    * materialProperties.domainParams[top[0]]    * frec +
              oldTemp[top[1]]    * materialProperties.domainParams[top[1]]    * frec +
              oldTemp[bottom[0]] * materialProperties.domainParams[bottom[0]] * frec +
              oldTemp[bottom[1]] * materialProperties.domainParams[bottom[1]] * frec +
              oldTemp[left[0]]   * materialProperties.domainParams[left[0]]   * frec +
              oldTemp[left[1]]   * materialProperties.domainParams[left[1]]   * frec +
              oldTemp[right[0]]  * materialProperties.domainParams[right[0]]  * frec +
              oldTemp[right[1]]  * materialProperties.domainParams[right[1]]  * frec +
              oldTemp[center]    * materialProperties.domainParams[center]    * frec;
	        

        // [d)] Remove some of the heat due to air flow (5% of the new air)
        pointTemp = (materialProperties.domainMap[center] == 0)  
                    ? (parameters.airFlowRate * materialProperties.CoolerTemp) + ((1.f - parameters.airFlowRate) * pointTemp)
                    : pointTemp;

        newTemp[center] = pointTemp;

      } // for j
    }// for i

    // [7] Calculate average temperature in the middle column
    middleColAvgTemp = 0.0f;
    
    for (i = 0; i < materialProperties.edgeSize; i++)
    {
      middleColAvgTemp += newTemp[i * materialProperties.edgeSize +
                                  materialProperties.edgeSize / 2];
    }
   
    middleColAvgTemp /= materialProperties.edgeSize;                         

    // [8] Store time step in the output file if necessary
    if ((file_id != H5I_INVALID_HID)  && (parameters.diskWriteIntensity > 0) && ((iteration % parameters.diskWriteIntensity) == 0))
    {
      StoreDataIntoFile(file_id,
                        newTemp,
                        materialProperties.edgeSize,
                        iteration / parameters.diskWriteIntensity,
                        iteration);
    }

    // [9] Swap new and old values
    swap(newTemp, oldTemp);

    // [10] Print progress and average temperature of the middle column                
    if ( ((float)(iteration) >= (parameters.nIterations - 1) / 10.0f * (float)printCounter) 
        && (!(parameters.warmUp || parameters.batchMode)))
    {
      printf("Progress %ld%% (Average Temperature %.2f degrees)\n", 
             (iteration + 1) * 100L / (parameters.nIterations), middleColAvgTemp);
      printCounter++;
    }
  }// for iteration

  //-------------------- stop the stop watch  --------------------------------//  
  double totalTime = omp_get_wtime() - elapsedTime;
  
  // [11] Print final result
  if (!parameters.warmUp)
  {
    if (!parameters.batchMode)
      printf("\nExecution time of sequential version: %.5fs\n", totalTime);
    else
      printf("%s;%s;%f;%e;%e\n", outputFileName.c_str(), "seq", 
                                 middleColAvgTemp, totalTime, 
                                 totalTime / parameters.nIterations);   
  }

  // Close the output file
  if (file_id != H5I_INVALID_HID) 
    H5Fclose(file_id);

  // [12] Return correct results in the correct array
  if (parameters.nIterations & 1)
  {
    memcpy(seqResult, tempArray, materialProperties.nGridPoints * sizeof(float));
  }

  _mm_free(tempArray);
}// end of SequentialHeatDistribution
//------------------------------------------------------------------------------

/**
 * Parallel version of the Heat distribution in heterogenous 2D medium
 * with non-overlapped file output. 
 * @param [out] parResult          - Final heat distribution
 * @param [in]  materialProperties - Material properties
 * @param [in]  parameters         - parameters of the simulation
 * @param [in]  outputFileName     - Output file name (if NULL string, do not store)
 *
 * @note This is one of two functions that students are allowed to change and 
 *       optimize. Do not modify the given code, add only openmp pragmas or 
 *       openmp calls.                                                      
 */
void ParallelHeatDistributionNonOverlapped(float*                     parResult,
                                           const TMaterialProperties& materialProperties,
                                           const TParameters&         parameters,
                                           string                     outputFileName)
{
  // Create a new output hdf5 file
  hid_t file_id = H5I_INVALID_HID;
  
  if (outputFileName != "")
  {
    if (outputFileName.find(".h5") == string::npos)
      outputFileName.append("_par1.h5");
    else
      outputFileName.insert(outputFileName.find_last_of("."), "_par1");
    
    file_id = H5Fcreate(outputFileName.c_str(),
                        H5F_ACC_TRUNC,
                        H5P_DEFAULT,
                        H5P_DEFAULT);
    if (file_id < 0) 
      throw(ios::failure("Cannot create output file"));
  }

  // we need a temporary array to prevent mixing of data form step t and t+1
  float* tempArray = (float*) _mm_malloc(materialProperties.nGridPoints * sizeof(float),
                                        DATA_ALIGNMENT);
  // t+1 values
  float* newTemp = tempArray;
  // t - values
  float* oldTemp = parResult;

  if (!parameters.batchMode) 
  {
    printf("\nStarting parallel simulation (non-overlapped) ... \n");
  }
  //---------------------- prest the stop watch ------------------------------//
  double elapsedTime = omp_get_wtime();
  size_t i, j;
  size_t iteration, printCounter = 1;
  float middleColAvgTemp = 0.0f;

  //--------------------------------------------------------------------------//
  //-------- START OF THE PART WHERE STUDENTS MAY ADD/EDIT OMP PRAGMAS -------//
  //--------------------------------------------------------------------------//,
  //private(iteration) lokalni kopie pro kazde vlakno - jsou inicializovany na default constructor
  //firstprivate(printCounter) lokalni kopie pro kazde vlakno - okopirovani puvodni hodnoty   
  #pragma omp parallel private(iteration) firstprivate(printCounter)
  {
    //automaticka paralelizace smycky
    #pragma omp for
    for (i = 0; i < materialProperties.nGridPoints; i++)
    {
      tempArray[i] = materialProperties.initTemp[i];
      parResult[i] = materialProperties.initTemp[i];
    }
    
    for (iteration = 0; iteration < parameters.nIterations; iteration++)
    {
      // calculate one iteration of the heat distribution
      // We skip the grid points at the edges
      #pragma omp for private(j) firstprivate(newTemp)
      //i je implicitne privatni
      for (i = 2; i < materialProperties.edgeSize - 2; i++)
      {
        #pragma omp simd
        for (j = 2; j < materialProperties.edgeSize - 2; j++)
        {
          // [a)] Calculate neighbor indices
          const size_t center =  i * materialProperties.edgeSize + j;
          const size_t top[2]    =  {center - materialProperties.edgeSize,
                                     center - 2 * materialProperties.edgeSize};
          const size_t bottom[2] =  {center + materialProperties.edgeSize,
                                     center + 2 * materialProperties.edgeSize};
          const size_t left[2]   =  {center - 1, center - 2};
          const size_t right[2]  =  {center + 1, center + 2};
  
          // [b)] The reciprocal value of the sum of domain parameters for normalization
          const float frec = 1.0f / (materialProperties.domainParams[top[0]]    +
                                     materialProperties.domainParams[top[1]]    +
                                     materialProperties.domainParams[bottom[0]] +
                                     materialProperties.domainParams[bottom[1]] +
                                     materialProperties.domainParams[left[0]]   +
                                     materialProperties.domainParams[left[1]]   + 
                                     materialProperties.domainParams[right[0]]  +
                                     materialProperties.domainParams[right[1]]  +
                                     materialProperties.domainParams[center]    );
  
          // [c)] Calculate new temperature in the grid point
          float pointTemp = 
                oldTemp[top[0]]    * materialProperties.domainParams[top[0]]    * frec +
                oldTemp[top[1]]    * materialProperties.domainParams[top[1]]    * frec +
                oldTemp[bottom[0]] * materialProperties.domainParams[bottom[0]] * frec +
                oldTemp[bottom[1]] * materialProperties.domainParams[bottom[1]] * frec +
                oldTemp[left[0]]   * materialProperties.domainParams[left[0]]   * frec +
                oldTemp[left[1]]   * materialProperties.domainParams[left[1]]   * frec +
                oldTemp[right[0]]  * materialProperties.domainParams[right[0]]  * frec +
                oldTemp[right[1]]  * materialProperties.domainParams[right[1]]  * frec +
                oldTemp[center]    * materialProperties.domainParams[center]    * frec;
            
  
          // [d)] Remove some of the heat due to air flow (5% of the new air)
          pointTemp = (materialProperties.domainMap[center] == 0)  
                      ? (parameters.airFlowRate * materialProperties.CoolerTemp) + ((1.f - parameters.airFlowRate) * pointTemp)
                      : pointTemp;
  
          newTemp[center] = pointTemp;
  
        } // for j
      }// for i
 
      //calculate average temperature in the middle column
      #pragma omp single
      {
        middleColAvgTemp = 0.0f;
      }
      #pragma omp for reduction (+:middleColAvgTemp)
      for (i = 0; i < materialProperties.edgeSize; i++)
      {
        middleColAvgTemp += newTemp[i*materialProperties.edgeSize +
                            materialProperties.edgeSize / 2];
      }
   
      //zapis provadi hlavni vlakno 
      #pragma omp master
      {
        middleColAvgTemp /= materialProperties.edgeSize;                         
        
        // Store time step in the output file if necessary
        if ((file_id != H5I_INVALID_HID)  && (parameters.diskWriteIntensity > 0) && ((iteration % parameters.diskWriteIntensity) == 0))
        {
          StoreDataIntoFile(file_id,
                            newTemp,
                            materialProperties.edgeSize,
                            iteration / parameters.diskWriteIntensity,
                            iteration);
        }

        // swap new and old values
        swap(newTemp, oldTemp);
      
        if ( ((float)(iteration) >= (parameters.nIterations - 1) / 10.0f * (float)printCounter) 
            && !parameters.batchMode)
        {
          printf("Progress %ld%% (Average Temperature %.2f degrees)\n", 
                (iteration + 1) * 100L / (parameters.nIterations), middleColAvgTemp);
          ++printCounter;
        }
      }//end pragma master
      //master neobsahuje implicitni barieru na konci
      #pragma omp barrier
    }// for iteration
  } // pragma parallel  

  //--------------------------------------------------------------------------//
  //------------ END OF THE PART WHERE STUDENTS MAY ADD OMP PRAGMAS ----------//
  //--------------------------------------------------------------------------//
  
  double totalTime = omp_get_wtime() - elapsedTime;
  
  if (!parameters.batchMode)
  {
    printf("\nExecution time of parallel (non-overlapped) version: %.5fs\n", totalTime);
  }
  else
  {
    printf("%s;%s;%f;%e;%e\n", outputFileName.c_str(), "par1", 
                               middleColAvgTemp, totalTime, 
                               totalTime / parameters.nIterations);  
  }

  //-------------------- stop the stop watch  --------------------------------//

  // Close the output file
  if (file_id != H5I_INVALID_HID) H5Fclose(file_id);

  // return correct results in the correct array
  if (parameters.nIterations & 1)
  {
    memcpy(parResult, tempArray, materialProperties.nGridPoints * sizeof(float));
  }

  _mm_free(tempArray);
}// end of ParallelHeatDistribution
//------------------------------------------------------------------------------
 



/**
 * Parallel version of the Heat distribution in heterogenous 2D medium
 * with non-overlapped file output. 
 * @param [out] parResult          - Final heat distribution
 * @param [in]  materialProperties - Material properties
 * @param [in]  parameters         - parameters of the simulation
 * @param [in]  outputFileName     - Output file name (if NULL string, do not store)
 *
 * @note This is one of two functions that students are allowed to change and 
 *       optimize. Do not modify the given code, add only openmp pragmas or 
 *       openmp calls.                                                      
 */
void ParallelHeatDistributionNonOverlappedTasks(float*                     parResult,
                                                const TMaterialProperties& materialProperties,
                                                const TParameters&         parameters,
                                                string                     outputFileName)
{
  // Create a new output hdf5 file
  hid_t file_id = H5I_INVALID_HID;
  
  if (outputFileName != "")
  {
    if (outputFileName.find(".h5") == string::npos)
      outputFileName.append("_par2.h5");
    else
      outputFileName.insert(outputFileName.find_last_of("."), "_par2");
    
    file_id = H5Fcreate(outputFileName.c_str(),
                        H5F_ACC_TRUNC,
                        H5P_DEFAULT,
                        H5P_DEFAULT);
    if (file_id < 0) 
      throw(ios::failure("Cannot create output file"));
  }

  // we need a temporary array to prevent mixing of data form step t and t+1
  float* tempArray = (float *) _mm_malloc(materialProperties.nGridPoints * sizeof(float),
                                          DATA_ALIGNMENT);
  // t+1 values
  float* newTemp = tempArray;
  // t - values
  float* oldTemp = parResult;

  if (!parameters.batchMode) 
    printf("\nStarting parallel simulation (non-overlapped) ... \n");
  
  //---------------------- prest the stop watch ------------------------------//
  double elapsedTime = omp_get_wtime();
  size_t i, j;
  size_t iteration, printCounter = 1;
  float  middleColAvgTemp = 0.0f;

  //--------------------------------------------------------------------------//
  //-------- START OF THE PART WHERE STUDENTS MAY ADD/EDIT OMP PRAGMAS -------//
  //--------------------------------------------------------------------------//  
  #pragma omp parallel private(iteration) firstprivate(printCounter)
  {
    #pragma omp single
    {
      #pragma omp task
      {
        for  (i = 0; i < materialProperties.nGridPoints; i++)
        {
          tempArray[i] = materialProperties.initTemp[i];
          parResult[i] = materialProperties.initTemp[i];
        }
      }
    }
    
    for (iteration = 0; iteration < parameters.nIterations; iteration++)
    {
      // calculate one iteration of the heat distribution
      // We skip the grid points at the edges
      // only one trhead generates tasks, the nowait allows that thread to do some work 
      // as well 
      #pragma omp single nowait private(i,j) firstprivate(newTemp)
      {
        #pragma omp task
        {
          for (i = 2; i < materialProperties.edgeSize - 2; i++)
          {
            #pragma omp simd
            for (j = 2; j < materialProperties.edgeSize - 2; j++)
            {
              // [a)] Calculate neighbor indices
              const size_t center =  i * materialProperties.edgeSize + j;
              const size_t top[2]    =  {center - materialProperties.edgeSize,
                                      center - 2 * materialProperties.edgeSize};
              const size_t bottom[2] =  {center + materialProperties.edgeSize,
                                      center + 2 * materialProperties.edgeSize};
              const size_t left[2]   =  {center - 1, center - 2};
              const size_t right[2]  =  {center + 1, center + 2};
      
              // [b)] The reciprocal value of the sum of domain parameters for normalization
              const float frec = 1.0f / (materialProperties.domainParams[top[0]]    +
                                         materialProperties.domainParams[top[1]]    +
                                         materialProperties.domainParams[bottom[0]] +
                                         materialProperties.domainParams[bottom[1]] +
                                         materialProperties.domainParams[left[0]]   +
                                         materialProperties.domainParams[left[1]]   + 
                                         materialProperties.domainParams[right[0]]  +
                                         materialProperties.domainParams[right[1]]  +
                                         materialProperties.domainParams[center]    );
      
              // [c)] Calculate new temperature in the grid point
              float pointTemp = 
                    oldTemp[top[0]]    * materialProperties.domainParams[top[0]]    * frec +
                    oldTemp[top[1]]    * materialProperties.domainParams[top[1]]    * frec +
                    oldTemp[bottom[0]] * materialProperties.domainParams[bottom[0]] * frec +
                    oldTemp[bottom[1]] * materialProperties.domainParams[bottom[1]] * frec +
                    oldTemp[left[0]]   * materialProperties.domainParams[left[0]]   * frec +
                    oldTemp[left[1]]   * materialProperties.domainParams[left[1]]   * frec +
                    oldTemp[right[0]]  * materialProperties.domainParams[right[0]]  * frec +
                    oldTemp[right[1]]  * materialProperties.domainParams[right[1]]  * frec +
                    oldTemp[center]    * materialProperties.domainParams[center]    * frec;
                
      
              // [d)] Remove some of the heat due to air flow (5% of the new air)
              pointTemp = (materialProperties.domainMap[center] == 0)  
                          ? (parameters.airFlowRate * materialProperties.CoolerTemp) + ((1.f - parameters.airFlowRate) * pointTemp)
                          : pointTemp;
      
              newTemp[center] = pointTemp;
              
            } // for j
          }// for i
        }
      }
      //kvuli nowait
      #pragma omp barrier 

      //calculate average temperature in the middle column
      #pragma omp single
      {
        middleColAvgTemp = 0.0f;
        #pragma omp task
        {          
          for (i = 0; i < materialProperties.edgeSize; i++)
          {
            middleColAvgTemp += newTemp[i * materialProperties.edgeSize +
                                        materialProperties.edgeSize / 2];
          }
        }
      }

      //zapis provadi hlavni vlakno 
      #pragma omp master
      {
        middleColAvgTemp /= materialProperties.edgeSize;                         
            
        // Store time step in the output file if necessary
        if ((file_id != H5I_INVALID_HID) && (parameters.diskWriteIntensity > 0) && ((iteration % parameters.diskWriteIntensity) == 0))
        {
          StoreDataIntoFile(file_id,
                            newTemp,
                            materialProperties.edgeSize,
                            iteration / parameters.diskWriteIntensity,
                            iteration);
        }

        // swap new and old values
        swap(newTemp, oldTemp);
            
        if ( ((float)(iteration) >= (parameters.nIterations - 1) / 10.0f * (float)printCounter) 
                && !parameters.batchMode)
        {
          printf("Progress %ld%% (Average Temperature %.2f degrees)\n", 
                 (iteration+1) * 100L / (parameters.nIterations), middleColAvgTemp);
            
          printCounter++;
        } 
      }//end pragma master
      #pragma omp barrier   
    }// for iteration
  } // pragma parallel  

  //--------------------------------------------------------------------------//
  //------------ END OF THE PART WHERE STUDENTS MAY ADD OMP PRAGMAS ----------//
  //--------------------------------------------------------------------------//
  
  double totalTime = omp_get_wtime() - elapsedTime;
  
  if (!parameters.batchMode)
    printf("\nExecution time of parallel (non-overlapped, wit tasks) version: %.5fs\n", totalTime);
  else
    printf("%s;%s;%f;%e;%e\n", outputFileName.c_str(), "par2", 
                               middleColAvgTemp, totalTime, 
                               totalTime / parameters.nIterations);  

  //-------------------- stop the stop watch  --------------------------------//

  // Close the output file
  if (file_id != H5I_INVALID_HID) H5Fclose(file_id);

  // return correct results in the correct array
  if (parameters.nIterations & 1)
  {
    memcpy(parResult, tempArray, materialProperties.nGridPoints * sizeof(float));
  }

  _mm_free(tempArray);
}// end of ParallelHeatDistributionTasks
//------------------------------------------------------------------------------
 




/**
 * Parallel version of the Heat distribution in heterogenous 2D medium
 * with overlapped file output. 
 * @param [out] parResult          - Final heat distribution
 * @param [in]  materialProperties - Material properties
 * @param [in]  parameters         - parameters of the simulation
 * @param [in]  outputFileName     - Output file name (if NULL string, do not store)
 *
 * @note This is one of two functions that students are allowed to change and 
 *       optimize. Do not modify the given code, add only openmp pragmas or 
 *       openmp calls.                                                      
 */
void ParallelHeatDistributionOverlapped(float*                     parResult,
                                        const TMaterialProperties& materialProperties,
                                        const TParameters&         parameters,
                                        string                     outputFileName)
{
  // Create a new output hdf5 file
  hid_t file_id = H5I_INVALID_HID;
  
  if (outputFileName != "")
  {
    if (outputFileName.find(".h5") == string::npos)
      outputFileName.append("_par3.h5");
    else
      outputFileName.insert(outputFileName.find_last_of("."), "_par3");
    
    file_id = H5Fcreate(outputFileName.c_str(),
                        H5F_ACC_TRUNC,
                        H5P_DEFAULT,
                        H5P_DEFAULT);
    if (file_id < 0) 
      throw(ios::failure("Cannot create output file"));
  }

  // we need a temporary array to prevent mixing of data form step t and t+1
  float* tempArray = (float *) _mm_malloc(materialProperties.nGridPoints * sizeof(float),
                                          DATA_ALIGNMENT);
  float* buffer    = (float *) _mm_malloc(materialProperties.nGridPoints * sizeof(float), 
                                          DATA_ALIGNMENT);
  bool bufferFreeFlag    = false;
  bool writeFinishedFlag = false;
  
  // t+1 values
  float* newTemp = tempArray;
  // t - values
  float* oldTemp = parResult;

  if (!parameters.batchMode) 
    printf("\nStarting parallel simulation (overlapped) ... \n");
    
  //---------------------- prest the stop watch ------------------------------//
  double elapsedTime = omp_get_wtime();
  size_t i, j;
  size_t iteration;
  size_t printCounter = 1;
  float  middleColAvgTemp = 0.0f;

  //--------------------------------------------------------------------------//
  //---------------------------- START OF YOUR CODE --------------------------//
  //--------------------------------------------------------------------------//
  //vnoreny parallel
  omp_set_nested(true);
  size_t shareIteration;

  //1.cast inicializace
  #pragma omp parallel for    
  for (i = 0; i < materialProperties.nGridPoints; i++)
  {
    tempArray[i] = materialProperties.initTemp[i];
    parResult[i] = materialProperties.initTemp[i];      
    buffer[i]    = 0.0f;
  }

  //sdilene flagy mezi sekcemi
  //2 vlakna pro sekci vypocet + I/O
  #pragma omp parallel num_threads(2) shared(bufferFreeFlag, writeFinishedFlag) firstprivate(printCounter)
  {
    #pragma omp sections
    {    
      //2.cast vypocet
      #pragma omp section
      {
        //vnoreny parallelismus pro vypocet, kde se nastartuji zbyle vlakna
        #pragma omp parallel num_threads(parameters.nThreads - 1) private(iteration) firstprivate(printCounter)
        {
          for (iteration = 0; iteration < parameters.nIterations; iteration++)
          {
            // calculate one iteration of the heat distribution
            // We skip the grid points at the edges
            #pragma omp for private(j) firstprivate(newTemp)
            //i je implicitne privatni
            for (i = 2; i < materialProperties.edgeSize - 2; i++)
            {
              #pragma omp simd
              for (j = 2; j < materialProperties.edgeSize - 2; j++)
              {
                // [a)] Calculate neighbor indices
                const size_t center =  i * materialProperties.edgeSize + j;
                const size_t top[2]    =  {center - materialProperties.edgeSize,
                                           center - 2 * materialProperties.edgeSize};
                const size_t bottom[2] =  {center + materialProperties.edgeSize,
                                           center + 2 * materialProperties.edgeSize};
                const size_t left[2]   =  {center - 1, center - 2};
                const size_t right[2]  =  {center + 1, center + 2};
        
                // [b)] The reciprocal value of the sum of domain parameters for normalization
                const float frec = 1.0f / (materialProperties.domainParams[top[0]]    +
                                           materialProperties.domainParams[top[1]]    +
                                           materialProperties.domainParams[bottom[0]] +
                                           materialProperties.domainParams[bottom[1]] +
                                           materialProperties.domainParams[left[0]]   +
                                           materialProperties.domainParams[left[1]]   + 
                                           materialProperties.domainParams[right[0]]  +
                                           materialProperties.domainParams[right[1]]  +
                                           materialProperties.domainParams[center]    );
        
                // [c)] Calculate new temperature in the grid point
                float pointTemp = 
                      oldTemp[top[0]]    * materialProperties.domainParams[top[0]]    * frec +
                      oldTemp[top[1]]    * materialProperties.domainParams[top[1]]    * frec +
                      oldTemp[bottom[0]] * materialProperties.domainParams[bottom[0]] * frec +
                      oldTemp[bottom[1]] * materialProperties.domainParams[bottom[1]] * frec +
                      oldTemp[left[0]]   * materialProperties.domainParams[left[0]]   * frec +
                      oldTemp[left[1]]   * materialProperties.domainParams[left[1]]   * frec +
                      oldTemp[right[0]]  * materialProperties.domainParams[right[0]]  * frec +
                      oldTemp[right[1]]  * materialProperties.domainParams[right[1]]  * frec +
                      oldTemp[center]    * materialProperties.domainParams[center]    * frec;
                  
        
                // [d)] Remove some of the heat due to air flow (5% of the new air)
                pointTemp = (materialProperties.domainMap[center] == 0)  
                            ? (parameters.airFlowRate * materialProperties.CoolerTemp) + ((1.f - parameters.airFlowRate) * pointTemp)
                            : pointTemp;
        
                newTemp[center] = pointTemp;
        
              } // for j
            }// for i
       
            //calculate average temperature in the middle column
            #pragma omp single
            {
              middleColAvgTemp = 0.0f;
            }

            #pragma omp for reduction (+:middleColAvgTemp)
            for (i = 0; i < materialProperties.edgeSize; i++)
            {
              middleColAvgTemp += newTemp[i*materialProperties.edgeSize +
                                  materialProperties.edgeSize / 2];
            }

            #pragma omp single
            {
              middleColAvgTemp /= materialProperties.edgeSize; 
            }
            // Store time step in the output file if necessary
            if ((file_id != H5I_INVALID_HID)  && (parameters.diskWriteIntensity > 0) && ((iteration % parameters.diskWriteIntensity) == 0))
            {
              bool tempBufferFreeFlag;
              //aktivni cekani
              while(1)
              {
                //zjisteni zda do bufferu muzu zapisovat
                #pragma omp flush(bufferFreeFlag)
                #pragma omp atomic read
                tempBufferFreeFlag = bufferFreeFlag;
                //pokud buffer nechce vyprazdnit, zapisuji tam data
                if (!tempBufferFreeFlag)
                {
                  #pragma omp for firstprivate(buffer)
                  for (uint k = 0; k < materialProperties.nGridPoints; ++k)
                  {
                    buffer[k] = newTemp[k];
                  }

                  #pragma omp single
                  {
                    //pocet iteraci si ulozim pro zapis do souboru
                    #pragma omp flush(shareIteration)
                    #pragma omp atomic write
                    shareIteration = iteration;
                    //nastaveni flagu na vyprazdneni bufferu ->zapis do souboru
                    #pragma omp flush(bufferFreeFlag)
                    #pragma omp atomic write
                    bufferFreeFlag = true;
                  }
                  break;
                }
              }//end while
              #pragma omp flush
            }

            #pragma omp master
            {
              // swap new and old values
              swap(newTemp, oldTemp);
            
              if ( ((float)(iteration) >= (parameters.nIterations - 1) / 10.0f * (float)printCounter) 
                  && !parameters.batchMode)
              {
                printf("Progress %ld%% (Average Temperature %.2f degrees)\n", 
                      (iteration + 1) * 100L / (parameters.nIterations), middleColAvgTemp);
                ++printCounter;
              }
            }//end pragma master
            //master neobsahuje implicitni barieru na konci
            #pragma omp barrier
          } // for iteration
        } //end inner parallel
        //nastaveni flagu ze skoncil zapis  
        #pragma omp flush
        #pragma omp atomic write
        writeFinishedFlag = true;
        #pragma omp flush(writeFinishedFlag)
      }// end section

      //3.cast zapis
      #pragma omp section
      {
        //pomocne promene
        bool tempBufferFreeFlag;
        bool tempFinished;
        size_t tempIteration = 0;
        //aktivni cekani
        while(1)
        { //zjisteni zda chce buffer vyprazdnit -> tedy zapsat do souboru
          #pragma omp flush(bufferFreeFlag)
          #pragma omp atomic read
          tempBufferFreeFlag = bufferFreeFlag;
          if(tempBufferFreeFlag)
          { //pokud ano, zjistim pocet iteraci, potrebnych ve funkci StoreDataIntoFile
            #pragma omp flush(shareIteration)
            #pragma omp atomic read
            tempIteration = shareIteration;

            StoreDataIntoFile(file_id,
                  buffer,
                  materialProperties.edgeSize,
                  tempIteration / parameters.diskWriteIntensity,
                  tempIteration);
            //po zapisu nastavim flag bufferu zpet 
            #pragma omp flush(bufferFreeFlag)
            #pragma omp atomic write
            bufferFreeFlag = false;
          }
          //pokud zapis skoncil, vyskocim z aktivniho cekani
          #pragma omp flush
          #pragma omp atomic read
          tempFinished = writeFinishedFlag;
          if(tempFinished)
          {
              break;
          }
        }//end while
        #pragma omp flush
      }//end section
    }// end sections
  } // pragma parallel  
  
      
  //--------------------------------------------------------------------------//
  //---------------------------- END OF YOUR CODE ----------------------------//
  //--------------------------------------------------------------------------// 
  
  double totalTime = omp_get_wtime() - elapsedTime;
  
  if (!parameters.batchMode)
    printf("\nExecution time of parallel (overlapped) version: %.5fs\n", totalTime);
  else
    printf("%s;%s;%f;%e;%e\n", outputFileName.c_str(), "par3", 
                               middleColAvgTemp, totalTime, 
                               totalTime / parameters.nIterations);   

  //-------------------- stop the stop watch  --------------------------------//

  // Close the output file
  if (file_id != H5I_INVALID_HID) H5Fclose(file_id);

  // return correct results in the correct array
  if (parameters.nIterations & 1)
  {
    memcpy(parResult, tempArray, materialProperties.nGridPoints * sizeof(float));
  }

  _mm_free(tempArray);
  _mm_free(buffer);
}// end of ParallelHeatDistribution
//------------------------------------------------------------------------------
  


/**
 * Store time step into output file (as a new dataset in Pixie format
 * @param [in] h5fileID  - handle to the output file
 * @param [in] Data      - data to write
 * @param [in] edgeSize  - size of the domain
 * @param [in] iteration - id of iteration);
 */
void StoreDataIntoFile(hid_t        h5fileId,
                       const float* data,
                       const size_t edgeSize,
                       const size_t snapshotId,
                       const size_t iteration)
{
  hid_t    dataset_id, dataspace_id, group_id, attribute_id;
  hsize_t  dims[2] = {edgeSize, edgeSize};

  string groupName = "Timestep_" + to_string((unsigned long long) snapshotId);

  // Create a group named "/Timestep_snapshotId" in the file.
  group_id = H5Gcreate(h5fileId,
                       groupName.c_str(),
                       H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);


  // Create the data space. (2D matrix)
  dataspace_id = H5Screate_simple(2, dims, NULL);

  // create a dataset for temperature and write data
  string datasetName = "Temperature";
  dataset_id = H5Dcreate(group_id,
                         datasetName.c_str(),
                         H5T_NATIVE_FLOAT,
                         dataspace_id,
                         H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
  H5Dwrite(dataset_id,
           H5T_NATIVE_FLOAT,H5S_ALL, H5S_ALL,H5P_DEFAULT,
           data);

  // close dataset
  H5Sclose(dataspace_id);


  // write attribute
  string atributeName = "Time";
  dataspace_id = H5Screate(H5S_SCALAR);
  attribute_id = H5Acreate2(group_id, atributeName.c_str(),
                            H5T_IEEE_F64LE, dataspace_id,
                            H5P_DEFAULT, H5P_DEFAULT);

  double snapshotTime = double(iteration);
  H5Awrite(attribute_id, H5T_IEEE_F64LE, &snapshotTime);
  H5Aclose(attribute_id);


  // Close the dataspace.
  H5Sclose(dataspace_id);

  // Close to the dataset.
  H5Dclose(dataset_id);
  
  // Close the group
  H5Gclose(group_id);
}// end of StoreDataIntoFile
//------------------------------------------------------------------------------





/**
 * Main function of the project
 * @param [in] argc
 * @param [in] argv
 * @return
 */
int main(int argc, char *argv[])
{
  ParseCommandline(argc, argv, parameters);

  // Create material properties and load from file
  TMaterialProperties materialProperties;
  try
  {
    materialProperties.LoadMaterialData(parameters.materialFileName);
  }
  catch (const std::ios::failure& e) // Error while processing HDF5 file
  {
    fprintf(stderr, "[ERROR]: Error while processing the HDF5 file.\n");  
    exit(EXIT_FAILURE); 
  }
  catch (const std::bad_alloc& e) // Error in allocation of material properties
  {
    fprintf(stderr, "[ERROR]: Bad allocation of material properties.\n");
    exit(EXIT_FAILURE);
  }
  
  parameters.edgeSize = materialProperties.edgeSize;

  parameters.PrintParameters();

  // Memory allocation for output matrices.
  seqResult = (float*) _mm_malloc(materialProperties.nGridPoints * sizeof(float),
                                  DATA_ALIGNMENT);
  parResult = (float*) _mm_malloc(materialProperties.nGridPoints * sizeof(float),
                                  DATA_ALIGNMENT);

  // first touch for seq version
  for (size_t i = 0; i < materialProperties.nGridPoints; i++)
  {
    seqResult[i] = 0.0f;
  }

  // first touch policy
  #pragma omp parallel for
  for (size_t i = 0; i < materialProperties.nGridPoints; i++)
  {
    parResult[i] = 0.0f;
  }


  /// Warm-up the simulation (bring data into cache)..
  WarmUp(seqResult, materialProperties, parameters);

  // run sequential version if needed
  if (parameters.IsRunSequntial())
  {
    SequentialHeatDistribution(seqResult,
                               materialProperties,
                               parameters,
                               parameters.outputFileName);
  }
  if (parameters.IsRunParallelNonOverlapped())
  {
    // run the parallel version with non-overlapped file output
    ParallelHeatDistributionNonOverlapped(parResult,
                                          materialProperties,
                                          parameters,
                                          parameters.outputFileName);

  }
  if (parameters.IsRunParallelNonOverlappedTasks())
  {
    // run the parallel version with non-overlapped file output
    ParallelHeatDistributionNonOverlappedTasks(parResult,
                                              materialProperties,
                                              parameters,
                                              parameters.outputFileName);
  }
  if (parameters.IsRunParallelOverlapped())
  {
    // run the parallel version with non-overlapped file output
    ParallelHeatDistributionOverlapped(parResult,
                                       materialProperties,
                                       parameters,
                                       parameters.outputFileName); 
  }
  
  // Validate the outputs
  if (parameters.IsValidation())
  {
    if (parameters.debugFlag)
    {
      printf("---------------- Sequential results ---------------\n");
      PrintArray(seqResult, materialProperties.edgeSize);

      printf("----------------- Parallel results ----------------\n");
      PrintArray(parResult, materialProperties.edgeSize);
    }

    if (VerifyResults(seqResult, parResult, parameters))
    {
      printf("Verification OK \n");
    }
    else
    {
      printf("Verification FAILED \n ");
    }
  }

  /* Memory deallocation*/
  _mm_free(seqResult);
  _mm_free(parResult);

  return EXIT_SUCCESS;
}// end of main
//------------------------------------------------------------------------------

