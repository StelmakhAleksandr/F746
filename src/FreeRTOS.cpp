#include <FreeRTOS.h>
#include <task.h>

extern "C" uint32_t ticksElapsed()
{
    return xTaskGetTickCount() * portTICK_PERIOD_MS;
}

extern "C" void SysTick_Handler()
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        xPortSysTickHandler();
    }
}

extern "C" void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer, uint32_t* pulIdleTaskStackSize)
{
    static StaticTask_t Idle_TCB;
    static StackType_t Idle_Stack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &Idle_TCB;
    *ppxIdleTaskStackBuffer = &Idle_Stack[0];
    *pulIdleTaskStackSize = (uint32_t)configMINIMAL_STACK_SIZE;
}

extern "C" void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer, StackType_t** ppxTimerTaskStackBuffer, uint32_t* pulTimerTaskStackSize)
{
    static StaticTask_t Timer_TCB;
    static StackType_t Timer_Stack[configTIMER_TASK_STACK_DEPTH];

    *ppxTimerTaskTCBBuffer = &Timer_TCB;
    *ppxTimerTaskStackBuffer = &Timer_Stack[0];
    *pulTimerTaskStackSize = (uint32_t)configTIMER_TASK_STACK_DEPTH;
}
