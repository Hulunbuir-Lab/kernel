.PHONY: clean

CXXFLAGS = -march=loongarch64 -mabi=lp64d -g -std=c++20 -ffreestanding -fno-stack-protector -nostartfiles -Iinclude -fno-rtti -fno-exceptions
LDFLAGS = -Tkernel.ld -g -std=c++20 -ffreestanding -fno-stack-protector -nostartfiles -fno-rtti -fno-exceptions
SOURCES = $(wildcard kernel.cpp \
		     util/*.cpp \
		     uart/*.cpp \
		     mem/*.cpp \
		     exception/*.cpp \
		     clock/*.cpp \
		     timer/*.cpp \
		     lwp/*.cpp \
	   )

OBJS = loader.o exception/context.o $(SOURCES:%.cpp=%.o)

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
