#include "SceneManager.h"

volatile uint32_t micros()
{
    return TIM5->CNT;
}

extern "C" void TIM5_IRQHandler(void)
{
    if (TIM5->SR & TIM_SR_UIF) {
        TIM5->ARR = 0xFFFFFFFF;
        TIM5->CNT = 0;
        TIM5->SR &= ~TIM_SR_UIF;
    }
}

void delay_us(uint32_t us)
{
    uint32_t start = micros();
    while ((micros() - start) < us)
        ;
}

extern "C" void EXTI9_5_IRQHandler()
{
    if (EXTI->PR & EXTI_PR_PR6) {
        EXTI->PR = EXTI_PR_PR6;
        printf("EXTI9_5 interrupt = %d\n", (int)(GPIOC->IDR & GPIO_IDR_ID6));
    }
}

SceneManager::SceneManager()
    : m_gasSensor(ADC3, 0, GPIOB, 12)
    , m_lm35Sensor(ADC3, 8)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    TIM5->PSC = 108_MHz / 1_MHz - 1;
    TIM5->ARR = 0xFFFF;
    TIM5->EGR = TIM_EGR_UG;
    TIM5->DIER |= TIM_DIER_UIE;
    TIM5->CNT = 0;
    TIM5->CR1 |= TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM5_IRQn);

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER0_1;
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0_0 | GPIO_PUPDR_PUPDR0_1);
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    GPIOF->MODER |= (3U << (10 * 2));
    GPIOF->PUPDR &= ~(3U << (10 * 2));

    m_gasSensor.init();
    m_gasSensor.calib();

    m_timer = lv_timer_create([](lv_timer_t* timer) {
        SceneManager* manager = static_cast<SceneManager*>(lv_timer_get_user_data(timer));

        manager->m_lm35Sensor.init();
        int temp = manager->m_lm35Sensor.readRaw();
        printf("Temperature: %d  ADC = %d\n", (int)manager->m_lm35Sensor.readCelsius(), temp);
        manager->m_temperature.push_back(manager->m_lm35Sensor.readCelsius());

        manager->m_gasSensor.init();

        int airQuality = manager->m_gasSensor.readRaw();
        printf("Air Quality: %d ADC %d\n", (int)manager->m_gasSensor.readPpm(), airQuality);
        manager->m_airQuality.push_back(manager->m_gasSensor.readPpm());

        manager->m_humidity.push_back(62 + std::rand() % 2);

        if (manager->m_function) {
            manager->m_function();
        }
    },
        1000,
        this);
}
