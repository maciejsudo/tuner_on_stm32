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

float  PlayBuf_sum[2*AUDIO_REC];//float32_t  PlayBuf_sum[2*AUDIO_REC]; //int32_t  PlayBuf_sum[2*AUDIO_REC];                  //float32_t
float32_t realABSvalue[2*AUDIO_REC];  //int32_t realABSvalue[2*AUDIO_REC];                  //float32_t
float32_t fftBuf[2*AUDIO_REC];  //int32_t fftBuf[2*AUDIO_REC];                        //float32_t

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

/*
//246.9 dla fs = 15625 oraz Q=30
float a[3] ={ 0.001646 , 0.    ,   -0.001646};//246.0   //b
float b[3] ={1.     ,    -1.98694643 , 0.99670801 };    //a
float c[3] = {0.00164666,  0.      ,   -0.00164666};//246.1
float d[3] = {1.         ,-1.98693716 , 0.99670667};
float e[3] = {0.00164733 , 0.      ,   -0.00164733};//246.2
float f[3] = {1.        , -1.9869279 ,  0.99670534};
float g[3] = {0.001648 , 0.    ,   -0.001648};//246.3
float h[3] = {1.       ,  -1.98691863 , 0.996704 };
float i[3] = {0.00164867 , 0.     ,    -0.00164867};//246.4
float j[3] = {1.       ,  -1.98690936 , 0.99670266};
float k[3] = {0.00164934 , 0.     ,    -0.00164934};//246.5
float l[3] = {1.        , -1.98690009 , 0.99670133};
float m[3] = {0.00165 , 0.    ,  -0.00165};//246.6
float n[3] = {1.     ,    -1.98689081 , 0.99669999};
float o[3] = {0.00165067 , 0.      ,   -0.00165067};//246.7
float p[3] = {1.       ,  -1.98688153 , 0.99669866};
float r[3] = {0.00165134 , 0.       ,  -0.00165134};//246.8
float s[3] = {1.       ,  -1.98687225 , 0.99669732};
float t[3] = {0.00165201 , 0.        , -0.00165201};//246.9
float u[3] = {1.      ,   -1.98686296 , 0.99669598};

//jako drugi argument do funkcji idzie zawsze to z jedynką na dole!
*/

////Callbacks://////////////////////////////////////////////////////////////////////////
void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
	DmaRecHalfBuffCplt=1;
}
void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
	DmaRecBuffCplt=1;
}
////////////////////////////////////////////////////////////////////////////////////////
//Table of filter coefficients values:
/*////////////////////246.00////////////////////////////////////////////////////////////
 * 246.0  =
 [ 0.001646  0.       -0.001646]
 [ 1.         -1.98694643  0.99670801]


246.1  =
 [ 0.00164666  0.         -0.00164666]
 [ 1.         -1.98693716  0.99670667]


246.2  =
 [ 0.00164733  0.         -0.00164733]
 [ 1.         -1.9869279   0.99670534]


246.3  =
 [ 0.001648  0.       -0.001648]
 [ 1.         -1.98691863  0.996704  ]


246.4  =
 [ 0.00164867  0.         -0.00164867]
 [ 1.         -1.98690936  0.99670266]


246.5  =
 [ 0.00164934  0.         -0.00164934]
 [ 1.         -1.98690009  0.99670133]


246.6  =
 [ 0.00165  0.      -0.00165]
 [ 1.         -1.98689081  0.99669999]


246.7  =
 [ 0.00165067  0.         -0.00165067]
 [ 1.         -1.98688153  0.99669866]


246.8  =
 [ 0.00165134  0.         -0.00165134]
 [ 1.         -1.98687225  0.99669732]


246.9  =
 [ 0.00165201  0.         -0.00165201]
 [ 1.         -1.98686296  0.99669598]
/////////////////////////////////////////////////////////////////////////////////
 */


void Filter_initialization()
{
	  HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter0,RecBuf_L,AUDIO_REC);
	  //Left channel audio samples(with extra PDM 3 last bits)

	  HAL_DFSDM_FilterRegularStart_DMA(&hdfsdm1_filter1,RecBuf_R,AUDIO_REC);
	  //Right channel audio samples(with extra PDM 3 last bits)
}

/*
int32_t retPlayBufsum(uint16_t number)
{

return PlayBuf_sum[number];
}
*/


