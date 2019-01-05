// Copyright 2017, Bradley Peterson, Weber State University, all rights reserved.

#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <cmath>

//To prevent those using g++ from trying to use a library
//they don't have
#ifndef __GNUC__
#include <conio.h>
#else
#include <stdio.h>
#endif

using std::stack;
using std::istringstream;
using std::ostringstream;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::pow;

template <typename T>
struct Node {
	T info;
	Node* llink;
	Node* rlink;
};

class TreeParser {

private:
	Node<string> * root;
	stack<string> mathStack;
	double castStrToDouble(string const &s);
	string castDoubleToStr(const double d);
	void initialize();
	void inOrderTraversal(Node<string> * p);
	void postOrderTraversal(Node<string> *p);
	bool isDigit(char c);
	bool isOperator(char c);
	void processExpression(Node<string>*p);
	void computeAnswer(Node<string>*p);

protected:
	string expression;
	int position;

public:
	TreeParser();
	void inOrderTraversal();
	void postOrderTraversal();
	void displayParseTree();
	void processExpression(string &expression);
	string computeAnswer();

};

void TreeParser::initialize() {

	expression = "";
	position = 0;
	while (!mathStack.empty()) {
		mathStack.pop();
	}
}

double TreeParser::castStrToDouble(const string &s) {
	istringstream iss(s);
	double x;
	iss >> x;
	return x;
}

string TreeParser::castDoubleToStr(const double d) {
	ostringstream oss;
	oss << d;
	return oss.str();
}

TreeParser::TreeParser() {
	initialize();
}

bool TreeParser::isDigit(char c) {

	if (c == '0' || c == '1' || c == '2' ||
		c == '3' || c == '4' || c == '5' ||
		c == '6' || c == '7' || c == '8' ||
		c == '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool TreeParser::isOperator(char c) {
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TreeParser::displayParseTree() {
	cout << "The expression seen using in-order traversal: ";
	inOrderTraversal();
	cout << endl;
	cout << "The expression seen using post-order traversal: ";
	postOrderTraversal();
	cout << endl;
}

void TreeParser::inOrderTraversal() {

	inOrderTraversal(root);
}

void TreeParser::inOrderTraversal(Node<string> * p) {
	if (p != nullptr)
	{
		inOrderTraversal(p->llink);
		cout << p->info << " ";
		inOrderTraversal(p->rlink);
	}
}

void TreeParser::postOrderTraversal() {

	postOrderTraversal(root);
}

void TreeParser::postOrderTraversal(Node<string> *p) {
	if (p != nullptr)
	{
		postOrderTraversal(p->llink);
		postOrderTraversal(p->rlink);

		cout << p->info << " ";
	}
}


void TreeParser::processExpression(string &expression) {

	this->expression = expression;
	position = 0;

	root = new Node<string>();
	root->llink = nullptr;
	root->rlink = nullptr;

	processExpression(root);

}

void TreeParser::processExpression(Node<string>*p) {
	while (position<=expression.length())
	{
		if (expression[position] == '(')
		{
			Node<string> *tNode = new Node<string>();
			tNode->llink = nullptr;
			tNode->rlink = nullptr;

			p->llink = tNode;

			position++;
			processExpression(p->llink);

		}
		else if (isDigit(expression[position]))
		{
			string tString;

			while (isDigit(expression[position]))
			{
				tString += expression[position];
				position++;
			}

			p->info = tString;
			return;
		}
		else if (isOperator(expression[position]))
		{
			p->info = expression[position];
			Node<string> *tNode = new Node<string>();

			tNode->llink = nullptr;
			tNode->rlink = nullptr;

			p->rlink = tNode;

			position++;
			processExpression(p->rlink);
		}
		else if (expression[position] == ')')
		{
			position++;
			return;
		}
		else if (expression[position] == ' ')
		{
			position++;
		}

	}
}

string TreeParser::computeAnswer() {
	computeAnswer(root);
	return mathStack.top();
}

void TreeParser::computeAnswer(Node<string>* p) {
	if (p != nullptr)
	{
		computeAnswer(p->llink);
		computeAnswer(p->rlink);
		if (p->info == "+")
		{
			double answer; string num1; string num2; string finished;

			num1 = mathStack.top();
			mathStack.pop();
			num2 = mathStack.top();
			mathStack.pop();

			answer = castStrToDouble(num2) + castStrToDouble(num1);
			finished = castDoubleToStr(answer);

			mathStack.push(finished);

		}
		else if (p->info == "-")
		{
			double answer; string num1; string num2; string finished;

			num1 = mathStack.top();
			mathStack.pop();
			num2 = mathStack.top();
			mathStack.pop();

			answer = castStrToDouble(num2) - castStrToDouble(num1);
			finished = castDoubleToStr(answer);

			mathStack.push(finished);
		}
		else if (p->info == "*")
		{
			double answer; string num1; string num2; string finished;

			num1 = mathStack.top();
			mathStack.pop();
			num2 = mathStack.top();
			mathStack.pop();

			answer = castStrToDouble(num2) * castStrToDouble(num1);
			finished = castDoubleToStr(answer);

			mathStack.push(finished);
		}
		else if (p->info == "/")
		{
			double answer; string num1; string num2; string finished;

			num1 = mathStack.top();
			mathStack.pop();
			num2 = mathStack.top();
			mathStack.pop();

			answer = castStrToDouble(num2) / castStrToDouble(num1);
			finished = castDoubleToStr(answer);

			mathStack.push(finished);
		}
		else if (p->info == "^")
		{
			double answer; string num1; string num2; string finished;

			num1 = mathStack.top();
			mathStack.pop();
			num2 = mathStack.top();
			mathStack.pop();

			answer = pow(castStrToDouble(num2), castStrToDouble(num1));
			finished = castDoubleToStr(answer);

			mathStack.push(finished);
		}
		else
		{
			mathStack.push(p->info);
		}
	}
}

void pressAnyKeyToContinue() {
	printf("Press any key to continue\n");
	//Linux and Mac users with g++ don't need this
	//But everyone else will see this message.
#ifndef __GNUC__
	_getch();
#else
	int c;
	fflush(stdout);
	do c = getchar(); while ((c != '\n') && (c != EOF));
#endif
}

int main() {
	TreeParser *tp = new TreeParser;
	string expression = "(4+7)";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 11
	expression = "(7-4)";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 3
	expression = "(4^3)";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 64
	expression = "((2-5)-5)";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display -8
	expression = "(5*(6/2))";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 15
	expression = "((1 + 2) * (3 + 4))";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 21
	expression = "(543+321)";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 864
	expression = "(((((3+12)-7)*120)/(2+3))^3)";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 7077888
	expression = "(((((9+(2*(110-(30/2))))*8)+1000)/2)+(((3^4)+1)/2))";
	tp->processExpression(expression);
	tp->displayParseTree();
	cout << "The result is: " << tp->computeAnswer() << endl; //Should display 1337
	pressAnyKeyToContinue();
	return 0;
}