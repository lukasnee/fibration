#include "system.hpp"
#include "ioStream.hpp"
#include "logger.hpp"
#include "resources.hpp"
#include "shell.hpp"
#include "version.hpp"

#include "ticks.hpp"
#include "timer.hpp"

extern "C"
{
#include "stm32f3xx_hal.h"
#include "stm32f3xx_it.h"
}
#include <cstdint>
#include <limits>

extern "C" void vApplicationMallocFailedHook(void)
{
    FIBSYS_PANIC();
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM7)
    {
        HAL_IncTick();
    }
    if (htim->Instance == TIM6)
    {
        static bool firstIgnored = false;

        if (firstIgnored == false)
        {
            firstIgnored = true;
        }
        else
        {
            Periph::getTim6().overflowCallback();
        }
    }
}

static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {};

    // NUCLEO-F303RE HSE crystal not present
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        FIBSYS_PANIC();
    }
    /* Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        FIBSYS_PANIC();
    }

    PeriphClkInit.PeriphClockSelection =
        RCC_PERIPHCLK_I2S | RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_ADC12;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1; // TODO not sure what the clk NUMBA
    PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
    PeriphClkInit.I2sClockSelection = RCC_I2SCLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        FIBSYS_PANIC();
    }
}

void initPlatform()
{
    HAL_Init();
    SystemClock_Config();
}

uint32_t FibSys::getSysTick()
{
    return HAL_GetTick();
}

std::uint32_t FibSys::getUptimeInMs()
{
    return cpp_freertos::Ticks::TicksToMs(FibSys::getSysTick());
}

void FibSys::getUptime(std::uint32_t &days, std::uint32_t &hours, std::uint32_t &minutes, std::uint32_t &seconds,
                       std::uint32_t &milliseconds)
{
    std::uint32_t uptimeInMs = getUptimeInMs();
    std::uint32_t secondsTotal = uptimeInMs / 1000;
    std::uint32_t minutesTotal = secondsTotal / 60;
    std::uint32_t hoursTotal = minutesTotal / 60;
    std::uint32_t daysTotal = hoursTotal / 24;

    milliseconds = uptimeInMs % 1000;
    seconds = secondsTotal % 60;
    minutes = minutesTotal % 60;
    hours = hoursTotal % 24;
    days = daysTotal;
}

static void hexDumpWords(std::uint32_t address, std::size_t size, std::size_t width)
{
    for (std::size_t i = 0; i < size; i += width)
    {
        auto pAddress = reinterpret_cast<std::uint32_t *>(address) + i;
        printf("%08lX: ", reinterpret_cast<std::uint32_t>(pAddress));
        for (std::size_t j = 0; j < width; j++)
            if (i + j < size)
                printf("%08lX ", pAddress[j]);
            else
                printf("   ");
        printf("\n");
    }
}

extern "C" void fibsys_hardfault(ExceptionStackFrame *pExceptionStackFrame, char stackPointerInitial)
{
    printf(ANSI_COLOR_RESET "S Y S T E M   H A R D F A U L T (uptime: %lu ms)\n", FibSys::getUptimeInMs());
    printf("%cSP frame - r0: %08lX, "
           "r1: %08lX, "
           "r2: %08lX, "
           "r3: %08lX,\n"
           "r12: %08lX, "
           "lr: %08lX, "
           "ret_addr %08lX, "
           "xpsr: %08lX\n",
           stackPointerInitial, pExceptionStackFrame->r0, pExceptionStackFrame->r1, pExceptionStackFrame->r2,
           pExceptionStackFrame->r3, pExceptionStackFrame->r12, pExceptionStackFrame->lr,
           pExceptionStackFrame->return_address, pExceptionStackFrame->xpsr);
    const struct
    {
        std::uint32_t mask;
        const char *strName;
    } CM4_CFSR_MAP[] = {{SCB_CFSR_USGFAULTSR_Msk, "USGFAULTSR"},
                        {SCB_CFSR_BUSFAULTSR_Msk, "BUSFAULTSR"},
                        {SCB_CFSR_MEMFAULTSR_Msk, "MEMFAULTSR"},
                        {SCB_CFSR_MMARVALID_Msk, "MMARVALID"},
                        {SCB_CFSR_MLSPERR_Msk, "MLSPERR"},
                        {SCB_CFSR_MSTKERR_Msk, "MSTKERR"},
                        {SCB_CFSR_MUNSTKERR_Msk, "MUNSTKERR"},
                        {SCB_CFSR_DACCVIOL_Msk, "DACCVIOL"},
                        {SCB_CFSR_IACCVIOL_Msk, "IACCVIOL"},
                        {SCB_CFSR_BFARVALID_Msk, "BFARVALID"},
                        {SCB_CFSR_LSPERR_Msk, "LSPERR"},
                        {SCB_CFSR_STKERR_Msk, "STKERR"},
                        {SCB_CFSR_UNSTKERR_Msk, "UNSTKERR"},
                        {SCB_CFSR_IMPRECISERR_Msk, "IMPRECISERR"},
                        {SCB_CFSR_PRECISERR_Msk, "PRECISERR"},
                        {SCB_CFSR_IBUSERR_Msk, "IBUSERR"},
                        {SCB_CFSR_DIVBYZERO_Msk, "DIVBYZERO"},
                        {SCB_CFSR_UNALIGNED_Msk, "UNALIGNED"},
                        {SCB_CFSR_NOCP_Msk, "NOCP"},
                        {SCB_CFSR_INVPC_Msk, "INVPC"},
                        {SCB_CFSR_INVSTATE_Msk, "INVSTATE"},
                        {SCB_CFSR_UNDEFINSTR_Msk, "UNDEFINSTR"}};

    printf("CFSR: %08lX (", SCB->CFSR);
    uint8_t first = 1;
    for (size_t i = 0; i < sizeof(CM4_CFSR_MAP) / sizeof(CM4_CFSR_MAP[0]); i++)
    {
        if (SCB->CFSR & CM4_CFSR_MAP[i].mask)
        {
            if (!first)
            {
                printf("|");
            }
            printf(CM4_CFSR_MAP[i].strName);
            first = 0;
        }
    }
    printf("), HFSR: %08lX,\nDFSR: %08lX, MMFAR: %08lX, BFAR: %08lX, AFSR: %08lX\n", SCB->HFSR, SCB->DFSR, SCB->MMFAR,
           SCB->BFAR, SCB->AFSR);
    printf("process stack dump:\n");
    hexDumpWords(__get_PSP(), 32, 4);
    FibSys::hardwareReboot();
}

extern "C" void fibsys_panic(const char *strFile, std::uint32_t line)
{
    printf(ANSI_COLOR_RESET "S Y S T E M   P A N I C (uptime: %lu ms)\n", FibSys::getUptimeInMs());
    printf("%s:%lu\n", strFile, line);
    printf("process stack dump:\n");
    hexDumpWords(__get_PSP(), 32, 4);
    FibSys::hardwareReboot();
}

void FibSys::hardwareReboot()
{
    __NVIC_SystemReset();
    /* shall never return ! */
}

