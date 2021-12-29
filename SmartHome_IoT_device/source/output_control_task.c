/*
 * output_control_task.c
 *
 *  Created on: 28 Dec 2021
 *      Author: arnom
 */
#include "cybsp.h"
#include "cyhal.h"
#include "cy_retarget_io.h"


/* Task header files */
#include "publisher_task.h"
#include "mqtt_task.h"
#include "subscriber_task.h"

/* Configuration file for MQTT client */
#include "mqtt_client_config.h"


/* PWM Frequency       */
#define PWM_FREQUENCY (60)
/* PWM Duty-cycle = 50% */
#define PWM_DUTY_CYCLE (50.0f)



/******************************************************************************
* Global Variables
*******************************************************************************/
/* FreeRTOS task handle for this task. */
TaskHandle_t output_control_task_handle;



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


    /* Enable global interrupts */
    __enable_irq();


    /* Initialize the PWM */
    result = cyhal_pwm_init(&pwm_ventilator_control, P10_1, NULL);
    if(CY_RSLT_SUCCESS != result)
    {
        printf("API cyhal_pwm_init failed with error code: %lu\r\n", (unsigned long) result);
        CY_ASSERT(false);
    }
    /* Set the PWM output frequency and duty cycle */
    result = cyhal_pwm_set_duty_cycle(&pwm_ventilator_control, PWM_DUTY_CYCLE, PWM_FREQUENCY);
    if(CY_RSLT_SUCCESS != result)
    {
        printf("API cyhal_pwm_set_duty_cycle failed with error code: %lu\r\n", (unsigned long) result);
        CY_ASSERT(false);
    }
    /* Start the PWM */
    result = cyhal_pwm_start(&pwm_ventilator_control);
    if(CY_RSLT_SUCCESS != result)
    {
        printf("API cyhal_pwm_start failed with error code: %lu\r\n", (unsigned long) result);
        CY_ASSERT(false);
    }

    while(true){

    }
}
