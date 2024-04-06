#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>

class DynamicArray;
class MyString;
class ElementString;
class StackString;

bool isFunction(const char c);
bool isOperator(char c);
bool isNumber(char c);
int precedence(char c);


class DynamicArray {
private:
	int* array;
	int size;
	int capacity;

public:
	DynamicArray(int initialCapacity = 8) {
		size = 0;
		capacity = initialCapacity;
		array = new int[capacity];
		for (int i = 0; i < capacity; i++) {
			array[i] = 1;
		}
	}

	~DynamicArray() {
		delete[] array;
	}

	int& operator[](int index) {
		if (index >= size) {
			capacity *= 2;
			int* tempArray = new int[capacity];
			int i = 0;
			for (i; i < size; ++i)
				tempArray[i] = array[i];
			for (i; i < capacity; ++i)
				tempArray[i] = 1;
			delete[] array;
			array = tempArray;
			size = index + 1;
		}
		return array[index];
	}
};

class MyString {
public:
	char* buffer;
	int size;
	int capacity = 64;

	MyString(const char* string = "") {
		this->size = std::strlen(string);
		setCapacityNeeded(this->size);
		buffer = new char[this->capacity];
		std::memcpy(buffer, string, this->size + 1);
	}

	MyString(const MyString& other) {
		this->size = std::strlen(other.buffer);
		setCapacityNeeded(this->size);
		buffer = new char[this->capacity];
		std::memcpy(buffer, other.buffer, this->size + 1);
	}

	MyString(const char other) {
		this->size = 1;
		this->capacity = 2;
		buffer = new char[this->capacity];
		buffer[0] = other;
		buffer[1] = '\0';
	}

	~MyString() {
		if (buffer != nullptr) {
			delete[] buffer;
			buffer = nullptr;
		}
	}

	int length() const {
		return this->size;
	}

	MyString reverse() {
		MyString r;
		for (int i = size - 1; i >= 0; i--) { // Poprawiamy pêtlê aby iterowaæ od ostatniego do pierwszego indeksu
			r += buffer[i]; // Dodajemy znaki w odwrotnej kolejnoœci do nowego obiektu MyString
		}
		*this = r; // Zapisujemy odwrócony ³añcuch znaków do bie¿¹cego obiektu
		return r; // Zwracamy odwrócony ³añcuch znaków
	}

	void setCapacityNeeded(int s) {
		while (capacity - 1 < s) {
			capacity *= 2;
		}
	};

	void resize() {
		char* newBuffer = new char[capacity];
		std::memcpy(newBuffer, buffer, this->size + 1);
		delete[] buffer;
		buffer = newBuffer;
		buffer[this->size] = '\0';
	}

	char& operator[](int index) {
		return this->buffer[index];
	}

	const char& operator[](int index) const {
		return this->buffer[index];
	}

	MyString& operator=(const MyString& other) {
		if (this != &other) {
			if (this->capacity < other.capacity) {
				setCapacityNeeded(this->size + other.size);
			}
			resize();
			this->size = other.size;
			std::memcpy(this->buffer, other.buffer, this->size + 1);
		}
		return *this;
	}

	MyString operator+=(const char c) {
		setCapacityNeeded(this->size + 2);
		resize();
		buffer[this->size] = c;
		buffer[++this->size] = '\0';
		return *this;
	}


