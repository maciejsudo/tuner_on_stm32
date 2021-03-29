#include "dfsdm_routine.h"
#include "main.h"
#include "stm32f7xx_hal_dfsdm.h"
#include "cmsis_os.h"
#include "arm_math.h"
#include <stdlib.h>
#include <complex.h>

#define AUDIO_REC 2048//1024 ZMIENIONO 11.02.
int32_t RecBuf_R[AUDIO_REC];
int32_t RecBuf_L[AUDIO_REC];
int32_t PlayBuf_R[AUDIO_REC];
int32_t PlayBuf_L[AUDIO_REC];

float  PlayBuf_sum[2*AUDIO_REC];
float32_t realABSvalue[2*AUDIO_REC];
float32_t fftBuf[2*AUDIO_REC];

float32_t Correl_wyn[4*AUDIO_REC];

float32_t afterfilter[2*AUDIO_REC];
int temp;
float fund_freq;

uint8_t DmaRecHalfBuffCplt=0;
uint8_t DmaRecBuffCplt=0;

extern uint8_t  end_conv;
extern  DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
extern DFSDM_Filter_HandleTypeDef hdfsdm1_filter1;

extern arm_rfft_fast_instance_f32 fft_handler;


float lowpassPlayBuf[2*AUDIO_REC];
float highpassPlayBuf[2*AUDIO_REC];
float outlowpass[2*AUDIO_REC];
float outhighpass[2*AUDIO_REC];



////Callbacks://////////////////////////////////////////////////////////////////////////
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
	DmaRecHalfBuffCplt=1;
}
void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
	DmaRecBuffCplt=1;
}



void Filter_initialization()
{
	  HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0,RecBuf_L,AUDIO_REC);
	  //Left channel audio samples(with extra PDM 3 last bits)

	  HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter1,RecBuf_R,AUDIO_REC);
	  //Right channel audio samples(with extra PDM 3 last bits)
}




void retBufsum()
{
	for(int i=0;i<AUDIO_REC;i++)
	{
		PlayBuf_sum[i*2]=(PlayBuf_L[i]);// 0,2,4,6..itd
		PlayBuf_sum[(i*2)+1]=(PlayBuf_R[i]);//probki 1,3,5,7..itd

	}


}




void fftPlayBufvalue()
{




	for (int i=0;i<AUDIO_REC;i++)//filtracja cyfrowa(podział próbek na pół!!)
	{

		lowpassPlayBuf[i]=PlayBuf_sum[i];//[0,1,2,3,4,0000]
		lowpassPlayBuf[i+AUDIO_REC]= 0;//reszye zapełnij zerami!

		highpassPlayBuf[i] = 0;//[0000,1,2,3,4]
		highpassPlayBuf[i + AUDIO_REC]=PlayBuf_sum[i];//reszte zapełnij zerami

	}

	arm_rfft_fast_init_f32(&fft_handler,2*AUDIO_REC);
	arm_rfft_fast_f32(&fft_handler,&lowpassPlayBuf,&outlowpass,0);//DFT [0,1]
	arm_abs_f32(&outlowpass,&outlowpass,2*AUDIO_REC);//amplitude optimalization

	arm_rfft_fast_f32(&fft_handler,&highpassPlayBuf,&outhighpass,0);//DFT [1,0]
	arm_abs_f32(&outhighpass,&outhighpass,2*AUDIO_REC);//amplitude optimalization

	for (int i = 0;i<2*AUDIO_REC;i++)
	{
		PlayBuf_sum[i] =pow(outlowpass[i],0.67) + pow(outhighpass[i],0.67);// power
	}
//na tym etapie dlaeży policzyć IDFT !!!
	arm_rfft_fast_init_f32(&fft_handler,AUDIO_REC*2);
	arm_rfft_fast_f32(&fft_handler,&PlayBuf_sum ,&fftBuf,1);//ifft!!
	arm_abs_f32(&fftBuf,&realABSvalue,AUDIO_REC*2);


// teraz DFT spowrotem:

	arm_rfft_fast_init_f32(&fft_handler,AUDIO_REC*2);
	arm_rfft_fast_f32(&fft_handler,&realABSvalue ,&fftBuf,0);//ifft!!
	arm_abs_f32(&fftBuf,&realABSvalue,AUDIO_REC*2);






}

int find_maximum(int32_t a[], int n)
{
  int c, index = 6;//=0;//BYLO 6

  for (c = 7; c < n; c++)//for (c = 1; c < n; c++) //C STARTOWE BYLO  7
  {
   //if(c>7)//warunek na ominięcie 1 prążka!
     //{
	  if (a[c] > a[index])
    	{
		  index = c;
    	}
     //}
  }
  return index;

}



void calculate_filter_response(float a[],float b[])
{
	afterfilter[0]=a[0]*PlayBuf_sum[0];
	afterfilter[1]=a[0]*PlayBuf_sum[1] + a[1]*PlayBuf_sum[0] - b[1]*afterfilter[0];

	for (int i=2;i<2*AUDIO_REC;i++)
	{
 	afterfilter[i]=	a[0]*PlayBuf_sum[i] + a[1]*PlayBuf_sum[i-1] + a[2]*PlayBuf_sum[i-2] - b[1]*afterfilter[i-1] - b[2]* afterfilter[i-2];
	}


}


void estimatefreq(float realABSvalue[])
{

int max_bin = find_maximum(realABSvalue,AUDIO_REC*2);
float resolut=3.81469727;

float inter_bin = max_bin + log(realABSvalue[max_bin+1]/realABSvalue[max_bin-1])*0.5 / log(realABSvalue[max_bin] * realABSvalue[max_bin]/(realABSvalue[max_bin+1]*realABSvalue[max_bin-1]));

fund_freq = inter_bin * resolut;


}


void DFSDM_Buffer_mod()
{
	if(DmaRecHalfBuffCplt==1)
	{
		for(int i=0;i<AUDIO_REC/2;i++)
		{
			PlayBuf_R[i]=RecBuf_R[i]>>8;
			PlayBuf_L[i]=RecBuf_L[i]>>8;
		}
		DmaRecHalfBuffCplt=0;
	}
	if(DmaRecBuffCplt==1)
	{
		for(int i=AUDIO_REC/2;i<AUDIO_REC;i++)
		{
			PlayBuf_R[i]=RecBuf_R[i]>>8;
			PlayBuf_L[i]=RecBuf_L[i]>>8;
		}
		DmaRecBuffCplt=0;

	}

	//retBufsum();
	for(int i=0;i<AUDIO_REC;i++)
	{
		PlayBuf_sum[i*2]=(PlayBuf_L[i]);//probki 0,2,4,6..itd
		PlayBuf_sum[(i*2)+1]=(PlayBuf_R[i]);//probki 1,3,5,7..itd

	}


	fftPlayBufvalue();

	estimatefreq(realABSvalue);


}
