# tuner_on_stm32  
# Introduction:   

This project is made for developing and exploring basic DSP algorithms on ARM Cortex M7.   

Project mainly concentrates on fundamental frequency estimation algorithm based on FFT and IFFT ARM functions. In this project TouchGFX Designer was used to create graphical user interface - as photograph below shows, it allows to watch realtime spectrum visualisation, and estimated fundamental frequency of guitar string. It also got listed 6 strings typical frequency values used for Stadard E tunning (photo below shows all needed values).  

![alt text](https://github.com/maciejsudo/tuner_on_stm32/blob/master/tuner_photo.png?raw=true)  

Photo above shows proper estimation of E4 string frequency(~329.6Hz)  
# Block diagram of the system  

Photo below shows block diagram which contains all used onboard peripheralls such as MEMS microphones, DFSDM module,PDM2PCM and TouchGFX module. 
![alt text](https://github.com/maciejsudo/tuner_on_stm32/blob/master/block_diagram.PNG?raw=true)  

Briefly speaking the algorithm is following:
1. Two MEMS microphones receive signals, each one their own - Left and Right channel.    
2. Signals are converted via Delta-Sigma module (with use of Sinc filters).
3. Signals are decoded using PDM2PCM conversion.  
4. Signals are added toegether to create one extra long vector twice as long as the vector from single channel.  
5. FFT proceed on long vector of samples (Spectrum realtime visualisation on TouchGFX).    
6. Skipping zero - offset in spectrum, and unnecessary samples nearby.  
7. Maximum FFT Spectrum amplitude estmation, and the number of sample that corresponds to that value.  
8. Gauss Interpolation, to increase frequency resolution.  
9. Updating estimated frequency value on TouchGFX.    
  
# To be continued...
