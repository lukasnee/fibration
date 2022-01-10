#include "shell.hpp"

#include "ticks.hpp"
extern "C"
{
#include "task.h"
}

#include "arm_math.h"
#include <cmath>

constexpr std::size_t canvasWidth = 50;
constexpr std::size_t canvasHeight = 26;

constexpr float scale = 0.4f;
constexpr std::size_t canvasSize = canvasWidth * canvasHeight;

std::array<float, canvasSize> z;
std::array<char, canvasSize> b;

Shell::Command donut("donut", nullptr, "tasty!", [] ShellCommandFunctionLambdaSignature {
    Shell::Command::Result result = Shell::Command::Result::okQuiet;

    int k;
    float A = 0, B = 0, i, j;
    printf("\x1b[2J");
    for (;;)
    {
        static bool delayUntilInitialized = false;
        static TickType_t delayUntilPreviousWakeTime = 0;
        if (!delayUntilInitialized)
        {
            delayUntilInitialized = true;
            delayUntilPreviousWakeTime = xTaskGetTickCount();
        }
        vTaskDelayUntil(&delayUntilPreviousWakeTime, 1000 / 15);

        memset(b.data(), ' ', sizeof(b));
        memset(z.data(), 0, sizeof(z));

        for (j = 0; 2 * PI > j; j += 0.07 / scale)
            for (i = 0; 2 * PI > i; i += 0.02 / scale)
            {
                float c = arm_sin_f32(i), d = arm_cos_f32(j), e = arm_sin_f32(A), f = arm_sin_f32(j),
                      g = arm_cos_f32(A), h = d + 2, D = 1 / (c * h * e + f * g + 5), l = arm_cos_f32(i),
                      m = arm_cos_f32(B), n = arm_sin_f32(B), t = c * h * g - f * e;
                int x = canvasWidth / 2 + 30 * D * (l * h * m - t * n), y = 12 + 15 * D * (l * h * n + t * m),
                    o = x + canvasWidth * y, N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
                if (canvasWidth > y && y > 0 && x > 0 && canvasWidth > x && D > z[o])
                {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        shell.printf("\x1b[H");
        for (k = 0; canvasSize + 1 > k; k++)
            shell.print(k % canvasWidth ? b[k] : 10);
        // shell.execute(Core::Commands::status);
        A += 0.04;
        B += 0.02;
    }

    return result;
});
