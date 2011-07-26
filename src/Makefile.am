AM_CFLAGS = -DSTATIC_ALLOC_CLIENTS=1000 -DSTATIC_ALLOC_LINKEDLIST_NODES=2000000
bin_PROGRAMS = FreeRSC
FreeRSC_SOURCES = circularbuffer.c \
		client.c \
		clientupdater.c \
		entitystatelist.c \
		gameobject.c \
		item.c \
		linkedlist.c \
		listener.c \
		npc.c \
		packethandler.c \
		player.c \
		server.c \
		util.c \
		world.c

FreeRSC_LDADD = -levrsc -lm -ldl -lrt

FreeRSC_LDFLAGS = -L/usr/local/lib -static
