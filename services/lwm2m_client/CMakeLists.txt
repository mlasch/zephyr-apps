# SPDX-License-Identifier: Apache-2.0

zephyr_library()
zephyr_interface_library_named(lwm2m_client)

zephyr_include_directories(include)

zephyr_library_sources(lwm2m_client.c)
