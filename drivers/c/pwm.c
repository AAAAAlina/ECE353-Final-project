#include "pwm.h"
#include "sysctrl.h"


//*****************************************************************************
//Configure PWM
//*****************************************************************************
uint8_t pwmConfig( 
  uint32_t gena,
  uint32_t genb
){
	PWM0_Type *pwm;
	pwm = (PWM0_Type*) PWM1_BASE;
	
		SYSCTL -> RCGCPWM |= 0x2;
		while (!(SYSCTL->PRPWM & SYSCTL_PRPWM_R1)){
		}
		//disable timer
		pwm->_3_CTL &= ~0x1;
		
		//divide 64 by default
		SYSCTL->RCC |= SYSCTL_RCC_USEPWMDIV;
		//set pwm generator to countdown
		pwm->_3_GENA = gena;
		pwm->_3_GENB = genb;
		
		//enable timer
		pwm->_3_CTL |= 0x1;
		
	return 1;
}
//*****************************************************************************
// Enables the PWM output.
//*****************************************************************************
void pwmEnable(uint16_t frequency)
{
		PWM0_Type *pwm;
		pwm = (PWM0_Type*) PWM1_BASE;
	
		//Write period into LOAD register
		pwm->_3_LOAD = 50000000 / (64 * frequency);
	
		//50% duty cycle
		pwm->_3_CMPA = 50000000 / (2*64*frequency); 
		pwm->_3_CMPB = 50000000 / (2*64*frequency); 
		
		//Enable PWM output
		pwm->ENABLE |= PWM_ENABLE_PWM3A | PWM_ENABLE_PWM3B;
}
//*****************************************************************************
// Disables the PWM output.
//*****************************************************************************
void pwmDisable(void)
{	
		PWM0_Type *pwm;
		pwm = (PWM0_Type*) PWM1_BASE;
		//Disable PWM output
		pwm->ENABLE &= ~(PWM_ENABLE_PWM3A | PWM_ENABLE_PWM3B);

}
