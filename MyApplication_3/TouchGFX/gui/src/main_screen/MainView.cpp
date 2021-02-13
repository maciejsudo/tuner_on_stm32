#include <gui/main_screen/MainView.hpp>
#include <math.h>
#include <stdlib.h>
#include "dfsdm_routine.h"
//#include <string.h>
//#include <gui_generated/main_screen/MainViewBase.hpp>
//#include <texts/TextKeysAndLanguages.hpp>

//extern uint32_t fftoutputBuf[1024];
//float yscale=0.00018;
float yscale=0.000005;
extern float_t realABSvalue[4096];//uint32_t realABSvalue[2048];//realABSvalue[512]; //float32_t

extern uint8_t end_conv;
extern uint8_t begin_conv;
extern int temp;

extern float freq_sum;

extern float fund_freq;
static float freq_average;
MainView::MainView()
{
    tickCounter = 0;
}

void MainView::setupScreen()
{
    MainViewBase::setupScreen();


}

void MainView::tearDownScreen()
{
    MainViewBase::tearDownScreen();
}



void MainView::handleTickEvent()
{

//ogarnąć wyswietlanie widma FFT!!!
	tickCounter++;
	//begin_conv=0;
	/*
	if(abs(fund_freq - 82.00) == 5 || abs(fund_freq - 110.00) == 5 || abs(fund_freq - 146.00) == 5 || abs(fund_freq - 196.00) == 5 || abs(fund_freq - 246.00) == 5 || abs(fund_freq - 329.00) == 5 )
	{
	freq_average +=fund_freq;
	}
	if( tickCounter%3 == 0)
	{
	updatewildcard(freq_average/3);
	freq_average=0;
	}
	*/
	if(abs(fund_freq - 82.00) < 10 || abs(fund_freq - 110.00) < 10 || abs(fund_freq - 146.00) < 3 || abs(fund_freq - 196.00) < 10 || abs(fund_freq - 246.00) < 3 || abs(fund_freq - 329.00) < 3 )
	{
		updatewildcard(fund_freq);

	}
	//1//freq_average+=fund_freq;

	//if (tickCounter%3==0)
	//{
	//fund_freq = ;
	/*
	if (tickCounter%10==0)
	{
	updatewildcard(freq_average/10);
	freq_average=0;
	}

	*/



	//if(end_conv==1)// //hotfix
	//{
// -----UWAGA-- CHWILOWA -- MODYFIKACJA

	for (int i=0;i<128;i++)//bylo 512 a potem 128 ale jest tylko 32 pierwsze próbki!
    {

    	//graph.addDataPoint((float)((float)realABSvalue[i] * yscale+20));
		graph.addDataPoint(int(realABSvalue[i] * yscale+20));

    }

	//updatewildcard(fund_freq);

	//}



}

void MainView::updatewildcard(float newval)
{
	//Unicode::snprintfFloat(buffer, 20, "%6f", 3.14159f);
    // buffer=" 3.142"
	//
	//Unicode::snprintf(textArea3Buffer,TEXTAREA3_SIZE,"%f",newval);
	Unicode::snprintfFloat(textArea3Buffer,TEXTAREA3_SIZE , "%3.3f", newval);
	textArea3.setWildcard(textArea3Buffer);
	textArea3.resizeToCurrentText();
	textArea3.invalidate();


}

