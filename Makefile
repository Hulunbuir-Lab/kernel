.PHONY: clean

CXXFLAGS = -march=$(ARCH) -mabi=$(ABI) -g -ffreestanding -fno-stack-protector -nostartfiles -Iinclude
LDFLAGS = -Tarch/$(ARCH)/kernel.ld -g -ffreestanding -fno-stack-protector -nostartfiles
SOURCES = $(wildcard system/*.cpp \
		     util/*.cpp \
		     arch/$(ARCH)/uart/*.cpp \
		     mm/*.cpp \
		     kernel.cpp \
	   )

OBJS = $(SOURCES:%.cpp=%.o) arch/$(ARCH)/loader.o

kernel: $(OBJS)
	$(CXX) $^ -o kernel $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.s
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	find . -name \*.o -exec rm {} \;
	if [ -f kernel ]; then rm kernel; fi
