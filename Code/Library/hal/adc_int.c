#include "adc_int.h"
#include "gpio.h"
#include "tc.h"
#include "intc.h"
#include "led.h"
#include "compiler.h"
#include "adcifa.h"
#include "time_keeper.h"
#include "dac_dma.h"
#include "sysclk.h"

#define ADC_INT_SEOS0 1
#define ADC_INT_SEOS1 16
#define ADC_INT_SEOC0 2
#define ADC_INT_SEOC1 32

	// GPIO pin/adc-function map.
	static const gpio_map_t ADCIFA_GPIO_MAP = {
	{AVR32_ADCREF0_PIN,AVR32_ADCREF0_FUNCTION},
	{AVR32_ADCREFP_PIN,AVR32_ADCREFP_FUNCTION},
	{AVR32_ADCREFN_PIN,AVR32_ADCREFN_FUNCTION},
		
	{AVR32_ADCIN0_PIN, AVR32_ADCIN0_FUNCTION},
	{AVR32_ADCIN1_PIN, AVR32_ADCIN1_FUNCTION},
	{AVR32_ADCIN2_PIN, AVR32_ADCIN2_FUNCTION},
	{AVR32_ADCIN3_PIN, AVR32_ADCIN3_FUNCTION},
	{AVR32_ADCIN4_PIN, AVR32_ADCIN4_FUNCTION},
	{AVR32_ADCIN5_PIN, AVR32_ADCIN5_FUNCTION},
	{AVR32_ADCIN6_PIN, AVR32_ADCIN6_FUNCTION},
	{AVR32_ADCIN7_PIN, AVR32_ADCIN7_FUNCTION},
	{AVR32_ADCIN8_PIN, AVR32_ADCIN8_FUNCTION},
	{AVR32_ADCIN9_PIN, AVR32_ADCIN9_FUNCTION},
	{AVR32_ADCIN10_PIN, AVR32_ADCIN10_FUNCTION},
	{AVR32_ADCIN11_PIN, AVR32_ADCIN11_FUNCTION},
	{AVR32_ADCIN12_PIN, AVR32_ADCIN12_FUNCTION},
	{AVR32_ADCIN13_PIN, AVR32_ADCIN13_FUNCTION},
	{AVR32_ADCIN14_PIN, AVR32_ADCIN14_FUNCTION},
	{AVR32_ADCIN15_PIN, AVR32_ADCIN15_FUNCTION}

		
	};

	volatile avr32_adcifa_t *adcifa = &AVR32_ADCIFA; // ADCIFA IP registers address

	static volatile int sequencer_item_count, channel_count;
	
	static volatile int32_t sample_counter, oversampling_counter;
	
	static volatile int number_of_samples, oversampling, oversampling_divider;
	bool continuous_mode;
	
	static volatile uint32_t last_adc_int_time, adc_int_period;
	
	//32bits version
	//int32_t adci_buffer[ADCI_INPUT_CHANNELS][ADCI_BUFFER_SIZE];
	//16bits version
	static volatile int32_t internal_buffer[MAX_CHANNELS];
	int16_t *adci_buffer[MAX_CHANNELS];
	uint8_t even_odd;
	

			// ADC Configuration
	volatile adcifa_opt_t adc_config_options = {
				.frequency                = ADC_FREQUENCY,  // ADC frequency (Hz)
				.reference_source         = ADCIFA_REF1V, // Reference Source
				.sample_and_hold_disable  = false,    // Disable Sample and Hold Time
				.single_sequencer_mode    = false,     // Single Sequencer Mode
				.free_running_mode_enable = false,    // Free Running Mode
				.sleep_mode_enable        = false     // Sleep Mode
			};
			
			// Sequencer Configuration
			adcifa_sequencer_opt_t adcifa_sequence_opt = {
				.convnb               = 0, // Number of sequence
				.resolution           = ADCIFA_SRES_12B,         // Resolution selection
				.trigger_selection    = ADCIFA_TRGSEL_ITIMER,      // Trigger selection
				.start_of_conversion  = ADCIFA_SOCB_ALLSEQ,      // Conversion Management
				.sh_mode              = ADCIFA_SH_MODE_STANDARD, // Oversampling Management
				.half_word_adjustment = ADCIFA_HWLA_NOADJ,       // Half word Adjustment
				.software_acknowledge = ADCIFA_SA_NO_EOS_SOFTACK // Software Acknowledge
			};
			
			

// Conversions in the Sequencer Configuration
adcifa_sequencer_conversion_opt_t adcifa_sequencer0_conversion_opt[SLOTS_PER_SEQUENCER];
			
			
			
