// main code here!!

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "gpio.h"
#include "usart.h"

    void tbsPcUartAfterSwap(void);

#define PC_UART_BUFFER_SIZE 1024

    TripleBufferSystem tbsPcUart;

    // MARK:setup
    void user_setup(void)
    {
        setbuf(stdout, NULL);
        printf("Hello World!!\n");
        TripleBufferSystem_Init(&tbsPcUart, PC_UART_BUFFER_SIZE);
        TripleBufferSystem_setFunc(&tbsPcUart, __disable_irq, tbsPcUartAfterSwap);
        HAL_UART_Receive_IT(&huart2, TripleBufferSystem_NextWriteBuffer(tbsPcUart), 1); // 1byte
    }

    // MARK:loop
    void user_loop(void)
    {
        const uint32_t now = HAL_GetTick();
        static uint32_t pre = now;
        static uint32_t pre2 = now;

        if (now - pre >= 100)
        {
            if (HAL_GPIO_ReadPin(DebugButton_GPIO_Port, DebugButton_Pin) == GPIO_PIN_SET)
            {
                HAL_GPIO_TogglePin(DebugLED_GPIO_Port, DebugLED_Pin);
            }
            // printf("now,%lu\n", now);
            pre = now;
        }

        if (now - pre2 >= 1000)
        {
            unsigned char receiveData[PC_UART_BUFFER_SIZE];
            int tbsPcUartError;
            const size_t receiveDataLength = TripleBufferSystem_Read(&tbsPcUart, receiveData, PC_UART_BUFFER_SIZE, &tbsPcUartError);

            if (receiveDataLength == 0 && tbsPcUartError == 1)
            {
                printf("Buffer Full!!\n");
                __disable_irq();
                TripleBufferSystem_Destroy(&tbsPcUart);
                TripleBufferSystem_Init(&tbsPcUart, PC_UART_BUFFER_SIZE);
                TripleBufferSystem_setFunc(&tbsPcUart, __disable_irq, tbsPcUartAfterSwap);
                tbsPcUartAfterSwap();
            }

            for (size_t i = 0; i < receiveDataLength; i++)
            {
                if (receiveData[i] == '\r')
                {
                    receiveData[i] = '\n';
                }
                printf("%c", receiveData[i]);
            }
            if (receiveDataLength != 0)
            {
                fflush(NULL);
            }

            pre2 = now;
        }
    }

    // MARK:★System Functions
    // 1
    // 2
    // 3行ないとミニマップで重なる:(

    // MARK:_write (for printf)
    int _write(int file, char *ptr, int len)
    {
        HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, 10);
        return len;
    }

    // MARK:HAL_UART_RxCpltCallback//PcUart割り込み
    void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
    {
        if (huart == &huart2)
        {
            TripleBufferSystem_HeadMove(&tbsPcUart);
            HAL_UART_Receive_IT(&huart2, TripleBufferSystem_NextWriteBuffer(tbsPcUart), 1);
        }
    }

    // MARK:★User Functions
    // 1
    // 2
    // 3行ないとミニマップで重なる:(

    // MARK:tbsPcUartAfterSwap
    void tbsPcUartAfterSwap(void)
    {
        HAL_UART_AbortReceive_IT(&huart2);
        HAL_UART_Receive_IT(&huart2, TripleBufferSystem_NextWriteBuffer(tbsPcUart), 1); // 1byte
        __enable_irq();
    }

#ifdef __cplusplus
}
#endif
