#include	<iostream>
#include	<cstdlib>
#include	<cstdio>
#include    <fstream>
#include    <string>
#include	"List.h"
#include    "LibStudent.h"
#include    "LibBook.h"
#include	<iomanip>


using namespace std;

bool ReadFile(string, List*);
bool DeleteRecord(List*, char*);
bool Display(List*, int, int);
bool InsertBook(string, List*);
bool SearchStudent(List*, char* id, LibStudent&);
bool computeAndDisplayStatistics(List*);
bool printStuWithSameBook(List*, char*);
bool displayWarnedStudent(List*, List*, List*);
int menu();

bool printStudent_Booklist(List*, int, int, int);
bool redundant(LibStudent, LibStudent);
int calculateJulianDay(int, int, int);
int check_overdue(LibBook);

struct Stats
{
	char course[3];
	int total_students;
	int total_borrowed;
	int total_overdue;
	double total_fine;
};


int main() {
	menu();

	cout << "\n\n";
	system("pause");

	return 0;
}

bool ReadFile(string filename, List* list)
{
	ifstream infile(filename);
	string line;
	char s[50], data[50];
	LibStudent student, temp_student;
	LibBook book;
	bool skip = false;

	if (infile)
	{
		while (!infile.eof())
		{
			skip = false;
			infile.getline(s, 14);
			infile.clear();
			infile.getline(student.id, sizeof(student.id));

			infile.getline(s, 8);
			infile.clear();
			infile.getline(student.name, sizeof(student.name));

			infile.getline(s, 10);
			infile.clear();
			infile.getline(student.course, sizeof(student.course));

			infile.getline(s, 16);
			infile.clear();
			infile.getline(student.phone_no, sizeof(student.phone_no));

			getline(infile, line);
			getline(infile, line);

			for (int i = 1; i <= list->count; i++)
			{
				list->get(i, temp_student);
				if (redundant(student, temp_student))
				{
					skip = true;
					break;
				}
			}

			if (skip == true)
				continue;

			list->insert(student);
		}

		cout << "\n\nREAD FILE\n\n";

		cout << endl << list->count << " Records have been successfully read\n\n";

		infile.close();
		return true;
	}
	else
	{
		cout << "Couldn't open the file." << endl;
		return false;
	}
}

bool redundant(LibStudent student, LibStudent node)
{
	int condition = 0;
	if (strcmp(node.id, student.id) == 0)
		condition++;
	if (strcmp(node.name, student.name) == 0)
		condition++;
	if (strcmp(node.course, student.course) == 0)
		condition++;
	if (strcmp(node.phone_no, student.phone_no) == 0)
		condition++;
	return (condition == 4);
}

bool printStuWithSameBook(List* list, char* callNum)
{
	LibStudent student;
	int borrowed = 0;

	//allocate dynamic memory for an array to integer pointer student_position to store which student borrowed the book
	int* student_position = new int[list->count];
	int* book_pos = new int[list->count];
	int index = 0;


	for (int i = 1; i <= list->count; i++) //loop through each Node
	{
		list->get(i, student);
		for (int j = 0; j < student.totalbook; j++) //loop through each student borrowed books(max 15)
		{
			if (strcmp(student.book[j].callNum, callNum) == 0)
			{
				borrowed++;
				book_pos[index] = j;
				student_position[index] = i;
				index++;
				break;
			}
		}
	}

	if (borrowed > 0)
	{
		cout << "There are " << borrowed << " students that borrow the book with call number " << callNum << " as shown below: \n\n";
		//loop to get the information of which student has borrowed book with callNum
		for (int i = 0; i < borrowed; i++)
		{
			list->get(student_position[i], student); //student_positoin stores which node(student) in a list has borrowed the book
			cout << "Student Id = " << student.id << endl;
			cout << "Name = " << student.name << endl;
			cout << "Course = " << student.course << endl;
			cout << "Phone Number = " << student.phone_no << endl;
			cout << "Borrow Date: "; student.book[book_pos[i]].borrow.print(cout);
			cout << endl;
			cout << "Due Date: "; student.book[book_pos[i]].due.print(cout);
			cout << endl << endl;
		}
	}
	else
		cout << "There is no students that borrow the book with call number " << callNum << endl;

	delete[] student_position;
	delete[] book_pos;

	return (borrowed > 0);
}

