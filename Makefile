.PHONY: clean

CXXFLAGS = -march=$(ARCH) -mabi=$(ABI) -nostdlib -I. -Iarch/$(ARCH)/
LDFLAGS = -Tkernel.ld
SOURCES = $(wildcard system/*.cpp \
		     common/string/*.cpp \
		     common/variable/*.cpp \
		     arch/$(ARCH)/uart/*.cpp \
		     kernel.cpp \
	   )

OBJS = $(SOURCES:%.cpp=%.o) loader.o

kernel: $(OBJS)
	$(LD) $^ -o kernel $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.s	
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	find . -name \*.o -exec rm {} \;
	if [ -f kernel ]; then rm kernel; fi
