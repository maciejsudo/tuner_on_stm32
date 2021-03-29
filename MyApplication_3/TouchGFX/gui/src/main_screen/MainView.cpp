#include <gui/main_screen/MainView.hpp>
#include <math.h>
#include <stdlib.h>
#include "dfsdm_routine.h"

float yscale=0.0005;
extern float_t realABSvalue[4096];

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

//wyswietlanie widma FFT!!!
	tickCounter++;

	if(abs(fund_freq - 82.00) < 10 || abs(fund_freq - 110.00) < 10 || abs(fund_freq - 146.00) < 3 || abs(fund_freq - 196.00) < 10 || abs(fund_freq - 246.00) < 3 || abs(fund_freq - 329.00) < 3 )
	{
		updatewildcard(fund_freq);

	}



	for (int i=0;i<256;i++)
    {

		graph.addDataPoint(int(realABSvalue[i] * yscale+20));

    }





}

void MainView::updatewildcard(float newval)
{

	Unicode::snprintfFloat(textArea3Buffer,TEXTAREA3_SIZE , "%3.3f", newval);
	textArea3.setWildcard(textArea3Buffer);
	textArea3.resizeToCurrentText();
	textArea3.invalidate();


}

