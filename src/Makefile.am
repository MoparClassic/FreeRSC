AM_CFLAGS = -O2 -pedantic -I../libs/libev -DPRINT_PAYLOAD -DSTATIC_ALLOC_CLIENTS=1000 -DSTATIC_ALLOC_LINKEDLIST_NODES=2000000
bin_PROGRAMS = FreeRSC
FreeRSC_SOURCES = circularbuffer.c \
		client.c \
		clientupdater.c \
		dataoperations.c \
		entitystatelist.c \
		entityutil.c \
		gameobject.c \
		item.c \
		linkedlist.c \
		listener.c \
		npc.c \
		packethandler.c \
		player.c \
		playerutil.c \
		server.c \
		tile.c \
		util.c \
		world.c

FreeRSC_LDADD = -lm -ldl -lrt -lgmp ../libs/libev/libevrsc.la

FreeRSC_LDFLAGS = -L/usr/local/lib
