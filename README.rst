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
    #ifdef NO_MIN
      v_circus_cdc_init_fs();
    #else
      min_init_context(&min_ctx, 0);
    #endif
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
    #ifdef NO_MIN
      uint8_t* step = pc_circus_cdc_receive_fs(Buf, Len);
      USBD_CDC_SetRxBuffer(&hUsbDeviceFS, step);
    #else
    #endif
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
    #ifdef NO_MIN
      v_circus_vcp_service();
    #else
      min_poll(&min_ctx, NULL, 0); // (&min_ctx, (uint8_t *)buf, (uint8_t)buf_len)
    #endif
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
    #ifdef NO_MIN
        // VCP demonstration - Echo all data received over VCP  back to the host
        int len = l_circus_vcp_recv(buf, MAIN_BUF_SIZE);  // Read up to MAIN_BUF_SIZE bytes
        if (len > 0)    // If some data was read, send it back :
          len = l_circus_vcp_send(buf, len);
    #else
    #endif
    
        toggle_count++;
      }
    }

Attempt at MIN callbacks (``main.c``):

::

    #ifndef NO_MIN
    /* MIN callbacks: */
    
    // Tell MIN how much space there is to write to the serial port. This is used
    // inside MIN to decide whether to bother sending a frame or not.
    uint16_t min_tx_space(uint8_t _port)
    {
      return 0;
    }
    
    // Send a character on the designated port.
    void min_tx_byte(uint8_t _port, uint8_t byte)
    {
    
    }
    
    // Tell MIN the current time in milliseconds.
    uint32_t min_time_ms(void)
    {
      return HAL_GetTick();
    }
    
    void min_application_handler(uint8_t min_id, uint8_t const *min_payload, uint8_t len_payload, uint8_t port)
    {
      // In this simple example application we just echo the frame back when we get one
      bool result = min_queue_frame(&min_ctx, min_id, min_payload, len_payload);
      if(!result) {
        //printf("Queue failed\n");
      }
    }
    
    void min_tx_start(uint8_t _port)
    {
    
    }
    
    void min_tx_finished(uint8_t _port)
    {
    
    }
    
    #endif

