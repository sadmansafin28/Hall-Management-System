#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ====================== DATA STRUCTURE ======================
typedef struct Student {
    char name[100];
    char uniID[20];
    char dob[15];
    char fatherName[100];
    char fatherContact[20];
    char motherName[100];
    char motherContact[20];
    char studentPhone[20];
    char bloodGroup[6];
    int roomNo;
    char bed;
    double due;
    struct Student* next;
} Student;

Student* activeHead = NULL;
Student* archiveHead = NULL;

// ====================== FUNCTION DECLARATIONS ======================
void centerText(const char* text);
int isValidRoom(int room);
int isOccupied(int room, char bed);
Student* findStudent(Student* head, const char* id);
int isValidBloodGroup(const char* bg);
void showVacantRooms();
void addStudent();
void updateStudent();
void removeStudent();
void printStudentList(Student* listHead, const char* status);
void searchByID();
void generateCSV();
void printAllData();
void showMainMenu();
int login();

// ====================== HELPER FUNCTIONS ======================
void centerText(const char* text) {
    int width = 80;
    int pad = (width - strlen(text)) / 2;
    printf("%*s%s\n", pad, "", text);
}

int isValidRoom(int room) {
    int floor = room / 100;
    int rnum = room % 100;
    return (floor >= 1 && floor <= 8) && (rnum >= 1 && rnum <= 6);
}

int isOccupied(int room, char bed) {
    Student* temp = activeHead;
    while (temp) {
        if (temp->roomNo == room && toupper(temp->bed) == toupper(bed))
            return 1;
        temp = temp->next;
    }
    return 0;
}

