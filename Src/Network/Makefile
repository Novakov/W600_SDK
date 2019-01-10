#/**************************************************************************
# * Kevin 2014-02-24                                                       *
# **************************************************************************/

#---------------------------------------------------------------------------
# Constant Variable definition
#---------------------------------------------------------------------------

ifeq ($(COMPILER_OS_CYGWIN),1)
TOPDIR=../..
endif

include $(TOPDIR)/Tools/toolchain.def

#---------------------------------------------------------------------------
# Target definition (User)
#---------------------------------------------------------------------------
GOAL  = $(LIB_DIR)/wmlwip.$(LIBTYPE)

#---------------------------------------------------------------------------
# Source section (User)
#---------------------------------------------------------------------------
ASM_SRC			+= 

#ifeq ($(LWIP_141), 1)
#LWIP_dir := lwip1.4.1
#LWIP_API_PREFIX := api
#else
#LWIP_dir := lwip2.0.3
#LWIP_API_PREFIX := api2.0.3
#endif
#
#C_SRC := $(LWIP_API_PREFIX)/tls_netconn.c \
#	$(LWIP_API_PREFIX)/tls_sockets.c \
#	$(LWIP_dir)/sys_arch.c \
#	$(LWIP_dir)/api/api_lib.c \
#	$(LWIP_dir)/api/api_msg.c \
#	$(LWIP_dir)/api/err.c \
#	$(LWIP_dir)/api/netbuf.c \
#	$(LWIP_dir)/api/netdb.c \
#	$(LWIP_dir)/api/netifapi.c \
#	$(LWIP_dir)/api/sockets.c \
#	$(LWIP_dir)/api/tcpip.c \
#	$(LWIP_dir)/netif/ethernetif.c \
#	$(LWIP_dir)/core/tcp.c \
#	$(LWIP_dir)/core/tcp_in.c \
#	$(LWIP_dir)/core/tcp_out.c \
#	$(LWIP_dir)/core/udp.c \
#	$(LWIP_dir)/core/def.c \
#	$(LWIP_dir)/core/dns.c \
#	$(LWIP_dir)/core/init.c \
#	$(LWIP_dir)/core/mem.c \
#	$(LWIP_dir)/core/memp.c \
#	$(LWIP_dir)/core/netif.c \
#	$(LWIP_dir)/core/pbuf.c \
#	$(LWIP_dir)/core/raw.c \
#	$(LWIP_dir)/core/stats.c \
#	$(LWIP_dir)/core/sys.c \
#	$(LWIP_dir)/core/alg.c \
#	$(LWIP_dir)/core/ipv4/autoip.c \
#	$(LWIP_dir)/core/ipv4/icmp.c \
#	$(LWIP_dir)/core/ipv4/igmp.c 
#
#ifeq ($(LWIP_141), 1)
#C_SRC += $(LWIP_dir)/ethernet.c \
#	 $(LWIP_dir)/core/ipv4/inet.c \
#	 $(LWIP_dir)/core/ipv4/inet_chksum.c \
#	 $(LWIP_dir)/core/ipv4/ip.c \
#	 $(LWIP_dir)/core/ipv4/ip.c \
#	 $(LWIP_dir)/core/ipv4/ip_addr.c \
#	 $(LWIP_dir)/core/ipv4/ip_frag.c \
#	 $(LWIP_dir)/core/dhcp.c \
#	 $(LWIP_dir)/netif/etharp.c \
#	 $(LWIP_dir)/core/timers.c
#else
#C_SRC += $(LWIP_dir)/core/inet_chksum.c \
#	 $(LWIP_dir)/core/ip.c \
#	 $(LWIP_dir)/core/timeouts.c \
#	 $(LWIP_dir)/core/ipv6/ip6_frag.c \
#	 $(LWIP_dir)/core/ipv6/ip6_addr.c \
#	 $(LWIP_dir)/core/ipv6/ip6.c \
#	 $(LWIP_dir)/core/ipv6/dhcp6.c \
#	 $(LWIP_dir)/core/ipv6/ethip6.c \
#	 $(LWIP_dir)/core/ipv6/icmp6.c \
#	 $(LWIP_dir)/core/ipv6/inet6.c \
#	 $(LWIP_dir)/core/ipv6/mld6.c \
#	 $(LWIP_dir)/core/ipv6/nd6.c \
#	 $(LWIP_dir)/netif/ethernet.c \
#	 $(LWIP_dir)/netif/wm_ethernet.c \
#	 $(LWIP_dir)/core/ipv4/ip4_addr.c \
#	 $(LWIP_dir)/core/ipv4/dhcp.c \
#	 $(LWIP_dir)/core/ipv4/ip4_frag.c
#endif

