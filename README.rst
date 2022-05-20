MIN Echo
********

- `MIN protocol <https://github.com/min-protocol/min>`__

**MIN protocol demo. STM32F103C8T6, STM32CubeIDE, USB VCP**

This is essentially a Blinky with USB Virtual COM Port, echoing everything back.

**Purpose**:

Stitch MIN protocol to VCP in the STM32CubeIDE ecosystem.

**Present state of affairs**:

*MIN Echo* is echoing successfully using a simple circular buffer as a stub,
to be replaced with the MIN protocol.

Circular Buffer
===============

``Core/Inc/circus.h``, ``Core/Src/circus.c``. The buffer is plugged into the USB
middleware like this:

- ``usbd_cdc_if.c``, callbacks:

::

    static int8_t CDC_Init_FS(void)
    {
      /* USER CODE BEGIN 3 */
      v_circus_cdc_init_fs();
      /* Set Application Buffers */
      USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
      USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
      return (USBD_OK);
      /* USER CODE END 3 */
    }

::

    static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
    {
      /* USER CODE BEGIN 6 */
      uint8_t* step = pc_circus_cdc_receive_fs(Buf, Len);
      USBD_CDC_SetRxBuffer(&hUsbDeviceFS, step);
      USBD_CDC_ReceivePacket(&hUsbDeviceFS);
      return (USBD_OK);
      /* USER CODE END 6 */
    }

- ``stm32f1xx_it.c``, SysTick callback, where the polling happens:

::

    void SysTick_Handler(void)
    {
      /* USER CODE BEGIN SysTick_IRQn 0 */
    
      /* USER CODE END SysTick_IRQn 0 */
      HAL_IncTick();
      /* USER CODE BEGIN SysTick_IRQn 1 */
      // USB VCP library : background processing of data transmission
      v_circus_vcp_service();
      /* USER CODE END SysTick_IRQn 1 */
    }

- ``main.c``:

::

    int main(void)
    {
      HAL_Init();
      SystemClock_Config();
      MX_GPIO_Init();
      MX_USB_DEVICE_Init();
    
      uint8_t buf[MAIN_BUF_SIZE];
      unsigned int toggle_count = 0;
      while (1)
      {
        if (toggle_count % 100000 == 0)
        {
          HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        }
    
        // VCP demonstration - Echo all data received over VCP  back to the host
        int len = l_circus_vcp_recv(buf, MAIN_BUF_SIZE);  // Read up to MAIN_BUF_SIZE bytes
        if (len > 0)    // If some data was read, send it back:
          len = l_circus_vcp_send(buf, len);
    
        toggle_count++;
      }
    }

