AM_CFLAGS = -DUNIT_TESTING -DUNIT_TESTING_VERBOSITY_LEVEL=1 -DSTATIC_ALLOC_CLIENTS=100 -DSTATIC_ALLOC_LINKEDLIST_NODES=2000 -DMAX_PLAYERS=10
bin_PROGRAMS = FreeRSCUnitTests
FreeRSCUnitTests_SOURCES = AllTests.c \
			CuTest.c \
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
			ClientUpdaterTests.c

FreeRSCUnitTests_LDADD = -lrt
FreeRSCUnitTests_LDFLAGS = -L/usr/local/lib
