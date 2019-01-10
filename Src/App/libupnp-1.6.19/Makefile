#/**************************************************************************
# * Kevin 2014-02-24                                                       *
# **************************************************************************/

#---------------------------------------------------------------------------
# Constant Variable definition
#---------------------------------------------------------------------------

include $(TOPDIR)/Tools/toolchain.def

#---------------------------------------------------------------------------
# Target definition (User)
#---------------------------------------------------------------------------
GOAL  = $(LIB_DIR)/wmupnp.$(LIBTYPE)

#---------------------------------------------------------------------------
# Source section (User)
#---------------------------------------------------------------------------
ASM_SRC			+= 
C_SRC			+= ixml/nodeList.c
C_SRC			+= ixml/attr.c
C_SRC			+= ixml/document.c
C_SRC			+= ixml/element.c
C_SRC			+= ixml/ixml.c
C_SRC			+= ixml/ixmldebug.c
C_SRC			+= ixml/ixmlmembuf.c
C_SRC			+= ixml/ixmlparser.c
C_SRC			+= ixml/namedNodeMap.c
C_SRC			+= ixml/node.c
C_SRC			+= threadutil/FreeList.c
C_SRC			+= threadutil/LinkedList.c
C_SRC			+= threadutil/wm_upnp_task.c
C_SRC			+= upnp/gena/gena_callback2.c
C_SRC			+= upnp/gena/gena_ctrlpt.c
C_SRC			+= upnp/gena/gena_device.c
C_SRC			+= upnp/api/upnpapi.c
C_SRC			+= upnp/api/upnpdebug.c
C_SRC			+= upnp/api/UpnpString.c
C_SRC			+= upnp/api/upnptools.c
C_SRC			+= upnp/genlib/client_table/client_table.c
C_SRC			+= upnp/genlib/miniserver/miniserver.c
C_SRC			+= upnp/genlib/net/http/httpparser.c
C_SRC			+= upnp/genlib/net/http/httpreadwrite.c
C_SRC			+= upnp/genlib/net/http/parsetools.c
C_SRC			+= upnp/genlib/net/http/statcodes.c
C_SRC			+= upnp/genlib/net/http/webserver.c
C_SRC			+= upnp/genlib/net/uri/uri.c
C_SRC			+= upnp/genlib/net/sock.c
C_SRC			+= upnp/genlib/service_table/service_table.c
C_SRC			+= upnp/genlib/util/membuffer.c
C_SRC			+= upnp/genlib/util/strintmap.c
C_SRC			+= upnp/genlib/util/upnp_timeout.c
C_SRC			+= upnp/genlib/util/util.c
C_SRC			+= upnp/soap/soap_common.c
C_SRC			+= upnp/soap/soap_ctrlpt.c
C_SRC			+= upnp/soap/soap_device.c
C_SRC			+= upnp/ssdp/ssdp_ctrlpt.c
C_SRC			+= upnp/ssdp/ssdp_device.c
C_SRC			+= upnp/ssdp/ssdp_server.c
C_SRC			+= upnp/urlconfig/urlconfig.c
C_SRC			+= upnp/uuid/sysdep.c
C_SRC			+= upnp/uuid/uuid.c
C_SRC			+= upnp/win_dll.c
C_SRC			+= upnp/inet_pton.c
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
	
