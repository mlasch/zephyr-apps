/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(minimal_app, LOG_LEVEL_DBG);

//volatile static uint64_t static_memory_block[30000] = { 0 };

int main(void)
{
//    static_memory_block[0] = 0xaa;
    LOG_INF("Start minimal app");
}
