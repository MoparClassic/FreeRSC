AM_CFLAGS = -DNDEBUG -O2 -pedantic -I../libs/libev -DUNIT_TESTING -DUNIT_TESTING_VERBOSITY_LEVEL=1 -DSTATIC_ALLOC_CLIENTS=100 -DSTATIC_ALLOC_LINKEDLIST_NODES=2000 -DMAX_PLAYERS=10
bin_PROGRAMS = FreeRSCUnitTests
FreeRSCUnitTests_SOURCES = AllTests.c \
			CuTest.c \
			../src/entityutil.c \
			../src/playerutil.c \
			../src/tile.c \
			../src/util.c \
			UtilTests.c \
			../src/linkedlist.c \
			LinkedListTests.c \
			../src/entitystatelist.c \
			EntityStateListTests.c \
			../src/circularbuffer.c \
			CircularBufferTests.c \
			../src/world.c \
			WorldTests.c \
			../src/client.c \
			ClientTests.c \
			../src/player.c \
			PlayerTests.c \
			../src/gameobject.c \
			GameObjectTests.c \
			../src/item.c \
			ItemTests.c \
			../src/npc.c \
			NpcTests.c \
			../src/clientupdater.c \
			ClientUpdaterTests.c \
			EncryptionTests.c

FreeRSCUnitTests_LDADD = -lrt -lgmp ../libs/libev/libevrsc.la
FreeRSCUnitTests_LDFLAGS = -static -L/usr/local/lib