bool displayWarnedStudent(List* list, List* type1, List* type2)
{
	Node* cur;
	int day = 29, month = 3, year = 2020;


	if (list->empty())
	{
		cout << "The list is empty.\n";
		return false;
	}

	cur = list->head;

	while (cur != NULL)
	{
		int overdue = 0;
		//check for student borrowed books
		for (int i = 0; i < cur->item.totalbook; i++)
			overdue += check_overdue(cur->item.book[i]);

		if (overdue > 2)
			type1->insert(cur->item);
		else if (cur->item.total_fine > 50.00 && overdue == cur->item.totalbook)
			type2->insert(cur->item);

		cur = cur->next;
	}
	if (type1->count == 0)
		cout << "\nNo student has been warned for type1.\n\n";
	else
	{
		cout << "\nType 1 warned List: \n\n";
		printStudent_Booklist(type1, day, month, year);
	}

	if (type2->count == 0)
		cout << "\nNo student has been warned for type2.\n\n";
	else
	{
		cout << "\nType 2 warned List: \n\n";
		printStudent_Booklist(type2, day, month, year);
	}


	return true;
}

bool printStudent_Booklist(List* list, int day, int month, int year)
{
	if (!list->empty())
	{
		Node* cur = list->head;

		cur = list->head;
		int count = 1;
		while (cur != NULL)
		{
			cout << "\nStudent " << count++;
			cur->item.print(cout);

			int* book_pos = new int[cur->item.totalbook];
			int index = 0;
			//loop to check which book the student has overdue
			for (int i = 0; i < cur->item.totalbook; i++)
			{
				if (cur->item.book[i].due.year == year)
				{
					if (cur->item.book[i].due.month < month || (cur->item.book[i].due.month == month && cur->item.book[i].due.day <= day))
					{
						// Check for overdue books whose due date is in the past
						if (month - cur->item.book[i].due.month >= 1 || day - cur->item.book[i].due.day >= 10)
							book_pos[index++] = i;
					}
				}
				else if (cur->item.book[i].due.year < 2020)
					book_pos[index++] = i;
			}
			cout << "\nBook list that overdued:\n";
			for (int i = 0; i < index; i++)
			{
				cout << "\nBook " << i + 1 << endl;
				cur->item.book[book_pos[i]].print(cout);
			}

			delete[] book_pos;
			cur = cur->next;
		}
		return true;
	}
	cout << "The list is empty, couldn't print the list.\n";
	return false;
}




int menu()
{
	List studentList;
	List warnedList1;
	List warnedList2;
	while (true) {
		int choice;
		cout << "Menu\n\n";
		cout << "1. Read file" << endl;
		cout << "2. Delete record" << endl;
		cout << "3. Search student" << endl;
		cout << "4. Insert book" << endl;
		cout << "5. Display output" << endl;
		cout << "6. Compute and Display Statistics" << endl;
		cout << "7. Student with Same Book" << endl;
		cout << "8. Display Warned Student" << endl;
		cout << "9. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;


		switch (choice)
		{
		case 1:
		{
			string filename;
			filename = "student.txt";
			ReadFile(filename, &studentList);
			break;
		}

		case 2:
		{
			char id[10];
			cout << "Enter the student ID to delete: ";
			cin >> id;
			if (DeleteRecord(&studentList, id))
			{
				cout << "\nStudent record deleted successfully.\n" << endl;
			}
			else
			{
				cout << "\nStudent with ID " << id << " not found in the list.\n" << endl;

			}
			break;
		}
		case 3:
		{
			char id[10];
			cout << "Enter the student ID to search: ";
			cin >> id;
			LibStudent student;
			SearchStudent(&studentList, id, student);

			break;

		}
		case 4:
		{
			string filename;
			filename = "book.txt";
			InsertBook(filename, &studentList);
			break;


		}
		case 5:
		{
			int source, detail;

			cout << "\n\nDISPLAY OUTPUT\n\n";
			do {
				cout << "\nWhere do you want to display the output(1 - File / 2 - Screen): ";
				cin >> source;
			} while (source != 1 && source != 2);

			do {
				cout << "\nDo you want to display book list for every student (1 - YES / 2 - NO): ";
				cin >> detail;
			} while (detail != 1 && detail != 2);

			Display(&studentList, source, detail);
			cout << endl;
			break;
		}
		case 6:
		{
			computeAndDisplayStatistics(&studentList);
			break;
		}
		case 7:
		{
			char callNum[20];
			cout << "Enter the call number of the book: ";
			cin >> callNum;
			printStuWithSameBook(&studentList, callNum);
			break;

		}
		case 8:
		{
			displayWarnedStudent(&studentList, &warnedList1, &warnedList2);
			break;
		}
		case 9:
		{
			cout << "\n\nExiting the program." << endl;
			return 0;

		}
		default:
			cout << "Invalid choice! Please enter a valid choice (1-9)." << endl;
			break;


		}
	}

	return 0;

}

