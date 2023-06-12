######### Name and Folder #########
NAME 	:= philo

FILES 	:= main.c \
			utils.c \
			init_philo.c \
			death_monitor.c \
			clean_philo.c

PATH_LIBFT	:= libft/

######### Compileur and various options ########

CC    =    cc

CFLAGS    =    -Werror -Wall -Wextra

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


$(NAME)    :$(OBJS) ${PATH_LIBFT}libft.a makefile
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(PATH_LIBFT)libft.a -g3 -lpthread -fsanitize=thread

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

