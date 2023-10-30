NOCOLOR='\033[0m'
RED='\033[0;31m'
GREEN='\033[0;32m'
ORANGE='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
LIGHTGRAY='\033[0;37m'
DARKGRAY='\033[1;30m'
LIGHTRED='\033[1;31m'
LIGHTGREEN='\033[1;32m'
YELLOW='\033[1;33m'
LIGHTBLUE='\033[1;34m'
LIGHTPURPLE='\033[1;35m'
LIGHTCYAN='\033[1;36m'
WHITE='\033[1;37m'

NAME = ircserv

CC = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC_DIR = srcs

OBJ_DIR = objs

SRCS = 	$(SRC_DIR)/main.cpp \
				 	$(SRC_DIR)/Channel.cpp \
				 	$(SRC_DIR)/User.cpp \
				 	$(SRC_DIR)/Server.cpp \
				 	${SRC_DIR}/commands/Command.cpp \
				 	${SRC_DIR}/commands/Join.cpp \
				 	${SRC_DIR}/commands/Invite.cpp \
				 	${SRC_DIR}/commands/Kick.cpp \
				 	${SRC_DIR}/commands/Privmsg.cpp \

OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CXXFLAGS) -I ./headers -c $< -o $@

$(NAME): $(OBJS)
	@echo ${LIGHTBLUE}$(NAME) compiling !${NOCOLOR}
	@$(CC) $(CXXFLAGS) -I ./headers $(OBJS) -o $@
	@echo ${LIGHTGREEN}$(NAME) compiled !${NOCOLOR}

clean:
	@rm -rf $(OBJ_DIR)
	@echo ${LIGHTRED}$(NAME) objects removed !${NOCOLOR}

fclean: clean
	@rm -f $(NAME)
	@echo ${LIGHTRED}$(NAME) removed !${NOCOLOR}

re: fclean all

.PHONY: all clean fclean re
