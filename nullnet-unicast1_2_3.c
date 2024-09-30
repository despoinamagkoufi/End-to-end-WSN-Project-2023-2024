/*
 * Copyright (c) 2017, RISE SICS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         NullNet unicast example
 * \author
*         Simon Duquennoy <simon.duquennoy@ri.se>
 *
 */

#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "arch/dev/sensor/sht11/sht11-sensor.h"
#include <string.h>
#include <stdio.h> /* For printf() */
#include "utils.h"

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Configuration */
#define SEND_INTERVAL (20 * CLOCK_SECOND)
static linkaddr_t dest_addr =         {{ 0x24, 0x10, 0xea, 0x13, 0x00, 0x74, 0x12, 0x00 }};

#if MAC_CONF_WITH_TSCH
#include "net/mac/tsch/tsch.h"
static linkaddr_t coordinator_addr =  {{ 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};
#endif /* MAC_CONF_WITH_TSCH*/ 

/*---------------------------------------------------------------------------*/
PROCESS(nullnet_example_process, "NullNet unicast example");
AUTOSTART_PROCESSES(&nullnet_example_process);

/*---------------------------------------------------------------------------*/

static struct data_form sensor_data;

/* Input callback for handling received data */
void input_callback(const void *data, uint16_t len,
  const linkaddr_t *src, const linkaddr_t *dest)
{
  if(len == sizeof(struct data_form)) 
{
    struct data_form received_data;
    memcpy(&received_data, data, sizeof(received_data));
    char temp_str[20], hum_str[20];

// Convert double to string for temperature and humidity
    double2str(temp_str, received_data.temp);
    double2str(hum_str, received_data.hum);
    
  
   LOG_INFO("Received data from ");
    LOG_INFO_LLADDR(src);
    LOG_INFO_(": ID=%u, Count=%u, Temp=%s C, Hum=%s %%RH\n", 
              received_data.ID, received_data.count, temp_str, hum_str);
  }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(nullnet_example_process, ev, data)
{
  static struct etimer periodic_timer;
  static unsigned count = 0;

  PROCESS_BEGIN();
	
#if MAC_CONF_WITH_TSCH
  tsch_set_coordinator(linkaddr_cmp(&coordinator_addr, &linkaddr_node_addr));
#endif /* MAC_CONF_WITH_TSCH */


  /* Initialize NullNet */
  nullnet_buf = (uint8_t *)&sensor_data;
  nullnet_len = sizeof(sensor_data);
  nullnet_set_input_callback(input_callback);

  sensor_data.ID = (linkaddr_node_addr.u8[0] ) | (linkaddr_node_addr.u8[1] );

  sensor_data.count = 0;
  sensor_data.temp = 0;
  sensor_data.hum = 0;

// Activate the SHT11 sensor
  SENSORS_ACTIVATE(sht11_sensor);


  if(!linkaddr_cmp(&dest_addr, &linkaddr_node_addr)) {
    etimer_set(&periodic_timer, SEND_INTERVAL);
    while(1) {
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

      /* Read sensor values */
      int temp_raw = sht11_sensor.value(SHT11_SENSOR_TEMP);
      int hum_raw = sht11_sensor.value(SHT11_SENSOR_HUMIDITY);

      // Check for valid sensor readings
      if (temp_raw != -1 && hum_raw != -1) {
        sensor_data.temp = temperature_int2double(temp_raw);
        sensor_data.hum = humidity_int2double(hum_raw);
      } else {
        LOG_WARN("Sensor reading failed\n");
      }

      sensor_data.count = count;

      // Convert double values to string for logging purposes
      char temp_str[20], hum_str[20];
      double2str(temp_str, sensor_data.temp);
      double2str(hum_str, sensor_data.hum);      


      // Log the sensor data
      LOG_INFO("Sending data: ID=%02x%02x, Count=%u, Temp=%s C, Hum=%s %%RH to ", 
               linkaddr_node_addr.u8[0], linkaddr_node_addr.u8[1], sensor_data.count, temp_str, hum_str);
      LOG_INFO_LLADDR(&dest_addr);
      LOG_INFO_("\n");

      NETSTACK_NETWORK.output(&dest_addr);
      count++;
      etimer_reset(&periodic_timer);
    }
  }

  PROCESS_END();
}
/*-------------------------------------------------------------------------*/
	
