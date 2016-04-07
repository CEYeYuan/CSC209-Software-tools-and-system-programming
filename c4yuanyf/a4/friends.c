#include "friends.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * Create a new user with the given name.  Insert it at the tail of the list 
 * of users whose head is pointed to by *user_ptr_add.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if a user by this name already exists in this list.
 *   - 2 if the given name cannot fit in the 'name' array
 *       (don't forget about the null terminator).
 */
int create_user(const char *name, User **user_ptr_add) {
    if (strlen(name) >= MAX_NAME) {
        return 2;
    }

    User *new_user = malloc(sizeof(User));
    if (new_user == NULL) {
        perror("malloc");
        exit(1);
    }
    strncpy(new_user->name, name, MAX_NAME); // name has max length MAX_NAME - 1

    for (int i = 0; i < MAX_NAME; i++) {
        new_user->profile_pic[i] = '\0';
    }

    new_user->first_post = NULL;
    new_user->next = NULL;
    for (int i = 0; i < MAX_FRIENDS; i++) {
        new_user->friends[i] = NULL;
    }

    // Add user to list
    User *prev = NULL;
    User *curr = *user_ptr_add;
    while (curr != NULL && strcmp(curr->name, name) != 0) {
        prev = curr;
        curr = curr->next;
    }
    //BUG FIXED
    if (*user_ptr_add == NULL) {
        *user_ptr_add = new_user;
        return 0;
    } else if (curr != NULL) {
        free(new_user);
        return 1;
    } else {
        prev->next = new_user;
        return 0;
    }
}

void safe_write(int fd, const void *buf, int count){
  int ret = write(fd, buf, count);
  if(ret == -1){
    perror("write:");
    exit(-1);
  }
}

int safe_read(int fd, void *buf, int count){
  int ret = read(fd, buf, count);
  if(ret == -1){
    perror("read:");
    exit(-1);
  }
  return ret;
}


/*
* given a message, the name of the client, and the head of the active
* client list, notify all of them with the msg
*/
void notify(char *msg, char *name, List *head, int len){
     while (head != NULL) {
        if(head->name == NULL || strcmp(head->name, name) != 0)
            ;
        else
            safe_write(head->fd, msg,len);
        head = head->next;
    }
}

/*
*  when there is new user connected in, we want to added it into the fd-name list 
*/

void add_fd(int fd, List** head){
    if (*head == NULL){
        //the list has not been initialized
        *head = malloc(sizeof(List));
        (*head)->fd = fd;
        (*head)->inited = fd;
        (*head)->inbuf = 0;
        (*head)->room = sizeof((*head)->buf);
        (*head)->after = (*head)->buf;
        (*head)->next = NULL;
        memset((*head)->buf, '\0', 100);
        return;
    }else{
        // if there is invalid node (user connect and then disconnect), we can 
        //make use of that node; if not, just add the node to the end of the list
        List *node = find_by_fd(-1, *head);
        if(node != NULL){
            node->fd = fd;
            node->inited = 0;
            node->inbuf = 0;
            node->room = sizeof((*head)->buf);
            node->after = (*head)->buf;
            memset(node->buf, '\0', 100);
            return;
        }else{
            List *cur = *head;
            while( cur->next != NULL){
                cur = cur->next;
            }
            cur->next = malloc(sizeof(List));
            (cur->next)->fd = fd;
            (cur->next)->inbuf = 0;
            (cur->next)->inited = 0;
            (cur->next)->room = sizeof(cur->next->buf);
            (cur->next)->after = (cur->next)->buf;
            (cur->next)->next = NULL;
            memset(cur->next->buf, '\0', 100);
            return;
        }

    }
}


/*
*   given a name, return the corresponding user node
*/
List* find_by_name(char *name, List* head){
    while (head != NULL) {
        if(head->name == NULL || strcmp(head->name, name) != 0)
            head = head->next;
        else
            return head;

    }
    return NULL;
}
/*
*   given an fd, return the corresponding user  
*/
List* find_by_fd(int fd,  List* head){
    while (head != NULL && fd != head->fd) {
        head = head->next;
    }
    return head;
}

/*
*   given an fd, set the name in that node
*/
void set_name(List *node, char *name){
    assert(node != NULL);
    assert(node->inited == 0);
    node->name = malloc(sizeof(char) * (strlen(name) + 1));
    strncpy(node->name, name,strlen(name) + 1);
    return;
}

/*
*   given an user list, build the corresponding fdset
*/
void build_fdset(fd_set *set,  List* head){
     FD_ZERO(set);
      while (head != NULL) {
        if(head->fd > 0)
            FD_SET(head->fd, set);
        head = head->next;
      }
}

/*
* after a user disconnected, since that fd may be used for newly connected user,
* the bind between the fd and old user should be removed
* when we invalid a fd, we just find that node, and set it's value to -1
*/
void invalid(int fd, List *head){
    List *p = find_by_fd(fd, head);
    // since we only call unset on quit function, at that time, there must be one 
    //user using that fd
    assert(p != NULL);
    p->fd = -1;
    p->inbuf = 0;
    p->room = sizeof((p)->buf);
    p->after = p->buf;
    p->inited = 0;
    p->where = 0;
    p->next = NULL;
    if(p->name != NULL)
        free(p->name);
}

/*
*   before we call select, we need to find the max fd in 
*/
int find_max_fd(List *head){
    int max = 0;
    while (head != NULL) {
        if(head->fd > max)
            max = head->fd;
        head = head->next;
    }
    return max;
}




/* 
 * Return a pointer to the user with this name in
 * the list starting with head. Return NULL if no such user exists.
 *
 * NOTE: You'll likely need to cast a (const User *) to a (User *)
 * to satisfy the prototype without warnings.
 */
User *find_user(const char *name, const User *head) {

    while (head != NULL && strcmp(name, head->name) != 0) {
        head = head->next;
    }

    return (User *)head;
}


/*
 * Print the usernames of all users in the list starting at curr.
 * Names should be printed to standard output, one per line.
 */
char* list_users(const User *curr) {
    char *ret = NULL;
    int len = 0; //the len of the final string
    User *tmp = (User *)curr;
    while (tmp != NULL) {
        len += strlen(tmp->name)+2;
        //to keep the same format, 2 more char is used for leading '\t' and 
        //trailing '\n'. Also, null terminator for each string is removed 
        //=1since we only need to add one at the end of the string
        tmp = tmp->next;
    }
    char *p = "User List\n";
    len += 1 + strlen(p);//1 for the null terminator
    ret = malloc(sizeof(char) * len);
    char *head = ret;
    head += snprintf(head, strlen(p)+1, "%s", p);//overwrite the NULL terminator
    while (curr != NULL) {
        head += snprintf(head, strlen(curr->name)+1+2, "\t%s\n",curr->name);
        curr = curr->next;
    }
    *(head) = '\0';//end of the string
    //printf("Composed :\n"); 
    //printf("%s",ret);
    return ret;
}



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
int make_friends(const char *name1, const char *name2, User *head) {
    User *user1 = find_user(name1, head);
    User *user2 = find_user(name2, head);

    if (user1 == NULL || user2 == NULL) {
        return 4;
    } else if (user1 == user2) { // Same user
        return 3;
    }

    int i, j;
    for (i = 0; i < MAX_FRIENDS; i++) {
        if (user1->friends[i] == NULL) { // Empty spot
            break;
        } else if (user1->friends[i] == user2) { // Already friends.
            return 1;
        }
    }

    for (j = 0; j < MAX_FRIENDS; j++) {
        if (user2->friends[j] == NULL) { // Empty spot
            break;
        } 
    }

    if (i == MAX_FRIENDS || j == MAX_FRIENDS) { // Too many friends.
        return 2;
    }

    user1->friends[i] = user2;
    user2->friends[j] = user1;
    return 0;
}




/*
 *  Print a post.
 *  Use localtime to print the time and date.
 */
char* print_post(const Post *post) {
    if (post == NULL) {
        return NULL;
    }
    int len = 0;
    // Print author
    //printf("From: %s\n", post->author);
    len += strlen(post->author) + 6 + 1;// 6 for "From: ", 1 for "\n"
    // Print date
    //printf("Date: %s\n", asctime(localtime(post->date)));
    len += strlen(asctime(localtime(post->date))) + 6 + 1;
    //6 for "Date: ", 1 for "\n"
    // Print message
    //printf("%s\n", post->contents);
    len += strlen(post->contents) + 1 + 1;
    // 1 for "\n", additonal one for the null terminator
    char *ret = malloc(sizeof(char) * len);
    char *head = ret;
    head += snprintf(head, strlen(post->author) + 6 +2, "From: %s\n", post->author);
    head += snprintf(head, strlen(asctime(localtime(post->date)))+6+2, 
        "Date: %s\n", asctime(localtime(post->date)));
    head += snprintf(head, strlen(post->contents)+2, "%s\n", post->contents);
    *head = '\0';
   // printf("Composed: \n");
    //printf("%s", ret);

    return ret;
}


/* 
 * Print a user profile.
 * For an example of the required output format, see the example output
 * linked from the handout.
 * Return:
 *   - 0 on success.
 *   - 1 if the user is NULL.
 */
char* print_user(const User *user) {
    if (user == NULL) {
        return NULL;
    }
    char *line_breaker = "------------------------------------------\n";
    int len = 0 ;
    // Print name
    //printf("Name: %s\n\n", user->name);
    len += strlen(user->name) + 2 + 6;//2 for "\n\n", 6 for "Name: "
    //printf("------------------------------------------\n");
    len += strlen(line_breaker);

    // Print friend list.
    //printf("Friends:\n");
    len += strlen("Friends:\n");
    for (int i = 0; i < MAX_FRIENDS && user->friends[i] != NULL; i++) {
        //printf("%s\n", user->friends[i]->name);
        len += strlen(user->friends[i]->name) + 1;
    }
    //printf("------------------------------------------\n");
    len += strlen(line_breaker);

    // Print post list.
    //printf("Posts:\n");
    len += strlen("Posts:\n");
    const Post *curr = user->first_post;
    while (curr != NULL) {
        char *post = print_post(curr);
        //printf("%s", post);
        len += strlen(post);
        free(post); //prevent memory leak
        curr = curr->next;
        if (curr != NULL) {
            //printf("\n===\n\n");
            len += strlen("\n===\n\n");
        }
    }
    //printf("------------------------------------------\n");
    len += strlen(line_breaker) + 1;// for the null terminator
    //now compose the final string 
    char *ret = malloc(sizeof(char) * len);
    char *head = ret;
    head += snprintf(head, strlen(user->name) + 2 + 6 +1, "Name: %s\n\n", user->name);
    head += snprintf(head, strlen(line_breaker) + 1, "%s", line_breaker);
    head += snprintf(head, strlen("Friends:\n") + 1, "Friends:\n");
    for (int i = 0; i < MAX_FRIENDS && user->friends[i] != NULL; i++) {
        head += snprintf(head, strlen(user->friends[i]->name) + 2, "%s\n", user->friends[i]->name);
    }
    head += snprintf(head, strlen(line_breaker) + 1, "%s", line_breaker);
    head += snprintf(head, strlen("Posts:\n") + 1, "Posts:\n");
    curr = user->first_post;
    while (curr != NULL) {
        char *post = print_post(curr);
        head += snprintf(head, strlen(post) + 1, "%s", post);
        free(post); //prevent memory leak
        curr = curr->next;
        if (curr != NULL) {
            head += snprintf(head, strlen("\n===\n\n") + 1,"\n===\n\n");
        }
    }
    head += snprintf(head, strlen(line_breaker) + 1, "%s", line_breaker);
    *head = '\0';
    //printf("Composed: \n");
    //printf("%s", ret);
    //printf("%s\n","end" );
    return ret;
}


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
int make_post(const User *author, User *target, char *contents) {
    if (target == NULL || author == NULL) {
        return 2;
    }

    int friends = 0;
    for (int i = 0; i < MAX_FRIENDS && target->friends[i] != NULL; i++) {
        if (strcmp(target->friends[i]->name, author->name) == 0) {
            friends = 1;
            break;
        }
    }

    if (friends == 0) {
        return 1;
    }

    // Create post
    Post *new_post = malloc(sizeof(Post));
    if (new_post == NULL) {
        perror("malloc");
        exit(1);
    }
    strncpy(new_post->author, author->name, MAX_NAME);
    new_post->contents = contents;
    new_post->date = malloc(sizeof(time_t));
    if (new_post->date == NULL) {
        perror("malloc");
        exit(1);
    }
    time(new_post->date);
    new_post->next = target->first_post;
    target->first_post = new_post;

    return 0;
}

