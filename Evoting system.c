#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif


#define MAX_MEMBERS 7
#define MAX_VOTER 100
#define MAX_NAME_LENGTH 50
// Global variables for hardcoded usernames and password

char username1[50]="maha";
char username2[50]="annie";
char username3[50]="mubeen";
char username4[50]="arfa";
char password[10]="123456";
char required_username[50];
char required_password[10];


// Structures for representing department, member, and voter information

struct Dept {
	char dept_name[MAX_NAME_LENGTH];
	char dept_hod[MAX_NAME_LENGTH];
	char dept_section[10];
};
struct Member {
	char name[MAX_NAME_LENGTH];
	char reg_no[50];
	int age;
	int semester;
	struct Dept department;
};
struct voters {
	char name[MAX_NAME_LENGTH];
	char reg_no[50];
	int voted_for;
};
//function prototype
void displayTermsAndConditions(); // Function to display terms and conditions
void print_not_elligible();	// Function to print messages for ineligibility
bool is_valid_reg_no(const char *reg_no);   // Function to validate registration numbers
void vote(struct voters voter[], int *num_voter, struct Member members[], int num_members);   // Function for voting process
void displayVotes(struct voters voter[], int num_voter, struct Member members[], int num_members);   // Function to display votes
void display_result(struct Member members[], int num_members, struct voters voter[], int num_voter);  // Function to display election results
void user_login();  // Function for user login
void winElection(struct Member members[], int num_members, struct voters voter[], int num_voter);  // Function to determine the election winner
void reg_election(struct Member members[], int *num_members);  // Function for election registration
void display_members(struct Member members[], int num_members);  // Function to display registered members
void system_clear();  // Function to clear the console screen

// Function to search for candidates by department name
void searchCandidateByDeptName(struct Member members[], int num_members, struct voters voter[], int num_voter) {
	char searchDeptName[MAX_NAME_LENGTH];
	printf("Enter the department name to search for candidates: ");
	scanf("%s", searchDeptName);

	int foundCandidates = 0;

	for (int i = 0; i < num_members; i++) {
    	if (strcasecmp(members[i].department.dept_name, searchDeptName) == 0) {
        	if (foundCandidates == 0) {
            	printf("\nCandidates Found in Department %s:\n", searchDeptName);
        	}

        	printf("Name: %s\n", members[i].name);
        	printf("Registration Number: %s\n", members[i].reg_no);
        	printf("Age: %d\n", members[i].age);
        	printf("Semester Number: %d\n", members[i].semester);
        	printf("Department HOD: %s\n", members[i].department.dept_hod);
        	printf("Department Section: %s\n", members[i].department.dept_section);

        	// Display vote count for this candidate
        	int vote_count = 0;
        	for (int j = 0; j < num_voter; j++) {
            	if (voter[j].voted_for == i) {
                	vote_count++;
            	}
        	}
        	printf("Vote Count: %d\n", vote_count);

        	printf("---------------------------------------\n");

        	// Display votes for this candidate
        	printf("Votes Received:\n");
        	printf("---------------------------------------\n");
        	for (int j = 0; j < num_voter; j++) {
            	if (voter[j].voted_for == i) {
                	printf("Voter Name: %s\n", voter[j].name);
                	printf("Voter Registration Number: %s\n", voter[j].reg_no);
                	printf("---------------------------------------\n");
            	}
        	}

        	foundCandidates++;
    	}
	}

	if (foundCandidates == 0) {
    	printf("\nNo candidates found in department %s.\n", searchDeptName);
	}
	system_clear();
}

// Function to read data from files during program start
void readDataFromFile(struct Member members[], int *num_members, struct voters voter[], int *num_voter) {
	FILE *membersFile = fopen("members.txt", "r");
	FILE *voterFile = fopen("voters.txt", "r");

	if (membersFile == NULL || voterFile == NULL) {
    	perror("Error opening file");
    	exit(1);
	}

	// Read members data
	while (fread(&members[*num_members], sizeof(struct Member), 1, membersFile) == 1) {
    	(*num_members)++;
	}

	// Read voters data
	while (fread(&voter[*num_voter], sizeof(struct voters), 1, voterFile) == 1) {
    	(*num_voter)++;
	}

	fclose(membersFile);
	fclose(voterFile);
}

// Function to create files if they don't exist
void createFilesIfNotExist() {
	FILE *membersFile = fopen("members.txt", "w"); // Open for writing
	FILE *voterFile = fopen("voters.txt", "w");   // Open for writing

	if (membersFile == NULL || voterFile == NULL) {
    	perror("Error creating files");
    	exit(1);
	}

	fclose(membersFile);
	fclose(voterFile);
}


