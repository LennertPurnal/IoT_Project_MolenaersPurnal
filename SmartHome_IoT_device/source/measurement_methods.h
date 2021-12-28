/*
 * measurement_methods.h
 *
 *  Created on: 28 dec. 2021
 *      Author: LennertP
 */

#ifndef SOURCE_MEASUREMENT_METHODS_H_
#define SOURCE_MEASUREMENT_METHODS_H_

/* Macro for ADC Channel configuration*/
#define SINGLE_CHANNEL 1
#define MULTI_CHANNEL  2

/* Channel 0 input pin */
#define VPLUS_CHANNEL_0             (P10_0)


/* Channel 1 VPLUS input pin */
#define VPLUS_CHANNEL_1             (P10_1)

/* Channel 1 VREF input pin */
#define VREF_CHANNEL_1              (P10_2)

/* Number of scans every time ADC read is initiated */
#define NUM_SCAN                    (1)

float getPPM();
void adc_single_channel_init(void);

//////////////////////////////////////////////////////
#define mI2C_SCL                (CYBSP_I2C_SCL)
#define mI2C_SDA                (CYBSP_I2C_SDA)

#define READ_TEMP_CMD			(0xF6U)
#define REG_CONTROL_ADDR		(0xF4U)
#define INIT_TEMP_CMD			(0x2E)
#define BMP_180_ADDR			(0x77UL)

#define I2C_FREQ                (400000UL)
#define CMD_TO_CMD_DELAY        (1000UL)

void temperature_i2c_init(void);
float getTemperature();

#endif /* SOURCE_MEASUREMENT_METHODS_H_ */
