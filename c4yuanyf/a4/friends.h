#ifndef PORT
    #define PORT 54247
#endif
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <stdio.h>
#include <time.h>
#define MAX_NAME 32     // Max username and profile_pic filename lengths
#define MAX_FRIENDS 10  // Max number of friends a user can have

typedef struct user {
    char name[MAX_NAME];
    char profile_pic[MAX_NAME];  // This is a *filename*, not the file contents.
    struct post *first_post;
    struct user *friends[MAX_FRIENDS];
    struct user *next;
} User;

typedef struct post {
    char author[MAX_NAME];
    char *contents;
    time_t *date;
    struct post *next;
} Post;

typedef struct list{
    int fd;
    char *name;
    int inited;
    struct list *next;
    char buf[100];
    int inbuf; // how many bytes currently in buffer?
    int room; // how much room left in buffer?
    char *after; // pointer to position after the (valid) data in buf
    int where; // location of network newline
} List;


/*
* wrapper function of read/write system call
*/
int safe_read(int fd, void *buf, int count);
void safe_write(int fd, const void *buf, int count);

/*
* given a message, the name of the client, and the head of the active
* client list, notify all of them with the msg
*/
void notify(char *msg, char *name, List *head, int len);
/*
*   given an fd, return the corresponding user name 
*/
List* find_by_fd(int fd, List* head);

/*
*   given a name, return the corresponding user node
*/
List* find_by_name(char *name, List* head);

/*
*  when there is new user connected in, we want to added it into fd-name list
*/
void add_fd(int fd, List** head);

/*
*   given an fd, set the name in that node
*/
void set_name(List *node, char* name);

/*
*   given an user list, build the corresponding fdset
*/
void build_fdset(fd_set *set,  List* head);

/*
* after a user disconnected, since that fd may be used for newly connected user,
* the bind between the fd and old user should be removed
* There are two cases:
* 1.if the user is not initialized yet, even before the name and fd is bind, 
* we can simply invalid all it's data
* 2. if the user is initialzied already, we just invalid that fd. 
* When next time the user is logged back in, we can reuse these info
*/
void invalid(int fd,  List* head);

/*
*   before we call select, we need to find the max fd in 
*/
int find_max_fd(List* head);

/*
* after a user disconnected, since that fd may be used for newly connected user,
* the bind between the fd and old user should be removed
* when we invalid a fd, we just find that node, and set it's value to -1
*/
int create_user(const char *name, User **user_ptr_add);


/*
 * Return a pointer to the user with this name in
 * the list starting with head. Return NULL if no such user exists.
 *
 * NOTE: You'll likely need to cast a (const User *) to a (User *)
 * to satisfy the prototype without warnings.
 */
User *find_user(const char *name, const User *head);


/*
 * Print the usernames of all users in the list starting at curr.
 * Names should be printed to standard output, one per line.
 */
char* list_users(const User *curr);



/*
 * Make two users friends with each other.  This is symmetric - a pointer to
 * each user must be stored in the 'friends' array of the other.
 *
 * New friends must be added in the first empty spot in the 'friends' array.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if the two users are already friends.
 *   - 2 if the users are not already friends, but at least one already has
 *     MAX_FRIENDS friends.
 *   - 3 if the same user is passed in twice.
 *   - 4 if at least one user does not exist.
 *
 * Do not modify either user if the result is a failure.
 * NOTE: If multiple errors apply, return the *largest* error code that applies.
 */
int make_friends(const char *name1, const char *name2, User *head);


/*
 * Print a user profile.
 * For an example of the required output format, see the example output
 * linked from the handout.
 * Return:
 *   - 0 on success.
 *   - 1 if the user is NULL.
 */
char* print_user(const User *user);


/*
 * Make a new post from 'author' to the 'target' user,
 * containing the given contents, IF the users are friends.
 *
 * Insert the new post at the *front* of the user's list of posts.
 *
 * Use the 'time' function to store the current time.
 *
 * 'contents' is a pointer to heap-allocated memory - you do not need
 * to allocate more memory to store the contents of the post.
 *
 * Return:
 *   - 0 on success
 *   - 1 if users exist but are not friends
 *   - 2 if either User pointer is NULL
 */
int make_post(const User *author, User *target, char *contents);

