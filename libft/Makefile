NAME    =    libft.a

CC    =    cc

CFLAGS    =    -Werror -Wall -Wextra 

AR    =		ar

ARFLAGS =    rcs

SANITY    =    -fsanitize=address

FILES    =    ft_atoi.c \
			ft_bzero.c \
			ft_calloc.c \
			ft_isalnum.c \
			ft_isascii.c \
			ft_isdigit.c \
			ft_isprint.c \
			ft_isalpha.c \
			ft_itoa.c \
			ft_memchr.c \
			ft_memcmp.c \
			ft_memcpy.c \
			ft_memmove.c \
			ft_memset.c \
			ft_putchar_fd.c \
			ft_putstr_fd.c \
			ft_putendl_fd.c \
			ft_putnbr_fd.c \
			ft_split.c \
			ft_strchr.c \
			ft_strdup.c \
			ft_striteri.c \
			ft_strjoin.c \
			ft_strlcat.c \
			ft_strlcpy.c \
			ft_strlen.c \
			ft_strmapi.c \
			ft_strcmp.c \
			ft_strncmp.c \
			ft_strnstr.c \
			ft_strrchr.c \
			ft_strtrim.c \
			ft_substr.c \
			ft_tolower.c \
			ft_toupper.c \
			get_next_line.c \
			ft_lstadd_back.c \
			ft_lstadd_front.c \
			ft_lstsize.c \
			ft_lstlast.c \
			ft_lstnew.c \
			ft_lstdelone.c \
			ft_lstclear.c \
			ft_lstiter.c \
			ft_lstmap.c
	
HFILES    =    libft.h\

ODIR    =    ./OBJS/

HDIR    =    ./

SRCS    =    $(FILES)

OBJS    =    $(addprefix $(ODIR), $(FILES:.c=.o))

BOBJS    =    $(addprefix $(ODIR), $(BFILES:.c=.o))

all    :     $(ODIR) $(NAME)

$(NAME)    :    $(OBJS) 
	$(AR) $(ARFLAGS) $(NAME) $(OBJS) 
	ranlib $(NAME)

$(ODIR)    :
	mkdir $(ODIR)

$(ODIR)%.o    :    %.c $(HFILES)
	$(CC) $(CFLAGS) -I $(HDIR) -o $@ -c $<

clean    :
	-rm -rf $(OBJS)
	-rm -rf $(ODIR)

fclean    :    clean
	-rm $(NAME)

re    :    
	make fclean
	make all

clearm    :
	clear
	make

.PHONY    :    all clean fclean re