.PHONY: clean

CXXFLAGS = -march=loongarch64 -mabi=lp64d -g -ffreestanding -fno-stack-protector -nostartfiles -Iinclude -fno-rtti -fno-exceptions
LDFLAGS = -Tkernel.ld -g -ffreestanding -fno-stack-protector -nostartfiles -fno-rtti -fno-exceptions
SOURCES = $(wildcard kernel.cpp \
		     util/*.cpp \
		     uart/*.cpp \
		     mem/*.cpp \
		     exception/*.cpp \
		     clock/*.cpp \
	   )

OBJS = loader.o $(SOURCES:%.cpp=%.o)

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
