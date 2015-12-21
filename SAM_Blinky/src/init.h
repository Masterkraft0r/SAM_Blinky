/*
 * init.h
 *
 * Created: 17.12.2015 20:00:47
 *  Author: Masterkraft0r
 */ 


#ifndef INIT_H_
#define INIT_H_

uint32_t tc0ch0_init(uint32_t ul_freq)
{
  uint32_t ul_div;
  uint32_t ul_tcclks;
  uint32_t ul_sysclk = sysclk_get_cpu_hz();
  
  pmc_enable_periph_clk(ID_TC0);
  if(tc_find_mck_divisor(ul_freq, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk) == 0)
  {
    return 0;
  }
  tc_init(TC0, 0, ul_tcclks | TC_CMR_CPCTRG);
  tc_write_rc(TC0, 0, (ul_sysclk/ul_div)/ul_freq);
  NVIC_EnableIRQ((IRQn_Type) ID_TC0);
  tc_enable_interrupt(TC0, 0, TC_IER_CPCS);
  tc_start(TC0, 0);
  
  return 1;
}

uint32_t pwmch0_init(void)
{
  uint32_t error;
  pwm_clock_t clock_settings = {
    .ul_clka = 1000 * 100,
    .ul_clkb = 0,
    .ul_mck = sysclk_get_main_hz()
  };
  pwm_channel_t pwm_channel_instance = {
    .ul_prescaler = PWM_CMR_CPRE_CLKA,
    .ul_period = 100,
    .ul_duty = 50,
    .channel = PWM_CHANNEL_0
  };
  
  pmc_enable_periph_clk(ID_PWM);
  pwm_channel_disable(PWM, PWM_CHANNEL_0);
  error = pwm_init(PWM, &clock_settings);
  if(error != 0);
  {
    return error;
  }
  pwm_channel_init(PWM, &pwm_channel_instance);
  pwm_channel_enable(PWM, PWM_CHANNEL_0);
  gpio_configure_pin(LED0_GPIO, PIO_PERIPH_A);
  
  return 0;
}

#endif /* INIT_H_ */