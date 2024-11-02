RED=\033[1;31m
GREEN=\033[1;32m
YELLOW=\033[1;33m
BLUE=\033[1;34m
MAGENTA=\033[1;35m
CYAN=\033[1;36m
END=\033[0m

NAME = libftpp.a

CXX= c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++23

ifdef DEBUG
CXXFLAGS += -g -fsanitize=address
else
CXXFLAGS += -O3
endif

DIR_S = srcs
DIR_I = incs
DIR_O = obj

INCS = -I $(DIR_I)
SRCS = $(shell find $(DIR_S) -type f -name '*.cpp')
HEADERS = $(shell find $(DIR_I) -type f -name '*.hpp')
OBJS = $(SRCS:$(DIR_S)/%.cpp=$(DIR_O)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(MAGENTA)Creating $@$(END)"
	@ar -rcs $@ $^
	@echo "$(GREEN)Done!$(END)"

$(DIR_O)/%.o: $(DIR_S)/%.cpp $(HEADERS)
	@mkdir -p $(dir $@)
	@echo "$(BLUE)Compiling $(notdir $<)$(END)"
	@$(CXX) $(CXXFLAGS) $(INCS) -c $< -o $@

clean:
	@echo "$(RED)Removing objs$(END)"
	@rm -rf $(DIR_O)

fclean: clean
	@echo "$(RED)Removing $(NAME)$(END)"
	@rm -rf $(NAME)

re: fclean all

debug:
	@$(MAKE) re DEBUG=1

.PHONY: all clean fclean re debug