/*
 * measurement_methods.c
 *
 *  Created on: 28 dec. 2021
 *      Author: LennertP
 *
 * This file contains methods for the adc reading (to get air quality)
 * as well as methods for temperature reading from the GY-68 BMP180 sensor
 */
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include <stdio.h>

#include <math.h>

#include "measurement_methods.h"

/* ADC Object */
cyhal_adc_t adc_obj;

/* ADC Channel 0 Object */
cyhal_adc_channel_t adc_chan_0_obj;

/* Default ADC configuration */
const cyhal_adc_config_t adc_config = {
        .continuous_scanning=false, // Continuous Scanning is disabled
        .average_count=1,           // Average count disabled
        .vref=CYHAL_ADC_REF_VDDA,   // VREF for Single ended channel set to VDDA
        .vneg=CYHAL_ADC_VNEG_VSSA,  // VNEG for Single ended channel set to VSSA
        .resolution = 12u,          // 12-bit resolution
        .ext_vref = NC,             // No connection
        .bypass_pin = NC };       // No connection


long int adc_single_channel_process(void);

float getPPM(){
	 return (float)adc_single_channel_process()/2;
}

void adc_single_channel_init(void)
{
    /* Variable to capture return value of functions */
    cy_rslt_t result;

    /* Initialize ADC. The ADC block which can connect to pin 10[0] is selected */
    result = cyhal_adc_init(&adc_obj, VPLUS_CHANNEL_0, NULL);
    if(result != CY_RSLT_SUCCESS)
    {
        printf("ADC initialization failed. Error: %ld\n", (long unsigned int)result);
        CY_ASSERT(0);
    }

    /* ADC channel configuration */
    const cyhal_adc_channel_config_t channel_config = {
            .enable_averaging = false,  // Disable averaging for channel
            .min_acquisition_ns = 1000, // Minimum acquisition time set to 1us
            .enabled = true };          // Sample this channel when ADC performs a scan

    /* Initialize a channel 0 and configure it to scan P10_0 in single ended mode. */
    result  = cyhal_adc_channel_init_diff(&adc_chan_0_obj, &adc_obj, VPLUS_CHANNEL_0,
                                          CYHAL_ADC_VNEG, &channel_config);
    if(result != CY_RSLT_SUCCESS)
    {
        printf("ADC single ended channel initialization failed. Error: %ld\n", (long unsigned int)result);
        CY_ASSERT(0);
    }

    printf("ADC is configured in single channel configuration\r\n\n");
    printf("Provide input voltage at pin P10_0. \r\n\n");
}

long int adc_single_channel_process(void)
{
    /* Variable to store ADC conversion result from channel 0 */
    int32_t adc_result_0 = 0;

    /* Read input voltage, convert it to millivolts and print input voltage */
    adc_result_0 = cyhal_adc_read_uv(&adc_chan_0_obj)/1000;
    printf("Channel 0 input: %4ldmV\r\n", (long int)adc_result_0);

    return (long int) adc_result_0;
}

////////////////////////////////////////////////////////////////////////////////
/*
 * Below this are the temperature reading functions
 */

//I2C objects
cyhal_i2c_t mI2C;
cyhal_i2c_cfg_t mI2C_cfg;

//coeficients
double mc, md, c5, c6;

void handle_error(void)
{
     /* Disable all interrupts. */
    __disable_irq();

    CY_ASSERT(0);
}

int readInt(uint8_t* addr, int16_t* value, cyhal_i2c_t* i2c_obj){
	uint8_t buffer[2];

	if (CY_RSLT_SUCCESS == cyhal_i2c_master_write( i2c_obj, BMP_180_ADDR,
													 addr, (1UL), 0, false))
	{
		/* Read response packet from the slave. */
		//if (CY_RSLT_SUCCESS == cyhal_i2c_master_read( &mI2C, I2C_SLAVE_ADDR,
		//                                     buffer, PACKET_SIZE , 0, true))
		if (CY_RSLT_SUCCESS == cyhal_i2c_master_read( i2c_obj, BMP_180_ADDR,
														 buffer, (2UL) , 0, true))
		{
			*value = (int16_t)((buffer[0] << 8) | buffer[1]);
			return 1;
		}
	}
	*value = 0;
	return 0;
}