Student* findStudent(Student* head, const char* id) {
    Student* temp = head;
    while (temp) {
        if (strcmp(temp->uniID, id) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

int isValidBloodGroup(const char* bg) {
    const char* valid[] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-", NULL};
    for (int i = 0; valid[i] != NULL; i++) {
        if (strcmp(bg, valid[i]) == 0) return 1;
    }
    return 0;
}

// ====================== VACANT ROOMS ======================
void showVacantRooms() {
    printf("\n=== VACANT ROOMS AND BEDS ===\n\n");
    for (int floor = 1; floor <= 8; ++floor) {
        printf("Floor %d:\n", floor);
        for (int r = 1; r <= 6; ++r) {
            int room = floor * 100 + r;
            printf("Room %03d: ", room);
            int vacant = 0;
            for (char b = 'A'; b <= 'H'; ++b) {
                if (!isOccupied(room, b)) {
                    printf("%c ", b);
                    vacant = 1;
                }
            }
            if (!vacant) printf("(Full)");
            printf("\n");
        }
        printf("\n");
    }
    printf("=============================\n");
}

// Helper to safely read a line with fgets (removes newline)
void safeRead(char* buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

// ====================== ADD STUDENT ======================
void addStudent() {
    system("cls");
    printf("========================================\n");
    centerText("ADD STUDENT DETAILS");
    printf("========================================\n");

    Student* newS = (Student*)malloc(sizeof(Student));
    if (!newS) {
        printf("Memory allocation failed!\n");
        getchar();
        return;
    }
    newS->next = NULL;

    printf("\nStudent Name          : ");
    safeRead(newS->name, sizeof(newS->name));

    printf("University ID         : ");
    safeRead(newS->uniID, sizeof(newS->uniID));

    printf("Date of Birth (DD-MM-YYYY): ");
    safeRead(newS->dob, sizeof(newS->dob));

    printf("Blood Group (e.g. A+): ");
    safeRead(newS->bloodGroup, sizeof(newS->bloodGroup));
    if (!isValidBloodGroup(newS->bloodGroup)) {
        printf("WARNING: Invalid blood group! Set to N/A\n");
        strcpy(newS->bloodGroup, "N/A");
    }

    printf("Father's Name         : ");
    safeRead(newS->fatherName, sizeof(newS->fatherName));

    printf("Father's Contact      : ");
    safeRead(newS->fatherContact, sizeof(newS->fatherContact));

    printf("Mother's Name         : ");
    safeRead(newS->motherName, sizeof(newS->motherName));

    printf("Mother's Contact      : ");
    safeRead(newS->motherContact, sizeof(newS->motherContact));

    printf("Student's Phone       : ");
    safeRead(newS->studentPhone, sizeof(newS->studentPhone));

    printf("\nPress Enter to show vacant rooms...");
    getchar();
    showVacantRooms();

    printf("\nEnter Room Number (e.g. 102): ");
    scanf("%d", &newS->roomNo);
    getchar();                    // consume newline after scanf

    printf("Enter Bed (A-H): ");
    scanf(" %c", &newS->bed);
    getchar();
    newS->bed = toupper(newS->bed);

    if (!isValidRoom(newS->roomNo) || newS->bed < 'A' || newS->bed > 'H' || isOccupied(newS->roomNo, newS->bed)) {
        printf("\nERROR: Invalid or occupied room/bed!\n");
        free(newS);
        getchar();
        return;
    }

    if (findStudent(activeHead, newS->uniID)) {
        printf("\nERROR: Student with this ID already exists!\n");
        free(newS);
        getchar();
        return;
    }

    newS->due = 17000.0;
    newS->next = activeHead;
    activeHead = newS;

    printf("\nSUCCESS: Student added successfully!\n");
    printf("Initial Due Amount: 17000 TK\n");
    printf("Press any key to continue...");
    getchar();
}

// ====================== REMOVE STUDENT ======================
void removeStudent() {
    system("cls");
    printf("========================================\n");
    centerText("REMOVE STUDENT");
    printf("========================================\n");

    char id[20];
    printf("\nEnter University ID to remove: ");
    scanf("%s", id);
    getchar();

    Student* curr = activeHead;
    Student* prev = NULL;

    while (curr) {
        if (strcmp(curr->uniID, id) == 0) {
            if (prev == NULL)
                activeHead = curr->next;
            else
                prev->next = curr->next;

            curr->next = archiveHead;
            archiveHead = curr;

            printf("\nSUCCESS: Student moved to archive.\n");
            printf("Name: %s | Room: %d-%c\n", curr->name, curr->roomNo, curr->bed);
            getchar();
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    printf("\nERROR: Student with ID %s not found!\n", id);
    getchar();
}

// ====================== UPDATE STUDENT ======================
void updateStudent() {
    system("cls");
    printf("========================================\n");
    centerText("UPDATE STUDENT INFO");
    printf("========================================\n");

    char id[20];
    printf("\nEnter University ID: ");
    scanf("%s", id);
    getchar();

    Student* s = findStudent(activeHead, id);
    if (!s) {
        printf("\nERROR: Student not found!\n");
        getchar();
        return;
    }

    char temp[100];
    printf("\nPress Enter to keep current value.\n\n");

    printf("Name (%s): ", s->name);
    safeRead(temp, sizeof(temp));
    if (strlen(temp) > 0) strcpy(s->name, temp);

    printf("DOB (%s): ", s->dob);
    safeRead(temp, sizeof(temp));
    if (strlen(temp) > 0) strcpy(s->dob, temp);

    printf("Blood Group (%s): ", s->bloodGroup);
    safeRead(temp, sizeof(temp));
    if (strlen(temp) > 0) {
        if (isValidBloodGroup(temp))
            strcpy(s->bloodGroup, temp);
        else
            printf("Invalid blood group! Keeping previous.\n");
    }

    printf("Father Name (%s): ", s->fatherName);
    safeRead(temp, sizeof(temp));
    if (strlen(temp) > 0) strcpy(s->fatherName, temp);

    printf("Father Contact (%s): ", s->fatherContact);
    safeRead(temp, sizeof(temp));
    if (strlen(temp) > 0) strcpy(s->fatherContact, temp);

    printf("Mother Name (%s): ", s->motherName);
    safeRead(temp, sizeof(temp));
    if (strlen(temp) > 0) strcpy(s->motherName, temp);

    printf("Mother Contact (%s): ", s->motherContact);
    safeRead(temp, sizeof(temp));
    if (strlen(temp) > 0) strcpy(s->motherContact, temp);

    printf("Student Phone (%s): ", s->studentPhone);
    safeRead(temp, sizeof(temp));
    if (strlen(temp) > 0) strcpy(s->studentPhone, temp);

    printf("\nSUCCESS: Student information updated!\n");
    getchar();
}

// ====================== OTHER FUNCTIONS (print, search, csv, etc.) ======================
void printStudentList(Student* listHead, const char* status) {
    printf("\n--- %s ---\n\n", status);
    if (!listHead) {
        printf("No students found.\n");
        return;
    }
    Student* temp = listHead;
    while (temp) {
        printf("Name           : %s\n", temp->name);
        printf("University ID  : %s\n", temp->uniID);
        printf("Blood Group    : %s\n", temp->bloodGroup);
        printf("Room           : %d - %c\n", temp->roomNo, temp->bed);
        printf("DOB            : %s\n", temp->dob);
        printf("Father         : %s (%s)\n", temp->fatherName, temp->fatherContact);
        printf("Mother         : %s (%s)\n", temp->motherName, temp->motherContact);
        printf("Student Phone  : %s\n", temp->studentPhone);
        printf("Due Amount     : %.2f TK\n", temp->due);
        printf("----------------------------------------\n");
        temp = temp->next;
    }
}

void searchByID() {
    system("cls");
    printf("========================================\n");
    centerText("SEARCH BY ID");
    printf("========================================\n");

    char id[20];
    printf("Enter University ID: ");
    scanf("%s", id);
    getchar();

    Student* s = findStudent(activeHead, id);
    if (!s) {
        printf("\nERROR: Student not found!\n");
        getchar();
        return;
    }

    printf("\n--- STUDENT DETAILS ---\n\n");
    printf("Name           : %s\n", s->name);
    printf("University ID  : %s\n", s->uniID);
    printf("Blood Group    : %s\n", s->bloodGroup);
    printf("Room           : %d - %c\n", s->roomNo, s->bed);
    printf("DOB            : %s\n", s->dob);
    printf("Father         : %s (%s)\n", s->fatherName, s->fatherContact);
    printf("Mother         : %s (%s)\n", s->motherName, s->motherContact);
    printf("Student Phone  : %s\n", s->studentPhone);
    printf("Due Amount     : %.2f TK\n", s->due);
    printf("----------------------------------------\n");

    printf("\nPress any key...");
    getchar();
}

void generateCSV() {
    system("cls");
    printf("========================================\n");
    centerText("GENERATE CSV REPORT");
    printf("========================================\n");

    FILE* file = fopen("hall_management_data.csv", "w");
    if (!file) {
        printf("ERROR: Cannot create CSV file!\n");
        getchar();
        return;
    }

    fprintf(file, "Status,Name,UniID,BloodGroup,DOB,FatherName,FatherContact,MotherName,MotherContact,StudentPhone,RoomNo,Bed,Due\n");

    for (Student* t = activeHead; t; t = t->next)
        fprintf(file, "Present,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%c,%.2f\n",
                t->name, t->uniID, t->bloodGroup, t->dob,
                t->fatherName, t->fatherContact, t->motherName,
                t->motherContact, t->studentPhone, t->roomNo, t->bed, t->due);

    for (Student* t = archiveHead; t; t = t->next)
        fprintf(file, "Removed,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%c,%.2f\n",
                t->name, t->uniID, t->bloodGroup, t->dob,
                t->fatherName, t->fatherContact, t->motherName,
                t->motherContact, t->studentPhone, t->roomNo, t->bed, t->due);

    fclose(file);
    printf("SUCCESS: CSV file generated -> hall_management_data.csv\n");
    getchar();
}

void printAllData() {
    system("cls");
    printf("========================================\n");
    centerText("ALL STUDENT RECORDS");
    printf("========================================\n");

    printStudentList(activeHead, "PRESENT STUDENTS");
    printStudentList(archiveHead, "REMOVED / ARCHIVED STUDENTS");

    printf("\nPress any key to return...");
    getchar();
}

// ====================== LOGIN ======================
int login() {
    system("cls");
    char user[20], pass[20];

    printf("========================================\n");
    centerText("HALL MANAGEMENT SYSTEM - LOGIN");
    printf("========================================\n\n");

    printf("Username: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);
    getchar();

    if (strcmp(user, "admin") == 0 && strcmp(pass, "admin123") == 0) {
        printf("\nLogin Successful!\n");
        getchar();
        return 1;
    } else {
        printf("\nInvalid credentials!\n");
        getchar();
        return 0;
    }
}

// ====================== MAIN MENU ======================
void showMainMenu() {
    int choice;
    while (1) {
        system("cls");
        printf("========================================\n");
        centerText("HALL MANAGEMENT SYSTEM");
        printf("========================================\n\n");

        printf("1. Add New Student\n");
        printf("2. Update Student Info\n");
        printf("3. Remove Student (to Archive)\n");
        printf("4. Search Student by ID\n");
        printf("5. Show Vacant Rooms\n");
        printf("6. View All Students\n");
        printf("7. Generate CSV Report\n");
        printf("8. Exit\n\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addStudent(); break;
            case 2: updateStudent(); break;
            case 3: removeStudent(); break;
            case 4: searchByID(); break;
            case 5:
                system("cls");
                showVacantRooms();
                printf("\nPress any key...");
                getchar();
                break;
            case 6: printAllData(); break;
            case 7: generateCSV(); break;
            case 8:
                printf("\nThank you for using Hall Management System!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Try again.\n");
                getchar();
        }
    }
}

// ====================== MAIN ======================
int main() {
    if (!login()) {
        printf("Access Denied.\n");
        return 0;
    }

    system("cls");
    printf("\n\n\n\n\n\n\n");
    centerText("WELCOME TO HALL MANAGEMENT SYSTEM");
    printf("\n\n\n\n\n\n\n");
    centerText("Press any key to continue...");
    getchar();

    showMainMenu();
    return 0;
}
