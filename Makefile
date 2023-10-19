# NAME		=	webserv

# SRCS		=	main.cpp Client.cpp HttpRequest.cpp HttpResponse.cpp Parser.cpp Server.cpp WebServ.cpp
# SRC			=	$(addprefix source/,	$(SRCS))
# OBJ			=	$(addprefix obj/, $(SRCS:.cpp=.o))


# CXX			=	g++

# CXXFLAGS	=	-Wall -Werror -Wextra -std=c++98

# RM			=	rm -rf

# GREEN	=	\033[0;32m
# YELLOW	=	\033[0;33m
# RESET	=	\033[0m
# _SUCCESS	=	✅ $(GREEN)Successfully compiled$(RESET)


# all: $(NAME)

# $(NAME): obj $(OBJ)
# 	@$(CXX) $(OBJ) -o $(NAME)
# 	@printf "$(_SUCCESS) webserv is ready.\n"

# obj:
# 	@mkdir -p obj

# .cpp.o:
# 	@$(CXX) $(CXXFLAGS) -c $(SRC) -o $@
# 	@echo -n "#"

# clean:
# 	@$(RM) obj
# 	@printf "$(YELLOW) objects removed.\n $(RESET)"

# fclean: clean
# 	@$(RM) $(NAME)

# re: fclean all

# .PHONY: all clean fclean re

NAME = webserv
SRCS = main.cpp Webserv.cpp Server.cpp 
#Client.cpp HttpRequest.cpp HttpResponse.cpp Parser.cpp

# Directories
SRC_DIR = source/
OBJ_DIR = obj/

# Compiler and flags
CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98

# Source and object file paths
SRCS_PATH = $(addprefix $(SRC_DIR), $(SRCS))
OBJS = $(patsubst $(SRC_DIR)%.cpp, $(OBJ_DIR)%.o, $(SRCS_PATH))
#OBJS		:=	$(addprefix $(OBJ_DIR), $(SRCS:.cpp=.o))



GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m
_SUCCESS = ✅ $(GREEN)Successfully compiled$(RESET)


# Main target
all: $(OBJ_DIR) $(NAME)

# Create the object directory if it doesn't exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Compile source files into object files
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo -n "#"

# Link object files into the final executable
$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) -Iinclude $^ -o $@
	@printf "\n$(_SUCCESS) webserv now is ready.\n"

clean:
	@rm -rf $(OBJ_DIR)
	@printf "$(YELLOW)Objects removed.\n$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@printf "$(YELLOW)Executable removed.\n$(RESET)"

re: fclean all

.PHONY: all clean fclean re