// Function to write data to files before program exit
void writeDataToFile(struct Member members[], int num_members, struct voters voter[], int num_voter) {
	FILE *membersFile = fopen("members.txt", "w");
	FILE *voterFile = fopen("voters.txt", "w");

	if (membersFile == NULL || voterFile == NULL) {
    	printf("Error writing data to file.\n");
    	exit(1);
	}

	// Write members data
	fwrite(members, sizeof(struct Member), num_members, membersFile);

	// Write voters data
	fwrite(voter, sizeof(struct voters), num_voter, voterFile);

	fclose(membersFile);
	fclose(voterFile);
}



//main function
int main() {
	int option;
 	int storeDataChoice;
	struct Member members[MAX_MEMBERS];
	int num_members = 0;
	struct voters voter[MAX_VOTER];
	int num_voter = 0;


 	// Load data from file
	readDataFromFile(members, &num_members, voter, &num_voter);
 	// Create files if they don't exist
	createFilesIfNotExist();


	// Menu
	printf("##################################################\n");
	printf("Welcome to E-VOTING SYSTEM OF COMSATS UNIVERSITY.\n");
	printf("##################################################\n");
	printf("--------------------------------------------------\n");
	do {

    	printf("MENU\n");
    	printf("--------------------------------------------------\n");
    	printf("Select an option from the menu:\n\n");
    	printf("1. Register For Election.\n");
    	printf("2. Display Registered Members.\n");
    	printf("3. Vote For Candidate.\n");
    	printf("4. Display Votes.\n");
    	printf("5. Display Result.\n");
    	printf("6. Elected Student President of COMSATS University Lahore.\n");
    	printf("7. Search Candidate Detail By Department Name.\n");
    	printf("8. Exit.\n");

    	printf("\n--------------------------------------------------\n");

    	printf("Write the corresponding number of your selected option: ");
    	scanf("%d", &option);

    	switch (option) {
        	case 1:
            	reg_election(members, &num_members);
            	break;
        	case 2:
            	display_members(members, num_members);
            	break;
        	case 3:
            	vote(voter, &num_voter, members, num_members);
            	break;
      	case 4:
        	user_login();

        	if ((strcmp(required_username, username1) == 0 || strcmp(required_username, username2) == 0 ||
                  	strcmp(required_username, username3) == 0 || strcmp(required_username, username4) == 0) &&
                  	strcmp(required_password, password) == 0) {
                    	displayVotes(voter, num_voter, members, num_members);
                       	} else {
                            	printf("Invalid username or password\n");
                            	}
        	break;

      	case 5:
          	user_login();
           	if ((strcmp(required_username, username1) == 0 || strcmp(required_username, username2) == 0 ||
                  	strcmp(required_username, username3) == 0 || strcmp(required_username, username4) == 0) &&
                  	strcmp(required_password, password) == 0){
         	display_result(members, num_members, voter, num_voter);}
          	else {
                	printf("Invalid username or password\n");
                            	}
          	break;
      	case 6:
          	user_login();
           	if ((strcmp(required_username, username1) == 0 || strcmp(required_username, username2) == 0 ||
                  	strcmp(required_username, username3) == 0 || strcmp(required_username, username4) == 0) &&
                  	strcmp(required_password, password) == 0){

               	winElection(members, num_members, voter, num_voter);
                	}
        	else {
                	printf("Invalid username or password\n");}

        	break;

         	case 7:
          	searchCandidateByDeptName(members, num_members, voter, num_voter);
              	break;
              	case 8:
                  	// Before closing the program, ask the user if they want to store their data

	printf("Do you want to store the data? (1: Yes, 0: No): ");
	scanf("%d", &storeDataChoice);

	if (storeDataChoice == 1) {
    	// Store data to file
    	writeDataToFile(members, num_members, voter, num_voter);
	}


            	printf("\nExiting the program. Goodbye!\n");
            	exit(0);
        	default:
            	printf("Invalid choice. Please try again.\n");

    	}
	} while (option != 8);



	return 0;
}

void displayTermsAndConditions()
 {
	printf("\n\nTerms and Conditions:\n\n");
	printf("\tThis is your first attempt to run for Student President in this election.\n");
	printf("\tNone of your fees are out-due.\n");
	printf("\tYou have not been given a fine or rustication warning due to violation of any laws in university or in general.\n");
	printf("\tYour enrollment period in COMSATS Lahore is more that 2 years.\n");
	printf("\tYou are aware of and complying to complete your responsibilities as the Student President.\n");
	printf("\tShould you win the elections, you swear to maintain your position as the Student President for exactly 1 year. \n");

}

