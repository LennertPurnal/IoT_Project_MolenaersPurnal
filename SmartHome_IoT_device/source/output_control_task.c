/*
 * output_control_task.c
 *
 *  Created on: 28 Dec 2021
 *      Author: arnom
 */
#include "cybsp.h"
#include "cyhal.h"
#include "cy_retarget_io.h"
#include "FreeRTOS.h"


/* Task header files */
#include "output_control_task.h"
#include "publisher_task.h"

/* Configuration file for MQTT client */
#include "mqtt_client_config.h"

/* Middleware libraries */
#include "cy_mqtt_api.h"
#include "cy_retarget_io.h"

/* PWM Frequency       */
#define PWM_FREQUENCY (60)
/* PWM Duty-cycle = 50% */
#define PWM_DUTY_CYCLE (50.0f)



/******************************************************************************
* Global Variables
*******************************************************************************/
/* FreeRTOS task handle for this task. */
TaskHandle_t output_control_task_handle;

QueueHandle_t output_control_task_q;


void handle_pwm_error(cy_rslt_t result){
	if(CY_RSLT_SUCCESS != result)
	{
		printf("API cyhal_pwm_set_duty_cycle failed with error code: %lu\r\n", (unsigned long) result);
		CY_ASSERT(false);
	}
}


/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for the CPU. It configures the PWM and puts the CPU
* in Sleep mode to save power.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/

void output_control_task(void  *pvParameters)
{
    /* PWM object */
    cyhal_pwm_t pwm_ventilator_control;
    /* API return code */
    cy_rslt_t result;

    output_control_data_t output_control_q_data;
    action_status_t action_status = INACTIVE; //start status is inactive

    //initialize the variables, as 1000 are unrealistic values, these indicate unitialized variables
    //control is encoded as: 0 is OFF, 1 is ON
    float temperature = 1000, control = 0, set_temperature = 1000, prevControl=0;

    /* Initialize the PWM */
    result = cyhal_pwm_init(&pwm_ventilator_control, P10_1, NULL);
    if(CY_RSLT_SUCCESS != result)
    {
        printf("API cyhal_pwm_init failed with error code: %lu\r\n", (unsigned long) result);
        CY_ASSERT(false);
    }
    /* Set the PWM output frequency and duty cycle */
    result = cyhal_pwm_set_duty_cycle(&pwm_ventilator_control, 0, PWM_FREQUENCY);
    if(CY_RSLT_SUCCESS != result)
	handle_pwm_error(result);
    /* Start the PWM */
    result = cyhal_pwm_start(&pwm_ventilator_control);
    if(CY_RSLT_SUCCESS != result)
    {
        printf("API cyhal_pwm_start failed with error code: %lu\r\n", (unsigned long) result);
        CY_ASSERT(false);
    }

    output_control_task_q = xQueueCreate((5u), sizeof(output_control_data_t));
    //vTaskDelete(NULL);
    while(true){
    	if (pdTRUE == xQueueReceive(output_control_task_q, &output_control_q_data, portMAX_DELAY))
		{
    		switch (output_control_q_data.unit){
				case CURRENT_TEMP:
				{
					temperature = output_control_q_data.data;
					printf("Received current temperature through queue = %f\r\n", temperature);
					break;
				}
				case CONTROL:
				{
					control = output_control_q_data.data;
					printf("Received control through queue = %f\r\n", control);
					break;
				}
				case SET_TEMP:
				{
					set_temperature = output_control_q_data.data;
					printf("Received set_temperature through queue = %f\r\n", set_temperature);
					break;
				}
    		}
    		//when change of settings or temp happens: re-evaluate and set control measures and send status change
    		if (temperature != 1000 && control != 1000 && set_temperature != 1000){
				//This is executed when all variables are set
				if(control == 1){
					if (temperature > (set_temperature + 1)){
						// if temperature is too high: turn on fan (cooling)
						result = cyhal_pwm_set_duty_cycle(&pwm_ventilator_control, PWM_DUTY_CYCLE, PWM_FREQUENCY);
						handle_pwm_error(result);
						action_status = COOLING;
					}
					else {
						result = cyhal_pwm_set_duty_cycle(&pwm_ventilator_control, 0, PWM_FREQUENCY);
						handle_pwm_error(result);
						action_status = INACTIVE;
					}
				}
				else if (prevControl != control){
					result = cyhal_pwm_set_duty_cycle(&pwm_ventilator_control, 0, PWM_FREQUENCY);
					handle_pwm_error(result);
					action_status = INACTIVE;
				}
				//if control is of, and was off earlier: do nothing
			}
			prevControl = control;
			xQueueSend(action_status_q, &action_status, 1000/portTICK_PERIOD_MS);
		}
    }
}

