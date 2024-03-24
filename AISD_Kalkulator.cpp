#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>

#define MAXLENGTH 256

class MyString {
private:
	char* buffer;
	int size;

public:
	MyString(const char* string = "") {
		this->size = std::strlen(string);
		buffer = new char[this->size + 1];
		std::strcpy(buffer, string);
	}

	MyString(const MyString& other) {
		this->size = std::strlen(other.buffer);
		buffer = new char[this->size + 1];
		std::strcpy(buffer, other.buffer);
	}

	MyString(char other) {
		this->size = 1;
		buffer = new char[1];
		buffer[0] = other;
	}

	~MyString() {
		if (buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr; // Ustawienie wskaznika na nullptr, aby uniknanie problemow z ponownym zwalnianiem pamieci
		}
	}

	int length() const {
		return this->size;
	}

	void read() {
		char* temp = new char[MAXLENGTH];
		std::cin.getline(temp, MAXLENGTH);
		this->size = std::strlen(temp);

		//for (int i = 0; temp[i] != '.' && temp[i] != '\0'; i++) {
		//	temp[i] = std::toupper(temp[i]);
		//}

		delete[] buffer;
		buffer = new char[this->size + 1];
		std::strcpy(buffer, temp);

		delete[] temp;
	}

	char& operator[](int index) {
		return this->buffer[index];
	}

	const char& operator[](int index) const {
		return this->buffer[index];
	}

	MyString& operator=(const MyString& other) {
		if (this != &other) {
			delete[] this->buffer;
			this->size = std::strlen(other.buffer);
			this->buffer = new char[this->size + 1];
			std::strcpy(buffer, other.buffer);
		}
		return *this;
	}

	MyString operator+(const char c) const {
		MyString result;
		result.size = this->size + 1;
		delete[] result.buffer;
		result.buffer = new char[result.size + 1];
		std::strcpy(result.buffer, this->buffer);
		result[result.size - 1] = c;
		result[result.size] = '\0';
		return result;
	}

	MyString operator+(const MyString& s) const {
		MyString result;
		result.size = this->size + s.size;
		delete[] result.buffer;
		result.buffer = new char[result.size + 1];
		std::strcpy(result.buffer, this->buffer);
		std::strcat(result.buffer, s.buffer);
		result[result.size] = '\0';
		return result;
	}


	friend std::ostream& operator<<(std::ostream& out, const MyString& string) {
		out << string.buffer;
		return out;
	}
};

class ElementString {
public:
	MyString data;
	ElementString* next;

	ElementString(const MyString& data, ElementString* next = nullptr) 
		: data(data), next(next) {}
};

class StackString {
private:
	ElementString* top;

public:
	StackString() : top(nullptr) {}

	~StackString() {
		while (!isEmpty()) {
			pop();
		}
	}

	void push(const MyString& value) {
		ElementString* newElement = new ElementString(value, top);
		top = newElement;
	}

	MyString pop() {
		if (isEmpty()) {
			//std::cerr << "Stack underflow!" << std::endl;
			return MyString();
		}
		else {
			ElementString* temp = top;
			top = top->next;
			MyString value(temp->data);
			delete temp;
			return value;
		}
	}

	MyString peek() const {
		if (isEmpty()) {
			std::cerr << "Stack is empty!" << std::endl;
			return MyString();
		}
		else {
			return top->data;
		}
	}

	bool isEmpty() const {
		return top == nullptr;
	}

	friend std::ostream& operator<<(std::ostream& out, const StackString& stack) {
		ElementString* current = stack.top;
		while (current != nullptr) {
			out << current->data << std::endl;
			current = current->next;
		}
		return out;
	}

};

class StackInt {
private:
	struct Element {
		int data;
		Element* next;

		Element(int data, Element* next = nullptr)
			: data(data), next(next) {}
	};

	Element* top;

public:
	StackInt() : top(nullptr) {}

	~StackInt() {
		while (!isEmpty()) {
			pop();
		}
	}

	void push(int value) {
		Element* newElement = new Element(value, top);
		top = newElement;
	}

	int pop() {
		if (isEmpty()) {
			//std::cerr << "Stack underflow!" << std::endl;
			return 0;
		}
		else {
			Element* temp = top;
			top = top->next;
			int value = temp->data;
			delete temp;
			return value;
		}
	}