void print_not_elligible() {

printf("Either your age is less than 19 or\n");
printf("Semester is more than 7 so candidate you cannot be elected.\n");
printf("\nRegistration Failed\n");
system_clear();

}

bool is_valid_reg_no(const char *reg_no) {

	return (strlen(reg_no) == 12 &&
        	isalpha(reg_no[0]) &&
        	isalpha(reg_no[1]) &&
        	isdigit(reg_no[2]) &&
        	isdigit(reg_no[3]) &&
        	reg_no[4] == '-' &&
        	isalpha(reg_no[5]) &&
        	isalpha(reg_no[6]) &&
        	isalpha(reg_no[7]) &&
        	reg_no[8] == '-' &&
        	isdigit(reg_no[9]) &&
        	isdigit(reg_no[10]) &&
        	isdigit(reg_no[11]));

}

void vote(struct voters voter[], int *num_voter, struct Member members[], int num_members) {
	printf("Enter Voter details:\n");

printf("Name: ");
getchar();
fgets(voter[*num_voter].name, sizeof(voter[*num_voter].name), stdin);
voter[*num_voter].name[strcspn(voter[*num_voter].name, "\n")] = '\0';


 	do {
    	printf("Registration Number (####-###-###): ");
    	scanf("%s", voter[*num_voter].reg_no);

    	if (!is_valid_reg_no(voter[*num_voter].reg_no)) {
        	printf("Invalid registration number format. Please enter again.\n");
    	}

	} while (!is_valid_reg_no(voter[*num_voter].reg_no));
  	for(int l=0;l<*num_voter;l++){
	if(strcmp(voter[l].reg_no,voter[*num_voter].reg_no)==0){
    	printf("Already a vote from this registeration number.\n");
    	system_clear();
    	return;
	}

}
  for (int t = 0; t < num_members; t++) {
    	if (strcmp(voter[*num_voter].reg_no, members[t].reg_no) == 0) {
        	printf("A member can't vote for themselves.\n");
        	system_clear();
        	return;
    	}
	}
  printf("Registered Members:\n");
	for (int i = 0; i < num_members; i++) {
    	printf("NUMBER:  %d\n",i+1);
    	printf("   NAME:         	%s\n",  members[i].name);
    	printf("   REG NO:       	%s\n",  members[i].reg_no);
    	printf("   DEPARTMENT NAME:  %s\n",  members[i].department.dept_name);


	}

	int selected_member;
	do {
    	printf("Enter the corresponding number of the member that you want to vote for: ");
    	scanf("%d", &selected_member);
    	if (selected_member<1||selected_member >num_members) {
        	printf("Invalid choice. Please enter a valid number.\n");
    	}
	} while (selected_member < 1 || selected_member > num_members);

	printf("Vote casted successfully for %s. Thank you for voting! \n\n", members[selected_member - 1].name);
	voter[*num_voter].voted_for = selected_member - 1;


	if(*num_voter<MAX_VOTER){
	(*num_voter)++;
	}
	else {
    	printf("Voter limit has Exceeded. Unable to vote! \n\n");
    	return;
	}
system_clear();
}
void displayVotes(struct voters voter[], int num_voter, struct Member members[], int num_members) {
	printf("\nVotes Casted:\n");
	for (int i = 0; i < num_voter; i++) {
    	printf("VOTER :  \n");
    	printf("  %s\n%s\n", voter[i].name, voter[i].reg_no);
    	if (voter[i].voted_for >= 0 && voter[i].voted_for < num_members) {
        	printf("VOTED FOR : \n");
        	printf(" %s\n", members[voter[i].voted_for].name);
        	printf(" %s\n", members[voter[i].voted_for].reg_no);
        	printf(" %s\n", members[voter[i].voted_for].department.dept_name);

    	} else {
        	printf("Invalid vote recorded.\n");
    	}

    	printf("---------------------------------------\n");
	}
system_clear();
}


void display_result(struct Member members[], int num_members, struct voters voter[], int num_voter) {
	int winnerIndex = 0;
	int maxVotes = 0;
	printf("\nElection Result:\n");
	printf("--------------------------------------------------------\n");
	printf("| %-10s | %-20s | %-15s | %-10s | %-s\n", "Member #", "Name", "Reg No", "Votes", "Graph");
	printf("--------------------------------------------------------\n");

	for (int p = 0; p < num_members; p++) {
    	int vote_count = 0;
    	for (int z = 0; z < num_voter; z++) {
        	if (voter[z].voted_for == p) {
            	vote_count++;
        	}
    	}
    	printf("| %-10d | %-20s | %-15s | %-10d | ", p + 1, members[p].name, members[p].reg_no, vote_count);
    	for (int i = 0; i < vote_count; i++) {
        	printf("*");
    	}
    	printf("\n");


	}

	printf("--------------------------------------------------------\n");
system_clear();
}


