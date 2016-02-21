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
    	return 2;
    if(find_user(name,*user_ptr_add) != NULL)
    	return 1;

    else if(*user_ptr_add == NULL){
    	//not inited yet
    	*user_ptr_add = malloc(sizeof(User));
    	if(*user_ptr_add == NULL)//malloc fails
    		return -1;
    	strcpy((*user_ptr_add)->name,name);
    	(*user_ptr_add)->next = NULL;
    	return 0;
    }else{
    	//check if the name is already used while walk through the list
    	User *current = *user_ptr_add;
    	while(current->next != NULL){
    		current = current->next;
    	}	
		//add the user to the end of the list
		User *new_user = malloc(sizeof(User));
		if(new_user == NULL)// malloc fails
			return -1;
		strcpy(new_user->name,name);
		new_user->next = NULL;
		current->next = new_user;
		return 0;	
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
    	return 2;	
    FILE *file = fopen(filename, "r");
    if(file == NULL)
    	return 1;
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
    User *u1 = find_user(name1,head);
    User *u2 = find_user(name1,head);
    if(u1 == NULL || u2 == NULL)
    	return 4;
    //check if they're already friends. due to symmetric, only need
	//to check one of them
	User** friend_list = u1->friends;
	int index = 0;
	for(index = 0 ; index < MAX_FRIENDS ; index++){
		if(friend_list[index] != NULL){
			if(strcmp(friend_list[index]->name,name2) == 0)
				return 1;
		}
	}
	
    //check if the a user has already get max friends
    int empty_u1=-1;//the first empty spot of their friend_list
    int empty_u2=-1;
    int i =0;
	for(i=0 ; i < MAX_FRIENDS ; i++){
		if(u1->friends[i] == NULL && empty_u1 == -1){
			//make sure only return the first empty spot
			empty_u1=i;
		}
		if(u2->friends[i] == NULL && empty_u2 == -1){
			//make sure only return the first empty spot
			empty_u2=i;
		}
	}
	if(empty_u1 == -1 ||empty_u2 == -1)
		return 2;
	(*u1).friends[empty_u1] = u2;
	(*u2).friends[empty_u2] = u1;
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
 
int print_user(const User *user) {
	if(user == NULL)
		return 1;
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
	if(user->name != NULL){//should always happen
		printf("Name: %s\n",user->name );
	}
	printf("\n------------------------------------------\n");
	printf("Friends:\n");
	//print friend list
	User **friends = ((User *)user)->friends;
	int i=0;
	while(friends[i] != NULL && i < MAX_FRIENDS){
		printf("%s\n",friends[i]->name);
		i++;
	}
	printf("------------------------------------------\n");
	printf("Posts:\n");
	//print post list
	Post *p = user->first_post;
	if(p != NULL){
		while(p->next != NULL){//last post need special treatment
			printf("From: %s\n",p->author);
			printf("Date: %s\n\n",ctime(p->date));
			printf("%s\n\n",p->contents);
			printf("===\n\n");
			p = p->next;
		}
		printf("From: %s\n",p->author);//print the last post
		printf("Date: %s\n\n",ctime(p->date));
		printf("%s\n",p->contents);
	}
	printf("------------------------------------------\n");
    return 0;
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
    if(author == NULL || target == NULL)
    	return 2;
    User **list = ((User *)author)->friends;
    int i = 0;
    int isFriend=0;
    while(list[i] != NULL){
    	if(strcmp(list[i]->name,target->name) == 0){
    		isFriend = 1;
    		break;
   		}
   		i++;
    }
    if(isFriend == 0)
    	return 1;
    else{
    	Post *p = malloc(sizeof(Post));
    	if(p == NULL)//check if malloc fails
    		return -1;
    	strcpy(p->author,author->name);
    	p->contents = contents;
    	p->date = malloc(sizeof(time_t));
    	if(p->date == NULL)//check if malloc fails
    		return -1;
    	time(p->date);
    	p->next = target->first_post;
    	target->first_post = p;
    	return 0;
    }
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
    User *del = find_user(name,*user_ptr_del);
    if(del == NULL)
    	return 1;
  	if(strcmp((*user_ptr_del)->name,name) == 0)//first user of the list
  		*user_ptr_del = (*user_ptr_del)->next;
  	else{
  		//find the previous user of the deleted, set its next ptr to del->next
  		User *prev = *user_ptr_del;
  		while(strcmp(prev->name,name) != 0){
  			prev = prev->next;
  		}
  		prev->next = del->next;
  		free(del);
  		//remove the deleted user from the friend list
  		User *cur = *user_ptr_del;
  		while(cur != NULL){
  			User **friends = cur->friends;
  			int i =0;
  			for(i=0 ; i<MAX_FRIENDS ; i++){
  				if(friends[i] != NULL && strcmp(friends[i]->name,name) == 0){
  					//they are friends
  					friends[i] = NULL;
  					break;// same friends can't repeat twice
  				}
  			}
  			cur = cur->next;
  		}
  	}
  	free(del);
  	return 0;
}
