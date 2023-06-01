NAME := ircserv
CXX := c++
CXXFLAGS := -Wextra -Werror -Wall -std=c++98

SERVER_DIR:= server
SERVER_SRCS:= Server.cpp 
SERVER_OBJS := $(addprefix $(SERVER_DIR)/,$(SERVER_SRCS:.cpp=.o))


CLIENT_DIR:= client
CLIENT_SRCS:= Client.cpp
CLIENT_OBJS := $(CLIENT_DIR/CLIENT_SRCS:%.cpp=%.o)
CLIENT_OBJS := $(addprefix $(CLIENT_DIR)/,$(CLIENT_SRCS:.cpp=.o))

CHANNEL_DIR:= channel
CHANNEL_SRCS:= Channel.cpp Mode.cpp
CHANNEL_OBJS := $(addprefix $(CHANNEL_DIR)/,$(CHANNEL_SRCS:.cpp=.o))

PARSE_DIR:= parse
PARSE_SRCS:= parse.cpp
PARSE_OBJS := $(addprefix $(PARSE_DIR)/,$(PARSE_SRCS:.cpp=.o))

INCLUDE:= -I $(SERVER_DIR) -I $(CLIENT_DIR) -I $(CHANNEL_DIR) -I $(PARSE_DIR) -I includes

MAIN_SRCS := main.cpp
MAIN_OBJS := $(MAIN_SRCS:%.cpp=%.o)

OBJS:= $(MAIN_OBJS) $(SERVER_OBJS) $(CLIENT_OBJS) $(CHANNEL_OBJS) $(PARSE_OBJS)

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