# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: feliciencatteau <feliciencatteau@studen    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/08 19:50:26 by pfaria-d          #+#    #+#              #
#    Updated: 2023/10/27 16:34:59 by feliciencat      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NOCOLOR			= \033[0m
BOLD			= \033[1m
WHITE			= \033[1;37m
RED				= \033[0;31m
LIGHTPURPLE		= \033[1;35m
CLEAR			= \r\033[K
LIGHTGRAY		= \033[0;37m
GREEN			= \033[0;32m
GRAYDRK			= \033[0;30m

SRC 		= main2.cpp\
					Socket.cpp\
					Server.cpp\
					User.cpp\
					Channel.cpp\
					Command.cpp\
					Join.cpp \
					Privmsg.cpp \
					Kick.cpp \
					Invite.cpp 

OBJS_DIR	= ./objs/
OBJS 		= ${addprefix ${OBJS_DIR}, $(SRC:.cpp=.o)}

CC 			= c++

FLAGS		= -Wall -Wextra -Werror -std=c++98

RM  		= rm -rf

NAME		 = ircserv

all: 		$(NAME)

${OBJS_DIR}%.o: %.cpp
			@printf "${CLEAR}${LIGHTGRAY}${BOLD}	Compiling...${NOCOLOR}"
			@$(CC) $(FLAGS) -c $< -o $@

$(NAME):	$(OBJS)
			@$(CC) $(FLAGS) $(OBJS) -o $(NAME) 
			@printf "${CLEAR}${GREEN}${BOLD}	Compiled!\n${NOCOLOR}"

${OBJS}:	| ${OBJS_DIR}

${OBJS_DIR}:
			@mkdir ${OBJS_DIR}

clean:
			@printf "${CLEAR}${LIGHTPURPLE}${BOLD}	Cleaned!\n${NOCOLOR}"
			@$(RM) $(OBJS_DIR)

fclean:		clean
			@$(RM) $(NAME)

re:			fclean $(NAME)
