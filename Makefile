.PHONY: clean

CXXFLAGS = -march=loongarch64 -mabi=lp64d -nostdlib -I. -Iarch/loongarch64/
LDFLAGS = -Tkernel.ld
SOURCES = $(wildcard arch/loongarch64/uart/*.cpp \
		     system/*.cpp \
		     kernel.cpp \
	   )
OBJS = $(SOURCES:%.cpp=%.o)

kernel: $(OBJS)
	@$(LD) $^ -o kernel $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@find . -name \*.o -exec rm {} \;