ifeq ($(LWIP_141), 1)
C_SRC				:= lwip1.4.1/ethernet.c \
				 lwip1.4.1/sys_arch.c \
				 lwip1.4.1/api/api_lib.c \
				 lwip1.4.1/api/api_msg.c \
				 lwip1.4.1/api/err.c \
				 lwip1.4.1/api/netbuf.c \
				 lwip1.4.1/api/netdb.c \
				 lwip1.4.1/api/netifapi.c \
				 lwip1.4.1/api/sockets.c \
				 lwip1.4.1/api/tcpip.c \
				 lwip1.4.1/netif/etharp.c \
				 lwip1.4.1/netif/ethernetif.c \
				 lwip1.4.1/core/tcp.c \
				 lwip1.4.1/core/tcp_in.c \
				 lwip1.4.1/core/tcp_out.c \
				 lwip1.4.1/core/timers.c \
				 lwip1.4.1/core/udp.c \
				 lwip1.4.1/core/def.c \
				 lwip1.4.1/core/dhcp.c \
				 lwip1.4.1/core/dns.c \
				 lwip1.4.1/core/init.c \
				 lwip1.4.1/core/mem.c \
				 lwip1.4.1/core/memp.c \
				 lwip1.4.1/core/netif.c \
				 lwip1.4.1/core/pbuf.c \
				 lwip1.4.1/core/raw.c \
				 lwip1.4.1/core/stats.c \
				 lwip1.4.1/core/sys.c \
				 lwip1.4.1/core/alg.c \
				 lwip1.4.1/core/ipv4/autoip.c \
				 lwip1.4.1/core/ipv4/icmp.c \
				 lwip1.4.1/core/ipv4/igmp.c \
				 lwip1.4.1/core/ipv4/inet.c \
				 lwip1.4.1/core/ipv4/inet_chksum.c \
				 lwip1.4.1/core/ipv4/ip.c \
				 lwip1.4.1/core/ipv4/ip_addr.c \
				 lwip1.4.1/core/ipv4/ip_frag.c \
				 api/tls_netconn.c \
				 api/tls_sockets.c
else
C_SRC				:= api2.0.3/tls_sockets.c \
				 api2.0.3/tls_netconn.c \
				 lwip2.0.3/core/dns.c \
				 lwip2.0.3/core/def.c \
				 lwip2.0.3/core/init.c \
				 lwip2.0.3/core/mem.c \
				 lwip2.0.3/core/memp.c \
				 lwip2.0.3/core/netif.c \
				 lwip2.0.3/core/pbuf.c \
				 lwip2.0.3/core/raw.c \
				 lwip2.0.3/core/stats.c \
				 lwip2.0.3/core/sys.c \
				 lwip2.0.3/core/tcp.c \
				 lwip2.0.3/core/tcp_in.c \
				 lwip2.0.3/core/tcp_out.c \
				 lwip2.0.3/core/udp.c \
				 lwip2.0.3/core/inet_chksum.c \
				 lwip2.0.3/core/ip.c \
				 lwip2.0.3/core/timeouts.c \
				 lwip2.0.3/core/alg.c \
				 lwip2.0.3/core/ipv4/ip4_frag.c \
				 lwip2.0.3/core/ipv4/ip4_addr.c \
				 lwip2.0.3/core/ipv4/ip4.c \
				 lwip2.0.3/core/ipv4/dhcp.c \
				 lwip2.0.3/core/ipv4/etharp.c \
				 lwip2.0.3/core/ipv4/igmp.c \
				 lwip2.0.3/core/ipv4/icmp.c \
				 lwip2.0.3/core/ipv4/autoip.c \
				 lwip2.0.3/core/ipv6/ip6_frag.c \
				 lwip2.0.3/core/ipv6/ip6_addr.c \
				 lwip2.0.3/core/ipv6/ip6.c \
				 lwip2.0.3/core/ipv6/dhcp6.c \
				 lwip2.0.3/core/ipv6/ethip6.c \
				 lwip2.0.3/core/ipv6/icmp6.c \
				 lwip2.0.3/core/ipv6/inet6.c \
				 lwip2.0.3/core/ipv6/mld6.c \
				 lwip2.0.3/core/ipv6/nd6.c \
				 lwip2.0.3/sys_arch.c \
				 lwip2.0.3/netif/ethernetif.c \
				 lwip2.0.3/netif/ethernet.c \
				 lwip2.0.3/netif/wm_ethernet.c \
				 lwip2.0.3/api/tcpip.c \
				 lwip2.0.3/api/api_lib.c \
				 lwip2.0.3/api/api_msg.c \
				 lwip2.0.3/api/err.c \
				 lwip2.0.3/api/netbuf.c \
				 lwip2.0.3/api/netdb.c \
				 lwip2.0.3/api/netifapi.c \
				 lwip2.0.3/api/sockets.c
endif
#---------------------------------------------------------------------------
# Implicit rules
#---------------------------------------------------------------------------
.c.o:
	$(CC) $(CFLAGS) -c -o $*.o $< $(INCLUDES)

.s.o:
	$(ASM) $(ASMFLAGS) -c -o $*.o $< $(INCLUDES)

#---------------------------------------------------------------------------
# Explicit ruls
#---------------------------------------------------------------------------
OBJ_FILES	= $(C_SRC:%.c=%.o) $(ASM_SRC:%.s=%.o)

all: $(GOAL)

$(GOAL): $(OBJ_FILES)
	$(AR) $(ARFLAGS) $@ $(OBJ_FILES)
	
.PHONY: clean
clean:
	$(RM) -f $(GOAL)
	$(RM) -f $(OBJ_FILES:.o=.d) $(OBJ_FILES)
