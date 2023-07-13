# Philosophers

**Philosophers** is a project within the 42-school curriculum that aims to solve the dining philosophers problem and familiarize you with threads and mutexes.

## Description
In this project, there are *n* philosophers seated around a table with *n* forks. Each philosopher has a time to die, a time to eat, and a time to sleep. Additionally, there is an optional fourth argument that defines how many times a philosopher must eat.

To eat, each philosopher must have two forks, which means they will use their own fork and one of the forks belonging to their neighboring philosopher on the right or left. Once a philosopher has finished eating, they sleep for a certain time specified in the argument. Afterwards, they wait to acquire two forks again before eating.

Meanwhile, your program must periodically check if any philosopher has died. This is determined by calculating the difference between the last time a philosopher has eaten and the current time, and comparing it to the "time to death" given in the argument. If this time difference exceeds the "time to death," your program must stop and display the ID of the philosopher who died, along with the time it happened, with a maximum delay of 10ms. Additionally, no further output should be written after a philosopher has died.

To learn the basics of threads and mutex functions and how to use them, I strongly recommend watching the initial videos from the course [UNIX Threads in C](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2) on the CodeVault YouTube channel.

## Tips
- Before writing any messages, you can check a variable to determine if any philosopher has died. However, remember that the philosophers should not communicate with each other. Organize your structures, mutexes, and threads accordingly.
- To verify the correctness of your program, you can run the following check: `./philosopher 3 1000 340 200`. Your message should be displayed between 1000ms and 1010ms. If not, there might be an issue with your death check.
- Personally, I chose to create two different routines: one for the odd philosophers and another one for the even philosophers. I instruct each even philosopher to grab their right fork first and then check for the left fork. For odd philosophers, I ask them to grab their left fork first and then look for the right fork.
- Adding a small delay before starting the while loop in your routine can help synchronize your philosophers. Personally, this minor addition significantly improved performance.
- For testing your philosophers, I highly recommend using a tester I found to be the best: [42 Philosophers Tester by dantonik](https://github.com/dantonik/42-philosophers-tester).

## Common Warnings
- Don't forget to destroy your mutexes and threads at the end of your program, paying attention to the order.
- Be mindful of deadlocks! Consider them from the very beginning, as trying to fix them after your code works might require starting from scratch. My advice is to use the minimum number of mutexes possible. In my code, I use one mutex for each fork and only one additional mutex whenever I check or modify a value or write text.

## Useful Resources
- [UNIX Threads in C](https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)
- [42 Philosophers Tester by dantonik](https://github.com/dantonik/42-philosophers-tester)
- Flags for data race sanitizer: -fsanitize=thread
- Command for data race detection with Valgrind: --tool=helgrind
- Note that using Valgrind on an executable compiled with the sanitizer, as well as both of these commands, will significantly slow down your program's performance. They are only meant for checking the correct execution of your code. Remember to remove them when you're done.

Happy coding!

**Fixed** : Thanks to [Xavier](https://github.com/xaviecat), he shows me that I forgot to managed properly the case of a negative "time to death/sleep/think or a negative/NULL number of philosophers".
