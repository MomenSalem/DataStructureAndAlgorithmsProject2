//Student name : Momen Musa Fuad Salem
//ID : 1200034
//Section : 2
//Doctor : Ahmad Abu Snaina
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>//library for is digit function && strcasecmp
struct Topics
{
    char topic_name[20];
    struct Topics *next_topic;
};
typedef struct Topics Topics;
struct Courses
{
    char course_name[30];
    int course_hours;
    char course_code[20];
    char course_department[20];
    Topics *topicslinkedlist;
    struct Courses *left;
    struct Courses *right;
    int height;
};
typedef struct Courses Courses;
int checkIfDigit(char theword[30]);
int getHeight(Courses *coursestree);
void insertTopic(Topics *topicslinkedlist, Topics *topicnode);
int max(int x, int y);
Courses *singleRotateRight(Courses *coursestree);
Courses *singleRotateLeft(Courses *coursestree);
Courses *doubleRotateRight(Courses *K3);
Courses *doubleRotateLeft(Courses *K3);
Courses *insretAVLNode(Courses *coursestree, Courses *coursenode);
Courses *findCourse(char key[20], Courses *coursesAVL);
int courseCodeExist(Courses *tree, char coursecode[20]);
void printTopics(Courses *course);
void printSpecificCourse(Courses *course);
Topics *findTopic(Courses *course, char topicname[20]);
int isEmpty(Courses *tree);
void printInOrderLexi(Courses *t);
void printInOrderSpecificDep(Courses *avlcourses, char department[20]);
Courses *deleteCourse(Courses *root, char coursecode[20]);
Courses *deleteCoursesStartSpecificLetter(Courses *root, char letter);
Courses *deleteCoursesInDepartment(Courses *root, char department[20]);
void printCoursesInFile(Courses *tree, FILE *outfile);

