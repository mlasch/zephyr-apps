/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_event.h>
#include <errno.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(wifi, LOG_LEVEL_DBG);

#define SSID "SSID"
#define PSK "PASSWORD"

static K_SEM_DEFINE(wifi_connected, 0, 1);

static struct net_mgmt_event_callback wifi_cb;

static void handle_wifi_connect_result(struct net_mgmt_event_callback *cb)
{
    const struct wifi_status *status = (const struct wifi_status *)cb->info;

    if (status->status)
    {
        LOG_INF("Connection request failed (%d)\n", status->status);
    }
    else
    {
        LOG_INF("Connected\n");
        k_sem_give(&wifi_connected);
    }
}

static void handle_wifi_disconnect_result(struct net_mgmt_event_callback *cb)
{
    const struct wifi_status *status = (const struct wifi_status *)cb->info;

    if (status->status)
    {
        printk("Disconnection request (%d)\n", status->status);
    }
    else
    {
        LOG_WRN("Disconnected\n");
        k_sem_take(&wifi_connected, K_NO_WAIT);
    }
}

static void wifi_mgmt_event_handler(struct net_mgmt_event_callback *cb, uint32_t mgmt_event, struct net_if *iface)
{
    switch (mgmt_event)
    {

        case NET_EVENT_WIFI_CONNECT_RESULT:
            handle_wifi_connect_result(cb);
            break;

        case NET_EVENT_WIFI_DISCONNECT_RESULT:
            handle_wifi_disconnect_result(cb);
            break;

        default:
            LOG_INF("Wi-Fi mgmt_event_handler, event: %d", mgmt_event);
            break;
    }
}

void wifi_connect(void)
{
    int ret;
    net_mgmt_init_event_callback(&wifi_cb, wifi_mgmt_event_handler,
                             NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT);

    net_mgmt_add_event_callback(&wifi_cb);

    struct net_if *iface = net_if_get_first_wifi();


    /* Setting the Wi-Fi interface into station mode is not strictly required
     * Do it anyway.
     */
    struct wifi_mode_info mode_info = {0};

    mode_info.oper = WIFI_MGMT_SET;
    mode_info.mode = WIFI_STA_MODE;
    ret = net_mgmt(NET_REQUEST_WIFI_MODE, iface, &mode_info, sizeof(mode_info));
    if (ret) {
        LOG_ERR("Set mode failed");
    }

    struct wifi_connect_req_params wifi_params = {0};

    wifi_params.ssid = SSID;
    wifi_params.psk = PSK;
    wifi_params.ssid_length = strlen(SSID);
    wifi_params.psk_length = strlen(PSK);
    wifi_params.channel = WIFI_CHANNEL_ANY;
    wifi_params.security = WIFI_SECURITY_TYPE_PSK;
    wifi_params.band = WIFI_FREQ_BAND_2_4_GHZ;
    wifi_params.mfp = WIFI_MFP_OPTIONAL;

    printk("Connecting to SSID: %s\n", wifi_params.ssid);

    if (net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &wifi_params, sizeof(struct wifi_connect_req_params)))
    {
        printk("WiFi Connection Request Failed\n");
    }
}

void wifi_status(void)
{
    struct net_if *iface = net_if_get_default();

    struct wifi_iface_status status = {0};

    if (net_mgmt(NET_REQUEST_WIFI_IFACE_STATUS, iface, &status,	sizeof(struct wifi_iface_status)))
    {
        printk("WiFi Status Request Failed\n");
    }

    printk("\n");

    if (status.state >= WIFI_STATE_ASSOCIATED) {
        printk("SSID: %-32s\n", status.ssid);
        printk("Band: %s\n", wifi_band_txt(status.band));
        printk("Channel: %d\n", status.channel);
        printk("Security: %s\n", wifi_security_txt(status.security));
        printk("RSSI: %d\n", status.rssi);
    }
}

void wifi_disconnect(void)
{
    struct net_if *iface = net_if_get_default();

    if (net_mgmt(NET_REQUEST_WIFI_DISCONNECT, iface, NULL, 0))
    {
        printk("WiFi Disconnection Request Failed\n");
    }
}

void wait_for_wifi(void) {
    k_sem_take(&wifi_connected, K_FOREVER);
}
