NAME = ircserv
FUN = ircserv

### SETTINGS ###
CXX := g++
SRC_DIR := srcs/
OBJ_DIR := objs/
H_DIR := includes/
FLAGS := -g -std=c++98 -Wall -Wextra -Werror
F_FLAGS :=

### PRETTY COLORS ###
RED:=\033[31;1;4m
GREEN:=\033[32;1;4m
BLUE:=\033[34;1;4m
NOCOLOR:=\033[0m

### FILES ###

SRCS = 	main.cpp \
		ServerException.cpp \
		CommandException.cpp \
		Dispatch.cpp \
		Server.cpp \
		Client.cpp \
		Channel.cpp \
		ChannelManager.cpp \
		ArgParse.cpp \
		Utilities.cpp \
		Command.cpp \
		ClientHistory.cpp \

OBJS := $(patsubst %.cpp,$(OBJ_DIR)%.o,$(SRCS))

### RULES ###

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(FLAGS) $(F_FLAGS) $(OBJS) -o $@
	@printf "$(GREEN)|=:=:=:=:=:=:=:=:= DONE =:=:=:=:=:=:=:=:=|$(NOCOLOR)\n"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@printf "$(BLUE)|=:=:=:=:=:=:=:= BUILDING =:=:=:=:=:=:=:=|$(NOCOLOR)\n"
	@mkdir -p $(dir $@)
	$(CXX) $(FLAGS) $(F_FLAGS) -c $(addprefix -I,$(H_DIR)) $< -o $@

clean:
	@printf "$(RED)|=:=:=:=:=:=:=:= CLEANING =:=:=:=:=:=:=:=|$(NOCOLOR)\n"
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)

re: fclean all

# Adds define for fun printing.
fun: F_FLAGS += -DFUN_FLAGS
fun: fclean all

.PHONY: all clean fclean re
