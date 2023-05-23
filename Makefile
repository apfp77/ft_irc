NAME := ircserv
CXX := c++
CXXFLAGS := -Wextra -Werror -Wall -std=c++98

SERVER_DIR:= server
SERVER_SRCS:= 
SERVER_OBJS := $(SERVER_DIR/SERVER_SRCS:%.cpp=%.o)

CLIENT_DIR:= client
CLIENT_SRCS:= 
CLIENT_OBJS := $(CLIENT_DIR/CLIENT_SRCS:%.cpp=%.o)

CHANNEL_DIR:= channel
CHANNEL_SRCS:= 
CHANNEL_OBJS := $(CHANNEL_DIR/CHANNEL_SRCS:%.cpp=%.o)

INCLUDE:= -I $(SERVER_DIR) -I $(CLIENT_DIR) -I $(CHANNEL_DIR)

MAIN_SRCS := main.cpp
MAIN_OBJS := $(MAIN_SRCS:%.cpp=%.o)

OBJS:= $(MAIN_OBJS)  $(SERVER_OBJS) $(CLIENT_OBJS) $(CHANNEL_OBJS)

RM := rm -f

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(INCLUDE) 

all : $(NAME)


clean :
	$(RM) $(OBJS)

fclean :
	make clean
	$(RM) $(NAME)

re :
	make fclean
	make all

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $? -o $@ $(INCLUDE)

.PHONY: all re clean fclean