bool DeleteRecord(List* list, char* id)
{
	LibStudent tmp;
	Node* cur;
	int i = 1;

	if (list->empty()) // Check if the list is empty
	{
		cout << endl << "The list is empty" << endl;
		return false;
	}
	cur = list->head; 
	while (cur != NULL)
	{
		//Check if the first node has the matching ID.
		if (strcmp(cur->item.id, id) == 0)
		{
			list->remove(i); //if found,remove it
			return true;
		}
		cur = cur->next; //Update node to next pointer to skip current node
		i++;
	}
	return false;
}

bool SearchStudent(List *list, char *id, LibStudent& stu) {

	Node* cur;
	cur = list->head;

	if (list->empty()) {
		cout << "The list is empty.\n";
		return false;
	}

	// Traverse the linked list to find the student with the matching ID
	while (cur != NULL) {
		if (strcmp(cur->item.id, id) == 0) {
			// Student with matching ID found, update stu and print student information
			stu = cur->item;
			stu.print(cout); // Print student information using the print function of LibStudent
			return true;
		}
		cur = cur->next;
	}

	// Student with the given ID not found
	cout << "\nThe student is not in the list.\n\n";
	return false;
}

bool InsertBook(string filename, List* list) {

	LibStudent student;
	LibBook book;
	Node* cur;
	ifstream bookinfo;
	Date currentdate;
	char authorname[100];

	currentdate.day = 29;
	currentdate.month = 3;
	currentdate.year = 2020;

	bookinfo.open(filename);

	if (list->empty()) {
		cout << "The list is empty.\n" << "Please store the student information in the list first.\n";

		return false;
	}

	if (!bookinfo) {
		cout << "The file (" << filename << ") is not exist.\n";

		return false;
	}

	if (bookinfo.is_open()) {
		while (!bookinfo.eof()) {
			bookinfo >> student.id;
			bookinfo >> authorname;
			// Replace underscores with spaces
			for (int i = 0; i < strlen(authorname); i++) {
				if (authorname[i] == '_') {
					authorname[i] = ' ';
				}
			}
			//check the number of author
			char* startPos = authorname;
			char* pos = strchr(authorname, '/');
			int partCount = 0;
			while (pos != nullptr && partCount < 9) {
				// Allocate memory and copy the split part to char*			
				int partLength = pos - startPos;
				book.author[partCount] = new char[partLength + 1];
				strncpy(book.author[partCount], startPos, partLength);
				book.author[partCount][partLength] = '\0'; // Null-terminate the copied string
				// Update the start position for the next search
				startPos = pos + 1;
				// Find the next occurrence of the forward slash
				pos = strchr(startPos, '/');
				partCount++;
			}
			// Save the last part after the last forward slash (if any)
			if (partCount < 10) {
				book.author[partCount] = new char[strlen(startPos) + 1];
				strcpy(book.author[partCount], startPos);
			}
			bookinfo >> book.title >> book.publisher;
			// Replace underscores with spaces
			for (int i = 0; i < strlen(book.title); i++) {
				if (book.title[i] == '_') {
					book.title[i] = ' ';
				}
			}
			// Replace underscores with spaces
			for (int i = 0; i < strlen(book.publisher); i++) {
				if (book.publisher[i] == '_') {
					book.publisher[i] = ' ';
				}
			}
			bookinfo >> book.ISBN >> book.yearPublished >> book.callNum;
			//pass through the '/' symbol between date in the txt file
			char symbol;
			bookinfo >> book.borrow.day >> symbol >> book.borrow.month >> symbol >> book.borrow.year;
			bookinfo >> book.due.day >> symbol >> book.due.month >> symbol >> book.due.year;
			//calculate the fine
			int dueDays = calculateJulianDay(currentdate.year, currentdate.month, currentdate.day)
				- calculateJulianDay(book.due.year, book.due.month, book.due.day);
			book.fine = dueDays > 0 ? dueDays * 0.50 : 0.0;
			int listCount = 1; // Used to count the position of list
			cur = list->head;
			// Start the searching from the head of the list
			while (cur != NULL) {
				if (strcmp(cur->item.id, student.id) == 0) {
					// Student with matching ID found, insert book into the student list					
					student = cur->item; //Copy the student information into student
					student.book[cur->item.totalbook] = book;
					student.totalbook = ++cur->item.totalbook;
					student.calculateTotalFine();
					// Update student record
					if (!list->set(listCount, student)) {
						cout << "Failed to insert new record to " << student.id << endl;

						return false;
					}
				}
				listCount++;
				cur = cur->next;
			}
		}
		bookinfo.close();
		cout << "\n\nINSERT BOOK\n\n";
		cout << "\nBooks inserted successfully to student list.\n\n";
	}

	return true;
}

