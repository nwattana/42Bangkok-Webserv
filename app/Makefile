#  |  |  ___ \    \  |         |
#  |  |     ) |  |\/ |   _  |  |  /   _
# ___ __|  __/   |   |  (   |    <    __/
#    _|  _____| _|  _| \__,_| _|\_\ \___|
#                              by jcluzet
################################################################################
#                                     CONFIG                                   #
################################################################################

NAME        := webserv
CXX         	:= c++
CFLAGS    	:= -Wall -Wextra -Werror -std=c++98
INC_DIR    	:= $(shell find ./includes -type d)

################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

DEPS		:= $(shell find ./includes/ -name "*.hpp")
SRCS		:= $(shell find ./srcs/ -name "*.cpp")


OBJS        := ${SRCS:.cpp=.o}

.cpp.o:
	${CXX} ${CFLAGS} $(addprefix -I, $(INC_DIR)) -c $< -o ${<:.cpp=.o}

################################################################################
#                                  Makefile  objs                              #
################################################################################


CLR_RMV		:= \033[0m
RED		    := \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m
RM		    := rm -f

${NAME}:	${OBJS}
			@echo "$(GREEN)Compilation ${CLR_RMV}of ${YELLOW}$(NAME) ${CLR_RMV}..."
			${CXX} ${CFLAGS} -o ${NAME} ${OBJS}
			@echo "$(GREEN)$(NAME) created[0m ✔️"


all:		${NAME}

${OBJS}:	${DEPS}

bonus:		all

clean:
			@ ${RM} *.o */*.o */*/*.o
			@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)objs ✔️"

fclean:		clean
			@ ${RM} ${NAME}
			@ echo "$(RED)Deleting $(CYAN)$(NAME) $(CLR_RMV)binary ✔️"

# TODO remove me :D
faster_way_compilation:   fclean
			@make -j 4 all

re:			fclean all

test:		all
			@./${NAME}

.PHONY:		all clean fclean re


