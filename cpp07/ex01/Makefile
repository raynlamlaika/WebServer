NAME = Temp

SRC =   main.cpp

OBJ = $(SRC:.cpp=.o)

CPP = c++
CFLAGS = -std=c++98 -Wall -Werror -Wextra 

HEADER = iter.hpp 

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