	MyString operator+=(const MyString& s) {
		setCapacityNeeded(this->size + s.size);
		resize();
		this->size += s.size;
		if (this->size == 0)
			std::memcpy(this->buffer, s.buffer, s.size + 1);
		else
			std::memcpy(this->buffer + this->size - s.size, s.buffer, s.size + 1);

		this->buffer[this->size] = '\0';

		return *this;
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

class StringQueue {
private:
	ElementString* firstNode;
	ElementString* lastNode;

public:
	StringQueue() : firstNode(nullptr), lastNode(nullptr) {}

	~StringQueue() {
		clear();
	}

	void enqueue(MyString value) {
		ElementString* newNode = new ElementString(value, nullptr);

		if (isEmpty()) {
			firstNode = lastNode = newNode;
		}
		else {
			lastNode->next = newNode;
			lastNode = newNode;
		}
	}

	MyString dequeue() {
		if (!isEmpty()) {
			MyString frontData = firstNode->data;
			ElementString* temp = firstNode;
			firstNode = firstNode->next;
			delete temp;
			return frontData;
		}
		else {
			lastNode = nullptr;
			return MyString();
		}
	}

	MyString front() const {
		return firstNode->data;
	}

	bool isEmpty() const {
		return firstNode == nullptr;
	}

	void printQueue() const {
		if (isEmpty()) {
			std::cout << "Queue is empty.\n";
			return;
		}
		ElementString* current = firstNode;
		while (current != nullptr) {
			std::cout << current->data << " ";
			current = current->next;
		}
		std::cout << std::endl;
	}
	void clear() {
		while (firstNode != nullptr) {
			ElementString* temp = firstNode;
			firstNode = firstNode->next;
			delete temp;
		}
		lastNode = nullptr;
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
	else if (c == 'N' || c == 'M' || c == 'I') {
		return 3;
	}
	return 0;
}


void calculate(StringQueue& onp) {
	StackInt operands;

	while (!onp.isEmpty()) {

		if (isNumber(onp.front()[0])) {
			int currentNumber = ((int)onp.front()[0] - 48);
			int i = 0;
			while (onp.front()[i + 1] != ' ' && onp.front()[i + 1] != '\0') {
				++i;
				currentNumber = (currentNumber * 10) + ((int)onp.front()[i] - 48);
			}
			onp.dequeue();
			operands.push(currentNumber);
		}

		if (isOperator(onp.front()[0])) {
			char o = onp.dequeue()[0];
			printf("%c ", o);
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
					printf("ERROR\n");
					return;
				}
				operands.push(y / x);
				break;
			}
		}
		else if (onp.front()[0] == 'N') {
			char o = onp.dequeue()[0];
			printf("%c ", o);
			operands.printStack();
			int x = operands.pop();
			operands.push(-x);
		}
		else if (onp.front()[0] == 'I') {
			onp.dequeue();
			printf("IF ");
			operands.printStack();
			int f = operands.pop();
			int t = operands.pop();
			int x = operands.pop();
			if (x > 0)
				operands.push(t);
			else
				operands.push(f);
		}
		else if (onp.front()[0] == 'M') {
			int j = 3;
			int n = ((int)onp.front()[j] - 48);

			while (onp.front()[j + 1] != ' ' && onp.front()[j + 1] != '\0') {
				n = (n * 10) + ((int)onp.front()[++j] - 48);
			}

			if (onp.front()[1] == 'I') {
				printf("MIN%d ", n);
				onp.dequeue();
				operands.printStack();
				int min = operands.pop();
				for (j = 0; j < n - 1; j++) {
					int temp = operands.pop();
					if (temp < min)
						min = temp;
				}
				operands.push(min);
			}
			else {
				printf("MAX%d ", n);
				onp.dequeue();
				operands.printStack();
				int max = operands.pop();
				for (j = 0; j < n - 1; j++) {
					int temp = operands.pop();
					if (temp > max)
						max = temp;
				}
				operands.push(max);
			}
		}
	}
	printf("%d\n", operands.peek());
}



void readToONP(StringQueue& output) {
	StackString operators;
	MyString currentWord;
	MyString temp;
	DynamicArray wordCount(8);
	int depth = 0;

	char currentChar = ' ';
	while (currentChar != '.') {
		scanf("%c", &currentChar);
		currentWord += currentChar;

		if (currentChar != ' ') {
			if (isNumber(currentChar))
			{
				scanf("%c", &currentChar);
				while (currentChar != ' ') {
					currentWord += currentChar;
					scanf("%c", &currentChar);
				}
				output.enqueue(currentWord);
			}
			else if (isFunction(currentChar)) {
				MyString currentFunction;

				scanf("%c", &currentChar);
				while (currentChar != ' ') {
					currentWord += currentChar;
					scanf("%c", &currentChar);
				}

				currentFunction = currentWord;
				currentWord.size = 0;

				if (currentFunction[0] == 'M') {
					operators.push(currentFunction);
				}
				else {
					operators.push(currentFunction);
				}
			}
			else if (currentChar == 'N') {
				operators.push(currentChar);
			}
			else if (currentChar == '(') {
				depth++;
				operators.push(currentChar);
			}
			else if (currentChar == ')') {
				while (!operators.isEmpty() && operators.peek()[0] != '(') {
					if (operators.peek()[0] == 'M') {
						temp += operators.pop();
						MyString number;
						while (wordCount[depth + 1] > 0) {
							int currentDigit = wordCount[depth + 1] % 10;
							number += (char)(currentDigit + 48);
							wordCount[depth + 1] /= 10;
						}
						temp += number.reverse();

						output.enqueue(temp);
						temp = "";

						wordCount[depth + 1] = 1;
					}
					else if (operators.peek()[0] == 'I') {
						wordCount[depth + 1] -= 2;
						output.enqueue(operators.pop());
					}
					else
						output.enqueue(operators.pop());
				}
				depth--;
				operators.pop();
			}
			else if (currentChar == ',') {
				wordCount[depth] += 1;
				while (!operators.isEmpty() && operators.peek()[0] != '(') {
					if (operators.peek()[0] == 'M') {
						temp += operators.pop();
						MyString number;
						while (wordCount[depth + 1] > 0) {
							int currentDigit = wordCount[depth + 1] % 10;
							number += (char)(currentDigit + 48);
							wordCount[depth + 1] /= 10;
						}
						temp += number.reverse();

						output.enqueue(temp);

						wordCount[depth + 1] = 1;
						temp = "";
					}
					else if (operators.peek()[0] == 'I') {
						wordCount[depth + 1] -= 2;
						output.enqueue(operators.pop());
					}
					else
						output.enqueue(operators.pop());
				}
			}
			else if (isOperator(currentChar)) {
				while (!operators.isEmpty() && precedence(currentChar) <= precedence(operators.peek()[0])) {
					if (operators.peek()[0] == 'M') {
						temp += operators.pop();
						MyString number;
						while (wordCount[depth + 1] > 0) {
							int currentDigit = wordCount[depth + 1] % 10;
							number += (char)(currentDigit + 48);
							wordCount[depth + 1] /= 10;
						}
						temp += number.reverse();
						output.enqueue(temp);

						wordCount[depth + 1] = 1;
						temp = "";
					}
					else if (operators.peek()[0] == 'I') {
						wordCount[depth + 1] -= 2;
						output.enqueue(operators.pop());
					}
					else
						output.enqueue(operators.pop());
				}
				operators.push(currentChar);
			}
		}
		currentWord.size = 0;
	}

	while (!operators.isEmpty()) {
		if (operators.peek()[0] == 'M') {
			temp += operators.pop();
			MyString number;
			while (wordCount[depth + 1] > 0) {
				int currentDigit = wordCount[depth + 1] % 10;
				number += (char)(currentDigit + 48);
				wordCount[depth + 1] /= 10;
			}
			temp += number.reverse();
			output.enqueue(temp);

			wordCount[depth + 1] = 1;
			temp = "";
		}
		else if (operators.peek()[0] == 'I') {
			wordCount[depth + 1] -= 2;
			output.enqueue(operators.pop());
		}
		else
			output.enqueue(operators.pop());
	}


};

int main()
{
	int n;
	std::cin >> n >> std::ws;
	StringQueue kolejka;
	for (int i = 0; i < n; i++) {
		readToONP(kolejka);
		kolejka.printQueue();
		calculate(kolejka);

		if (!kolejka.isEmpty()) {
			kolejka.clear();
		}
	}

	return 0;
}