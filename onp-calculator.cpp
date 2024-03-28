	#define _CRT_SECURE_NO_WARNINGS
	#include <iostream>
	#include <cstring>

	class MyString;
	class ElementString;
	class StackString;

	bool isFunction(const char c);
	bool isOperator(char c);
	bool isNumber(char c);
	int precedence(char c);

	MyString stringToONP(const MyString& input);

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
		int capacity = 16;

		MyString(const char* string = "") {
			this->size = std::strlen(string);
			setCapacityNeeded(this->size);
			buffer = new char[this->capacity];
			std::strcpy(buffer, string);
			buffer[size] = '\0';
		}

		MyString(const MyString& other) {
			this->size = std::strlen(other.buffer);
			setCapacityNeeded(this->size);
			buffer = new char[this->capacity];
			std::strcpy(buffer, other.buffer);
			buffer[size] = '\0';
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

		void setCapacityNeeded(int s) {
			while (capacity - 1 < s) {
				capacity *= 2;
			}
		};

		void resize() {
			char* newBuffer = new char[capacity];
			std::strcpy(newBuffer, buffer);
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
				this->size = std::strlen(other.buffer);
				std::strcpy(this->buffer, other.buffer);
				this->buffer[this->size] = '\0';
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
				std::strcpy(this->buffer, s.buffer);
			else
				std::strcat(this->buffer, s.buffer);

			this->buffer[this->size] = '\0';

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& out, const MyString& string) {
			out << string.buffer;
			return out;
		}

		void read();

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
					for (j = 0; j < n - 1; j++) {
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
					for (j = 0; j < n - 1; j++) {
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

	
	void readToONP(MyString*& result) {
		StackString operators;
		MyString output;
		MyString currentWord; 
		DynamicArray wordCount(4);
		int depth = 0;

		char currentChar = ' ';
		while (currentChar != '.') {
			std::cin.get(currentChar);
			currentWord += currentChar;

			if (currentChar != ' ') {
				if (isNumber(currentChar))
				{
					std::cin.get(currentChar);
					while (currentChar != ' ') {
						currentWord += currentChar;
						std::cin.get(currentChar);
					}
					output += currentWord;
					output += ' ';
				}
				else if (isFunction(currentChar)) {
					MyString currentFunction;

					std::cin.get(currentChar);
					while (currentChar != ' ') {
						currentWord += currentChar;
						std::cin.get(currentChar);
					}

					currentFunction = currentWord;
					currentWord = "";
					
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
							output += operators.pop();
							output += (char)(wordCount[depth+1]) + 48;
							wordCount[depth+1] = 1;
						}
						else if (operators.peek()[0] == 'I') {
							wordCount[depth + 1] -= 2;
							output += operators.pop();
						}
						else
							output += operators.pop();
						output += ' ';
					}
					depth--;
					operators.pop();
				}
				else if (currentChar == ',') {
					wordCount[depth] += 1;
					while (!operators.isEmpty() && operators.peek()[0] != '(' ) {
						if (operators.peek()[0] == 'M') {
							output += operators.pop();
							output += (char)(wordCount[depth+1]) + 48;
							wordCount[depth+1] = 1;
						}
						else if (operators.peek()[0] == 'I') {
							wordCount[depth + 1] -= 2;
							output += operators.pop();
						}
						else
							output += operators.pop();
						output += ' ';
					}
				}
				else if (isOperator(currentChar)) {
					while (!operators.isEmpty() && precedence(currentChar) <= precedence(operators.peek()[0])) {
						if (operators.peek()[0] == 'M') {
							output += operators.pop();
							output += (char)(wordCount[depth+1]) + 48;
							wordCount[depth+1] = 1;
						}
						else if (operators.peek()[0] == 'I') {
							wordCount[depth + 1] -= 2;
							output += operators.pop();
						}
						else
							output += operators.pop();
						output += ' ';
					}
					operators.push(currentChar);
				}
			}
			currentWord = "";
		}

		while (!operators.isEmpty()) {
			if (operators.peek()[0] == 'M') {
				output += operators.pop();
				output += (char)(wordCount[depth+1]) + 48;
				wordCount[depth+1] = 1;
			}
			else if (operators.peek()[0] == 'I') {
				wordCount[depth + 1] -= 2;
				output += operators.pop();
			}
			else
				output += operators.pop();
			output += ' ';
		}

		output[output.length() - 1] = '\0';
		delete[] result->buffer;
		result->buffer = new char[output.length() + 1];
		strcpy(result->buffer, output.buffer);
	};

	int main()
	{
		/* do testow
		MyString input("MIN ( 100 , MAX ( 1 , 34 , 2 ) , 80 ,  MIN ( 66 , 36  , 35 , 77 ) , 50 , 60 ) .");
		MyString onp = stringToONP(input);
		std::cout << input << '\n' << onp << '\n';
		calculate(onp);
		/**/

		/* na stos 9/10  */
		int n;
		std::cin >> n >> std::ws;
		for (int i = 0; i < n; i++) {
			MyString* onp = new MyString;
			readToONP(onp);

			std::cout << *onp << '\n';

			calculate(*onp);
			delete onp;
		}
		/**/


		return 0;
	}