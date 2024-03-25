#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>

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

	void read() { // TODO: Mozna byloby to lepiej zoptymalizowac + dodaæ od razu przerobienie na onp tutaj
		char ch;
		int index = 0;

		delete[] buffer;
		buffer = new char[1]; 
		buffer[0] = '\0'; 

		while (std::cin.get(ch) && ch != '.') {
			char* temp = new char[index + 2]; 
			std::strcpy(temp, buffer); 
			temp[index] = ch;
			temp[++index] = '\0'; 

			delete[] buffer; 
			buffer = temp; 
		}

		this->size = index;
		//konwersja na onp tu wsadziæ np
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

bool isFunction(const char c) {
	return c == 'M' || c == 'I';
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
	else if (c == 'N' || c == 'M') {
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

		if (currentChar != ' ') {
			if (isNumber(input[i]))
			{
				for (++i; input[i] != ' '; i++) {
					currentChar = input[i];
					currentWord = currentWord + currentChar;
				}
				output = output + currentWord + " ";
			}

			else if (isFunction(currentChar)) {
				int bracketCounter = 1;
				int wordCounter = 1;
				MyString currentFunction;

				for (++i; input[i-1] != '('; i++) {
					currentChar = input[i];
					currentWord = currentWord + currentChar;
				}
				currentWord[currentWord.length()-2] = '\0';
				
				currentFunction = currentWord;
				currentWord = "";

				while (bracketCounter != 0) {
					currentChar = input[++i];
					currentWord = currentWord + currentChar;

					if (currentChar == '(') {
						bracketCounter++;
					}
					else if (currentChar == ')') {
						--bracketCounter;
						if (bracketCounter == 0)
							currentWord[currentWord.length()-1] = '\0';
					}
					else if (currentChar == ',') {
						if (bracketCounter == 1) {
							wordCounter++;
						}
					}
				}
				
				if(currentFunction[0]=='M')
					output = output + toONP(currentWord) + " " + currentFunction + ((char)(wordCounter) + 48) + " ";
				else 
					output = output + toONP(currentWord) + " " + currentFunction + " ";
			}

			else if (currentChar == 'N') {
				operators.push(currentChar);
			}

			else if (currentChar == '(') {
				operators.push(currentChar);
			}

			else if (currentChar == ')') {
				while (!operators.isEmpty() && operators.peek()[0] != '(') {
					output = output + operators.pop()[0] + " ";
				}
				operators.pop();
			}

			else if (currentChar == ',') {
				while (!operators.isEmpty()) {
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
	
	output[output.length() - 1] = '\0';

	return output;
}

void calculate(MyString onp) {
	StackInt operands;

	if (onp.length() == 0) {
		return;
	}

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
		else if (onp[i] == 'I') {
			std::cout << "IF ";
			operands.printStack();
			int f = operands.pop();
			int t = operands.pop();
			int x = operands.pop();
			if (x > 0)
				operands.push(t);
			else
				operands.push(f);
		}
		else if (onp[i] == 'M') {
			int j = i + 3;
			int n = ((int)onp[j] - 48);

			while (onp[j + 1] != ' ' && onp[j + 1] != '\0') {
				n = (n * 10) + ((int)onp[++j] - 48);
			}

			if (onp[i + 1] == 'I') {
				std::cout << "MIN" << n << " ";
				operands.printStack();
				int min = operands.pop();
				for (int j = 0; j < n-1; j++) {
					int temp = operands.pop();
					if (temp < min)
						min = temp;
				}
				operands.push(min);
			}
			else {
				std::cout << "MAX" << n << " ";
				operands.printStack();
				int max = operands.pop();
				for (int j = 0; j < n-1; j++) {
					int temp = operands.pop();
					if (temp > max)
						max = temp;
				}
				operands.push(max);
			}
			i += 4;
		}
	}
	std::cout << operands.peek() << '\n';
}


int main()
{
	/* do testow 
	MyString input("MIN ( 100 , MAX ( 1 , 34 , 2 ) , 80 ,  MIN ( 66 , 36  , 35 , 77 ) , 50 , 60 ) .");
	MyString onp = toONP(input);
	std::cout << input << '\n' << onp << '\n';
	calculate(onp);
	/**/


	/* na stos 5 / 10 */
	int n;
	std::cin >> n >> std::ws;
	for (int i = 0; i < n; i++) {
		MyString* input = new MyString;
		input->read();

		MyString onp = toONP(*input);
		std::cout << onp << '\n';

		calculate(onp);
		delete input;
	}
	/**/

	/* do wyciagania info o testach 
	int n;
	std::cin >> n >> std::ws;
	std::cout << n << '\n';

	for (int i = 0; i < n; i++) {
		MyString* input = new MyString;
		input->read();

		MyString onp = toONP(*input);
		std::cout << "INPUT[" << i << "]: \"" << *input <<"\"\n";

		delete input;
	}
	*/

	return 0;
}

/*
input: 
		"IF ( N 4 , IF ( 3 , 7 , 9 ) , N ( 8 ) ) + N ( 5 * 9 ) ."
output: 
		4  N  3  7  9  IF  8  N  IF  5  9  *  N  +
		N 4
		IF 9 7 3 -4
		N 8 7 -4
		IF -8 7 -4
		* 9 5 -8
		N 45 -8
		+ -45 -8
		-53

*/