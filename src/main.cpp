#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>                // for gettimeofday()
#include <math.h>
#include "util.h"
#include <float.h>
#include <assert.h>
#include <unistd.h>
#include "extract.h"
#include "getmask.h"
#include "getqual.h"
#include "file.h"
#include "definitions.h"
#include "grad.h"
#include "pi.h"
#include "rbtrees.h"



/*
 *    Find the minimum and maximum values of a vector.
 */
void find_min_max(float *map, int length,
                  float *min_val, float *max_val,
                  int *index_min, int *index_max)
{
    int k, imin, imax;
    float min_=FLT_MAX, max_=FLT_MIN;
    
    for (k=0; k<length; k++) {
        if (map[k]<min_) {
            min_ = map[k];
            imin = k;
        }
        if (map[k]>max_) {
            max_ = map[k];
            imax = k;
        }
    }
    
    *min_val = min_;
    *max_val = max_;
    *index_min  = imin;
    *index_max  = imax;
}




/*
 *    Quality guide phase unwrapping algorithm
 */
void Quality_guide_phase_unwrapping(const char *data_path,
                                      const char *pname,
                                   int xsize,
                                   int ysize,
                                   int num_levels,
                                   int N,
                                   int flag_mask,
                                   int flag_qual,
                                   int int_qual_flag,
                                   float int_max_qual)
{
    /* data variables */
    
    int           *path;
    float         *phase, *qual_map, *soln, *mask;
    unsigned char *unwrap;
    
    
    /* other variables */
    
    FILE          *ifp, *ofp, *ifq, *ifm;
    char          prefix[120], fname[120];
    float         grad, minval, maxval, high_qual, quality, qual_int;
    int           k, length, index, p, q, iter=0;
    int           a, b, w, x, y;
    int           imin, imax, flag;
    
    
    /* adjoin-list variables */
    
    RBTrees *rbtrees = new RBTrees(xsize, ysize);
    
    
    
    /* * * * * * * * * * * * * * * * * * * * * * * *
     *                                             *
     *             ALLOCATE MEMORY                 *
     *                                             *
     * * * * * * * * * * * * * * * * * * * * * * * */
    
    length = xsize*ysize;
    
    AllocateFloat(&phase, length, "phase data");
    AllocateFloat(&qual_map, length, "quality data");
    AllocateFloat(&soln, length, "solution array");
    AllocateByte(&unwrap, length, "flag array");
    AllocateInt(&path, length, "integration path");
    AllocateFloat(&mask, length, "solution array");

    

    
    
    /* * * * * * * * * * * * * * * * * * * * * * * *
     *                                             *
     *      READ DATA: PHASE AND QUALITY MAP       *
     *                                             *
     * * * * * * * * * * * * * * * * * * * * * * * */
    
    
    // load phase
    strcpy(prefix, data_path);
    strcat(prefix,"\\data\\");
    strcat(prefix,pname);
    strcat(prefix, ".phase");
    
    OpenFile(&ifp, prefix, "rb");
    GetPhase(3, ifp, fname, phase, xsize, ysize);
    
    // normalize phase
    
    find_min_max(phase, length, &minval, &maxval, &imin, &imax);
    printf("Min&Max phase: %f %f\n",minval, maxval);
    
    for (k=0; k<length; k++)
        phase[k] = (phase[k] - minval)/(maxval-minval);
    
    
    
    // load mask
    
    if (flag_mask)
    {
        strcpy(prefix, data_path);
        strcat(prefix, "\\data\\");
        strcat(prefix, pname);
        strcat(prefix, ".mask");
        OpenFile(&ifm, prefix, "rb");
        GetPhase(3, ifm, fname, mask, xsize, ysize);
    }
    else
    {
        memset(mask, 1, sizeof(float)*length);
    }
    
    
    
    // load quality map and normalize data
    
    if (flag_qual)
    {
        strcpy(prefix, data_path);
        strcat(prefix,"\\data\\");
        strcat(prefix,pname);
        strcat(prefix, ".qual");
        OpenFile(&ifq, prefix, "rb");
        GetPhase(3, ifq, fname, qual_map, xsize, ysize);
    }
    else
    {
        GetQualityMap(gradient, qual_map, phase, unwrap, 1, xsize, ysize);
    }
    
    
    find_min_max(qual_map, length, &minval, &maxval, &imin, &imax);
    printf("\nMin. qual. val. %f\n", minval);
    printf("Max. qual. val. %f\n", maxval);
    
    
    for (k=0; k<length; k++) {
        qual_map[k] = (qual_map[k] - minval)/(maxval-minval);
        
        if (int_qual_flag)
            qual_map[k] = floorf(int_max_qual*qual_map[k]);
    }
    
    minval = 0;
    
    if (int_qual_flag)
        maxval = int_max_qual;
    else
        maxval = 1;
    
    
    
    if (flag_qual==0)
    {
        // save quality map
        strcpy(prefix, data_path);
        strcat(prefix, "\\data\\");
        strcat(prefix, pname);
        strcat(prefix, ".qual");
        OpenFile(&ifq, prefix, "w");
        WriteFloat(ifq, qual_map, length, fname);
    }
    

    
    
    /* * * * * * * * * * * * * * * * * * * * * * * *
     *                                             *
     *     QUALITY-GUIDE UNWRAPPING ALGORITHM      *
     *                                             *
     * * * * * * * * * * * * * * * * * * * * * * * */
    

    

    
    // Step 1. Find starting point (highest quality value).
    
    high_qual = maxval;
    index = imax;
    
    printf("\nStarting point: y=%d x=%d qual=%f index=%d\n", index/xsize, index%xsize, high_qual, index);
    
    
    // Step 2. Store its phase value in the solution array soln.
    
    soln[index] = phase[index];
    
    
    // Steo 2.1 save path integration
    
    path[index] = iter++;
    
    

    // Step 3. Mark as unwrapped.
    
    unwrap[index] = 1;
    
    

    // Step 4. Insert in the list.
    
    rbtrees->RB_insert_node(high_qual, index);
  
    rbtrees->higher = rbtrees->RBT;
    
    
    // unwrap while the adjoin list is not empty
    
    while (rbtrees->RBT != rbtrees->Tnill)
    {
        // Step 5.1. Remove the highest pixel
        
        rbtrees->get_higher(&qual_int, &index);
        
        rbtrees->RB_delete_higher_node(&(rbtrees->RBT), &(rbtrees->higher));
        y = index/xsize;
        x = index%xsize;
        

        // Step 6. Examine its four neighbor pixels.
        
        // left neighbor
        a = x - 1;
        b = y;
        k = b*xsize + a;
        if (a>=0 && unwrap[k]==0 && mask[k])
        {
            w = y*xsize + x-1;
            soln[k] = soln[index] + Gradient(phase[w], phase[w+1]);
            rbtrees->RB_insert_node(qual_map[k],k);
            unwrap[k] = 1;
            path[k] = iter++;
        }


        // right neighbor
        a = x + 1;
        b = y;
        k = b*xsize + a;
        if (a < xsize && unwrap[k]==0 && mask[k])
        {
            w = y*xsize + x;
            soln[k] = soln[index] - Gradient(phase[w], phase[w+1]);
            rbtrees->RB_insert_node(qual_map[k],k);
            unwrap[k] = 1;
            path[k] = iter++;
        }
        
        // upper neighbor
        a = x;
        b = y - 1;
        k = b*xsize + a;
        if (b >= 0 && unwrap[k]==0 && mask[k])
        {
            w = (y-1)*xsize + x;
            soln[k] = soln[index] + Gradient(phase[w], phase[w+xsize]);
            rbtrees->RB_insert_node(qual_map[k],k);
            unwrap[k] = 1;
            path[k] = iter++;
        }
        
        
        // lower neighbor
        a = x;
        b = y + 1;
        k = b*xsize + a;
        if (b < ysize && unwrap[k]==0 && mask[k])
        {
            w = y*xsize + x;
            soln[k] = soln[index] - Gradient(phase[w], phase[w+xsize]);
            rbtrees->RB_insert_node(qual_map[k],k);
            unwrap[k] = 1;
            path[k] = iter++;
        }
        
        if (rbtrees->RBT == rbtrees->Tnill)
             rbtrees->check_pruning();
    }
    
    
    /*  SAVE RESULT  */
    
    for (k=0; k<length; k++)
        soln[k] *= TWOPI;
    
    strcpy(prefix, data_path);
    strcat(prefix, "\\data\\");
    strcat(prefix, pname);
    OpenFile(&ofp, prefix, "w");
    WriteFloat(ofp, soln, length, fname);
    
    // save to file integration path
	strcpy(prefix, data_path);
    strcat(prefix, "\\data\\");
    strcat(prefix, pname);
    strcat(prefix, ".path");
    SaveIntToImage(path, "path integration", prefix, xsize, ysize);
    
    
    
    
    /* DEALLOCATE MEMORY */
    
    free(phase);
    free(qual_map);
    free(soln);
    free(unwrap);
    free(path);
    free(mask);
}



int main()
{
    double elapsed_time=0;
    int    iter, max_iter=1;
    char data_path[1024];
	struct timespec total_start, total_end;
    
    chdir("..");
    chdir("..");
    getcwd(data_path,1024);
    
    
    /*
     *   Peaks wrapped phase map, full precision.
     */

    
    int   num_levels    = 300;
    int   N             = 25;
    int   int_qual_flag = 0;
    float int_max_qual  = 1;
    int   flag_mask     = 0;
    int   flag_qual     = 0;

    
    for (iter=0; iter<max_iter; iter++)
	{
		// ** starting time
    
		clock_gettime(CLOCK_REALTIME, &total_start);
		
		Quality_guide_phase_unwrapping(data_path,"peaks512x512", 512, 512,
                                                   num_levels, N, flag_mask, flag_qual, int_qual_flag, int_max_qual);
												   
		clock_gettime(CLOCK_REALTIME, &total_end);
    
		elapsed_time += (total_end.tv_sec - total_start.tv_sec)*1000.0 + (total_end.tv_nsec - total_start.tv_nsec)/1000000.0;
	}

    
    elapsed_time/=(double)max_iter;
    
    printf("\nAverage elapsed time: %f ms\n", elapsed_time);
     

    
   
    return 0;
}