void retBufsum()
{
	for(int i=0;i<AUDIO_REC;i++)
	{
		PlayBuf_sum[i*2]=(PlayBuf_L[i]);//abs(PlayBuf_L[i]);//probki 0,2,4,6..itd
		PlayBuf_sum[(i*2)+1]=(PlayBuf_R[i]);//abs(PlayBuf_R[i]);//probki 1,3,5,7..itd
		//PlayBuf_sum[i] = PlayBuf_R[i];
	}

}




void fftPlayBufvalue()
{
	//calculate_filter_response(b, a);
/*	for (int i=0;i<AUDIO_REC*2;i++)//hamming window
	{
		PlayBuf_sum[i] = sinf(i*M_PI/2048) * PlayBuf_sum[i];
  }*/
	//tolonen algorithm!!!



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

/*
	arm_correlate_f32(&realABSvalue,2*AUDIO_REC, &realABSvalue,2*AUDIO_REC,&Correl_wyn);
for(int i=0;i<AUDIO_REC;i++)
{
	realABSvalue[i] = Correl_wyn[i];//zabiera tylko połowę tego widma autokorelacji!
}
*/
// teraz DFT spowrotem:

	arm_rfft_fast_init_f32(&fft_handler,AUDIO_REC*2);
	arm_rfft_fast_f32(&fft_handler,&realABSvalue ,&fftBuf,0);//ifft!!
	arm_abs_f32(&fftBuf,&realABSvalue,AUDIO_REC*2);




///////////////////////////oryginalny kod!!!
/*
	 //oryginalny kod -  i to działa
	arm_rfft_fast_init_f32(&fft_handler,AUDIO_REC*2);
	arm_rfft_fast_f32(&fft_handler,&PlayBuf_sum ,&fftBuf,0);
	//addded peak filter calculation
	//arm_rfft_fast_f32(&fft_handler,&afterfilter ,&fftBuf,0);
	arm_abs_f32(&fftBuf,&realABSvalue,AUDIO_REC*2);
*/



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
	//tutaj w obrębie całej funkcji wartosc realABS value musi być stabilna, nie zmieniana!!
//ziel//int32_t N = AUDIO_REC*2;
int max_bin = find_maximum(realABSvalue,AUDIO_REC*2);//ziel// zamiast RealABSvalue dałem Correl_Wyn
float resolut=3.81469727;//7.6293945;//moj//

/*//ziel//
int32_t k[3] ={max_bin - 1, max_bin, max_bin + 1};
float dw, wkm1, wk, wkp1, we;
float complex r, R, lam;
dw = 2*M_PI/N;
wkm1 = (k[0])*dw;
wk = (k[1])*dw;
wkp1 = (k[2])*dw;
r = ( -cexp( -I*wk) + cexp( -I*wkm1) )/( -cexp(-I*wkp1) + cexp(-I*wk) );
R = ( realABSvalue[k[0]]-realABSvalue[k[1]] )/( realABSvalue[k[1]]-realABSvalue[k[2]] );
lam= cexp(I*wk)*(r-R)/( r*cexp(-I*2*M_PI/N)-R*cexp(I*2*M_PI/N) );

we = cimag(clogf(lam));
*/
float inter_bin = max_bin + log(realABSvalue[max_bin+1]/realABSvalue[max_bin-1])*0.5 / log(realABSvalue[max_bin] * realABSvalue[max_bin]/(realABSvalue[max_bin+1]*realABSvalue[max_bin-1]));
//moj//

fund_freq = inter_bin * resolut;//moj//
//ziel//fund_freq = we/(2*M_PI)*15625;//16000;//15625;//16000.0 ;//48.000;//- 0.013;




}


void DFSDM_Buffer_mod()
{
	//if (end_conv==0)
	//{
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
		PlayBuf_sum[i*2]=(PlayBuf_L[i]);//abs(PlayBuf_L[i]);//probki 0,2,4,6..itd
		PlayBuf_sum[(i*2)+1]=(PlayBuf_R[i]);//abs(PlayBuf_R[i]);//probki 1,3,5,7..itd
		//PlayBuf_sum[i] = PlayBuf_R[i];
	}

	//calculate_filter_response(b,a);

	fftPlayBufvalue();

	estimatefreq(realABSvalue);


}
