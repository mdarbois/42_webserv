SRCS = main.cpp $(PARSING)Config.cpp $(PARSING)ServerConfig.cpp $(PARSING)LocationConfig.cpp $(PARSING)utils.cpp $(SERVER)ClientSocket.cpp $(SERVER)ServerManager.cpp $(SERVER)ServerSocket.cpp $(SERVER)Socket.cpp $(SERVER)ParserHTTP.cpp

PARSING = ./code/src/parsing/

SERVER = ./code/src/server/

INCS = $(INCLUDE)Config.hpp $(INCLUDE)ServerConfig.hpp $(INCLUDE)LocationConfig.hpp $(INCLUDE)utils.hpp $(INCLUDE)ClientSocket.hpp $(INCLUDE)ParserHTTP.hpp $(INCLUDE)ServerManager.hpp $(INCLUDE)ServerSocket.hpp $(INCLUDE)Socket.hpp

INCLUDE = ./code/includes/

OBJS = $(SRCS:.cpp=.o)

NAME = webserv

FLAGS = -g -Wall -Wextra -Werror -std=c++98 -ggdb

all: $(NAME)

$(NAME): $(OBJS)
	c++ $(FLAGS) $(OBJS) -o $@ -I .

%.o: %.cpp
	c++ $(FLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f $(PARSING)*.o
	rm -f $(SERVER)*.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