	int peek() const {
		if (isEmpty()) {
			std::cerr << "Stack is empty!" << std::endl;
			return 0;
		}
		else {
			return top->data;
		}
	}

	bool isEmpty() const {
		return top == nullptr;
	}

	void printStack() const {
		Element* current = top;
		while (current != nullptr) {
			std::cout << current->data << " ";
			current = current->next;
		}
		std::cout << std::endl;
	}
};

bool isFunction(const MyString& s) {
	if (s[0] == 'N' && s.length() == 1)
		return true;
	else 
		return false;
	
	/*
	if (s[0] == 'M' && s[1] == 'I' && s[2] == 'N' && s[3] == '(')
		return true;
	else if (s[0] == 'M' && s[1] == 'A' && s[2] == 'X' && s[3] == '(')
		return true;
	else
		return false;
		*/
}

bool isOperator(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/';
}

bool isNumber(char c) {
	return c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9';
}

int precedence(char c) {
	if (c == '+' || c == '-') {
		return 1;
	}
	else if (c == '*' || c == '/') {
		return 2;
	}
	else if (c == 'N') { 
		return 3;
	}
	return 0;
}

MyString toONP(const MyString& input) {
	StackString operators;
	MyString output;
	MyString currentWord;

	for (int i = 0; i < input.length(); i++) {
		char currentChar = input[i];
		currentWord = currentWord + currentChar;
		if (isNumber(input[i]))
		{
			int j = 1;
			for (j; input[i + j] != ' '; j++) {
				currentChar = input[i + j];
				currentWord = currentWord + currentChar;
			}
			i = i + j;
			output = output + currentWord + " ";
		}
		else if (isFunction(currentWord)) {
			operators.push(currentWord);
			currentWord = "";
		}
		else if (currentChar != ' ') {
			if (currentChar == '(') {
				operators.push(currentChar);
			}
			else if (currentChar == ')') {
				while (!operators.isEmpty() && operators.peek()[0] != '(') {
					output = output + operators.pop()[0] + " ";
				}
				operators.pop();
			}
			else if (isOperator(currentChar)) {
				while (!operators.isEmpty() && precedence(currentChar) <= precedence(operators.peek()[0])) {
					output = output + operators.pop()[0] + " ";
				}
				operators.push(currentChar);
			}
		}
		currentWord = "";
	}

	while (!operators.isEmpty()) {
		output = output + operators.pop()[0] + " ";
	}

	return output;
}

void calculate(MyString onp) {
	StackInt operands;

	for (int i = 0; i < onp.length(); i++) {

		if (isNumber(onp[i])) {
			int currentNumber = ((int)onp[i] - 48);
			while (onp[i + 1] != ' ') {
				++i;
				currentNumber = (currentNumber * 10) + ((int)onp[i] - 48);
			}
			operands.push(currentNumber);
		}

		if (isOperator(onp[i])) {
			char o = onp[i];
			std::cout << o << " ";
			operands.printStack();
			int x = operands.pop();
			int y = operands.pop();
			switch (o) {
			case '+':
				operands.push(y + x);
				break;
			case '-':
				operands.push(y - x);
				break;
			case '*':
				operands.push(y * x);
				break;
			case '/':
				if (x == 0) {
					std::cout << "ERROR\n";
					return;
				}
				operands.push(y / x);
				break;
			}
		}
		else if (onp[i] == 'N') {
			char o = onp[i];
			std::cout << o << " ";
			operands.printStack();
			int x = operands.pop();
			operands.push(-x);
		}
	}
	std::cout << operands.peek() << '\n';
}


int main()
{
	//MyString input("( 5 - 4 ) / N 4 / N ( 0 + 9 ) .");
	//MyString onp = toONP(input);
	//std::cout << onp << '\n';
	//calculate(onp);

	
	int n;
	std::cin >> n >> std::ws;

	for (int i = 0; i < n; i++) {
		MyString* input = new MyString;
		input->read();
		MyString onp = toONP(*input);
		std::cout << onp << '\n';
		calculate(onp);
		std::cout << "\n";
		delete input;
	}
	
	return 0;
}

/*
input: "( 5 - 4 ) / N 4 / N ( 0 + 9 ) ."

output: "5  4  -  4  N  /  0  9  +  N  /" 
		"- 4 5"
		"N 4 1"
		"/ -4 1"
		"+ 9 0 0"
		"N 9 0"
		"/ -9 0"
		"0"
*/