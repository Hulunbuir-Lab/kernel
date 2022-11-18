CXXFLAGS = -march=loongarch64 -mabi=lp64d -nostdlib -I. -Iarch/loongarch64/
LDFLAGS =
SOURCES = $(wildcard arch/loongarch64/uart/*.cpp \
		     system/*.cpp \
		     kernel.cpp \
	   )
OBJS = $(SOURCES:%.cpp=%.o)

kernel: $(OBJS)
	@$(LD) $^ -o kernel.bin $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
