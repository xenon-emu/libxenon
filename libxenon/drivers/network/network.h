#ifndef __include_network_h
#define __include_network_h

#ifdef __cplusplus
extern "C" {
#endif

#include <lwip/netif.h>
#include <lwip/dhcp.h>

void network_init(int withDHCP, ip_addr_t static_fallback_ip_addr, ip_addr_t static_fallback_gateway, ip_addr_t static_fallback_netmask);
void network_poll();
void network_print_config();

extern struct netif netif;

#ifdef __cplusplus
}
#endif

#endif