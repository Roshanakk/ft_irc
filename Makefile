NAME = ircsrv

### SETTINGS ###
CXX := c++
SRC_DIR := srcs/
OBJ_DIR := objs/
H_DIR := includes/
FLAGS := -g -std=c++98 -Wall -Wextra -Werror

### PRETTY COLORS ###
RED:=\033[31;1;4m
GREEN:=\033[32;1;4m
BLUE:=\033[34;1;4m
NOCOLOR:=\033[0m

### FILES ###

SRCS = 	main.cpp \
		ServerException.cpp \
		Dispatch.cpp \
		Server.cpp \
		Client.cpp \
		Channel.cpp \
		AUser.cpp \
		UserOperator.cpp \
		UserRegular.cpp \
		ArgParse.cpp \

OBJS := $(patsubst %.cpp,$(OBJ_DIR)%.o,$(SRCS))

### RULES ###

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o $@
	@printf "$(GREEN)|=:=:=:=:=:=:=:=:= DONE =:=:=:=:=:=:=:=:=|$(NOCOLOR)\n"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@printf "$(BLUE)|=:=:=:=:=:=:=:= BUILDING =:=:=:=:=:=:=:=|$(NOCOLOR)\n"
	@mkdir -p $(dir $@)
	$(CXX) $(FLAGS) -c $(addprefix -I,$(H_DIR)) $< -o $@

clean:
	@printf "$(RED)|=:=:=:=:=:=:=:= CLEANING =:=:=:=:=:=:=:=|$(NOCOLOR)\n"
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)

re: fclean all

.PHONY: all clean fclean re
