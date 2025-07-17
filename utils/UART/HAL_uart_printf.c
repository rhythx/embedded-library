#include <stdarg.h>

/**
- @brief 通过UART DMA发送格式化字符串 (类似printf)
- @param format: 格式控制字符串
- @param ...: 可变参数
- @retval None
- @note 此函数会等待上一次DMA传输完成后再发送新的数据，
- 因此在快速连续调用时可能会有阻塞。
- 如果格式化后的字符串超过 UART_PRINT_BUFFER_SIZE，会被截断。
**/
//！！！！需要修改huart的端口号
void uart_printf(const char *format, ...) {
	va_list args;
	int len;
	uint32_t primask = __get_PRIMASK(); // 保存中断状态
	
	__disable_irq(); // 关中断确保原子操作
	
	// 等待上次DMA完成
	while (HAL_UART_GetState(&huartx) == HAL_UART_STATE_BUSY_TX) {}
	
	va_start(args, format);
	len = vsnprintf(uart_tx_buffer, UART_PRINT_BUFFER_SIZE, format, args);
	va_end(args);
	
	if (len > 0) {
	    len = len < UART_PRINT_BUFFER_SIZE ? len : UART_PRINT_BUFFER_SIZE - 1;
	    HAL_UART_Transmit_DMA(&huartx, (uint8_t*)uart_tx_buffer, len);
	}
	
	__set_PRIMASK(primask); // 恢复中断状态
}