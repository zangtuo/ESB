
#include "nrf_gpio.h"
#include "time.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Function for timer initialization.
 */
static volatile NRF_TIMER_Type * timer_init(timer_t timer)
{
    volatile NRF_TIMER_Type * p_timer;

    // ��ʼ16 MHz����.
    NRF_CLOCK->EVENTS_HFCLKSTARTED  = 0;
    NRF_CLOCK->TASKS_HFCLKSTART     = 1;

    // �ȴ��ⲿ��������
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) 
    {
        // Do nothing.
    }

    switch (timer)
    {
        case TIMER0:
            p_timer = NRF_TIMER0;
            break;

        case TIMER1:
            p_timer = NRF_TIMER1;
            break;

        case TIMER2:
            p_timer = NRF_TIMER2;
            break;

        default:
            p_timer = 0;
            break;
    }
    return p_timer;
}


/** ��������������ĺ�����֮��ʹ����ΧӲ����ʱ��
 *
 * ����timer ��Ӧ�Ķ�ʱ��ģ��
 * ���� number_of_ms ��ʱ��������ĺ�������
 *  �˺�������������Ķ�ʱ�����ȴ��ӳ٣�Ȼ��رոö�ʱ����
 */

void nrf_timer_delay_ms(timer_t timer, uint_fast16_t volatile number_of_ms)
{
    volatile NRF_TIMER_Type * p_timer = timer_init(timer);

    if (p_timer == 0) 
    {
        while(true) 
        {
            // Do nothing.
        }
    }

    p_timer->MODE           = TIMER_MODE_MODE_Timer;        // ����Ϊ��ʱ��ģʽ
    p_timer->PRESCALER      = 9;                            // Prescaler 9 produces 31250 Hz timer frequency => 1 tick = 32 us.
    p_timer->BITMODE        = TIMER_BITMODE_BITMODE_16Bit;  // 16 bit ģʽ.
    p_timer->TASKS_CLEAR    = 1;                            // �嶨ʱ��.
    
    // With 32 us ticks, we need to multiply by 31.25 to get milliseconds.
    p_timer->CC[0]          = number_of_ms * 31;
    p_timer->CC[0]         += number_of_ms / 4; 
    p_timer->TASKS_START    = 1;                    // Start timer.

    while (p_timer->EVENTS_COMPARE[0] == 0)
    {
        // Do nothing.
    }

    p_timer->EVENTS_COMPARE[0]  = 0;
    p_timer->TASKS_STOP         = 1;                // Stop timer.
}
/** @} */

