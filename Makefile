.PHONY: clean

CXXFLAGS = -march=$(ARCH) -mabi=$(ABI) -g -ffreestanding -fno-stack-protector -nostartfiles -Iinclude
LDFLAGS = -Tarch/$(ARCH)/kernel.ld -g -ffreestanding -fno-stack-protector -nostartfiles
SOURCES = $(wildcard kernel.cpp \
		     system/*.cpp \
		     util/*.cpp \
		     arch/$(ARCH)/uart/*.cpp \
		     arch/$(ARCH)/mem/*.cpp \
		     arch/$(ARCH)/util/*.cpp \
		     mem/*.cpp \
	   )

OBJS = arch/$(ARCH)/loader.o $(SOURCES:%.cpp=%.o)

kernel: $(OBJS)
	$(CXX) $^ -o kernel.elf $(LDFLAGS)
	$(OBJCOPY) kernel.elf -O binary kernel

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.s
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	find . -name \*.o -exec rm {} \;
	if [ -f kernel ]; then rm kernel; fi
	if [ -f kernel.elf ]; then rm kernel.elf; fi
