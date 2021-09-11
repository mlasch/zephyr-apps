/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(minimal_app, LOG_LEVEL_DBG);

void main(void)
{
    LOG_INF("Start minimal app");
}