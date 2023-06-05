######### Name and Folder #########
NAME 	:= philo

FILES 	:= main.c \
			utils.c

PATH_LIBFT	:= libft/

######### Compileur and various options ########

CC    =    cc

CFLAGS    =    -Werror -Wall -Wextra -g3

AR    =		ar

ARFLAGS =    rcs

SANITY    =    -fsanitize=address

SANITY_THREAD    =    -fsanitize=thread


RM 		:= 	rm -rf

########## Rules and dependency ###########

HFILES    =    philosopher.h\

ODIR    =    ./OBJS/

HDIR    =    ./

OBJS    =    $(addprefix $(ODIR), $(FILES:.c=.o))

all    :     mklib $(ODIR) $(NAME)

$(NAME)    :$(OBJS) ${PATH_LIBFT}libft.a
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(PATH_LIBFT)libft.a -O3  -lpthread #-fsanitize=thread

$(ODIR)    :
			mkdir $(ODIR)

$(ODIR)%.o   :    %.c $(HFILES)
				$(CC) $(CFLAGS) -o $@ -c $<

mklib:
			make -j -C ${PATH_LIBFT}

clean    :
		-rm -rf $(OBJS)
		-rm -rf $(ODIR)
		make clean -C ${PATH_LIBFT}

fclean    :    clean
		-rm $(NAME)
		make fclean -C ${PATH_LIBFT}

re    :
	make fclean
	make all

clearm    :
	clear
	make

.PHONY    :    all clean fclean re    all clean fclean re