void user_login(){
 	printf("ONLY ADMIN CAN SEE THE VOTES\n");
           	printf("Enter the username: ");
           	scanf("%s", &required_username);
           	printf("Enter the password: ");
           	scanf("%s", &required_password);
          	system_clear();
}

void reg_election(struct Member members[], int *num_members) {
 	int TermCheckbox;
	printf("Enter member details:\n");

  printf("Name: ");
getchar();
fgets(members[*num_members].name, sizeof(members[*num_members].name), stdin);
members[*num_members].name[strcspn(members[*num_members].name, "\n")] = '\0';

 	do {
    	printf("Registration Number (####-###-###): ");
    	scanf("%s", members[*num_members].reg_no);

    	if (!is_valid_reg_no(members[*num_members].reg_no)) {
        	printf("Invalid registration number format. Please enter again.\n");
    	}

	} while (!is_valid_reg_no(members[*num_members].reg_no));
  	for(int k=0;k<*num_members;k++){
	if(strcasecmp(members[k].reg_no,members[*num_members].reg_no)==0){
    	printf("Already registered from this registeration number.\n");
    	system_clear();
    	return;
	}
   }

	printf("Age: ");
	scanf("%d", &members[*num_members].age);



	printf("Semester: ");
	scanf("%d", &members[*num_members].semester);
	if(members[*num_members].age<19||members[*num_members].semester>=8){
        	print_not_elligible();
        	return;
   }

	printf("Department Name: ");
	scanf("%s", members[*num_members].department.dept_name);
 	for (int i = 0; i < *num_members; i++) {
    	if (strcasecmp(members[i].department.dept_name, members[*num_members].department.dept_name) == 0) {
        	printf("Already registered from this department!\n");
        	system_clear();
        	return 0;
    	}
	}

	printf("Department HOD: ");
	getchar();
fgets(members[*num_members].department.dept_hod, sizeof(members[*num_members].department.dept_hod), stdin);
members[*num_members].department.dept_hod[strcspn(members[*num_members].department.dept_hod, "\n")] = '\0';

	printf("Department Section: ");
	scanf("%s", members[*num_members].department.dept_section);


	displayTermsAndConditions();
	printf("\n\nCheck the box to accept the terms and conditions: (enter 1 to check the box) \n\n");
	scanf("%d", &TermCheckbox);

	if (TermCheckbox == 1)
  	{
	printf("Candidate Registered successfully! Good Luck!! \n\n");}
	if(*num_members<MAX_MEMBERS){
    	(*num_members)++;
	}
	else {
     	printf("\n\nMaximum number of candidates reached. Cannot add more candidates.\n\n\n");
    	return;
	}
system_clear();
}




void display_members(struct Member members[], int num_members) {
	printf("\nRegistered Members:\n");
	for (int i = 0; i < num_members; i++) {
    	printf("Name: %s\n", members[i].name);
    	printf("Registration Number: %s\n", members[i].reg_no);
    	printf("Age: %d\n", members[i].age);
    	printf("Semester: %d\n", members[i].semester);
    	printf("Department Name: %s\n", members[i].department.dept_name);
    	printf("Department HOD: %s\n", members[i].department.dept_hod);
    	printf("Department Section: %s\n", members[i].department.dept_section);
    	printf("---------------------------------------\n");
	}
system_clear();

}
//to clear the screen
void system_clear(){
  #ifdef _WIN32
	system("pause");
#else

	sleep(2);
#endif

#ifdef _WIN32
	system("cls");
#else

	system("clear");
#endif
}
void winElection(struct Member members[], int num_members, struct voters voter[], int num_voter) {
	int winnerIndex = 0;
	int maxVotes = 0;

	for (int u = 0; u < num_members; u++) {
    	int votes_counts = 0;
    	for (int q = 0; q < num_voter; q++) {
        	if (voter[q].voted_for == u) {
            	votes_counts++;
        	}
    	}
    	if (votes_counts > maxVotes) {
        	maxVotes = votes_counts;
        	winnerIndex = u;
    	}
	}
	printf("\nPresident of COMSATS University: %s\n", members[winnerIndex].name);

 	system_clear();

}