int departmentexist = 0;//generic data type for printing a massage inside proper functions
int courseexist = 0;
int main()
{
    int filechoice;
    printf("Please enter 1 to read the courses file and build the AVL tree or anything else to exist :-\n");
    scanf("%d",&filechoice);
    if(filechoice == 1)
    {
        FILE *opencoursesfile;
        opencoursesfile = fopen("courses.txt", "r");
        if(opencoursesfile == NULL)//if the file name is false exit the system and print message
        {
            printf("Sorry, The courses file doesn't exist(must be courses.txt)");
            return 0;
        }
        Courses *courses_AVLT = NULL;
        char line[200];
        char *token = NULL;
        while(fgets(line, 200, opencoursesfile) != NULL)
        {
            if(line[strlen(line) - 1] == 10)//condition important for comparing in functions (The last thing in the course file line is to destination it is String so if i don't clear the new line character the compare operation well be false)
                line[strlen(line) - 1] = '\0';
            Courses *coursenode;
            coursenode = (Courses *)malloc(sizeof(Courses));
            Topics *topicslinkedlist;
            topicslinkedlist = (Topics *)malloc(sizeof(Topics));
            topicslinkedlist->next_topic = NULL;
            coursenode->topicslinkedlist = topicslinkedlist;
            token = strtok(line, ":");
            strcpy(coursenode->course_name, token);
            token = strtok(NULL, "#");
            coursenode->course_hours = atoi(token);
            token = strtok(NULL, "#");
            strcpy(coursenode->course_code, token);
            token = strtok(NULL, "/");
            strcpy(coursenode->course_department, token);
            token = strtok(NULL, ",");
            if(token == NULL)
            {
                printf("Sorry, we cann't add the course code = %s because there is no topics in it (must be greater then or equal 1)\n",coursenode->course_code);
            }
            else
            {
                while(token != NULL)
                {
                    Topics *topicnode;
                    topicnode = (Topics *)malloc(sizeof(Topics));
                    strcpy(topicnode->topic_name, token);
                    topicnode->next_topic = NULL;
                    insertTopic(coursenode->topicslinkedlist, topicnode);
                    token = strtok(NULL, ",");
                }
                int checkcode = courseCodeExist(courses_AVLT, coursenode->course_code);
                if(!checkcode)
                    courses_AVLT = insretAVLNode(courses_AVLT, coursenode);
                else
                    printf("Sorry, The course code %s is  exist and you cant repeat it !\n", coursenode->course_code);
            }
        }
        fclose(opencoursesfile);
        int menuchoice = 0;
        while(menuchoice != 9)
        {
            printf("Please choose one number of the following list below :\n");
            printf("1. Insert a new course\n2. Find a course (by enter it's course code) and update the information's course\n");
            printf("3. List courses in lexicographic order with their associated information(credit hours, Codes, and topics)\n");
            printf("4. List all topics associated with a given course\n5. List all courses in lexicographic order that belong to the same department\n");
            printf("6. Delete a course\n7. Delete all courses that start with a specific letter\n8. Delete all courses belong to a given department\n");
            printf("9. Save all courses in the file offered_courses\n");
            scanf("%d", &menuchoice);
            if(menuchoice == 1)
            {
                Courses *newcourse;
                newcourse = (Courses *)malloc(sizeof(Courses));
                newcourse->left = newcourse->right = NULL;
                Topics *topiclinkedlistincourse;
                topiclinkedlistincourse = (Topics *)malloc(sizeof(Topics));
                topiclinkedlistincourse->next_topic = NULL;
                newcourse->topicslinkedlist = topiclinkedlistincourse;
                printf("Please enter the name :\n");
                getchar();
                fgets(newcourse->course_name, 30, stdin);
                newcourse->course_name[strlen(newcourse->course_name) - 1] = '\0';
                printf("Please enter the course credit hour :\n");
                char checkhournumber[30];
                scanf("%s", checkhournumber);
                if(!checkIfDigit(checkhournumber))
                {
                    newcourse->course_hours = atoi(checkhournumber);
                    printf("Please enter the course code :\n");
                    getchar();
                    fgets(newcourse->course_code, 20, stdin);
                    newcourse->course_code[strlen(newcourse->course_code) - 1] = '\0';
                    int checkcode = courseCodeExist(courses_AVLT, newcourse->course_code);
                    if(!checkcode)
                    {
                        printf("Please enter the course department :\n");
                        fgets(newcourse->course_department, 20, stdin);
                        newcourse->course_department[strlen(newcourse->course_department) - 1] = '\0';
                        printf("Please enter the topics for the course code = %s (must be at least one topic)\n",newcourse->course_code);
                        int topicchoice = 1;//loop to add topics until the user enter any thing else 1
                        while(topicchoice == 1)
                        {
                            Topics *newtopic;
                            newtopic = (Topics *)malloc(sizeof(Topics));
                            newtopic->next_topic = NULL;
                            printf("Please enter the topic name for course name %s :\n",newcourse->course_name);
                            fgets(newtopic->topic_name, 20, stdin);
                            newtopic->topic_name[strlen(newtopic->topic_name) - 1] = '\0';
                            insertTopic(newcourse->topicslinkedlist, newtopic);
                            printf("Please enter 1 to add more topics or any thing else to exit adding topics for course code = %s :\n", newcourse->course_code);
                            char checkdigit[30];
                            scanf("%s",checkdigit);
                            if(!checkIfDigit(checkdigit))
                                topicchoice = atoi(checkdigit);
                            else
                                topicchoice = 0;//if the user did not enter digit exit the loop by change the value of topicchoice to zero
                            getchar();//get character to enter the name of the next topic
                        }
                        courses_AVLT = insretAVLNode(courses_AVLT, newcourse);
                        printf("The adding course code %s is DONE properly\n", newcourse->course_code);
                    }
                    else
                        printf("Sorry, The course code %s is already exist and you can't repeat it !\n", newcourse->course_code);
                }

            }
            else if(menuchoice == 2)
            {
                if(!isEmpty(courses_AVLT))
                {
                    printf("Please enter the course code to find it :\n");
                    getchar();
                    char coursecode[20];
                    fgets(coursecode, 20, stdin);
                    coursecode[strlen(coursecode) - 1] = '\0';
                    Courses *findcourse;
                    findcourse = findCourse(coursecode, courses_AVLT);
                    if(findcourse != NULL)
                    {
                        printSpecificCourse(findcourse);
                        printf("Enter 1 if you want to update the course name or any digit if you don't want :\n");
                        char checkdigit[30];//character to check if the enter is digit and not any thing else
                        scanf("%s", checkdigit);
                        if(!checkIfDigit(checkdigit))
                        {
                            int chname = atoi(checkdigit);//ch -> mean check the choice
                            if(chname == 1)
                            {
                                printf("Please enter the updated course name for course code %s:\n", findcourse->course_code);
                                getchar();
                                fgets(findcourse->course_name, 30, stdin);
                                findcourse->course_name[strlen(findcourse->course_name) - 1] = '\0';
                            }
                            printf("Enter 1 if you want to update the course credit hours or any digit if you don't want :\n");
                            char checkhour[30];
                            scanf("%s", checkhour);
                            if(!checkIfDigit(checkhour))
                            {
                                int chhour = atoi(checkhour);
                                if(chhour == 1)
                                {
                                    printf("Please enter the updated course credit hour  for course code %s:\n", findcourse->course_code);
                                    char coursehour[30];
                                    scanf("%s", coursehour);
                                    if(!checkIfDigit(coursehour))
                                    {
                                        findcourse->course_hours = atoi(coursehour);
                                    }
                                }
                                printf("Enter 1 if you want to update the course department or any digit if you don't want :\n");
                                char checkdep[30];//character to check if the user did not enter a digit
                                scanf("%s", checkdep);
                                if(!checkIfDigit(checkdep))
                                {
                                    int chdep = atoi(checkdep);
                                    if(chdep == 1)
                                    {
                                        printf("Please enter the updated course department  for course code %s:\n", findcourse->course_code);
                                        getchar();
                                        fgets(findcourse->course_department, 20, stdin);
                                        findcourse->course_department[strlen(findcourse->course_department) - 1] = '\0';
                                    }
                                    printf("Enter 1 if you want to update specific topic or any digit if you don't want :\n");//delete and add check it
                                    char checktopic[30];
                                    scanf("%s", checktopic);
                                    if(!checkIfDigit(checktopic))
                                    {
                                        int chtopic = atoi(checktopic);
                                        while(chtopic == 1)
                                        {
                                            printf("Please enter the topic name to update it  for course code %s:\n", findcourse->course_code);
                                            char topicname[20];
                                            getchar();
                                            fgets(topicname, 20, stdin);
                                            topicname[strlen(topicname) - 1] = '\0';
                                            Topics *findtopic;
                                            findtopic = findTopic(findcourse, topicname);
                                            if(findtopic != NULL)
                                            {
                                                printf("Please Enter the updated topic name :\n");
                                                fgets(findtopic->topic_name, 20, stdin);
                                                findtopic->topic_name[strlen(findtopic->topic_name) - 1] = '\0';
                                            }
                                            else
                                                printf("Sorry, The topic ( %s ) does not exist! \n", topicname);
                                            printf("Enter 1 if you want to update specific topic or any digit if you don't want :\n");
                                            char checktopic1[30];
                                            scanf("%s", checktopic1);
                                            if(!checkIfDigit(checktopic1))
                                                chtopic = atoi(checktopic1);
                                            else
                                                chtopic = 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                        printf("Sorry, The course does not exist in the Tree which code is = %s\n", coursecode);
                }
                else
                    printf("Sorry, The tree is Empty !\n");
            }
            else if(menuchoice == 3)
            {
                if(!isEmpty(courses_AVLT))
                {
                    printf("______________________________________________________________________________________________________________________\n");
                    printf("The courses in lexicographic are (by the course code):\n");
                    printInOrderLexi(courses_AVLT);
                    printf("______________________________________________________________________________________________________________________\n");
                }
                else
                    printf("Sorry, The tree is Empty !\n");
            }
            else if(menuchoice == 4)
            {
                if(!isEmpty(courses_AVLT))
                {
                    printf("Please enter the course code to print it's topic information :\n");
                    char coursecode[30];
                    getchar();
                    fgets(coursecode, 30, stdin);
                    coursecode[strlen(coursecode) - 1] = '\0';
                    Courses *coursefind;
                    coursefind = findCourse(coursecode, courses_AVLT);
                    if(coursefind == NULL)
                        printf("Sorry, The course code ( %s ) does not exist in the Tree\n", coursecode);
                    else
                    {
                        printf("-------------------------------------------------------\n");
                        printf("The topics for course code %s are : \n", coursecode);
                        printTopics(coursefind);
                        printf("-------------------------------------------------------\n");
                    }
                }
                else
                    printf("Sorry, The tree is Empty !\n");

            }
            else if(menuchoice == 5)
            {
                if(!isEmpty(courses_AVLT))
                {
                    printf("Please enter the department to print the courses in it in the lexicographic order :\n");
                    char department[20];
                    getchar();
                    fgets(department, 20, stdin);
                    department[strlen(department) - 1] = '\0';
                    printf("-----------------------------------------------------------------------------\n");
                    departmentexist = 0;//check if the value of the department increment inside the function so the department exist
                    printInOrderSpecificDep(courses_AVLT, department);
                    if(departmentexist == 0)
                        printf("Sorry, The department %s does not exist !!\n", department);
                    printf("-----------------------------------------------------------------------------\n");
                }
                else
                    printf("Sorry, The tree is Empty !\n");
            }
            else if(menuchoice == 6)
            {
                if(!isEmpty(courses_AVLT))
                {
                    printf("Please enter the course code to delete it :\n");
                    getchar();
                    char coursecode[20];
                    fgets(coursecode, 20, stdin);
                    coursecode[strlen(coursecode) - 1] = '\0';//print message if the course does not exist
                    printf("-------------------------------------------------------\n");
                    courseexist = 0;//update the value to check inside the function
                    courses_AVLT = deleteCourse(courses_AVLT, coursecode);//if the course code does not exist print a message inside the function
                    if(courseexist != 0)
                    {
                        printf("The course which code = %s is deleted from the tree\n", coursecode);
                        printf("The Delete process is DONE properly\n");
                    }
                    printf("-------------------------------------------------------\n");
                }
                else
                    printf("Sorry, The tree is Empty !\n");
            }
            else if(menuchoice == 7)
            {
                if(!isEmpty(courses_AVLT))
                {
                    printf("Please enter the first letter (of course code) to delete all courses  start with the letter\n");
                    getchar();
                    char chletter[30];//check if the user enter just one char
                    fgets(chletter, 30, stdin);
                    chletter[strlen(chletter) - 1] = '\0';
                    char letter;
                    letter = chletter[0];
                    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                    courseexist = 0;
                    courses_AVLT = deleteCoursesStartSpecificLetter(courses_AVLT, letter);
                    if(courseexist != 0)
                        printf("The Delete process is DONE properly\n");
                    else
                        printf("Sorry, There is no courses code start with letter %c in the tree !!!\n", letter);
                    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                }
                else
                    printf("Sorry, The tree is Empty !\n");
            }
            else if(menuchoice == 8)
            {
                if(!isEmpty(courses_AVLT))
                {
                    printf("Please enter the department to delete all courses in it :\n");
                    getchar();
                    char department[20];
                    fgets(department, 20, stdin);
                    printf("%d  \n", strlen(department));
                    department[strlen(department) - 1] = '\0';
                    printf("==========================================================\n");
                    departmentexist = 0;
                    courses_AVLT = deleteCoursesInDepartment(courses_AVLT, department);
                    if(departmentexist != 0)
                        printf("The Delete process is DONE properly\n");
                    else
                        printf("Sorry, The department %s does not exist in the tree !!\n", department);
                    printf("==========================================================\n");
                }
                else
                    printf("Sorry, The tree is Empty !\n");

            }
            else if(menuchoice == 9)
            {
                FILE *writecoursesfile;
                writecoursesfile = fopen("offered_courses.txt", "w");
                printCoursesInFile(courses_AVLT, writecoursesfile);
                fclose(writecoursesfile);
                if(isEmpty(courses_AVLT))
                    printf("Sorry, The tree is Empty !\n");
                else
                    printf("The data is saved to the file named(offered_courses.txt)\n");
                printf("Thank You");
            }
            else
            {
                printf("Please You must choose a number from 1 - 9 only !!!\n");
                fflush(stdin);
            }
        }
    }
    else
    {
        printf("Sorry, the courses file did not read properly (You must enter 1)\nThank You");
    }
    return 0;
}
int checkIfDigit(char theword[30])//function to check if the word is digit or no
{
    for(int i = 0 ; i < strlen(theword) ; i++)
    {
        if(!isdigit(theword[i]))
        {
            printf("You Must enter a digit only!\n");
            return 1;
        }
    }
    return 0;
}
int getHeight(Courses *coursestree)//get the height of the tree
{
    if(coursestree == NULL)//if there is no node the height is minus 1
        return -1;
    else
        return coursestree->height;
}
void insertTopic(Topics *topicslinkedlist, Topics *topicnode)//insert the topics in the linked list
{
    if(topicslinkedlist != NULL && topicnode != NULL)
    {
        Topics *temptopic;
        temptopic = topicslinkedlist;
        while(temptopic->next_topic != NULL)
        {
            temptopic = temptopic->next_topic;
        }
        temptopic->next_topic = topicnode;
    }
    else if(topicslinkedlist == NULL)
    {
        printf("Sorry, The Topic linked list didn't exist");
        exit(0);
    }
    else if(topicnode == NULL)
    {
        printf("Sorry, The new topic node didn't exist");
        exit(0);
    }
}
int max(int x, int y)//function to find the maximum number between two numbers
{
    return (x > y) ? x : y;//if x > y true then return x else return y
}
Courses *minCourse(Courses *course)//find min course from the most left root
{
    if(course == NULL)
        return NULL;
    else if(course->left == NULL)
        return course;
    else
        return minCourse(course->left);
}
Courses *singleRotateRight(Courses *coursestree)
{
    Courses *x;
    x = coursestree->left;
    coursestree->left = x->right;
    x->right = coursestree;
    coursestree->height = 1 + max(getHeight(coursestree->right), getHeight(coursestree->left));
    x->height = 1 + max(getHeight(coursestree), getHeight(x->left));
    return x;
}
Courses *singleRotateLeft(Courses *coursestree)
{
    Courses *y;
    y = coursestree->right;
    coursestree->right  = y->left;
    y->left = coursestree;
    coursestree->height = 1 + max(getHeight(coursestree->left), getHeight(coursestree->right));
    y->height = 1 + max(getHeight(coursestree), getHeight(y->right));
    return y;
}
Courses *doubleRotateRight(Courses *K3)
{
    K3->right  = singleRotateRight(K3->right);
    return singleRotateLeft(K3);
}
Courses *doubleRotateLeft(Courses *K3)
{
    K3->left  = singleRotateLeft(K3->left);
    return singleRotateRight(K3);
}
Courses *insretAVLNode(Courses *coursestree, Courses *coursenode)//inserts nodes to the tree
{
    if(coursestree == NULL)
    {
        coursestree = (Courses *)malloc(sizeof(Courses));
        if(coursestree == NULL)
            printf( "Out of space!!!" );
        else
        {
            coursestree = coursenode;
            coursestree->height = 0;
            coursestree->left = coursestree->right = NULL;
        }

    }
    else if(strcmp(coursenode->course_code,coursestree->course_code) < 0)
    {
        coursestree->left = insretAVLNode(coursestree->left, coursenode);
        if(getHeight(coursestree->left) - getHeight(coursestree->right) >= 2)
        {
            if(strcmp(coursenode->course_code,coursestree->left->course_code) < 0)
                coursestree = singleRotateRight(coursestree);
            else
                coursestree = doubleRotateLeft(coursestree);
        }

    }
    else if(strcmp(coursenode->course_code,coursestree->course_code) > 0)
    {
        coursestree->right = insretAVLNode(coursestree->right, coursenode);
        if(getHeight(coursestree->right) - getHeight(coursestree->left) >= 2)
        {
            if(strcmp(coursenode->course_code,coursestree->right->course_code) > 0)
                coursestree = singleRotateLeft(coursestree);
            else
                coursestree = doubleRotateRight(coursestree);
        }
    }
    coursestree->height = 1 + max(getHeight(coursestree->right), getHeight(coursestree->left));//update the height for the node
    return coursestree;
}
Courses *findCourse(char key[20], Courses *coursesAVL)//find the course by the course code
{
    if(coursesAVL == NULL)
        return NULL;
    if(strcmp(key, coursesAVL->course_code) < 0)
        return findCourse(key, coursesAVL->left);
    else if(strcmp(key, coursesAVL->course_code) > 0)
        return findCourse(key, coursesAVL->right);
    else
        return coursesAVL;
}
int courseCodeExist(Courses *tree, char coursecode[20])//function to check if the code exist in the tree(the course code must be unique
{
    Courses *temp;
    temp = findCourse(coursecode, tree);
    if(temp == NULL)//if the course does not exist return 0 else return 1
        return 0;
    return 1;
}
void printTopics(Courses *course)//function to print topics
{
    Topics *topic;
    topic = course->topicslinkedlist->next_topic;
    if(topic == NULL)
        printf("Sorry, There is no topic for course code = %s !!!!!",course->course_code);
    while(topic != NULL)
    {
        printf("%s   ", topic->topic_name);
        topic = topic->next_topic;
    }
    printf("\n");
}
void printSpecificCourse(Courses *course)//function to print specific course
{
    if(course == NULL)
    {
        printf("Sorry, The course code = %s does not exist", course->course_code);
        return ;
    }
    else
    {
        printf("The course name is : %s \t Credit hours : %d \t Course code : %s \t Course department : %s\n", course->course_name, course->course_hours, course->course_code, course->course_department);
        printf("The topics are :- ");
        printTopics(course);
    }
}
Topics *findTopic(Courses *course, char topicname[20])//function to find a topic in a course for update the topic
{
    Topics *topic;
    topic = course->topicslinkedlist->next_topic;
    while(topic != NULL)
    {
        if(strcmp(topicname, topic->topic_name) == 0)
            return topic;
        topic = topic->next_topic;
    }
    return NULL;
}
int isEmpty(Courses *tree)//function to check if the tree is empty
{
    if(tree == NULL)//if there is no node return 1 (the tree is empty) else return 0
        return 1;
    return 0;
}
void printInOrderLexi(Courses *avlcourses)//print in order
{
    if(avlcourses!= NULL)
    {
        printInOrderLexi(avlcourses->left);
        printf("The Course code is : %s \t Credit hours : %d\n", avlcourses->course_code, avlcourses->course_hours);
        printf("The topics are :- ");
        printTopics(avlcourses);
        printf("*******************\n");
        printInOrderLexi(avlcourses->right);
    }
}
void printInOrderSpecificDep(Courses *avlcourses, char department[20])//function to print courses in a given department
{
    if(avlcourses!= NULL)
    {
        printInOrderSpecificDep(avlcourses->left, department);
        if(strcasecmp(avlcourses->course_department, department) == 0)
        {
            printf("The course code : %s \t The course name : %s \t The credit hours : %d \nThe topics are : ",avlcourses->course_code, avlcourses->course_name, avlcourses->course_hours);
            printTopics(avlcourses);
            printf("###################\n");//to separate between the courses
            departmentexist++;
        }
        printInOrderSpecificDep(avlcourses->right, department);
    }
}
Courses *deleteCourse(Courses *root, char coursecode[20])//function to delete a course
{
    if(root == NULL)
    {
        printf("Sorry, The course code %s does not exist in the tree !!!\n", coursecode);
        return root;
    }
    if(strcmp(coursecode, root->course_code) < 0)
        root->left = deleteCourse(root->left, coursecode);
    else if(strcmp(coursecode, root->course_code) > 0)
        root->right = deleteCourse(root->right, coursecode);
    else//the needed node is the root so we want to delete it
    {
        courseexist++;
        if(root->left == NULL)
        {
            Courses *temp = root->right;
            free(root);
            return temp;
        }
        else if(root->right == NULL)
        {
            Courses *temp = root->left;
            free(root);
            return temp;
        }
        Courses *temp = minCourse(root->right);
        strcpy(root->course_code, temp->course_code);
        root->right = deleteCourse(root->right, temp->course_code);
    }
    if(root == NULL)
        return root;
    root->height = max(getHeight(root->left), getHeight(root->right));//check the balance after deleting process
    int balance = getHeight(root);
    if(balance > 1 && getHeight(root->left) >= 0)
        return singleRotateRight(root);
    if(balance > 1 && getHeight(root->left) < 0)
    {
        return doubleRotateLeft(root);//the same as single rotate left then single rotate right

    }
    if(balance < -1 && getHeight(root->right) <= 0)
        return singleRotateLeft(root);
    if(balance < -1 && getHeight(root->right) > 0)
    {
        return doubleRotateRight(root);//the same as single rotate right then single rotate left

    }
    return root;
}
Courses *deleteCoursesStartSpecificLetter(Courses *root, char letter)//function to delete courses that start with a letter
{
    if(root != NULL)
    {
        root->left = deleteCoursesStartSpecificLetter(root->left, letter);//check all the nodes in the tree if the first letter of it is a specific letter then delete it and check to the last node in the tree
        root->right = deleteCoursesStartSpecificLetter(root->right, letter);
        if(root->course_code[0] == letter)
        {
            courseexist++;
            printf("Delete course code (%s) from the tree\n",root->course_code);
            root = deleteCourse(root, root->course_code);
            root = deleteCoursesStartSpecificLetter(root, letter);//after delete the node call the function again because maybe the tree changed completely
        }
    }
    return root;
}
Courses *deleteCoursesInDepartment(Courses *root, char department[20])//function to delete all courses in a specific department
{
    if(root != NULL)
    {
        root->left = deleteCoursesInDepartment(root->left, department);//check all nodes in the tree
        root->right = deleteCoursesInDepartment(root->right, department);
        if(strcasecmp(root->course_department, department) == 0)
        {
            departmentexist++;
            printf("Delete course code (%s) in department %s from the tree\n", root->course_code, root->course_department);
            root = deleteCourse(root, root->course_code);
            root = deleteCoursesInDepartment(root, department);
        }
    }
    return root;
}
void printCoursesInFile(Courses *tree, FILE *outfile)//function for printing all courses in-order in a file
{
    if(tree!= NULL)
    {
        printCoursesInFile(tree->left, outfile); //print in order(left->root->right) = (small->medium->big) so print the tree in lexicographic
        fprintf(outfile, "%s:", tree->course_name);//print in file like the reading one
        fprintf(outfile, "%d#", tree->course_hours);
        fprintf(outfile, "%s#", tree->course_code);
        fprintf(outfile, "%s/", tree->course_department);
        Topics *topic;
        topic = tree->topicslinkedlist->next_topic;
        while(topic != NULL)//print topic
        {
            fprintf(outfile, "%s", topic->topic_name);
            if(topic->next_topic != NULL)
                fprintf(outfile, ", ");
            topic = topic->next_topic;
        }
        fprintf(outfile, "\n");
        printCoursesInFile(tree->right, outfile);
    }
}
