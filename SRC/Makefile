TARGET := TMPFS-MAKER
CXX := clang++

SRCS := TMP-MAKER.cpp
OBJS := $(SRCS:.cpp=.o)

CXXFLAGS := -std=c++20 -O2 -Wall -Wextra

LDFLAGS := -static \
           -static-libgcc \
           -static-libstdc++ \
           -Wl,--whole-archive -lunwind -llzma -Wl,--no-whole-archive

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
