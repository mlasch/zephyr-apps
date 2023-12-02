/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(wifi_sensor, LOG_LEVEL_DBG);

#include "wifi.h"

int main(void) {
    LOG_INF("Start Wi-Fi sensor");

    /* Init network */


    wifi_connect();
    wait_for_wifi();
    wifi_status();

}
