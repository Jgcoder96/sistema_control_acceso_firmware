#ifndef WIEGAND_INTERNAL_H
#define WIEGAND_INTERNAL_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

/**
 * @file wiegand_internal.h
 * @brief Contexto interno de interrupciones para el bus Wiegand.
 */

/**
 * @brief Contexto global para ensamblar los bits Wiegand.
 *
 * Contiene el buffer de bits, contador y un mutex para proteger 
 * la lectura/escritura entre la interrupción de hardware y la tarea de monitoreo.
 */
typedef struct {
  uint64_t           bit_buffer;       ///< Bits ensamblados hasta ahora
  volatile int       bit_count;        ///< Cantidad de bits recibidos
  volatile uint64_t  last_bit_time_us; ///< Timestamp del último bit (para timeout)
  portMUX_TYPE       mux;              ///< Spinlock para zona crítica
} wiegand_context_t;

extern wiegand_context_t *g_wiegand_ctx;

#endif