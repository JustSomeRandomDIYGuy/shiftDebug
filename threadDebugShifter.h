#ifndef THREAD_DEBUG_SHIFTER_H
	#define THREAD_DEBUG_SHIFTER_H    
	#include <Arduino.h>
	#include <SPI.h>
	
	//#define TDS_USE_SHIFT_OUT
	#define USE_DIRECT_PORT_MANIPULATION true
	#define USE_BEGIN_AND_END_TRANSMISSION false
	#define DISABLE_SUM_THREAD_ACTIVITY_ON_CH0 false
	#define ENABLE_SUM_THREAD_ACTIVITY_ON_CH0 true
	

	// --------------------- class ---------------------
	class ThreadDebugShifter{
		private:		
			// Variables
				bool				_debugEnabled = true;
				bool				_sumThreadActOnCh0 = DISABLE_SUM_THREAD_ACTIVITY_ON_CH0;
				uint8_t				_firstAvailableChannel = 0;
				uint8_t 			_latchPin; 
				uint16_t			_pinStates = 0;	
				#if USE_DIRECT_PORT_MANIPULATION					
					volatile uint32_t	*_latchOutRegister;
					uint32_t			_latchPinMask;
				#endif	
				void				_write16bits();
				void				_checkAllPins();	

		public:
			// Constructors								
									ThreadDebugShifter(uint8_t latchPin);
			// Functions
				void 				begin();
				void				sendThreadStartSignal(uint8_t threadID);
				void				sendThreadStopSignal(uint8_t threadID);
				void				disableDebugging();	
				void				enableSumThreadActivityOnCh0();						
				void				disableSumThreadActivityOnCh0();				
				uint16_t			getStates(); 
	};

#endif