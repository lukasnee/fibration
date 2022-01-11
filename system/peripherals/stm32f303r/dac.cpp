 F/*
    DAC class for modular synth project Fibration.
    Copyright (C) 2020 Lukas Neverauskis

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

	
#include "dac.hpp"
#include "common.hpp"
#include <string.h>

namespace Hardware {

#define DAC_SAMPLE_WIDTH 			12
#define DAC_SAMPLE_CONTAINER_WIDTH 	32
#define DAC_FRAME_SIZE 				4048

#define DAC_FRAME_BUFFER_SIZE (DAC_SAMPLE_CONTAINER_WIDTH/8 * DAC_FRAME_SIZE)
#define DAC_FULL_BUFFER_SIZE (2 * DAC_FRAME_BUFFER_SIZE)
// note. single DAC instance takes 32kB from stack !!!

//DAC_HandleTypeDef _hdac;

// typedef enum DAC_Channel_
// {
// 	DAC_CHANNEL_1 = 0,
// 	DAC_CHANNEL_2
// } DAC_Channel;

// typedef enum DAC_State_
// {
// 	DAC_STATE_STOPPED = 0,
// 	DAC_STATE_FRAME1_STREAMING = 1,
// 	DAC_STATE_FRAME2_STREAMING = 2
// }DAC_State;

// class DAC
// {
// public:
// 	DAC(DAC_Channel ch);
// 	~DAC();
// 	void start();
// 	void stop();
// 	void triangle(); // todo unsure if works
// 	void noise(); // todo unsure if works

// private:
// 	size_t getFrameBufferSize() { return DAC_FRAME_BUFFER_SIZE; };
// 	size_t getFullBufferSize() { return DAC_FULL_BUFFER_SIZE; };
// 	uint32_t * pPendingFrame()
// 	{
// 		return (
// 		(_state == DAC_STATE_STOPPED) ? NULL :
// 		(_state == DAC_STATE_FRAME1_STREAMING ?
// 				pBufferFrame2() : pBufferFrame1()));
// 	};
// 	uint32_t * pStreamingFrame()
// 	{
// 		return (
// 		(_state == DAC_STATE_STOPPED) ? NULL :
// 		(_state == DAC_STATE_FRAME1_STREAMING ?
// 				pBufferFrame1() : pBufferFrame2()));
// 	};

// 	uint32_t * const pBufferFrame1() { return &_buffer[1][0]; };
// 	uint32_t * const pBufferFrame2() { return &_buffer[2][0]; };
// 	uint32_t * const pBufferFrameFull() { return pBufferFrame1(); };

// 	DAC_Channel _ch;
// 	DAC_State _state;

// 	uint8_t _buffer[2][DAC_FRAME_SIZE];
// };

// DAC::DAC(DAC_Channel ch) :
// 		_ch(ch),
// 		_state(DAC_STATE_STOPPED)
// {
// 	//set buffer to midpoint value
// 	for(uint32_t *p = pBufferFrameFull();
// 		pBufferFrameFull()+getFrameBufferSize();
// 		p++) *p = (((1 << 12)-1)/2);

// 	DAC_ChannelConfTypeDef sConfig = {0};

// 	if (HAL_DAC_Init(&_hdac) != HAL_OK)
// 	{
// 		Error_Handler();
// 	}

// 	sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
// 	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

// 	if (HAL_DAC_ConfigChannel(&_hdac, &sConfig, _ch) != HAL_OK)
// 	{
// 		Error_Handler();
// 	}
// };

// DAC::~DAC()
// {
// 	HAL_DAC_DeInit(&_hdac);
// }

// void DAC::triangle()
// {
// 	HAL_DACEx_TriangleWaveGenerate(&_hdac, _ch, DAC_TRIANGLEAMPLITUDE_4095);
// 	HAL_DAC_Start(_hdac, _ch);
// }

// void DAC::noise()
// {
// 	HAL_DACEx_NoiseWaveGenerate(&_hdac, _ch, DAC_LFSRUNMASK_BITS11_0);
// }

// void DAC::start()
// {
// 	_state = DAC_STATE_FRAME1_STREAMING;
// 	HAL_DAC_Start_DMA(&_hdac, _ch, pBufferFrameFull(), getFullBufferSize(), DAC_ALIGN_12B_R);
// }

// void DAC::stop()
// {
// 	HAL_DAC_Stop_DMA(&_hdac, _ch);
// 	_state = DAC_STATE_STOPPED;
// }
// // TODO stopped setting up callbacks, test em.!;) FIX GIT, THIS IS NOT BEING VERSIONED!~!!
// void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
// {
// 	if(hdac == &_hdac)
// 	{
// 		_state = DAC_STATE_FRAME1_STREAMING;
// 	}
// }

// void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef* hdac)
// {
// 	if(hdac == &_hdac)
// 	{
// 		_state = DAC_STATE_FRAME2_STREAMING;
// 	}
// }
} // namespace Hardware

/*
       *** DAC data value to voltage correspondance ***
       ================================================
       [..]
       The analog output voltage on each DAC channel pin is determined
       by the following equation:
       [..]
       DAC_OUTx = VREF+ * DOR / 4095
       (+) with  DOR is the Data Output Register
       [..]
          VEF+ is the input voltage reference (refer to the device datasheet)
       [..]
        e.g. To set DAC_OUT1 to 0.7V, use
       (+)  Assuming that VREF+ = 3.3V, DAC_OUT1 = (3.3U * 868U) / 4095U = 0.7V
*/