// Function to calculate the Julian day
int calculateJulianDay(int year, int month, int day) {
	int a = (14 - month) / 12;
	int y = year + 4800 - a;
	int m = month + 12 * a - 3;

	return day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
}

bool Display(List* list, int source, int detail)
{
	int i = 0;
	if (detail > 2 || detail < 1 || source > 2 || source < 1) return false;
	Node* cur = list->head;
	ofstream outfile;

	if (list->empty())
	{
		cout << "The list is empty. " << endl;
		return false;
	}

	ostream* out = &outfile;

	if (source == 1) {
		if (detail == 1)
			outfile.open("student_booklist.txt");
		else
			outfile.open("student_info.txt");
	}

	if (source == 2)        //If source is 2, display on screen
		out = &cout;

	while (cur != NULL) {    //While not at the end of the list of students
		*out << "\nSTUDENT " << ++i;
		cur->item.print(*out);

		if (detail == 1) {
			*out << "\nBOOK LIST:\n";
			for (int j = 0; j < cur->item.totalbook; j++) {  //Loops through the array of boooks and print
				*out << "\nBook " << j + 1 << "\n";
				cur->item.book[j].print(*out);
			}
		}
		cur = cur->next;
		*out << "\n**************************\n";
	}

	if (source == 1) {
		if (detail == 1) {
			outfile.close();
			cout << "\n\n\nSuccessfully display output to student_booklist.txt\n\n\n";
		}
		else {
			outfile.close();
			cout << "\n\n\nSuccessfully display output to student_info.txt\n\n\n";
		}
	}

	cout << "\nSuccessfully display output\n\n\n";
	return true;
}

bool computeAndDisplayStatistics(List* list) {
	if (list->empty())
		return 0;
	Node* cur = list->head;
	Stats stat[15];         //Array for the attributes of the table for different courses
	int count = 0;          //The number of rows
	bool found = false;     //Student's course is in the stat array

	cout << endl;
	cout << setw(9) << left << "Course" << setw(20) << "Number of Students" << setw(23) << "Total Books Borrowed" << setw(23);
	cout << "Total Overdue Books" << setw(23) << "Total Overdue Fine(RM)" << endl;

	while (cur != NULL) {
		if (count != 0)     //Check if student's course is in the stat array
			for (int i = 0; i < count; i++)
				if (strcmp(stat[i].course, cur->item.course) == 0)
					found = true;
		if (count == 0 || !found) {      //If student's course is not in the stat array
			strcpy(stat[count].course, cur->item.course);
			stat[count].total_students = 1;
			stat[count].total_borrowed = cur->item.totalbook;
			stat[count].total_overdue = 0;
			for (int i = 0; i < cur->item.totalbook; i++)
				stat[count].total_overdue += check_overdue(cur->item.book[i]);
			stat[count].total_fine = cur->item.total_fine;
			count++;
		}
		else {                                   //Student's course is in the stat array
			for (int i = 0; i < count; i++) {
				if (strcmp(stat[i].course, cur->item.course) == 0) {
					stat[i].total_students++;
					stat[i].total_borrowed += cur->item.totalbook;
					for (int j = 0; j < cur->item.totalbook; j++)
						stat[i].total_overdue += check_overdue(cur->item.book[j]);
					stat[i].total_fine += cur->item.total_fine;
				}
			}
		}
		found = false;
		cur = cur->next;
	}
	for (int i = 0; i < count; i++) {        //Print out the table
		cout << setw(9) << left << stat[i].course << setw(20) << stat[i].total_students << setw(23) << stat[i].total_borrowed << setw(23);
		cout << stat[i].total_overdue << setw(23) << stat[i].total_fine << endl;
	}
	cout << endl;
	return 1;
}

int check_overdue(LibBook book) {        //If book is overdue, returns 1, otherwise, returns 0
	int day = 29, month = 3, year = 2020;
	if (book.due.year == year)
	{
		if (book.due.month < month || (book.due.month == month && book.due.day <= day))
		{
			// Check for overdue books whose due date is in the past
			if (month - book.due.month >= 1 || day - book.due.day >= 1)
				return 1;
		}
	}
	else if (book.due.year < 2020)
		return 1;
	return 0;
}




