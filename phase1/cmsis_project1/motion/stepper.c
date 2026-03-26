    #include "motion/stepper.h"
    #include "stm32f4xx.h"
    #include "drivers/gpio.h"
    #include "drivers/timer.h"

    #define STEPPER_STEP_PORT   GPIOA
    #define STEPPER_STEP_PIN    12U
    
    #define STEPPER_DIR_PORT    GPIOB
    #define STEPPER_DIR_PIN     12U

    #define STEPPER_EN_PORT    GPIOA
    #define STEPPER_EN_PIN     11U

    typedef struct{
        volatile uint32_t target_steps;
        volatile uint32_t steps_done;
        volatile uint32_t rate_hz;
        volatile uint8_t busy;
        volatile uint8_t enabled;
        volatile uint8_t dir;
        volatile uint8_t step_pin_state;
    } stepper_state_t;

    static stepper_state_t g_stepper = {0};

    static void stepper_timer_appy_rate(uint32_t rate_hz){
        uint32_t arr = 1000000U/(rate_hz*2U);
        if (arr == 0){
            return;
        }
        //Prescaler value alsready set in the amain.c program
        TIM_SetAutoReload(TIM6, arr - 1U);
        //Timer_SetFrequencyHz(rate_hz * 2U);
    }

void Stepper_Init(void){
    GPIO_EnableClock(STEPPER_STEP_PORT);
    GPIO_EnableClock(STEPPER_DIR_PORT);
    GPIO_EnableClock(STEPPER_EN_PORT);

    GPIO_SetMode(STEPPER_STEP_PORT, STEPPER_STEP_PIN, GPIO_MODE_OUTPUT);
    GPIO_SetMode(STEPPER_DIR_PORT, STEPPER_DIR_PIN, GPIO_MODE_OUTPUT);
    GPIO_SetMode(STEPPER_EN_PORT, STEPPER_EN_PIN, GPIO_MODE_OUTPUT);

    GPIO_WritePin(STEPPER_STEP_PORT,STEPPER_STEP_PIN,0U);
    GPIO_WritePin(STEPPER_DIR_PORT, STEPPER_DIR_PIN, 0U);
    GPIO_WritePin(STEPPER_EN_PORT, STEPPER_EN_PIN, 1U);     // HIGH is LOW

    g_stepper.target_steps = 0U;
    g_stepper.steps_done = 0U;
    g_stepper.rate_hz = 1000U;
    g_stepper.busy = 0U;
    g_stepper.enabled = 0U;
    g_stepper.dir = 0U;
    g_stepper.step_pin_state = 0U;

    stepper_timer_appy_rate(g_stepper.rate_hz);
}


void Stepper_Enable(bool en)
{
    g_stepper.enabled = en ? 1U : 0U;
    GPIO_WritePin(STEPPER_EN_PORT, STEPPER_EN_PIN, en ? 0U : 1U);
}

void Stepper_SetDir(bool dir)
{
    g_stepper.dir = dir ? 1U : 0U;
    GPIO_WritePin(STEPPER_DIR_PORT, STEPPER_DIR_PIN,g_stepper.dir);
}

void Stepper_SetRateHz(uint32_t rate_hz){
    if(rate_hz == 0U){
        return ;
    }

    g_stepper.rate_hz = rate_hz;
    stepper_timer_appy_rate(rate_hz);
}


BaseType_t Stepper_StartMove(uint32_t steps)
{
    if (steps == 0U || g_stepper.enabled == 0U || g_stepper.busy != 0){
        return pdFAIL;
    }

    g_stepper.target_steps = steps;
    g_stepper.steps_done = 0U;
    g_stepper.busy = 1U;
    g_stepper.step_pin_state = 0U;

    GPIO_WritePin(STEPPER_STEP_PORT, STEPPER_STEP_PIN, 0U);
    
    TIM_ClearUpdateFlag(TIM6);
    //Timer_ClearUpdateFlag();
    TIM_Start(TIM6);
    //Timer_Start();

    return pdPASS;
}



void Stepper_Stop(void)
{
    TIM_Stop(TIM6);
    //Timer_Stop();
    TIM_ClearUpdateFlag(TIM6);

    GPIO_WritePin(STEPPER_STEP_PORT, STEPPER_STEP_PIN, 0U);

    g_stepper.busy = 0U;
    g_stepper.step_pin_state = 0U;
}

bool Stepper_IsBusy(){
    return (g_stepper.busy == 1U);
}


uint32_t Stepper_GetStepsDone(void){
    return g_stepper.steps_done;
}


uint32_t Stepper_GetTargetSteps(void){
    return g_stepper.target_steps;
}

bool Stepper_TimerISR(void){
    if (!TIM_IsUpdateEvent(TIM6))
    {
        return false;
    }
    TIM_ClearUpdateFlag(TIM6);
    //Timer_ClearUpdateFlag();

    if(g_stepper.busy == 0U){
        GPIO_WritePin(STEPPER_STEP_PORT,STEPPER_STEP_PIN, 0U);
        return false;
    }

    if(g_stepper.step_pin_state == 0U){
        GPIO_WritePin(STEPPER_STEP_PORT, STEPPER_STEP_PIN, 1U);
        g_stepper.step_pin_state = 1U;
    }
    
    else{
        GPIO_WritePin(STEPPER_STEP_PORT,STEPPER_STEP_PIN,0U);
        g_stepper.step_pin_state = 0U;

        g_stepper.steps_done++;

        if(g_stepper.steps_done >= g_stepper.target_steps){
            Stepper_Stop();
            return true;
        }
    }

    return false;
}