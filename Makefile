SRCS = main.cpp ./code/src/parsing/Config.cpp ./code/src/parsing/ServerConfig.cpp ./code/src/parsing/LocationConfig.cpp ./code/src/parsing/utils.cpp

INCS = ./code/includes/Config.hpp ./code/includes/ServerConfig.hpp ./code/includes/LocationConfig.hpp ./code/includes/utils.hpp

OBJS = $(SRCS:.cpp=.o)

NAME = webserv

FLAGS = -g -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	c++ $(FLAGS) $(OBJS) -o $@ -I .

%.o: %.cpp
	c++ $(FLAGS) -c $< -o $@

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