int readUInt(uint8_t* addr, uint16_t* value, cyhal_i2c_t* i2c_obj){
	uint8_t buffer[2];

	if (CY_RSLT_SUCCESS == cyhal_i2c_master_write( i2c_obj, BMP_180_ADDR,
													 addr, (1UL), 0, false))
	{
		/* Read response packet from the slave. */
		//if (CY_RSLT_SUCCESS == cyhal_i2c_master_read( &mI2C, I2C_SLAVE_ADDR,
		//                                     buffer, PACKET_SIZE , 0, true))
		if (CY_RSLT_SUCCESS == cyhal_i2c_master_read( i2c_obj, BMP_180_ADDR,
														 buffer, (2UL) , 0, true))
		{
			*value = (uint16_t)((buffer[0] << 8) | buffer[1]);
			return 1;
		}
	}
	*value = 0;
	return 0;
}

void temperature_i2c_init(void){
	cy_rslt_t result;

	printf(">> Configuring I2C Master..... ");
	mI2C_cfg.is_slave = false;
	mI2C_cfg.address = 0;
	mI2C_cfg.frequencyhal_hz = I2C_FREQ;
	result = cyhal_i2c_init( &mI2C, mI2C_SDA, mI2C_SCL, NULL);
	if (result != CY_RSLT_SUCCESS)
	{
		handle_error();
	}
	result = cyhal_i2c_configure( &mI2C, &mI2C_cfg);
	if (result != CY_RSLT_SUCCESS)
	{
		handle_error();
	}
	printf("Done\r\n\n");

	//retrieve the calibration coeffitients
	int16_t MC , MD;
	uint16_t ac5, ac6;

	//register addresses to be read:
	uint8_t register_ac5 = (0xB2U), register_ac6 = (0xB4U), register_mc = (0xBCU), register_md = (0xBEU);

	if (readUInt(&register_ac5, &ac5, &mI2C) &&
		readUInt(&register_ac6, &ac6, &mI2C) &&
		readInt(&register_mc, &MC, &mI2C) &&
		readInt(&register_md, &MD, &mI2C))
	{
		mc = (pow(2,11) / pow(160,2)) * MC;
		md = MD / 160.0;
		c5 = (pow(2,-15) / 160) * ac5;
		c6 = ac6;
		printf("ac5 = %u, ac6 = %u, mc = %i, md =%i \r\n", ac5, ac6, MC, MD);
	} else {
		printf("failed to read calibration coefficients");
		handle_error();
	}
}

double temperature_i2c_read(void){
	uint8_t register_addr_cmd[2] = {REG_CONTROL_ADDR, INIT_TEMP_CMD};
	uint8_t read_temp_cmd[1] = {READ_TEMP_CMD};
	uint8_t buffer[2];

	if (CY_RSLT_SUCCESS == cyhal_i2c_master_write( &mI2C, BMP_180_ADDR,
													 register_addr_cmd, (2UL), 0, true))
	{
		//cyhal_system_delay_ms(CMD_TO_CMD_DELAY);
		if (CY_RSLT_SUCCESS == cyhal_i2c_master_write( &mI2C, BMP_180_ADDR,
														 read_temp_cmd, (1UL), 0, false))
		{
			if (CY_RSLT_SUCCESS == cyhal_i2c_master_read( &mI2C, BMP_180_ADDR,
															 buffer, (2UL) , 0, true))
			{
				double tu = buffer[0] * 256.0 + buffer[1];
				double a = c5 * (tu - c6);

				double temperature = a + (mc / (a + md));

				//printf("TU = %f \r\n", tu);
				printf("temperature = %f \r\n", temperature);

				return temperature;
			}
		}
	}
}

float getTemperature(){
	return (float)temperature_i2c_read();
}

