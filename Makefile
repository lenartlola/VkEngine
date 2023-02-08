NAME = VkEngine

SRCS = ./source/main.cpp ./source/VkEngine.cpp ./source/utils.cpp
INCS = ./include/VkEngine.hpp ./include/utils.hpp

CFLAGS = -std=c++17 -O2 -I./include
LDFLAGS = -lvulkan -lglfw -ldl -lpthread

MVK_DIR = ./libs/MoltenVK
GLFW_DIR_MACOS = ./libs/glfw_osx

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LDFLAGS += -lpthread -lX11 -lXxf86vm -lXrandr -lXi
endif
ifeq ($(UNAME_S),Darwin)
	LDFLAGS += -L /Applications/VulkanSDK/1.2.189.0/macOS/lib -L $(GLFW_DIR_MACOS)/lib-x86_64/ -L $(HOME)/.brew/Lib
	LDFLAGS += -rpath $(GLFW_DIR_MACOS)/lib-x86_64/ -rpath /Applications/VulkanSDK/1.2.189.0/macOS/lib
	CFLAGS += -I$(GLFW_DIR_MACOS)/include -I$(HOME)/.brew/include -I /Applications/VulkanSDK/1.2.189.0/MoltenVK/include/
endif

$(NAME): $(SRCS)
	g++ $(CFLAGS) -o $(NAME) $(SRCS) $(LDFLAGS)

debug: $(SRCS)
	g++ -std=c++17 -fsanitize=address -g3 -o $(NAME) $(SRCS) $(LDFLAGS) $(CFLAGS)

test: re
	./$(NAME)

clean:
	rm -rf $(NAME)

re: clean $(NAME)

.PHONY: test clean debug re