void FibSys::boot()
{
    initPlatform();
    // init system task
    static FibSys fibSys(0x200, FibSys::Priority::highest);
    // start task scheduler
    vTaskStartScheduler();
}

FibSys::FibSys(std::uint16_t stackDepth, BaseType_t priority) : Thread("FibSys", stackDepth, priority)
{
    if (Start() == false)
    {
        FIBSYS_PANIC();
    }
};

// void FibSys::collectStats()
// {
//     std::size_t minFreeHeapSize = std::numeric_limits<std::size_t>::max();

//     std::size_t freeHeapSize = xPortGetFreeHeapSize();
//     if(freeHeapSize < minFreeHeapSize)
//     {
//         minFreeHeapSize = freeHeapSize;
//     }
// }

Shell::Command versionCmd("version,ver", nullptr, "show firmware version", [] ShellCommandFunctionLambdaSignature {
    shell.printf("\nStarting up Fibration %s v%u.%u.%u (%s, %s %s)", Fib::Version::moduleName, Fib::Version::major,
                 Fib::Version::minor, Fib::Version::patch, Fib::Version::gitHash, Fib::Version::compileDate,
                 Fib::Version::compileTime);
    return Shell::Command::Result::okQuiet;
});

// FibSys thread
void FibSys::Run()
{
    static IOStream ioStreamUart2(Periph::getUart2());
    static AsciiStream textStreamUart2(ioStreamUart2);
    if (false == Logger::setAsciiStream(textStreamUart2))
    {
        FIBSYS_PANIC();
    }

    constexpr const char *strFibShellLabel = ANSI_COLOR_BLUE "FIB> " ANSI_COLOR_YELLOW;
    static Shell shell(strFibShellLabel, textStreamUart2, 0x200, FibSys::Priority::sysMedium);
    shell.execute(versionCmd);

    Periph::getAdc2().init();
    Periph::getAdc2().start();

    while (true)
    {
        // TODO:
        // this->collectStats();

        Delay(cpp_freertos::Ticks::MsToTicks(1000));
    }
}
