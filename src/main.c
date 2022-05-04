/*
 * Copyright (c) 2020, Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr.h>
#include <sys/printk.h>
#include <shell/shell.h>
#include <drivers/uart.h>
#include <drivers/gpio.h>
#include <net/net_mgmt.h>
#include <net/net_event.h>
#include <net/net_conn_mgr.h> 
#include <drivers/modem/gsm_ppp.h>    

#include <logging/log.h>

#define LED0_NODE DT_ALIAS(led1)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0 		DT_GPIO_LABEL(LED0_NODE, gpios)
#define LED0_PIN 	DT_GPIO_PIN(LED0_NODE, gpios)
#define LED0_FLAGS 	DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
// this means that the board don't have a LED2
#error "Unsuported board: led0 devicetree alias is not defined"
#define LED0 		""
#define LED0_PIN 	0
#define LED0_FLAGS 	0
#endif

#define SLEEP_TIME_MS 500


LOG_MODULE_REGISTER(sample_gsm_ppp, LOG_LEVEL_DBG);

static const struct device *gsm_dev;
static struct net_mgmt_event_callback mgmt_cb;
static bool connected;
static bool starting = true;

int main()
{
	int ret;
	bool led_status = false;

	const struct device *uart_dev =
		DEVICE_DT_GET(DT_BUS(DT_INST(0, simcom_sim7000)));

	const struct device *led0 = device_get_binding(LED0);
	if(led0 ==  NULL)
	{
		return;
	}

	ret = gpio_pin_configure(led0, LED0_PIN, GPIO_OUTPUT_ACTIVE | LED0_FLAGS);
	if(ret < 0)
	{
		return;
	}

	while(1)
	{
		gpio_pin_set(led0, LED0_PIN, (int)led_status);
		led_status = !led_status;
		k_msleep(SLEEP_TIME_MS);
	}

	return 0;
}

// static int cmd_sample_modem_suspend(const struct shell *shell,
// 				    size_t argc, char *argv[])
// {
// 	if (!connected) {
// 		shell_fprintf(shell, SHELL_NORMAL, "Not connected.\n");
// 		return -ENOEXEC;
// 	}

// 	gsm_ppp_stop(gsm_dev);

// 	return 0;
// }

// static int cmd_sample_modem_resume(const struct shell *shell,
// 				   size_t argc, char *argv[])
// {
// 	if (starting) {
// 		shell_fprintf(shell, SHELL_NORMAL,
// 			      "Please wait for network connection.\n");
// 		return -ENOEXEC;
// 	}

// 	if (connected) {
// 		shell_fprintf(shell, SHELL_NORMAL, "Already connected.\n");
// 		return -ENOEXEC;
// 	}

// 	gsm_ppp_start(gsm_dev);

// 	return 0;
// }

// SHELL_STATIC_SUBCMD_SET_CREATE(sample_commands,
// 	SHELL_CMD(resume, NULL,
// 		  "Resume the modem\n",
// 		  cmd_sample_modem_resume),
// 	SHELL_CMD(suspend, NULL,
// 		  "Suspend the modem\n",
// 		  cmd_sample_modem_suspend),
// 	SHELL_SUBCMD_SET_END
// );

// SHELL_CMD_REGISTER(sample, &sample_commands,
// 		   "Sample application commands", NULL);


// static void event_handler(struct net_mgmt_event_callback *cb,
// 			  uint32_t mgmt_event, struct net_if *iface)
// {
// 	if ((mgmt_event & (NET_EVENT_L4_CONNECTED
// 			   | NET_EVENT_L4_DISCONNECTED)) != mgmt_event) {
// 		return;
// 	}

// 	starting = false;

// 	if (mgmt_event == NET_EVENT_L4_CONNECTED) {
// 		LOG_INF("Network connected");
// 		connected = true;
// 		return;
// 	}

// 	if (mgmt_event == NET_EVENT_L4_DISCONNECTED) {
// 		LOG_INF("Network disconnected");
// 		connected = false;
// 		return;
// 	}
// }

// static void modem_on_cb(const struct device *dev, void *user_data)
// {
// 	ARG_UNUSED(dev);
// 	ARG_UNUSED(user_data);

// 	LOG_INF("GSM modem on callback fired");
// }

// static void modem_off_cb(const struct device *dev, void *user_data)
// {
// 	ARG_UNUSED(dev);
// 	ARG_UNUSED(user_data);

// 	LOG_INF("GSM modem off callback fired");
// }

// int main(void)
// {
// 	const struct device *uart_dev =
// 		DEVICE_DT_GET(DT_BUS(DT_INST(0, zephyr_gsm_ppp)));

// 	gsm_dev = DEVICE_DT_GET(DT_INST(0, zephyr_gsm_ppp));

// 	/* Optional register modem power callbacks */
// 	gsm_ppp_register_modem_power_callback(gsm_dev, modem_on_cb, modem_off_cb, NULL);

// 	LOG_INF("Board '%s' APN '%s' UART '%s' device %p (%s)",
// 		CONFIG_BOARD, CONFIG_MODEM_GSM_APN,
// 		DT_BUS_LABEL(DT_INST(0, zephyr_gsm_ppp)), uart_dev,
// 		gsm_dev->name);

// 	net_mgmt_init_event_callback(&mgmt_cb, event_handler,
// 				     NET_EVENT_L4_CONNECTED |
// 				     NET_EVENT_L4_DISCONNECTED);
// 	net_mgmt_add_event_callback(&mgmt_cb);

// 	return 0;
// }
