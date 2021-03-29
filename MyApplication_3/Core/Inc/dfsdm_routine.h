#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

void Filter_initialization();
void DFSDM_Buffer_mod();

void retBufsum();



void fftPlayBufvalue();

int find_maximum(int32_t a[], int n);
void calculate_filter_response(float a[],float b[]);
void estimatefreq(float realABSvalue[]);

#ifdef __cplusplus
}
#endif
