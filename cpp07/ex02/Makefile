NAME = Array

SRC =   main.cpp

OBJ = $(SRC:.cpp=.o)

CPP = c++
CFLAGS =-Wall -Werror -Wextra  -std=c++98 

HEADER = Array.hpp 

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP)  $(CFLAGS) $^ -o $(NAME)


%.o: %.cpp $(HEADER)
	$(CPP)  $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all