__attribute__((__interrupt__))
static void processData() {
	int ch;
	volatile int16_t value;

	if (sample_counter>=number_of_samples)  {
		if (continuous_mode) {
			sample_counter=0;
			oversampling_counter=0;
		} else {
			adcifa_disable_interrupt(adcifa, ADC_INT_SEOS0);
			//adcifa_disable_interrupt(adcifa, ADC_INT_SEOS1);
			adcifa_stop_itimer(adcifa);
		}
	} else {
	

	if (((adcifa->sr&ADC_INT_SEOS0) ==0) 
	//|| ((adcifa->sr&ADC_INT_SEOS1) ==0) 
	) {} else {
	adc_int_period=(get_time_ticks()-last_adc_int_time);
	last_adc_int_time=get_time_ticks();
		
	if (sample_counter>=0) {
		if (oversampling_counter<=0) {
			for (ch=0; ch<sequencer_item_count; ch++) {
				value=adcifa->resx[ch];
				internal_buffer[ch]=  value ;
			}
		}else {			
			for (ch=0; ch<sequencer_item_count; ch++) {		
				value=adcifa->resx[ch];
				internal_buffer[ch]+= value ;
				//adci_buffer[ch][even_odd][sample_counter]+=value;
			}			
		}
	}	else {
		sample_counter++; return;
	}
	//if (function_generator!=NULL) {
	//	DAC_set_value((*function_generator)(sampleCounter));
	//}
	oversampling_counter++;
	
	if (oversampling_counter>= oversampling) {
		
		oversampling_counter=0;
		for (ch=0; ch<channel_count; ch++) {
			int16_t *buffer=adci_buffer[ch];
			buffer[sample_counter]=internal_buffer[ch] / oversampling_divider;
		}
		sample_counter++;
	
	}		
	//DAC_set_value(even_odd*400);
	// acknowledge processing finished
	adcifa->scr=ADC_INT_SEOS0 | ADC_INT_SEOS1;
	}
	}
}


// Initializes ADC (configures Pins, starts Clock, sets defaults)
void Init_ADCI(uint32_t adc_frequency, uint8_t reference_source){

		// Assign and enable GPIO pins to the ADC function.
		gpio_enable_module(ADCIFA_GPIO_MAP, sizeof(ADCIFA_GPIO_MAP) / sizeof(ADCIFA_GPIO_MAP[0]));

		adc_config_options.frequency=adc_frequency;
		adc_config_options.reference_source=reference_source;

		// Get ADCIFA Factory Configuration
		adcifa_get_calibration_data(adcifa, &adc_config_options);
		if ((uint16_t)adc_config_options.offset_calibration_value == 0xFFFF){
			// Set default calibration if Engineering samples and part is not programmed
			adc_config_options.offset_calibration_value = 0x3B;
			adc_config_options.gain_calibration_value = 0x4210;
			adc_config_options.sh0_calibration_value = 0x210;
			adc_config_options.sh1_calibration_value = 0x210;
		}
		adc_config_options.offset_calibration_value = 0x3B; // offset correction

		// Configure ADCIFA core
		adcifa_configure(adcifa, &adc_config_options, sysclk_get_peripheral_bus_hz(AVR32_ADCIFA_ADDRESS));

		clear_adc_sequencer();
		continuous_mode=false;
		// Configure ADCIFA sequencer 1
		//adcifa_configure_sequencer(adcifa, 1, &adcifa_sequence_opt, adcifa_sequencer1_conversion_opt);
		
		adcifa_disable_interrupt(adcifa, 0xffffffff);
		INTC_register_interrupt( (__int_handler) &processData, AVR32_ADCIFA_SEQUENCER0_IRQ, AVR32_INTC_INT1);
		//INTC_register_interrupt( (__int_handler) &processData, AVR32_ADCIFA_SEQUENCER1_IRQ, AVR32_INTC_INT1);
//	int period_us=1000000/samplingrate;
}
void clear_adc_sequencer() {
	sequencer_item_count=0;
	adcifa_sequence_opt.convnb=sequencer_item_count;
}

int8_t adc_sequencer_add(int16_t* buffer, uint8_t input_p, uint8_t input_n, uint8_t gain) {
	
	if (sequencer_item_count<SLOTS_PER_SEQUENCER-1) {
		adcifa_sequencer0_conversion_opt[sequencer_item_count].channel_p=input_p;
		adcifa_sequencer0_conversion_opt[sequencer_item_count].channel_n=input_n;
		adcifa_sequencer0_conversion_opt[sequencer_item_count].gain=gain;
		
		adci_buffer[sequencer_item_count] = buffer;
		sequencer_item_count++;
		adcifa_sequence_opt.convnb=sequencer_item_count;
		return sequencer_item_count;
	} else {
		return -1;
	}
}

// starts sampling, captures one buffer length and then stops
void ADCI_Start_Sampling(int length, int samplingrate, int set_oversampling, int set_oversampling_divider, bool continuous){

	// Configure ADCIFA sequencer 0
	adcifa_sequence_opt.convnb=sequencer_item_count;
	adcifa_configure_sequencer(adcifa, 0, &adcifa_sequence_opt, &adcifa_sequencer0_conversion_opt);
	
	oversampling=set_oversampling;
	oversampling_divider=set_oversampling_divider;

	volatile int period_us=adc_config_options.frequency/(samplingrate*oversampling);	
	oversampling_counter=0;
	sample_counter=-10;
	number_of_samples=length;
	continuous_mode=continuous;
	channel_count=sequencer_item_count;

	adcifa_enable_interrupt(adcifa, ADC_INT_SEOS0);
	//adcifa_enable_interrupt(adcifa, ADC_INT_SEOS1);
	adcifa_start_itimer(adcifa, period_us);
}

// stops sampling immediately
void ADCI_Stop_Sampling(){
	adcifa_stop_itimer(adcifa);
	
}

// Returns true if one-shot sampling has finished
Bool ADCI_Sampling_Complete(){
	return (sample_counter>=number_of_samples);
}




//void set_DAC_generator_function(generatorfunction new_function_generator ) {
//	function_generator=new_function_generator;
	
//}

int16_t ADCI_get_sample(int channel, int sample) {
	int16_t *buffer=adci_buffer[channel];
	return buffer[sample];
}

int16_t* ADCI_get_buffer() {
	return adci_buffer;
}
	
int ADCI_get_sampling_status() {
	return sample_counter;
}

uint32_t get_adc_int_period() {return adc_int_period;}