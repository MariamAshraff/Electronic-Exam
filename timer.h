
#ifndef TIMER_H_
#define TIMER_H_

void timer_CTC_init_interrupt(void);
void timer_wave_nonPWM(void);
void timer_wave_fastPWM(void);
void timer_wave_phasecorrectPWM(void);

#endif
