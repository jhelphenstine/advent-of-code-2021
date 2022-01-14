CFLAGS +=-std=c11 -Wall -Wextra -Wpedantic -Waggregate-return -Wwrite-strings -Wfloat-equal -Wvla
METER_LIBS = input.c measure.c

.DEFAULT_GOAL power_meter

power_meter: $(METER_LIBS)

.PHONY: debug
debug: CFLAGS +=-g
debug: .DEFAULT_GOAL

.PHONY profile
profile: CFLAGS +=-pg
profile: LDFLAGS +=-pg
profile: .DEFAULT_GOAL

TEST_OBJECTS = $(subst *.c, *.o, $(wildcard test/test*.c))
test/testAll: CFLAGS += -I. -g
# In case we use this macro
# test/testAll: CPPFLAGS += -D_TESTING
test/testAll: LDLIBS += -lcunit
test/testAll: $(TEST_OBJECTS) $(METER_LIBS)

.PHONY: check
check: test/testAll
check:
	cd test;./testAll;./test.sh;cd ..

.PHONY: clean
clean:
	$(RM) *.o power_meter test/testAll
