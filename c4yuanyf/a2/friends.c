#include "friends.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Create a new user with the given name.  Insert it at the tail of the list
 * of users whose head is pointed to by *user_ptr_add.
 *
 * Return:
 *   - 0 if successful
 *   - 1 if a user by this name already exists in this list
 *   - 2 if the given name cannot fit in the 'name' array
 *       (don't forget about the null terminator)
 */


int create_user(const char *name, User **user_ptr_add) {
    if(strlen(name) >= MAX_NAME)//too long
    	return -2;

    else if(*user_ptr_add == NULL){
    	//not inited yet
    	*user_ptr_add = malloc(sizeof(User));
    	strcpy((*user_ptr_add)->name,name);
    	(*user_ptr_add)->next = NULL;
    }else{
    	//check if the name is already used while walk through the list
    	User *current = *user_ptr_add;
    	while(current->next != NULL){
    		if(strcmp(current->name,name) == 0)
    			return -1;
    		current = current->next;
    	}
    	// now hit the tail of the list
    	if(strcmp(current->name,name) == 0)
    		return -1;
    	else{
    		//add the user to the end of the list
    		User *new_user = malloc(sizeof(User));
    		strcpy(new_user->name,name);
    		new_user->next = NULL;
    		current->next = new_user;
    		return 0;
    	}
    }
    return 0;
}


/*
 * Return a pointer to the user with this name in
 * the list starting with head. Return NULL if no such user exists.
 *
 * NOTE: You'll likely need to cast a (const User *) to a (User *)
 * to satisfy the prototype without warnings.
 */
User *find_user(const char *name, const User *head) {
    if(head == NULL) 	//empty list
    	return NULL;
    else{
    	User *ret = (User *)head;
    	while (ret != NULL){
    		if(strcmp(ret->name,name) == 0)
    			return ret;
    		ret = ret->next;
    	}
    	return ret;
    }
}


/*
 * Print the usernames of all users in the list starting at curr.
 * Names should be printed to standard output, one per line.
 */
void list_users(const User *curr) {
	while(curr != NULL){
		if (curr-> name != NULL)
			fprintf(stdout, "%s\n",curr->name );
		else
			fprintf(stderr, "%s\n"," Name is NULL " );
		curr = curr->next;
	}
}


/*
 * Change the filename for the profile pic of the given user.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if the file does not exist.
 *   - 2 if the filename is too long.
 */
int update_pic(User *user, const char *filename) {
	if(user == NULL)	return -100;//should not happen
    if(strlen(filename) >= MAX_NAME)// too long
    	return -2;	
    FILE *file = fopen(filename, "r");
    if(file == NULL)
    	return -1;
    fclose(file);
    strcpy(user->profile_pic,filename);
    return 0;
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
    if(strcmp(name1,name2) == 0)
    	return 3;
    User *s1 = NULL;
    User *s2 = NULL;
    while(head != NULL){
    	if(strcmp(head->name,name1)==0)
    		s1 = head;
    	if(strcmp(head->name,name2)==0)
    		s2 = head;
    	head = head->next;
    }
    if(s1 == NULL || s2 == NULL)
    	return 4;
    //when reach this line, both two users are not null
    //since the friends are filled in order, we only need to check if the 
    //last friends is null to know if a user has already get max friends
    if((*s1).friends[MAX_FRIENDS-1] != NULL ||
       (*s2).friends[MAX_FRIENDS-1] != NULL )
		return 2;
	//now we're sure both of them are not null. Since friends are symmetric, 
	//if they're already friends, we only need to check one friend list
	User** friend_list = s1->friends;
	int index = 0;
	while(friend_list[index] != NULL){
		if(strcmp(friend_list[index]->name,name2) == 0)
			return 1;
		index++;
	}
	friend_list[index] = s2;//add s2 to s1's list

	friend_list = s2->friends;
	index = 0;
	while(friend_list[index] != NULL){
		if(strcmp(friend_list[index]->name,name1) == 0)
			return 1;
		index++;
	}
	friend_list[index] = s1;// add s1 to s2's list
	return 0;

}


/*
 * Print a user profile.
 * For an example of the required output format, see the example output
 * linked from the handout.
 * Return:
 *   - 0 on success.
 *   - 1 if the user is NULL.
 */
 typedef struct user {
    char name[MAX_NAME];
    char profile_pic[MAX_NAME];  // This is a *filename*, not the file contents.
    struct post *first_post;
    struct user *friends[MAX_FRIENDS];
    struct user *next;
} User; name ,friends
int print_user(const User *user) {
	if(user == NULL)
		return -1;
	if(user->profile_pic != NULL){//that member is not null
		FILE *file = fopen(user->profile_pic,"r");
		char line[256];
		if(file != NULL){//file do exist
			//print out the profile pic
			while(fgets(line,256,file) != NULL){
				//limit the max line to 256
				fprintf(stdin, "%s\n",line);
			}
			fclose(file);
		}
	}
	assert(user->name != NULL);
	printf("Name: %s\n",user->name );
	printf("\n------------------------------------------\n");
	printf("Friends:\n");
	printf("------------------------------------------\n");
    
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
    return -1;
}


/*
 * From the list pointed to by *user_ptr_del, delete the user
 * with the given name.
 * Remove the deleted user from any lists of friends.
 *
 * Return:
 *   - 0 on success.
 *   - 1 if a user with this name does not exist.
 */
int delete_user(const char *name, User **user_ptr_del) {
    return -1;
}
